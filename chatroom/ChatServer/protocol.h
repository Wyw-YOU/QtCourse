#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

// 补全NewUser枚举（解决“未声明”报错）
enum MsgType {
    Login = 1,        // 登录
    Message = 2,      // 聊天消息
    UserList = 3,     // 用户列表
    NewUser = 4,      // 新用户上线（补全！）
    Error = 5         // 错误
};

class Protocol
{
public:
    static QString packLogin(const QString& username) {
        QJsonObject obj;
        obj["type"] = Login;
        obj["username"] = username;
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }

    static QString packMessage(const QString& sender, const QString& text) {
        QJsonObject obj;
        obj["type"] = Message;
        obj["sender"] = sender;
        obj["text"] = text;
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }

    static QString packUserList(const QStringList& userList) {
        QJsonObject obj;
        obj["type"] = UserList;
        obj["userList"] = QJsonArray::fromStringList(userList);
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }

    // 补全NewUser消息打包（可选，客户端用）
    static QString packNewUser(const QString& username) {
        QJsonObject obj;
        obj["type"] = NewUser;
        obj["username"] = username;
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }

    static QString packError(const QString& text) {
        QJsonObject obj;
        obj["type"] = Error;
        obj["text"] = text;
        return QJsonDocument(obj).toJson(QJsonDocument::Compact);
    }

    static void unpackMsg(const QString& jsonStr, MsgType& type, QJsonObject& data) {
        QJsonDocument doc = QJsonDocument::fromJson(jsonStr.toUtf8());
        if (doc.isObject()) {
            data = doc.object();
            type = (MsgType)data["type"].toInt();
        }
    }
};

#endif // PROTOCOL_H
