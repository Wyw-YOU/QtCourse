// src/network/networkmanager.h
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMap>
#include <QMutex>
#include "tcpclient.h"
#include "src/utils/global.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    static NetworkManager* instance();

    // 连接管理
    bool connectToServer(const QString& host = SERVER_IP, quint16 port = SERVER_PORT);
    void disconnectFromServer();
    bool isConnected() const;

    // 用户认证
    void login(const QString& username, const QString& password);
    void registerUser(const UserInfo& userInfo, const QString& password);
    void logout();

    // 消息发送
    void sendTextMessage(int receiverId, const QString& content, ChatType chatType = ChatType_Single);
    void sendFileMessage(int receiverId, const QString& filePath, ChatType chatType = ChatType_Single);

    // 好友管理
    void requestFriendList();
    void addFriend(int friendId, const QString& message = "");
    void acceptFriendRequest(int requestId);
    void rejectFriendRequest(int requestId);

    // 群组管理
    void requestGroupList();
    void createGroup(const QString& groupName, const QList<int>& memberIds);
    void joinGroup(int groupId);

    // 用户状态
    void updateUserStatus(UserStatus status);

    // 请求管理
    quint32 generateRequestId();
    void addRequest(quint32 requestId, const QString& requestType);
    void removeRequest(quint32 requestId);

signals:
    // 连接状态
    void connectionChanged(bool connected);
    void connectionError(const QString& error);

    // 用户认证
    void loginSuccess(const UserInfo& userInfo);
    void loginFailed(const QString& error);
    void registerSuccess(const UserInfo& userInfo);
    void registerFailed(const QString& error);

    // 消息相关
    void messageReceived(const ChatMessage& message);
    void messageSent(const ChatMessage& message);
    void messageSendFailed(int messageId, const QString& error);

    // 好友相关
    void friendListReceived(const QList<ContactInfo>& friends);
    void friendRequestReceived(const FriendRequest& request);
    void friendAdded(const ContactInfo& friendInfo);
    void friendRemoved(int friendId);

    // 群组相关
    void groupListReceived(const QList<ContactInfo>& groups);
    void groupCreated(const ContactInfo& groupInfo);
    void groupMessageReceived(const ChatMessage& message);

    // 用户状态
    void userStatusUpdated(const UserStatusUpdate& update);

    // 网络响应
    void responseReceived(quint32 requestId, const NetResponse& response);

private:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager();

    // 消息处理
    void handleMessage(NetMessageType type, const QJsonObject& data, quint32 requestId);
    void handleLoginResponse(const QJsonObject& data);
    void handleRegisterResponse(const QJsonObject& data);
    void handleTextMessage(const QJsonObject& data);
    void handleFriendListResponse(const QJsonObject& data);
    void handleFriendRequest(const QJsonObject& data);

    // JSON转换
    QJsonObject userInfoToJson(const UserInfo& user);
    UserInfo jsonToUserInfo(const QJsonObject& json);
    QJsonObject chatMessageToJson(const ChatMessage& message);
    ChatMessage jsonToChatMessage(const QJsonObject& json);

    static NetworkManager* m_instance;
    TcpClient* m_tcpClient;
    QThread* m_networkThread;

    // 请求管理
    QMap<quint32, NetRequest> m_pendingRequests;
    QMutex m_requestMutex;
    quint32 m_nextRequestId;

    // 当前用户信息
    UserInfo m_currentUser;
    bool m_isLoggedIn;

    // 自动重连
    bool m_autoReconnect;

private slots:
    void onMessageReceived(NetMessageType type, const QJsonObject& data, quint32 requestId);
    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString& error);
};

#endif // NETWORKMANAGER_H
