#ifndef CHATMANAGER_H
#define CHATMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QList>

// 单条聊天消息
struct ChatMessage {
    QString messageId;        // 消息ID
    QString sender;           // 发送者
    QString receiver;         // 接收者（用户或群）
    int contentType;          // 消息类型：文本、图片、文件等
    QString content;          // 消息内容
    QDateTime timestamp;      // 时间戳
    int status;               // 状态：发送中、已发送、已读等
    QString filePath;         // 文件路径（如果是文件消息）
    qint64 fileSize;          // 文件大小
};

// 聊天会话
struct ChatSession {
    QString sessionId;        // 会话ID（对方账号或群ID）
    int sessionType;          // 会话类型：单聊、群聊
    QString lastMessage;      // 最后一条消息
    QDateTime lastTime;       // 最后消息时间
    int unreadCount;          // 未读消息数
};

class ChatManager : public QObject
{
    Q_OBJECT
public:
    static ChatManager* instance();

signals:

private:
    explicit ChatManager(QObject *parent = nullptr);
    QList<ChatMessage> m_messageCache;
    QList<ChatSession> m_sessions;
};

#endif // CHATMANAGER_H
