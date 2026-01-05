#include "networkmanager.h"

NetworkManager::NetworkManager(QObject *parent) : QObject(parent), m_dataManager(DataManager::instance())
{
    m_socket = new QTcpSocket(this);  //创建socket对象

    // 连接信号
    connect(m_socket, &QTcpSocket::connected, this, &NetworkManager::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &NetworkManager::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &NetworkManager::onreadyRead);
}

NetworkManager::~NetworkManager()
{

}

void NetworkManager::onConnected()
{
    qDebug() << "Connected to server";
    emit connectionStatusChanged(true, "连接服务器成功");
}

void NetworkManager::onDisconnected()
{
    qDebug() << "Disconnected from server";
    emit connectionStatusChanged(false, "连接异常，网络断开");
}

void NetworkManager::onreadyRead()
{
    qDebug() << "收到服务端数据";
    emit DataReach();
}


void NetworkManager::start()
{
    //连接服务器
    m_socket->connectToHost(QHostAddress(m_ip), m_port.toShort());
}

void NetworkManager::stop()
{
    //断开连接
    m_socket->disconnectFromHost();
}

bool NetworkManager::send_message(const char* data, int data_size)
{
    qint64 bytesWritten = m_socket->write(data, data_size);
    if (bytesWritten != data_size) {
        qDebug() <<  "错误, 发送数据失败";
        return false;
    }
    return m_socket->flush();
}

//bool NetworkManager::send_message(const char* data, int data_size)
//{
//    qint64 total_bytes_sent = 0;
//    const char* current_data = data;

//    while (total_bytes_sent < data_size) {
//        qint64 bytes_written = m_socket->write(current_data + total_bytes_sent,
//                                              data_size - total_bytes_sent);

//        if (bytes_written == -1) {
//            qDebug() << "发送数据时发生错误:" << m_socket->errorString();
//            return false;
//        }

//        if (bytes_written == 0) {
//            qDebug() << "发送缓冲区已满，等待可写信号...";
//            // 等待socket可写
//            if (!m_socket->waitForBytesWritten(100)) { // 等100ms
//                qDebug() << "等待可写超时";
//                return false;
//            }
//            continue;
//        }

//        total_bytes_sent += bytes_written;
//        qDebug() << "已发送" << bytes_written << "字节，总计" << total_bytes_sent << "/" << data_size;
//    }

//    // 刷新缓冲区
//    if (!m_socket->flush()) {
//        qDebug() << "刷新缓冲区失败";
//        return false;
//    }

//    qDebug() << "成功发送完整数据，大小:" << data_size << "字节";
//    return true;
//}

bool NetworkManager::read_message(MSG_HEADER &msg_header)
{
    qint64 bytesRead = m_socket->read(reinterpret_cast<char*>(&msg_header), sizeof(MSG_HEADER));
    return (bytesRead == sizeof(MSG_HEADER));
}


