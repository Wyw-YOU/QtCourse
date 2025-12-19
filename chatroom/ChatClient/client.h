#ifndef CLIENT_H
#define CLIENT_H

// 核心修复：删除枚举前向声明，直接包含protocol.h（MsgType定义在这）
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include "protocol.h" // 必须包含，因为MsgType定义在此

class Client : public QObject // 继承QObject才能用信号槽
{
    Q_OBJECT // 必须加此宏，否则emit/signal失效

public:
    explicit Client(QObject *parent = nullptr);

    // 公共接口
    void connectToServer(const QString& ip, quint16 port);
    void sendLogin(const QString& username);
    void sendMessage(const QString& text);
    void disconnectFromServer();

signals:
    void connectedToServer();          // 连接成功
    void disconnectedFromServer();     // 断开连接
    void socketError(QString err);     // 连接错误
    void receivedMessage(QString sender, QString text); // 收到聊天消息
    void userListUpdated(QStringList userList); // 用户列表更新
    void loginFailed(QString reason);  // 登录失败

private slots:
    void onReadyRead();                // 接收服务器消息
    void onSocketError(QAbstractSocket::SocketError err); // 错误处理

private:
    QTcpSocket* m_socket;    // TCP套接字（client.cpp中初始化）
    QString m_username;      // 登录用户名
};

#endif // CLIENT_H
