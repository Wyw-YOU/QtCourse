// src/network/networkmanager.cpp
#include "networkmanager.h"
#include "src/utils/logger.h"
#include <QJsonArray>
#include <QJsonDocument>

NetworkManager* NetworkManager::m_instance = nullptr;

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
    , m_tcpClient(nullptr)
    , m_networkThread(nullptr)
    , m_nextRequestId(1)
    , m_isLoggedIn(false)
    , m_autoReconnect(true)
{
    // 创建网络线程
    m_networkThread = new QThread();
    m_networkThread->setObjectName("NetworkThread");

    // 创建TCP客户端
    m_tcpClient = new TcpClient();
    m_tcpClient->moveToThread(m_networkThread);

    // 设置自动重连
    m_tcpClient->setAutoReconnect(m_autoReconnect);

    // 连接信号槽
    connect(m_tcpClient, &TcpClient::messageReceived,
            this, &NetworkManager::onMessageReceived);
    connect(m_tcpClient, &TcpClient::connected,
            this, &NetworkManager::onConnected);
    connect(m_tcpClient, &TcpClient::disconnected,
            this, &NetworkManager::onDisconnected);
    connect(m_tcpClient, &TcpClient::connectionError,
            this, &NetworkManager::onConnectionError);

    // 启动网络线程
    m_networkThread->start();

    LOG_INFO("NetworkManager", "Network manager initialized");
}

NetworkManager::~NetworkManager()
{
    LOG_INFO("NetworkManager", "Shutting down network manager");

    disconnectFromServer();

    if (m_networkThread) {
        m_networkThread->quit();
        m_networkThread->wait();
        delete m_networkThread;
    }
}

NetworkManager* NetworkManager::instance()
{
    if (!m_instance) {
        m_instance = new NetworkManager();
    }
    return m_instance;
}

bool NetworkManager::connectToServer(const QString& host, quint16 port)
{
    LOG_INFO("NetworkManager", QString("Connecting to server %1:%2").arg(host).arg(port));

    bool result = false;

    // 使用元对象调用，确保在正确的线程中执行
    QMetaObject::invokeMethod(m_tcpClient, [this, host, port, &result]() {
        result = m_tcpClient->connectToServer(host, port);
    }, Qt::BlockingQueuedConnection);

    return result;
}

void NetworkManager::disconnectFromServer()
{
    LOG_INFO("NetworkManager", "Disconnecting from server");

    QMetaObject::invokeMethod(m_tcpClient, &TcpClient::disconnectFromServer,
                              Qt::BlockingQueuedConnection);

    m_isLoggedIn = false;
    m_currentUser = UserInfo();

    emit connectionChanged(false);
}

bool NetworkManager::isConnected() const
{
    if (!m_tcpClient) {
        return false;
    }

    bool connected = false;

    QMetaObject::invokeMethod(m_tcpClient, [this, &connected]() {
        connected = m_tcpClient->isConnected();
    }, Qt::BlockingQueuedConnection);

    return connected;
}

