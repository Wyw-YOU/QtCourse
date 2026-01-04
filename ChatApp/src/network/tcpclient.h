// src/network/tcpclient.h
#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include <QQueue>
#include <QMap>
#include "src/utils/global.h"

class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject* parent = nullptr);
    ~TcpClient();

    // 连接服务器
    bool connectToServer(const QString& host, quint16 port);
    void disconnectFromServer();

    // 发送消息
    bool sendMessage(NetMessageType type, const QJsonObject& data, quint32 requestId = 0);
    bool sendMessage(const QByteArray& data);

    // 获取连接状态
    bool isConnected() const;
    QString connectionInfo() const;

    // 设置自动重连
    void setAutoReconnect(bool enable, int interval = 5000);

signals:
    void connected();
    void disconnected();
    void connectionError(const QString& error);
    void messageReceived(NetMessageType type, const QJsonObject& data, quint32 requestId);
    void dataReceived(const QByteArray& data);

public slots:
    void reconnect();

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);
    void onReadyRead();
    void onHeartbeatTimeout();
    void onReconnectTimeout();

private:
    void sendHeartbeat();
    void processIncomingData();
    void handleMessage(const NetMessageHeader& header, const QJsonObject& body);

    QTcpSocket* m_socket;
    QTimer* m_heartbeatTimer;
    QTimer* m_reconnectTimer;

    QString m_host;
    quint16 m_port;
    bool m_autoReconnect;
    int m_reconnectInterval;

    // 数据缓冲区
    QByteArray m_receiveBuffer;
    QMutex m_bufferMutex;

    // 发送队列
    QQueue<QByteArray> m_sendQueue;
    QMutex m_queueMutex;
    bool m_isSending;

    // 心跳计数
    int m_heartbeatCount;
    int m_missedHeartbeats;
    static const int MAX_MISSED_HEARTBEATS = 3;
};

#endif // TCPCLIENT_H
