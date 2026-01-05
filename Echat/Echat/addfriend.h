#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QWidget>
#include "datamanager.h"
#include "business.h"
#include <QDateTime>

namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget
{
    Q_OBJECT

public:
    explicit AddFriend(QWidget *parent = nullptr);
    ~AddFriend();

    void label_clear();
    void set_label(USER_INFO &user_info);
    void set_label(GROUP_INFO &group_info);
    int m_flag = 0; //好友

private slots:
    void on_searchBtn_clicked();

    void on_addBtn_clicked();

private:
    Ui::AddFriend *ui;
    DataManager& m_dataManager;
    QString m_add_account;
    QString m_add_name;
};

#endif // ADDFRIEND_H
