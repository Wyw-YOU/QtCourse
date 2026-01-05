#ifndef NOTICEITEM_H
#define NOTICEITEM_H

#include <QWidget>
#include "Protocol.h"
#include <QString>
#include <QDebug>
#include "datamanager.h"

namespace Ui {
class noticeitem;
}

class noticeitem : public QWidget
{
    Q_OBJECT

public:
    explicit noticeitem(QWidget *parent = nullptr);
    ~noticeitem();
    void setHeadImage(FRIEND_ADD_ASK &ask, QString user_account);
    void set_notice_text(FRIEND_ADD_ASK &ask, QString user_account);
    void update_text(int choice);
    void set_group_notice(GROUP_ADD_ASK &ask, QString user_account);

    FRIEND_ADD_ASK m_friend_add_ask_info;
    GROUP_ADD_ASK m_group_add_ask_info;;

    int m_flag = 0; //默认为好友通知

private slots:
    void on_acceptBtn_clicked();

    void on_rejectBtn_clicked();

private:
    Ui::noticeitem *ui;
    DataManager& m_dataManager;
};

#endif // NOTICEITEM_H
