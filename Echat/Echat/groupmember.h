#ifndef GROUPMEMBER_H
#define GROUPMEMBER_H

#include <QWidget>
#include "Protocol.h"
#include "business.h"

namespace Ui {
class GroupMember;
}

class GroupMember : public QWidget
{
    Q_OBJECT

public:
    explicit GroupMember(QWidget *parent = nullptr);
    ~GroupMember();
    void set_users_in_group_list_item(USER_INFO &friend_info);
    void set_users_in_group_list_item(USER_INFO &friend_info, QPixmap &avatarPixmap);
    USER_INFO m_friend_info;

private:
    Ui::GroupMember *ui;
};

#endif // GROUPMEMBER_H
