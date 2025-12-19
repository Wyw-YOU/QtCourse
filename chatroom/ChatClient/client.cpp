#include "client.h"
#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>

Client::Client(QObject *parent) : QObject(parent) {
    m_socket = new QTcpSocket(this);

    // 修复：替换弃用的error信号为errorOccurred（Qt5.15+标准）
    connect(m_socket, &QTcpSocket::connected, this, &Client::connectedToServer);
    connect(m_socket, &QTcpSocket::disconnected, this, &Client::disconnectedFromServer);
    connect(m_socket, &QTcpSocket::readyRead, this, &Client::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &Client::onSocketError); // 核心替换
}

void Client::connectToServer(const QString& ip, quint16 port) {
    m_socket->connectToHost(ip, port);
}

void Client::sendLogin(const QString& username) {
    m_username = username;
    QString loginMsg = Protocol::packLogin(username);
    m_socket->write(loginMsg.toUtf8());
    m_socket->flush();
    qDebug() << "[Client] 发送登录请求：" << username;
}

void Client::sendMessage(const QString& text) {
    if (m_socket->state() != QTcpSocket::ConnectedState) {
        emit socketError("未连接服务器，无法发送消息");
        return;
    }
    QString msg = Protocol::packMessage(m_username, text);
    m_socket->write(msg.toUtf8());
    m_socket->flush();
}

void Client::disconnectFromServer() {
    m_socket->disconnectFromHost();
}

void Client::onReadyRead() {
    while (m_socket->bytesAvailable() > 0) {
        QString jsonStr = m_socket->readAll();
        qDebug() << "[Client] 收到服务器消息：" << jsonStr;

        MsgType type; // 现在能正确识别（因为包含了protocol.h）
        QJsonObject data;
        Protocol::unpackMsg(jsonStr, type, data);

        switch (type) {
        case Message: {
            QString sender = data["sender"].toString();
            QString text = data["text"].toString();
            emit receivedMessage(sender, text);
            break;
        }
        case UserList: {
            QJsonValue listValue = data["userList"];
            QJsonArray arr = listValue.toArray();
            QStringList userList;
            for (int i=0; i<arr.size(); i++) {
                userList.append(arr[i].toString());
            }
            qDebug() << "[Client] 解析到用户列表：" << userList;
            emit userListUpdated(userList);
            break;
        }
        case Error: {
            QString reason = data["text"].toString();
            emit loginFailed(reason);
            break;
        }
        case NewUser: {
            QString username = data["username"].toString();
            qDebug() << "[Client] 新用户上线：" << username;
            break;
        }
        case Login:
            break;
        default:
            qDebug() << "[Client] 未知消息类型：" << type;
            break;
        }
    }
}

void Client::onSocketError(QAbstractSocket::SocketError err) {
    Q_UNUSED(err);
    emit socketError(m_socket->errorString());
}
