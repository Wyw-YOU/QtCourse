// src/network/tcpclient.cpp
#include "tcpclient.h"
#include "src/utils/logger.h"
#include <QHostAddress>
#include <QThread>

TcpClient::TcpClient(QObject* parent)
    : QObject(parent)
    , m_socket(nullptr)
    , m_heartbeatTimer(nullptr)
    , m_reconnectTimer(nullptr)
    , m_port(0)
    , m_autoReconnect(true)
    , m_reconnectInterval(5000)
    , m_isSending(false)
    , m_heartbeatCount(0)
    , m_missedHeartbeats(0)
{
    // 创建socket
    m_socket = new QTcpSocket(this);
    m_socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
    m_socket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

    // 连接信号槽
    connect(m_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred),
            this, &TcpClient::onError);
    connect(m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);

    // 创建心跳定时器
    m_heartbeatTimer = new QTimer(this);
    m_heartbeatTimer->setInterval(HEARTBEAT_INTERVAL);
    connect(m_heartbeatTimer, &QTimer::timeout, this, &TcpClient::onHeartbeatTimeout);

    // 创建重连定时器
    m_reconnectTimer = new QTimer(this);
    m_reconnectTimer->setSingleShot(true);
    connect(m_reconnectTimer, &QTimer::timeout, this, &TcpClient::reconnect);
}

TcpClient::~TcpClient()
{
    disconnectFromServer();
}

bool TcpClient::connectToServer(const QString& host, quint16 port)
{
    if (isConnected()) {
        LOG_WARNING("TcpClient", "Already connected to server");
        return true;
    }

    m_host = host;
    m_port = port;

    LOG_INFO("TcpClient", QString("Connecting to server %1:%2").arg(host).arg(port));

    // 清空缓冲区
    m_receiveBuffer.clear();

    // 连接服务器
    m_socket->connectToHost(host, port);

    // 等待连接（最多5秒）
    if (m_socket->waitForConnected(5000)) {
        LOG_INFO("TcpClient", "Connected to server successfully");
        return true;
    } else {
        QString error = m_socket->errorString();
        LOG_ERROR("TcpClient", QString("Failed to connect to server: %1").arg(error));
        emit connectionError(error);

        // 如果启用自动重连，则启动重连定时器
        if (m_autoReconnect) {
            m_reconnectTimer->start(m_reconnectInterval);
        }

        return false;
    }
}

void TcpClient::disconnectFromServer()
{
    LOG_INFO("TcpClient", "Disconnecting from server");

    // 停止定时器
    m_heartbeatTimer->stop();
    m_reconnectTimer->stop();

    // 断开连接
    if (m_socket->state() != QAbstractSocket::UnconnectedState) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState) {
            m_socket->waitForDisconnected(1000);
        }
    }

    // 清空队列
    {
        QMutexLocker locker(&m_queueMutex);
        m_sendQueue.clear();
        m_isSending = false;
    }

    m_heartbeatCount = 0;
    m_missedHeartbeats = 0;
}

bool TcpClient::sendMessage(NetMessageType type, const QJsonObject& data, quint32 requestId)
{
    QByteArray message = Utils::encodeMessage(type, data, requestId);
    return sendMessage(message);
}

bool TcpClient::sendMessage(const QByteArray& data)
{
    if (!isConnected()) {
        LOG_WARNING("TcpClient", "Cannot send message: not connected");
        return false;
    }

    {
        QMutexLocker locker(&m_queueMutex);
        m_sendQueue.enqueue(data);

        // 如果没有正在发送，则开始发送
        if (!m_isSending && !m_sendQueue.isEmpty()) {
            QByteArray dataToSend = m_sendQueue.dequeue();
            qint64 bytesWritten = m_socket->write(dataToSend);

            if (bytesWritten == -1) {
                LOG_ERROR("TcpClient", "Failed to write to socket");
                return false;
            }

            if (bytesWritten < dataToSend.size()) {
                // 没有完全写入，将剩余部分放回队列
                QByteArray remaining = dataToSend.mid(bytesWritten);
                m_sendQueue.prepend(remaining);
            }

            m_isSending = true;
        }
    }

    return true;
}

bool TcpClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

QString TcpClient::connectionInfo() const
{
    if (isConnected()) {
        return QString("%1:%2").arg(m_socket->peerAddress().toString()).arg(m_socket->peerPort());
    }
    return "Not connected";
}

void TcpClient::setAutoReconnect(bool enable, int interval)
{
    m_autoReconnect = enable;
    m_reconnectInterval = interval;
}

