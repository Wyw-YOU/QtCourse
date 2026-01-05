#include "addfriend.h"
#include "ui_addfriend.h"
#include "networkmanager.h"
#include "Protocol.h"

AddFriend::AddFriend(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddFriend),
    m_dataManager(DataManager::instance())
{
    ui->setupUi(this);
}

AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::on_searchBtn_clicked()
{
    if(m_flag == 0){
        ACCOUNT_QUERY_MSG msg;
        const char* data =  Business::construct_query_message(m_dataManager.getCurrentUser().account, ui->account_edit->text(), &msg, m_flag);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ACCOUNT_QUERY_MSG));
    }else{
        ACCOUNT_QUERY_MSG msg;
        const char* data =  Business::construct_query_message(m_dataManager.getCurrentUser().account, ui->account_edit->text(), &msg, m_flag);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ACCOUNT_QUERY_MSG));
    }
}

void AddFriend::label_clear()
{
    ui->headimg_label->clear();
    ui->account_label->hide();
    ui->name_label->hide();
    ui->addBtn->hide();
//    ui->account_label->clear();
//    ui->name_label->clear();
}

void AddFriend::set_label(USER_INFO &user_info)
{
//    ui->headimg_label->clear();
    QString account_text = "账号：";
    QString name_text = "昵称：";
    m_add_account = user_info.user_account;
    m_add_name = user_info.user_name;
    account_text.append(m_add_account);
    name_text.append(m_add_name);

    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(user_info.avatar_data, user_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);

    ui->account_label->setText(account_text);
    ui->name_label->setText(name_text);
    ui->account_label->show();
    ui->name_label->show();
    ui->addBtn->show();
}

void AddFriend::set_label(GROUP_INFO &group_info)
{
//    ui->headimg_label->clear();
    QString account_text = "群账号：";
    QString name_text = "群名称：";
    m_add_account = group_info.group_account;
    m_add_name = group_info.group_name;
    account_text.append(m_add_account);
    name_text.append(m_add_name);

    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(group_info.avatar_data, group_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);

    ui->account_label->setText(account_text);
    ui->name_label->setText(name_text);
    ui->account_label->show();
    ui->name_label->show();
    ui->addBtn->show();
}

void AddFriend::on_addBtn_clicked()
{
    if(m_flag == 0){
        ADD_FRIEND_MSG msg;
        const char* data = Business::construct_add_friend_message(m_dataManager.getCurrentUser().account, m_add_account, &msg);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ADD_FRIEND_MSG));
        m_dataManager.m_friend_add_ask = (FRIEND_ADD_ASK *)malloc(sizeof(FRIEND_ADD_ASK));
    }else{
        ADD_GROUP_MSG msg;
        const char* data = Business::construct_add_group_message(m_dataManager.getCurrentUser().account, m_add_account, &msg);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ADD_GROUP_MSG));
    }
}
