#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "Protocol.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <cstring>
#include "datamanager.h"

class NetworkManager : public QObject
{
    Q_OBJECT
public:
     explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    void start();
    void stop();
    bool send_message(const char* data, int data_size);
    bool read_message(MSG_HEADER &msg_header);
    bool read_remain_message(MSG_HEADER &msg_header, RESPONSE_MSG &response_msg);
    bool read_login_message(MSG_HEADER &msg_header, USER_QUERY_RESPONSE_MSG &response_msg);
    bool read_add_friend_list(MSG_HEADER &msg_header, FRIEND_ASK_NOTICE_MSG *msg);
    bool read_user_query_response(MSG_HEADER &msg_header, USER_QUERY_RESPONSE_MSG &response_msg);
    bool read_friend_list_msg(MSG_HEADER &msg_header, FRIEND_LIST_MSG *msg);
    bool read_chat_msg(MSG_HEADER &msg_header, CHAT_MSG *msg);
    bool read_group_query_response(MSG_HEADER &msg_header, GROUP_QUERY_RESPONSE_MSG &response_msg);
    bool read_add_group_list(MSG_HEADER &msg_header, GROUP_ASK_NOTICE_MSG *msg);
    bool read_group_list_msg(MSG_HEADER &msg_header, GROUP_LIST_MSG *msg);
    bool read_users_in_group_list_msg(MSG_HEADER &msg_header, GROUP_MEMBER_QUERY_RESPONSE_MSG*msg);

signals:
    // 定义信号来传递连接状态
    void connectionStatusChanged(bool connected, const QString& message);
    void DataReach();

private slots:
    void onConnected();
    void onDisconnected();
    void onreadyRead();

private:
    QTcpSocket *m_socket;
    QTimer *m_heartbeatTimer;
    QString m_ip = "47.104.7.161";
    QString m_port = "8080";

    DataManager& m_dataManager; // 引用单例实例
};

#endif // NETWORKMANAGER_H