bool NetworkManager::read_remain_message(MSG_HEADER &msg_header, RESPONSE_MSG &response_msg)
{
    // 接受剩余数据
    memcpy(&response_msg.msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    if (remain_data != sizeof(response_msg) - sizeof(MSG_HEADER))
    {
        qDebug() << "响应消息长度错误";
        return false;
    }

    char *msg_data = (char*)(&response_msg) + sizeof(MSG_HEADER);

    qint64 bytesRead = m_socket->read(msg_data, remain_data);

    if (bytesRead != remain_data)
    {
        qDebug() << "响应消息不完整";
        return false;
    }

    return true;
}

bool NetworkManager::read_login_message(MSG_HEADER &msg_header, USER_QUERY_RESPONSE_MSG &response_msg)
{
    // 接受剩余数据
    memcpy(&response_msg.msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    // 计算预期的消息体大小
    int expected_size = sizeof(USER_QUERY_RESPONSE_MSG) - sizeof(MSG_HEADER);

    qDebug() << "读取登录响应，剩余数据:" << remain_data << "期望大小:" << expected_size;


    if (remain_data != expected_size)
    {
        qDebug() << "响应消息长度错误";
        return false;
    }

    char *msg_data = (char*)(&response_msg) + sizeof(MSG_HEADER);

//    // 循环读取确保完整
//    qint64 total_read = 0;
//    while (total_read < remain_data) {
//        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);
//        if (bytes_read <= 0) {
//            qDebug() << "读取登录响应失败，已读取:" << total_read << "/" << remain_data;
//            return false;
//        }
//        total_read += bytes_read;

//        // 可选：添加小延迟避免过于频繁的读取
//        if (total_read < remain_data) {
//            m_socket->waitForReadyRead(10); // 等待10ms
//        }
//    }

//    qDebug() << "登录响应读取完成:" << total_read << "字节";
    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取登录响应失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "登录响应读取完成:" << total_read << "字节";

    return true;
}

bool NetworkManager::read_add_friend_list(MSG_HEADER &msg_header, FRIEND_ASK_NOTICE_MSG *msg)
{
    // 接受剩余数据
    memcpy(&msg->msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    // 计算每个FRIEND_ADD_ASK结构的大小
    int ask_size = sizeof(FRIEND_ADD_ASK);
    int expected_data_size = msg_header.total_count * ask_size;

    // 验证数据长度是否合理
    if (remain_data != expected_data_size) {
        qDebug() << "好友申请列表长度不匹配: 期望" << expected_data_size << "实际" << remain_data;
    }

    char *msg_data = (char*)msg + sizeof(MSG_HEADER);

//    // 使用循环读取确保完整
//    qint64 total_read = 0;
//    while (total_read < remain_data) {
//        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);
//        if (bytes_read <= 0) {
//            qDebug() << "读取好友申请列表失败，已读取:" << total_read << "/" << remain_data;
//            free(msg);
//            return false;
//        }
//        total_read += bytes_read;

//        // 如果还没读完，等待更多数据
//        if (total_read < remain_data) {
//            m_socket->waitForReadyRead(10); // 等待10ms
//        }
//    }
//        qDebug() << "好友申请列表读取完成:" << total_read << "字节";

    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取好友申请列表失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "好友申请列表读取完成:" << total_read << "字节";

    for (int i = 0; i < msg->msg_header.total_count; i++)
    {
         qDebug() <<
            msg->asks[i].user_account << "   " <<
            msg->asks[i].user_name << "   " <<
            msg->asks[i].friend_acccount <<  "   " <<
            msg->asks[i].friend_name << "   " <<
            msg->asks[i].friend_status << "   " <<
            msg->asks[i].add_time;
    }

    return true;
}

bool NetworkManager::read_user_query_response(MSG_HEADER &msg_header, USER_QUERY_RESPONSE_MSG &response_msg)
{
    // 接受剩余数据
    memcpy(&response_msg.msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    // 计算预期的消息体大小
    int expected_size = sizeof(USER_QUERY_RESPONSE_MSG) - sizeof(MSG_HEADER);

    if (remain_data != expected_size)
    {
        qDebug() << "响应消息长度错误";
        return false;
    }

    char *msg_data = (char*)(&response_msg) + sizeof(MSG_HEADER);

//    qint64 bytesRead = m_socket->read(msg_data, remain_data);

//    if (bytesRead != remain_data)
//    {
//        qDebug() << "响应消息不完整";
//        return false;
//    }
    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取用户查询响应失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "用户查询响应读取完成:" << total_read << "字节";

    return true;
}

bool NetworkManager::read_group_query_response(MSG_HEADER &msg_header, GROUP_QUERY_RESPONSE_MSG &response_msg)
{
    // 接受剩余数据
    memcpy(&response_msg.msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    // 计算预期的消息体大小
    int expected_size = sizeof(GROUP_QUERY_RESPONSE_MSG) - sizeof(MSG_HEADER);

    if (remain_data != expected_size)
    {
        qDebug() << "响应消息长度错误";
        return false;
    }

    char *msg_data = (char*)(&response_msg) + sizeof(MSG_HEADER);

//    qint64 bytesRead = m_socket->read(msg_data, remain_data);

//    if (bytesRead != remain_data)
//    {
//        qDebug() << "响应消息不完整";
//        return false;
//    }
    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取群聊查询响应消息失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "群聊查询响应消息读取完成:" << total_read << "字节";

    return true;
}

bool NetworkManager::read_friend_list_msg(MSG_HEADER &msg_header, FRIEND_LIST_MSG *msg)
{
    // 接受剩余数据
    memcpy(&msg->msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    char *msg_data = (char*)msg + sizeof(MSG_HEADER);

//    // 使用循环读取确保完整
//    qint64 total_read = 0;
//    while (total_read < remain_data) {
//        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);
//        if (bytes_read <= 0) {
//            qDebug() << "读取好友列表失败，已读取:" << total_read << "/" << remain_data;
//            free(msg);
//            return false;
//        }
//        total_read += bytes_read;

//        // 如果还没读完，等待更多数据
//        if (total_read < remain_data) {
//            m_socket->waitForReadyRead(10); // 等待10ms
//        }
//    }
//   qDebug() << "好友列表读取完成:" << total_read << "字节";

    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取好友列表失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "好友列表读取完成:" << total_read << "字节";

    for (int i = 0; i < msg->msg_header.total_count; i++)
    {
         qDebug() <<
            msg->friends[i].user_account << "   " <<
            msg->friends[i].user_name << "   " <<
            msg->friends[i].status;
    }

    return true;
}

bool NetworkManager::read_chat_msg(MSG_HEADER &msg_header, CHAT_MSG *msg)
{
    // 接受剩余数据
    memcpy(&msg->msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    char *msg_data = (char*)msg + sizeof(MSG_HEADER);


//    qint64 bytesRead = m_socket->read(msg_data, remain_data);

//    qDebug() << "remain_data_size:" << remain_data;
//    qDebug() << "bytesRead:" << bytesRead;
//    if (bytesRead != remain_data)
//    {
//        free(msg);
//        qDebug() << "响应消息不完整";
//        return false;
//    }

    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取历史消息失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "历史消息读取完成:" << total_read << "字节";

    return true;
}

bool NetworkManager::read_add_group_list(MSG_HEADER &msg_header, GROUP_ASK_NOTICE_MSG *msg)
{
    // 接受剩余数据
    memcpy(&msg->msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    // 计算每个FRIEND_ADD_ASK结构的大小
    int ask_size = sizeof(GROUP_ADD_ASK);
    int expected_data_size = msg_header.total_count * ask_size;

    // 验证数据长度是否合理
    if (remain_data != expected_data_size) {
        qDebug() << "群聊申请列表长度不匹配: 期望" << expected_data_size << "实际" << remain_data;
    }

    char *msg_data = (char*)msg + sizeof(MSG_HEADER);

//    // 使用循环读取确保完整
//    qint64 total_read = 0;
//    while (total_read < remain_data) {
//        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);
//        if (bytes_read <= 0) {
//            qDebug() << "读取群聊申请列表失败，已读取:" << total_read << "/" << remain_data;
//            free(msg);
//            return false;
//        }
//        total_read += bytes_read;

//        // 如果还没读完，等待更多数据
//        if (total_read < remain_data) {
//            m_socket->waitForReadyRead(10); // 等待10ms
//        }
//    }

//    qDebug() << "群聊申请列表读取完成:" << total_read << "字节";

    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取群聊申请列表失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "群聊申请列表读取完成:" << total_read << "字节";

    for (int i = 0; i < msg->msg_header.total_count; i++)
    {
         qDebug() <<
            msg->asks[i].user_account << "   " <<
            msg->asks[i].user_name << "   " <<
            msg->asks[i].group_acccount <<  "   " <<
            msg->asks[i].group_name << "   " <<
            msg->asks[i].status << "   " <<
            msg->asks[i].add_time;
    }

    return true;
}

bool NetworkManager::read_group_list_msg(MSG_HEADER &msg_header, GROUP_LIST_MSG *msg)
{
    // 接受剩余数据
    memcpy(&msg->msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    char *msg_data = (char*)msg + sizeof(MSG_HEADER);

    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取群组列表失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "群组列表读取完成:" << total_read << "字节";

    for (int i = 0; i < msg->msg_header.total_count; i++)
    {
         qDebug() <<
            msg->groups[i].group_account << "   " <<
            msg->groups[i].group_name << "   " <<
            msg->groups[i].member_count;
    }

    return true;
}

bool NetworkManager::read_users_in_group_list_msg(MSG_HEADER &msg_header, GROUP_MEMBER_QUERY_RESPONSE_MSG*msg)
{
    // 接受剩余数据
    memcpy(&msg->msg_header, &msg_header, sizeof(MSG_HEADER));

    int remain_data = msg_header.msg_length;

    char *msg_data = (char*)msg + sizeof(MSG_HEADER);

    // 使用循环读取确保完整
    qint64 total_read = 0;
    int wait_count = 0;
    const int max_wait_count = 500; // 最多等待500次*10ms=5秒

    while (total_read < remain_data && wait_count < max_wait_count) {
        qint64 bytes_read = m_socket->read(msg_data + total_read, remain_data - total_read);

        if (bytes_read > 0) {
            total_read += bytes_read;
            wait_count = 0; // 收到数据，重置等待计数
        } else {
            // 没有读到数据，等待并计数
            m_socket->waitForReadyRead(10); // 等待10ms
            wait_count++;
        }
    }

    if (total_read < remain_data) {
        qDebug() << "读取群成员信息列表失败，已读取:" << total_read << "/" << remain_data
                 << "，等待超时";
        return false;
    }

    qDebug() << "群成员信息列表列表读取完成:" << total_read << "字节";

    return true;
}
