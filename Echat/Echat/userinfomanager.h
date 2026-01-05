#ifndef USERINFOMANAGER_H
#define USERINFOMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QList>

// 用户基本信息
struct UserBasicInfo {
    QString account;          // 账号
    QString nickname;         // 昵称
//    QString avatar;           // 头像路径或URL
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

class UserInfoManager : public QObject
{
    Q_OBJECT
private:
    explicit UserInfoManager(QObject *parent = nullptr);

public:
    UserInfoManager(const UserInfoManager&) = delete;
    UserInfoManager& operator=(const UserInfoManager&) = delete;

    static UserInfoManager* getInstance()
    {
        static UserInfoManager instance;
        return &instance;
    }

signals:

private:
    UserBasicInfo m_currentUser;
    QList<FriendInfo> m_friends;
    QList<GroupInfo> m_groups;
};

#endif // USERINFOMANAGER_H
