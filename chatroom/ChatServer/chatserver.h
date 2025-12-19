#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QStringList>
#include "protocol.h"

class ChatServer : public QObject
{
    Q_OBJECT
private:
    explicit ChatServer(QObject *parent = nullptr);
    static ChatServer* instance;
    QTcpServer* m_server;
    QMap<QString, QTcpSocket*> m_clients; // 用户名->socket

public: // 关键：把需要外部调用的函数改为public
    static ChatServer* getInstance();
    void startServer(quint16 port);
    void stopServer();
    void disconnectAllClients(); // 补全声明（解决“无此成员”报错）
    void addClient(const QString& username, QTcpSocket* socket); // 改为public
    void removeClient(const QString& username); // 新增重载（解决调用不匹配）
    void removeClient(QTcpSocket* socket); // 保留原有版本
    void broadcast(const QString& msg); // 改为public
    void syncUserList();
    QStringList getUserList() const;

private slots:
    void onNewConnection();
    void onReadyRead();
    void onDisconnected();
};

#endif // CHATSERVER_H