void TcpClient::reconnect()
{
    if (!m_autoReconnect || isConnected()) {
        return;
    }

    LOG_INFO("TcpClient", "Attempting to reconnect...");
    connectToServer(m_host, m_port);
}

void TcpClient::onConnected()
{
    LOG_INFO("TcpClient", "Socket connected");

    // 启动心跳定时器
    m_heartbeatTimer->start();
    m_heartbeatCount = 0;
    m_missedHeartbeats = 0;

    // 停止重连定时器
    m_reconnectTimer->stop();

    emit connected();
}

void TcpClient::onDisconnected()
{
    LOG_INFO("TcpClient", "Socket disconnected");

    // 停止心跳定时器
    m_heartbeatTimer->stop();

    // 如果启用自动重连，则启动重连定时器
    if (m_autoReconnect) {
        LOG_INFO("TcpClient", QString("Will reconnect in %1 ms").arg(m_reconnectInterval));
        m_reconnectTimer->start(m_reconnectInterval);
    }

    emit disconnected();
}

void TcpClient::onError(QAbstractSocket::SocketError error)
{
    QString errorString = m_socket->errorString();
    LOG_ERROR("TcpClient", QString("Socket error %1: %2").arg(error).arg(errorString));
    emit connectionError(errorString);
}

void TcpClient::onReadyRead()
{
    QMutexLocker locker(&m_bufferMutex);

    // 读取所有可用数据
    QByteArray newData = m_socket->readAll();
    m_receiveBuffer.append(newData);

    LOG_DEBUG("TcpClient", QString("Received %1 bytes, total buffer: %2 bytes")
                               .arg(newData.size()).arg(m_receiveBuffer.size()));

    // 处理接收到的数据
    processIncomingData();
}

void TcpClient::onHeartbeatTimeout()
{
    m_heartbeatCount++;
    m_missedHeartbeats++;

    if (m_missedHeartbeats > MAX_MISSED_HEARTBEATS) {
        LOG_WARNING("TcpClient", QString("Missed %1 heartbeats, connection may be lost")
                        .arg(m_missedHeartbeats));
        // 如果连续错过多次心跳，则断开连接
        if (m_missedHeartbeats > MAX_MISSED_HEARTBEATS * 2) {
            LOG_ERROR("TcpClient", "Too many missed heartbeats, disconnecting");
            disconnectFromServer();
            return;
        }
    }

    // 发送心跳
    sendHeartbeat();
}

void TcpClient::onReconnectTimeout()
{
    reconnect();
}

void TcpClient::sendHeartbeat()
{
    QJsonObject heartbeatData;
    heartbeatData["count"] = m_heartbeatCount;
    heartbeatData["timestamp"] = QDateTime::currentMSecsSinceEpoch();

    sendMessage(NetMsg_Heartbeat, heartbeatData);
    LOG_DEBUG("TcpClient", QString("Sent heartbeat #%1").arg(m_heartbeatCount));
}

void TcpClient::processIncomingData()
{
    // 持续处理，直到没有完整的数据包
    while (m_receiveBuffer.size() >= static_cast<int>(sizeof(NetMessageHeader))) {
        // 解析消息头
        NetMessageHeader header = NetMessageHeader::fromByteArray(m_receiveBuffer.left(sizeof(NetMessageHeader)));

        // 检查是否有完整的消息体
        quint32 totalLength = sizeof(NetMessageHeader) + header.bodyLength;
        if (m_receiveBuffer.size() < static_cast<int>(totalLength)) {
            // 数据不完整，等待更多数据
            break;
        }

        // 提取完整的消息
        QByteArray messageData = m_receiveBuffer.left(totalLength);
        m_receiveBuffer.remove(0, totalLength);

        // 解码消息
        auto decoded = Utils::decodeMessage(messageData);

        // 处理消息
        handleMessage(decoded.first, decoded.second);
    }
}

void TcpClient::handleMessage(const NetMessageHeader& header, const QJsonObject& body)
{
    NetMessageType type = static_cast<NetMessageType>(header.messageType);

    LOG_DEBUG("TcpClient", QString("Received message: type=%1, length=%2, requestId=%3")
                               .arg(type).arg(header.bodyLength).arg(header.requestId));

    // 如果是心跳响应，重置心跳计数
    if (type == NetMsg_Heartbeat) {
        m_missedHeartbeats = 0;
        LOG_DEBUG("TcpClient", "Received heartbeat response");
        return;
    }

    // 如果是其他消息，发送信号
    emit messageReceived(type, body, header.requestId);

    // 同时发送原始数据信号
    QByteArray fullMessage;
    fullMessage.append(header.toByteArray());
    fullMessage.append(Utils::jsonToByteArray(body));
    emit dataReceived(fullMessage);
}
