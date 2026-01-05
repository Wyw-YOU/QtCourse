#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>
#include "Protocol.h"
#include <QDateTime>
#include <QLabel>
#include <QDebug>
#include "business.h"
#include "datamanager.h"

namespace Ui {
class friendItem;
}

// 聊天会话
struct ChatSession {
    QString sessionId;        // 会话ID（对方账号或群ID）
    QString sender;
    QString name;             // 昵称
    int sessionType;          // 会话类型：单聊、群聊
    QString lastMessage;      // 最后一条消息
    int lastTime;       // 最后消息时间
    int unreadCount;          // 未读消息数
};

class friendItem : public QWidget
{
    Q_OBJECT


public:
    explicit friendItem(QWidget *parent = nullptr);
    ~friendItem();

    void setHeadImage(USER_INFO &friend_info);
    void setfriendlist_item(USER_INFO &friend_info);
    void setSession_item();
    void setgrouplist_item(GROUP_INFO &group_info);
    void setHeadImage(GROUP_INFO &group_info);
    void set_users_in_group_list_item(USER_INFO &friend_info);

    USER_INFO m_friend_info;
    GROUP_INFO m_group_info;
    ChatSession m_session_info;

private:
    Ui::friendItem *ui;
    DataManager& m_dataManager;
};

#endif // FRIENDITEM_H
