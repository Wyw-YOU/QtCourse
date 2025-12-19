#include "chatserver.h"
#include <QDebug>

ChatServer* ChatServer::instance = nullptr;

ChatServer::ChatServer(QObject *parent) : QObject(parent) {
    m_server = new QTcpServer(this);
    connect(m_server, &QTcpServer::newConnection, this, &ChatServer::onNewConnection);
}

ChatServer* ChatServer::getInstance() {
    if (!instance) {
        instance = new ChatServer();
    }
    return instance;
}

void ChatServer::startServer(quint16 port) {
    if (m_server->listen(QHostAddress::Any, port)) {
        qDebug() << "服务器启动，监听端口：" << port;
    } else {
        qDebug() << "服务器启动失败：" << m_server->errorString();
    }
}

void ChatServer::stopServer() {
    m_server->close();
    m_clients.clear();
    qDebug() << "服务器停止";
}

// 补全disconnectAllClients实现（解决“无此成员”报错）
void ChatServer::disconnectAllClients() {
    for (QTcpSocket* socket : m_clients.values()) {
        if (socket->state() == QTcpSocket::ConnectedState) {
            socket->disconnectFromHost();
        }
    }
    m_clients.clear();
}

void ChatServer::onNewConnection() {
    QTcpSocket* socket = m_server->nextPendingConnection();
    if (!socket) return;

    qDebug() << "客户端连接：" << socket->peerAddress().toString();
    connect(socket, &QTcpSocket::readyRead, this, &ChatServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ChatServer::onDisconnected);
}

void ChatServer::onReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString jsonStr = socket->readAll();
    qDebug() << "收到消息：" << jsonStr;

    MsgType type;
    QJsonObject data;
    Protocol::unpackMsg(jsonStr, type, data);

    if (type == Login) {
        QString username = data["username"].toString();
        if (m_clients.contains(username)) {
            socket->write(Protocol::packError("昵称已被占用").toUtf8());
            socket->flush();
        } else {
            addClient(username, socket);
        }
    } else if (type == Message) {
        broadcast(jsonStr);
    }
}

void ChatServer::onDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    qDebug() << "客户端断开：" << socket->peerAddress().toString();
    removeClient(socket);
    socket->deleteLater();
}

void ChatServer::broadcast(const QString& msg) {
    qDebug() << "广播消息：" << msg;
    for (QTcpSocket* socket : m_clients.values()) {
        if (socket->state() == QTcpSocket::ConnectedState) {
            socket->write(msg.toUtf8());
            socket->flush();
        }
    }
}

void ChatServer::syncUserList() {
    QStringList userList = getUserList();
    QString listMsg = Protocol::packUserList(userList);
    broadcast(listMsg);
    qDebug() << "同步用户列表：" << userList;
}

void ChatServer::addClient(const QString& username, QTcpSocket* socket) {
    m_clients.insert(username, socket);
    qDebug() << "用户上线：" << username;
    syncUserList();
}

// 新增重载：按用户名移除（解决clienthandler调用不匹配）
void ChatServer::removeClient(const QString& username) {
    if (m_clients.contains(username)) {
        QTcpSocket* socket = m_clients[username];
        m_clients.remove(username);
        qDebug() << "用户下线：" << username;
        syncUserList();
        socket->disconnectFromHost();
    }
}

// 保留原有版本：按socket移除
void ChatServer::removeClient(QTcpSocket* socket) {
    QString username;
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
        if (it.value() == socket) {
            username = it.key();
            break;
        }
    }
    if (!username.isEmpty()) {
        removeClient(username); // 调用上面的重载
    }
}

QStringList ChatServer::getUserList() const {
    return m_clients.keys();
}
