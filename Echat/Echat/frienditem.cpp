#include "frienditem.h"
#include "ui_frienditem.h"

friendItem::friendItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::friendItem),
    m_dataManager(DataManager::instance())
{
    ui->setupUi(this);
    ui->unread_label->hide();
    ui->ret_dot_label->hide();
}

friendItem::~friendItem()
{
    delete ui;
}

void friendItem::setHeadImage(USER_INFO &friend_info)
{
    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(friend_info.avatar_data, friend_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);
}

void friendItem::setHeadImage(GROUP_INFO &group_info)
{
    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(group_info.avatar_data, group_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);
}

void friendItem::setSession_item()
{
    ui->friend_name->setText(m_session_info.name);
    if(m_session_info.sessionType == 1){
         ui->msg_label->setText(m_session_info.lastMessage);
    }else{
        // 群聊会话 - 添加安全检查
        QString displayText = m_session_info.lastMessage;

        // 检查群成员数据是否可用
        if (m_dataManager.m_groupMembers.contains(m_group_info.group_account)) {
            QVector<USER_INFO> users = m_dataManager.m_groupMembers[m_group_info.group_account];
            qDebug() << "群成员数量:" << users.size();

            QString senderName = "";
            // 查找发送者名称
            for(int i = 0; i < users.size(); i++) {
                if(QString::fromUtf8(users[i].user_account) == m_session_info.sender) {
                    senderName = QString::fromUtf8(users[i].user_name);
                    break;
                }
            }

            // 如果找到发送者，添加前缀
            if(!senderName.isEmpty()) {
                displayText = senderName + ": " + m_session_info.lastMessage;
            } else if (!m_session_info.sender.isEmpty()) {
                // 如果没找到发送者名称，但sender不为空，显示账号
                displayText = m_session_info.sender + ": " + m_session_info.lastMessage;
            }
        } else {
            qDebug() << "群成员数据未就绪，群账号:" << m_group_info.group_account;
            // 群成员数据未就绪，只显示消息内容
        }

        ui->msg_label->setText(displayText);
    }

    if(m_session_info.unreadCount != 0){
        ui->unread_label->setText(QString::number(m_session_info.unreadCount));
        ui->unread_label->show();
        ui->ret_dot_label->show();
    }else{
        ui->unread_label->hide();
        ui->ret_dot_label->hide();
    }

    if(m_session_info.lastTime != 0){
        // 将 Unix 时间戳（秒）转换为 QDateTime
        QDateTime dateTime = QDateTime::fromSecsSinceEpoch(m_session_info.lastTime);
        QString timeText;

        QDate currentDate = QDate::currentDate();
        QDate messageDate = dateTime.date();

        if (messageDate == currentDate) {
            // 今天显示具体时间
            timeText = dateTime.toString("hh:mm");
        } else if (messageDate == currentDate.addDays(-1)) {
            // 昨天显示"昨天"
            timeText = "昨天 " + dateTime.toString("hh:mm");
        } else if (messageDate.year() == currentDate.year()) {
            // 今年显示月日
            timeText = dateTime.toString("MM月dd日 hh:mm");
        } else {
            // 跨年显示完整日期
            timeText = dateTime.toString("yyyy年MM月dd日 hh:mm");
        }

        ui->time_label->setText(timeText);
    }else{
        ui->time_label->clear();
    }
}

void friendItem::setfriendlist_item(USER_INFO &friend_info)
{
    m_friend_info = friend_info;
    if(friend_info.status == 1){
        ui->msg_label->setText("[在线]");
        ui->friend_name->setText(friend_info.user_name);
        ui->time_label->clear();
        ui->time_label->setStyleSheet("border-image:url(:/new/prefix1/icons/status_online.png);");
        ui->time_label->setFixedHeight(20);
        ui->time_label->setFixedWidth(20);
    }else if(friend_info.status == 0){
        ui->msg_label->setText("[离线]");
        ui->friend_name->setText(friend_info.user_name);
        ui->time_label->clear();
        ui->time_label->setStyleSheet("border-image:url(:/new/prefix1/icons/status_outline.png);");
        ui->time_label->setFixedHeight(20);
        ui->time_label->setFixedWidth(20);
    }
}

void friendItem::setgrouplist_item(GROUP_INFO &group_info)
{
    m_group_info = group_info;

    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(group_info.avatar_data, group_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);

    QString member_count = "群人数：[";
    member_count.append(QString::number(group_info.member_count));
    member_count.append("人]");
    ui->msg_label->setText(member_count);
    QString group_name = group_info.group_name;
    group_name.append("(");
    group_name.append(group_info.group_account);
    group_name.append(")");
    ui->friend_name->setText(group_name);
    ui->time_label->clear();
}

void friendItem::set_users_in_group_list_item(USER_INFO &friend_info)
{
    m_friend_info = friend_info;

    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(friend_info.avatar_data, friend_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);

    ui->msg_label->hide();

    QString user_name = friend_info.user_name;
    user_name.append("(");
    user_name.append(friend_info.user_account);
    user_name.append(")");
    ui->friend_name->setText(user_name);
    ui->time_label->hide();
}