void NetworkManager::login(const QString& username, const QString& password)
{
    LOG_INFO("NetworkManager", QString("Login request for user: %1").arg(username));

    QJsonObject loginData;
    loginData["username"] = username;
    loginData["password"] = password; // 注意：实际应该使用加密传输
    loginData["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    quint32 requestId = generateRequestId();
    addRequest(requestId, "login");

    QMetaObject::invokeMethod(m_tcpClient, [this, loginData, requestId]() {
        m_tcpClient->sendMessage(NetMsg_Login, loginData, requestId);
    });
}

void NetworkManager::registerUser(const UserInfo& userInfo, const QString& password)
{
    LOG_INFO("NetworkManager", QString("Register request for user: %1").arg(userInfo.username));

    QJsonObject registerData = userInfoToJson(userInfo);
    registerData["password"] = password; // 注意：实际应该使用加密传输
    registerData["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    quint32 requestId = generateRequestId();
    addRequest(requestId, "register");

    QMetaObject::invokeMethod(m_tcpClient, [this, registerData, requestId]() {
        m_tcpClient->sendMessage(NetMsg_Register, registerData, requestId);
    });
}

void NetworkManager::sendTextMessage(int receiverId, const QString& content, ChatType chatType)
{
    if (!m_isLoggedIn) {
        LOG_WARNING("NetworkManager", "Cannot send message: user not logged in");
        return;
    }

    // 创建本地消息记录
    ChatMessage message;
    message.senderId = m_currentUser.id;
    message.receiverId = receiverId;
    message.content = content;
    message.messageType = MessageType_Text;
    message.chatType = chatType;
    message.status = MessageStatus_Sending;
    message.timestamp = QDateTime::currentDateTime();

    // 生成消息ID
    QString messageId = Utils::generateMessageId();

    // 准备网络消息
    QJsonObject messageData = chatMessageToJson(message);
    messageData["message_id"] = messageId;

    quint32 requestId = generateRequestId();
    addRequest(requestId, "send_message");

    // 发送消息
    QMetaObject::invokeMethod(m_tcpClient, [this, messageData, requestId]() {
        m_tcpClient->sendMessage(NetMsg_TextMessage, messageData, requestId);
    });

    // 发送信号（表示消息已发送，但未确认）
    emit messageSent(message);

    LOG_DEBUG("NetworkManager", QString("Text message sent to %1: %2").arg(receiverId).arg(content));
}

void NetworkManager::requestFriendList()
{
    if (!m_isLoggedIn) {
        LOG_WARNING("NetworkManager", "Cannot request friend list: user not logged in");
        return;
    }

    QJsonObject requestData;
    requestData["user_id"] = m_currentUser.id;
    requestData["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    quint32 requestId = generateRequestId();
    addRequest(requestId, "friend_list");

    QMetaObject::invokeMethod(m_tcpClient, [this, requestData, requestId]() {
        m_tcpClient->sendMessage(NetMsg_FriendList, requestData, requestId);
    });

    LOG_DEBUG("NetworkManager", "Friend list requested");
}

quint32 NetworkManager::generateRequestId()
{
    QMutexLocker locker(&m_requestMutex);
    return m_nextRequestId++;
}

void NetworkManager::addRequest(quint32 requestId, const QString& requestType)
{
    QMutexLocker locker(&m_requestMutex);

    NetRequest request;
    request.requestId = requestId;
    request.type = requestType;
    request.sendTime = QDateTime::currentDateTime();
    request.data = QJsonObject(); // 可以存储额外数据

    m_pendingRequests[requestId] = request;

    LOG_DEBUG("NetworkManager", QString("Request added: id=%1, type=%2").arg(requestId).arg(requestType));
}

void NetworkManager::removeRequest(quint32 requestId)
{
    QMutexLocker locker(&m_requestMutex);
    m_pendingRequests.remove(requestId);
}

void NetworkManager::onMessageReceived(NetMessageType type, const QJsonObject& data, quint32 requestId)
{
    LOG_DEBUG("NetworkManager", QString("Message received: type=%1, requestId=%2").arg(type).arg(requestId));

    // 处理消息
    handleMessage(type, data, requestId);
}

void NetworkManager::onConnected()
{
    LOG_INFO("NetworkManager", "Connected to server");
    emit connectionChanged(true);
}

void NetworkManager::onDisconnected()
{
    LOG_INFO("NetworkManager", "Disconnected from server");
    m_isLoggedIn = false;
    emit connectionChanged(false);
}

void NetworkManager::onConnectionError(const QString& error)
{
    LOG_ERROR("NetworkManager", QString("Connection error: %1").arg(error));
    emit connectionError(error);
}

void NetworkManager::handleMessage(NetMessageType type, const QJsonObject& data, quint32 requestId)
{
    switch (type) {
    case NetMsg_Response:
        handleResponse(data, requestId);
        break;
    case NetMsg_Login:
        handleLoginResponse(data);
        break;
    case NetMsg_Register:
        handleRegisterResponse(data);
        break;
    case NetMsg_TextMessage:
        handleTextMessage(data);
        break;
    case NetMsg_FriendList:
        handleFriendListResponse(data);
        break;
    case NetMsg_AddFriend:
        handleFriendRequest(data);
        break;
    case NetMsg_UserStatus:
        handleUserStatusUpdate(data);
        break;
    default:
        LOG_WARNING("NetworkManager", QString("Unhandled message type: %1").arg(type));
        break;
    }
}

void NetworkManager::handleResponse(const QJsonObject& data, quint32 requestId)
{
    // 移除请求
    removeRequest(requestId);

    NetResponse response;
    response.requestId = requestId;
    response.code = static_cast<NetResponseCode>(data["code"].toInt());
    response.message = data["message"].toString();
    response.data = data["data"].toObject();

    LOG_DEBUG("NetworkManager", QString("Response received for request %1: %2")
                                    .arg(requestId).arg(response.message));

    emit responseReceived(requestId, response);
}

void NetworkManager::handleLoginResponse(const QJsonObject& data)
{
    int code = data["code"].toInt();
    QString message = data["message"].toString();

    if (code == Response_Success) {
        QJsonObject userData = data["data"].toObject();
        m_currentUser = jsonToUserInfo(userData);
        m_isLoggedIn = true;

        LOG_INFO("NetworkManager", QString("Login successful: %1 (ID: %2)")
                                       .arg(m_currentUser.username).arg(m_currentUser.id));

        emit loginSuccess(m_currentUser);

        // 登录成功后请求好友列表
        requestFriendList();
    } else {
        LOG_ERROR("NetworkManager", QString("Login failed: %1").arg(message));
        emit loginFailed(message);
    }
}

void NetworkManager::handleRegisterResponse(const QJsonObject& data)
{
    int code = data["code"].toInt();
    QString message = data["message"].toString();

    if (code == Response_Success) {
        QJsonObject userData = data["data"].toObject();
        UserInfo userInfo = jsonToUserInfo(userData);

        LOG_INFO("NetworkManager", QString("Register successful: %1 (ID: %2)")
                                       .arg(userInfo.username).arg(userInfo.id));

        emit registerSuccess(userInfo);
    } else {
        LOG_ERROR("NetworkManager", QString("Register failed: %1").arg(message));
        emit registerFailed(message);
    }
}

void NetworkManager::handleTextMessage(const QJsonObject& data)
{
    ChatMessage message = jsonToChatMessage(data);

    LOG_DEBUG("NetworkManager", QString("Text message received from %1: %2")
                                    .arg(message.senderId).arg(message.content));

    emit messageReceived(message);
}

void NetworkManager::handleFriendListResponse(const QJsonObject& data)
{
    int code = data["code"].toInt();

    if (code != Response_Success) {
        LOG_ERROR("NetworkManager", "Failed to get friend list");
        return;
    }

    QList<ContactInfo> friends;
    QJsonArray friendArray = data["data"].toArray();

    for (const QJsonValue& value : friendArray) {
        QJsonObject friendObj = value.toObject();

        ContactInfo friendInfo;
        friendInfo.id = friendObj["id"].toInt();
        friendInfo.name = friendObj["name"].toString();
        friendInfo.group = friendObj["group"].toString("默认分组");
        friendInfo.avatar = friendObj["avatar"].toString();
        friendInfo.status = static_cast<UserStatus>(friendObj["status"].toInt());
        friendInfo.unreadCount = friendObj["unread_count"].toInt();

        if (friendObj.contains("last_message_time")) {
            friendInfo.lastMessageTime = QDateTime::fromMSecsSinceEpoch(friendObj["last_message_time"].toVariant().toLongLong());
        }

        friends.append(friendInfo);
    }

    LOG_INFO("NetworkManager", QString("Friend list received: %1 friends").arg(friends.size()));
    emit friendListReceived(friends);
}

void NetworkManager::handleFriendRequest(const QJsonObject& data)
{
    FriendRequest request;
    request.requestId = data["request_id"].toInt();
    request.fromUserId = data["from_user_id"].toInt();
    request.fromUsername = data["from_username"].toString();
    request.message = data["message"].toString();
    request.requestTime = QDateTime::fromMSecsSinceEpoch(data["timestamp"].toVariant().toLongLong());

    LOG_INFO("NetworkManager", QString("Friend request from %1: %2")
                                   .arg(request.fromUsername).arg(request.message));

    emit friendRequestReceived(request);
}

void NetworkManager::handleUserStatusUpdate(const QJsonObject& data)
{
    UserStatusUpdate update;
    update.userId = data["user_id"].toInt();
    update.status = static_cast<UserStatus>(data["status"].toInt());
    update.updateTime = QDateTime::fromMSecsSinceEpoch(data["timestamp"].toVariant().toLongLong());

    LOG_DEBUG("NetworkManager", QString("User status update: user=%1, status=%2")
                                    .arg(update.userId).arg(update.status));

    emit userStatusUpdated(update);
}

QJsonObject NetworkManager::userInfoToJson(const UserInfo& user)
{
    QJsonObject json;
    json["id"] = user.id;
    json["username"] = user.username;
    json["nickname"] = user.nickname;
    json["avatar"] = user.avatar;
    json["signature"] = user.signature;
    json["status"] = static_cast<int>(user.status);
    json["last_online"] = user.lastOnline.toMSecsSinceEpoch();

    return json;
}

UserInfo NetworkManager::jsonToUserInfo(const QJsonObject& json)
{
    UserInfo user;
    user.id = json["id"].toInt();
    user.username = json["username"].toString();
    user.nickname = json["nickname"].toString();
    user.avatar = json["avatar"].toString();
    user.signature = json["signature"].toString();
    user.status = static_cast<UserStatus>(json["status"].toInt());
    user.lastOnline = QDateTime::fromMSecsSinceEpoch(json["last_online"].toVariant().toLongLong());

    return user;
}

QJsonObject NetworkManager::chatMessageToJson(const ChatMessage& message)
{
    QJsonObject json;
    json["sender_id"] = message.senderId;
    json["receiver_id"] = message.receiverId;
    json["chat_type"] = static_cast<int>(message.chatType);
    json["content"] = message.content;
    json["message_type"] = static_cast<int>(message.messageType);
    json["status"] = static_cast<int>(message.status);
    json["timestamp"] = message.timestamp.toMSecsSinceEpoch();

    return json;
}

ChatMessage NetworkManager::jsonToChatMessage(const QJsonObject& json)
{
    ChatMessage message;
    message.id = json["id"].toInt();
    message.senderId = json["sender_id"].toInt();
    message.receiverId = json["receiver_id"].toInt();
    message.chatType = static_cast<ChatType>(json["chat_type"].toInt());
    message.content = json["content"].toString();
    message.messageType = static_cast<MessageType>(json["message_type"].toInt());
    message.status = static_cast<MessageStatus>(json["status"].toInt());
    message.timestamp = QDateTime::fromMSecsSinceEpoch(json["timestamp"].toVariant().toLongLong());

    return message;
}
