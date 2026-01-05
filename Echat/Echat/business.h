#ifndef BUSINESS_H
#define BUSINESS_H

#include <QObject>
#include "Protocol.h"
#include <QDebug>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>

class Business : public QObject
{
    Q_OBJECT

public:
    explicit Business(QObject *parent = nullptr);
    static const char *construct_login_message(QString account, QString password, LOGIN_MSG *msg);
    static const char *construct_register_message(QString name, QString account, QString password, const QByteArray &avatarData, REGISTET_MSG *msg);
    static const char *construct_query_message(QString user_account, QString query_account, ACCOUNT_QUERY_MSG *msg, int flag);
    static const char *construct_add_friend_message(QString user_account, QString add_account, ADD_FRIEND_MSG *msg);
    static const char *construct_friend_ask_response_message(QString user_account, QString add_account, ADD_FRIEND_MSG *msg, int choice);
    static const char *construct_chat_message(CHAT_MSG *msg);
    static const char *construct_HistroyMsgGet_message(QString user_account, HISTORY_MSG_GET *msg);
    static const char *construct_update_chatmsg_message(UPDATE_CHAT_MSG *msg, int &total_size);
    static void free_message_buffer(const char *buffer);
    static const char *construct_create_group_message(QString name, QString group_account, const QByteArray &avatarData, QString user_account, CREATE_GROUP_MSG *msg);
    static const char *construct_add_group_message(QString user_account, QString add_account, ADD_GROUP_MSG *msg);
    static const char *construct_group_ask_response_message(QString user_account, QString add_account, ADD_GROUP_MSG *msg, int choice);
    static const char *construct_update_group_chatmsg_message(UPDATE_CHAT_MSG *msg, int &total_size);

    static QPixmap getRoundedPixmap(const QPixmap& src, int radius);

signals:

};

#endif // BUSINESS_H
