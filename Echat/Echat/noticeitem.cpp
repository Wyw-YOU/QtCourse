#include "noticeitem.h"
#include "ui_noticeitem.h"
#include "networkmanager.h"
#include "business.h"

noticeitem::noticeitem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::noticeitem),
    m_dataManager(DataManager::instance())
{
    ui->setupUi(this);
}

noticeitem::~noticeitem()
{
    delete ui;
}

void noticeitem::setHeadImage(FRIEND_ADD_ASK &ask, QString user_account)
{
    QPixmap avatarPixmap;
    if(qstrcmp(user_account.toUtf8().data(), ask.user_account) == 0){
        // 如果只有avatarData，先转换为QPixmap
        QByteArray avatarData(ask.friend_avatar_data, ask.friend_avatar_size);
        avatarPixmap.loadFromData(avatarData);
    }else{
        QByteArray avatarData(ask.user_avatar_data, ask.user_avatar_size);
        avatarPixmap.loadFromData(avatarData);
    }

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);
}

void noticeitem::set_notice_text(FRIEND_ADD_ASK &ask, QString user_account)
{
    m_friend_add_ask_info = ask;
//    qDebug() << "user_account: " <<user_account.toUtf8().data();
//    qDebug() << "ask.user_account: " <<ask.user_account;
    if(qstrcmp(user_account.toUtf8().data(), ask.user_account) == 0){
        ui->msg_label->setText("请求添加对方为好友");
        ui->time_label->setText(ask.add_time);
        ui->friend_name->setText(ask.friend_name);
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("等待验证");
        ui->rejectBtn->setEnabled(false);
    }else{
        QString name = QString::fromUtf8(ask.user_account);
        ui->msg_label->setText("对方请求添加您为好友");
        ui->time_label->setText(ask.add_time);
        ui->friend_name->setText(ask.user_name);
    }

    if(ask.friend_status == -1){
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("已拒绝");
        ui->rejectBtn->setEnabled(false);
    }else if(ask.friend_status == 1){
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("已同意");
        ui->rejectBtn->setEnabled(false);
    }
}

void noticeitem::on_acceptBtn_clicked()
{
    if(m_flag == 0){
        ADD_FRIEND_MSG msg;
        const char* data =  Business::construct_friend_ask_response_message(m_friend_add_ask_info.user_account, m_friend_add_ask_info.friend_acccount, &msg, ACCEPT_FRIEND_ASK);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ADD_FRIEND_MSG));
    }else{
        ADD_GROUP_MSG msg;
        const char* data =  Business::construct_group_ask_response_message(m_group_add_ask_info.user_account, m_group_add_ask_info.group_acccount, &msg, ACCEPT_GROUP_ASK);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ADD_GROUP_MSG));
    }
}

void noticeitem::on_rejectBtn_clicked()
{
    if(m_flag == 0){
        ADD_FRIEND_MSG msg;
        const char* data =  Business::construct_friend_ask_response_message(m_friend_add_ask_info.user_account, m_friend_add_ask_info.friend_acccount, &msg, REJECT_FRIEND_ASK);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ADD_FRIEND_MSG));
    }else{
        ADD_GROUP_MSG msg;
        const char* data =  Business::construct_group_ask_response_message(m_group_add_ask_info.user_account, m_group_add_ask_info.group_acccount, &msg, REJECT_GROUP_ASK);
        m_dataManager.m_networkmanager->send_message(data, sizeof (ADD_GROUP_MSG));
    }
}

void noticeitem::update_text(int choice)
{
    if(choice == ACCEPT_FRIEND_ASK){
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("已同意");
        ui->rejectBtn->setEnabled(false);
    }else{
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("已拒绝");
        ui->rejectBtn->setEnabled(false);
    }
}

void noticeitem::set_group_notice(GROUP_ADD_ASK &ask, QString user_account)
{
    // 1.添加者   即creator account!=user_account  / user_account == user_account  注意：数据库也需要去筛选出非创建者的 添加者
    //          头像设置为群聊   隐藏按钮

    // 2.群主   即creator_account==user_account  数据库只会筛选出user_account不是creator_account的  这条消息没意义  剩下的即都是别人的群聊申请请求了
    //          头像设置为添加群聊者  打开通过和拒绝的按钮

    m_group_add_ask_info = ask;
    m_flag = 1;  // 改为群聊通知
    QPixmap avatarPixmap;
    if(qstrcmp(user_account.toUtf8().data(), ask.user_account) == 0){
        // 如果只有avatarData，先转换为QPixmap
        QByteArray avatarData(ask.group_avatar_data, ask.group_avatar_size);
        avatarPixmap.loadFromData(avatarData);

        ui->msg_label->setText("请求加入该群聊");
        ui->time_label->setText(ask.add_time);
        ui->friend_name->setText(ask.group_name);
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("等待验证");
        ui->rejectBtn->setEnabled(false);
    }else{
        QByteArray avatarData(ask.user_avatar_data, ask.user_avatar_size);
        avatarPixmap.loadFromData(avatarData);

        QString name = QString::fromUtf8(ask.user_account);
        QString ask_msg = "对方请求加入群聊：";
        ask_msg.append(ask.group_name);
        ask_msg.append("(");
        ask_msg.append(ask.group_acccount);
        ask_msg.append(")");
        ui->msg_label->setText(ask_msg);
        ui->time_label->setText(ask.add_time);
        ui->friend_name->setText(ask.user_name);
    }

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);

    if(ask.status == -1){
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("已拒绝");
        ui->rejectBtn->setEnabled(false);
    }else if(ask.status == 1){
        ui->acceptBtn->hide();
        ui->rejectBtn->setStyleSheet("QPushButton{background-color: rgba(0, 0, 0, 0);color: rgb(0, 0, 0);}");
        ui->rejectBtn->setText("已同意");
        ui->rejectBtn->setEnabled(false);
    }
}
