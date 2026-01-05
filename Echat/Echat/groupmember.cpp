#include "groupmember.h"
#include "ui_groupmember.h"

GroupMember::GroupMember(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GroupMember)
{
    ui->setupUi(this);
}

GroupMember::~GroupMember()
{
    delete ui;
}

void GroupMember::set_users_in_group_list_item(USER_INFO &friend_info)
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

    QString user_name = friend_info.user_name;
    user_name.append("(");
    user_name.append(friend_info.user_account);
    user_name.append(")");
    ui->name_label->setText(user_name);
}

void GroupMember::set_users_in_group_list_item(USER_INFO &friend_info, QPixmap &avatarPixmap)
{
    m_friend_info = friend_info;

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);

    QString user_name = friend_info.user_name;
    user_name.append("(");
    user_name.append(friend_info.user_account);
    user_name.append(")");
    ui->name_label->setText(user_name);
}
