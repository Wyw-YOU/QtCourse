#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include "Protocol.h"
#include <cstring>
#include <QTcpSocket>
#include <QPixmap>

class NetworkManager;

// 用户基本信息
struct UserBasicInfo {
    QString account;          // 账号
    QString nickname;         // 昵称
    QByteArray avatarData;           // 头像路径或URL
    QPixmap avatarPixmap;
//    QString signature;        // 个性签名
//    int gender;               // 性别
//    QDateTime birthday;       // 生日
//    QString email;            // 邮箱
    int onlineStatus;         // 在线状态
//    QDateTime lastLoginTime;  // 最后登录时间
};

// 好友信息
struct FriendInfo {
    QString account;          // 好友账号
    QString nickname;         // 好友昵称
//    QString remarkName;       // 备注名
//    QString avatar;           // 头像
//    QString signature;        // 签名
    int onlineStatus;         // 在线状态
//    QString group;            // 所属分组
//    QDateTime addTime;        // 添加时间
};

// 群组信息
struct GroupInfo {
    QString groupId;          // 群ID
    QString groupName;        // 群名称
//    QString avatar;           // 群头像
    QString owner;            // 群主
    int memberCount;          // 成员数量
//    int maxMembers;           // 最大成员数
//    QString notice;           // 群公告
//    QDateTime createTime;     // 创建时间
};

// 单条聊天消息
struct ChatMessage {
    long long messageId;        // 消息ID
    QString sender;           // 发送者
    QString receiver;         // 接收者（用户或群）
    int contentType;          // 消息类型：文本、图片、文件等
    QString content;          // 消息内容
    int timestamp;            // 时间戳
    int status;               // 状态：未读0、已读1
    QString filePath;         // 文件路径（如果是文件消息）
    int fileSize;             // 文件大小
//    bool need_send = false;   // 是否需要发送
};

//// 聊天会话
//struct ChatSession {
//    QString sessionId;        // 会话ID（对方账号或群ID）
//    int sessionType;          // 会话类型：单聊、群聊
//    QString lastMessage;      // 最后一条消息
//    QDateTime lastTime;       // 最后消息时间
//    int unreadCount;          // 未读消息数
//};


class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager& instance() {
        static DataManager instance;
        return instance;
    }

    void setCurrentUser(const UserBasicInfo& user);
    UserBasicInfo getCurrentUser() const;

    void setCurrentfriend_add_asks(FRIEND_ADD_ASK asks[], int count);
    QList<FRIEND_ADD_ASK> getCurrentfriend_add_asks() const;
    void update_friend_add_asks(FRIEND_ADD_ASK &ask);

    void setCurrent_friendlist(USER_INFO friends[], int count);
    QList<USER_INFO> getCurrentfriend_friendlist() const;

    void set_networkmanager(NetworkManager* networkmanager);
    void update_friendlist(USER_INFO &friend_info);

    void setCurrentgroup_add_asks(GROUP_ADD_ASK asks[], int count);
    QList<GROUP_ADD_ASK> getCurrentgroup_add_asks() const;
    void update_group_add_asks(GROUP_ADD_ASK &ask);

    void setCurrent_grouplist(GROUP_INFO groups[], int count);
    QList<GROUP_INFO> getCurrent_grouplist() const;

    void update_grouplist(GROUP_INFO &group_info);
    void update_users_in_group_list(GROUP_MEMBER_QUERY_RESPONSE_MSG* msg);

    NetworkManager* m_networkmanager;
    FRIEND_ADD_ASK *m_friend_add_ask = NULL;
    QList<USER_INFO> m_friends;
    QList<GROUP_INFO> m_groups;
    // 存储群成员信息：群号 -> 成员列表
    QMap<QString, QVector<USER_INFO>> m_groupMembers;

signals:
    void currentUserChanged();

private:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    UserBasicInfo m_currentUser;
    QList<ChatMessage> m_messageCache;
//    QList<ChatSession> m_sessions;
    QList<FRIEND_ADD_ASK> m_friend_add_asks;
    QList<GROUP_ADD_ASK> m_group_add_asks;

};

#endif // DATAMANAGER_H
