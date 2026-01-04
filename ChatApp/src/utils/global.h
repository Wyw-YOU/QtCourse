// src/utils/global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore>

// 消息类型枚举
enum MessageType {
    MessageType_Text = 0,      // 文本消息
    MessageType_Image = 1,     // 图片消息
    MessageType_File = 2,      // 文件消息
    MessageType_System = 3     // 系统消息
};

// 消息状态枚举
enum MessageStatus {
    MessageStatus_Sending = 0,  // 发送中
    MessageStatus_Sent = 1,     // 已发送
    MessageStatus_Received = 2, // 已接收
    MessageStatus_Read = 3      // 已读
};

// 用户状态枚举
enum UserStatus {
    UserStatus_Offline = 0,    // 离线
    UserStatus_Online = 1,     // 在线
    UserStatus_Busy = 2,       // 忙碌
    UserStatus_Away = 3        // 离开
};

// 聊天类型枚举
enum ChatType {
    ChatType_Single = 0,       // 单聊
    ChatType_Group = 1         // 群聊
};

// 网络命令枚举
enum NetCommand {
    Cmd_Login = 0x01,          // 登录
    Cmd_Register = 0x02,       // 注册
    Cmd_Message = 0x03,        // 发送消息
    Cmd_Heartbeat = 0x04,      // 心跳
    Cmd_FriendList = 0x05,     // 获取好友列表
    Cmd_GroupList = 0x06,      // 获取群组列表
    Cmd_AddFriend = 0x07,      // 添加好友
    Cmd_CreateGroup = 0x08     // 创建群组
};

// 服务器配置
const QString SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8888;
const int HEARTBEAT_INTERVAL = 30000; // 心跳间隔30秒

// 数据库配置
const QString DATABASE_NAME = "chat.db";

// 应用配置
const QString APP_NAME = "ChatApp";
const QString APP_VERSION = "1.0.0";
const QString COMPANY_NAME = "QtCourse";

// 全局类型定义
struct UserInfo {
    int id;
    QString username;
    QString nickname;
    QString avatar;
    QString signature;
    UserStatus status;
    QDateTime lastOnline;

    UserInfo() : id(-1), status(UserStatus_Offline) {}
};

struct ContactInfo {
    int id;
    QString name;
    QString group;
    QString avatar;
    UserStatus status;
    int unreadCount;
    QDateTime lastMessageTime;

    ContactInfo() : id(-1), unreadCount(0), status(UserStatus_Offline) {}
};

struct ChatMessage {
    int id;
    int senderId;
    int receiverId;
    ChatType chatType;
    QString content;
    MessageType messageType;
    MessageStatus status;
    QDateTime timestamp;

    ChatMessage() : id(-1), senderId(-1), receiverId(-1),
        chatType(ChatType_Single), messageType(MessageType_Text),
        status(MessageStatus_Sending) {}
};

// 网络消息类型枚举
enum NetMessageType {
    NetMsg_Invalid = 0,
    NetMsg_Login = 1,          // 登录
    NetMsg_Register = 2,       // 注册
    NetMsg_Logout = 3,         // 登出
    NetMsg_TextMessage = 4,    // 文本消息
    NetMsg_FileMessage = 5,    // 文件消息
    NetMsg_Heartbeat = 6,      // 心跳
    NetMsg_FriendList = 7,     // 好友列表
    NetMsg_GroupList = 8,      // 群组列表
    NetMsg_AddFriend = 9,      // 添加好友
    NetMsg_UserStatus = 10,    // 用户状态更新
    NetMsg_Response = 99       // 响应消息
};

// 网络响应状态码
enum NetResponseCode {
    Response_Success = 200,
    Response_BadRequest = 400,
    Response_Unauthorized = 401,
    Response_NotFound = 404,
    Response_ServerError = 500
};

// 网络消息头
struct NetMessageHeader {
    quint32 messageType;     // 消息类型
    quint32 bodyLength;      // 消息体长度
    quint64 timestamp;       // 时间戳
    quint32 requestId;       // 请求ID（用于匹配请求响应）

    NetMessageHeader()
        : messageType(0), bodyLength(0), timestamp(0), requestId(0) {}

    QByteArray toByteArray() const {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream << messageType << bodyLength << timestamp << requestId;
        return data;
    }

    static NetMessageHeader fromByteArray(const QByteArray& data) {
        NetMessageHeader header;
        if (data.size() < static_cast<int>(sizeof(NetMessageHeader))) {
            return header;
        }

        QDataStream stream(data);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> header.messageType >> header.bodyLength
            >> header.timestamp >> header.requestId;
        return header;
    }
};

// 网络请求结构
struct NetRequest {
    int requestId;
    QDateTime sendTime;
    QString type;
    QJsonObject data;

    NetRequest() : requestId(0) {}
};

// 网络响应结构
struct NetResponse {
    int requestId;
    NetResponseCode code;
    QString message;
    QJsonObject data;

    NetResponse() : requestId(0), code(Response_Success) {}
};

// 用户在线状态变更通知
struct UserStatusUpdate {
    int userId;
    UserStatus status;
    QDateTime updateTime;
};

// 好友请求
struct FriendRequest {
    int requestId;
    int fromUserId;
    QString fromUsername;
    QString message;
    QDateTime requestTime;
};

// 全局工具函数声明
namespace Utils {
QString getCurrentTimeString();
QString formatFileSize(qint64 bytes);
QString getDataPath();
QString getAvatarPath(int userId);

// 网络相关工具函数
QByteArray jsonToByteArray(const QJsonObject& json);
QJsonObject byteArrayToJson(const QByteArray& data);
QString generateMessageId();
QString generateRequestId();

// 消息编解码
QByteArray encodeMessage(NetMessageType type, const QJsonObject& data, quint32 requestId = 0);
QPair<NetMessageHeader, QJsonObject> decodeMessage(const QByteArray& data);
}

#endif // GLOBAL_H
