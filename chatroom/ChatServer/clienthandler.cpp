#include "clienthandler.h"
#include <QDebug> // 补全头文件

void ClientHandler::run() {
    QTcpSocket socket;
    if (!socket.setSocketDescriptor(m_socketDescriptor)) {
        emit log("客户端连接失败：" + socket.errorString());
        return;
    }

    emit log("新客户端连接：" + socket.peerAddress().toString());
    QString username;

    while (socket.waitForReadyRead()) {
        QString jsonStr = socket.readAll();
        MsgType type;
        QJsonObject data;
        Protocol::unpackMsg(jsonStr, type, data);

        switch (type) {
        case Login:
            username = data["username"].toString();
            if (ChatServer::getInstance()->getUserList().contains(username)) {
                socket.write(Protocol::packError("用户名已存在").toUtf8());
                socket.disconnectFromHost();
                return;
            }
            // 修复：addClient已改为public，可正常调用
            ChatServer::getInstance()->addClient(username, &socket);
            emit log("用户登录：" + username);
            break;
        case Message: {
            QString sender = data["sender"].toString();
            QString text = data["text"].toString();
            // 修复：broadcast已改为public，可正常调用
            ChatServer::getInstance()->broadcast(Protocol::packMessage(sender, text));
            emit log("收到消息：" + sender + " -> " + text);
            break;
        }
        case NewUser: // 补全case（解决枚举未处理警告）
        case UserList:
        case Error:
            break;
        }
    }

    if (!username.isEmpty()) {
        // 修复：调用重载的removeClient(QString)，匹配参数
        ChatServer::getInstance()->removeClient(username);
        emit log("用户下线：" + username);
    }
    socket.disconnectFromHost();
}
