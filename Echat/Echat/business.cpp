#include "business.h"

Business::Business(QObject *parent) : QObject(parent)
{

}

const char *Business::construct_login_message(QString account, QString password, LOGIN_MSG *msg)
{

    memset(msg, 0, sizeof(LOGIN_MSG));
    msg->msg_header.msg_type = LOGIN_REQUEST;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(LOGIN_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, account.toUtf8().constData());
    qstrcpy(msg->user_password, password.toUtf8().constData());

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_register_message(QString name, QString account, QString password, const QByteArray &avatarData, REGISTET_MSG *msg)
{
    memset(msg, 0, sizeof(REGISTET_MSG));
    msg->msg_header.msg_type = REGISTER_REQUEST;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(REGISTET_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_name, name.toUtf8().constData());
    qstrcpy(msg->user_account, account.toUtf8().constData());
    qstrcpy(msg->user_password, password.toUtf8().constData());

    // 处理头像数据
    if (!avatarData.isEmpty() && avatarData.size() <= MAX_AVATAR_SIZE) {
        msg->avatar_size = avatarData.size();
        memcpy(msg->avatar_data, avatarData.constData(), msg->avatar_size);
    } else {
        msg->avatar_size = 0;
    }

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_query_message(QString user_account, QString query_account, ACCOUNT_QUERY_MSG *msg, int flag)
{
    memset(msg, 0, sizeof(ACCOUNT_QUERY_MSG));
    if(flag == 0){
        msg->msg_header.msg_type = ACCOUNT_QUERY_REQUEST;
    }else if(flag == 1){
        msg->msg_header.msg_type = GROUP_QUERY_REQUEST;
    }else{
        msg->msg_header.msg_type = GROUP_MEMBER_QUERY;
    }
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(ACCOUNT_QUERY_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, user_account.toUtf8().constData());
    qstrcpy(msg->query_account, query_account.toUtf8().constData());

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_add_friend_message(QString user_account, QString add_account, ADD_FRIEND_MSG *msg)
{
    memset(msg, 0, sizeof(ADD_FRIEND_MSG));
    msg->msg_header.msg_type = ADD_FRIEND_REQUEST;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(ADD_FRIEND_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, user_account.toUtf8().constData());
    qstrcpy(msg->friend_account, add_account.toUtf8().constData());

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_friend_ask_response_message(QString user_account, QString add_account, ADD_FRIEND_MSG *msg, int choice)
{
    memset(msg, 0, sizeof(ADD_FRIEND_MSG));
    msg->msg_header.msg_type = choice;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(ADD_FRIEND_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, user_account.toUtf8().constData());
    qstrcpy(msg->friend_account, add_account.toUtf8().constData());

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_chat_message(CHAT_MSG *msg)
{
    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_HistroyMsgGet_message(QString user_account, HISTORY_MSG_GET *msg)
{
    memset(msg, 0, sizeof(HISTORY_MSG_GET));
    msg->msg_header.msg_type = HISTORY_MSG_REQUEST;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(HISTORY_MSG_GET) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, user_account.toUtf8().constData());
    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

//const char *Business::construct_update_chatmsg_message(UPDATE_CHAT_MSG *msg, int &total_size)
//{
//    msg->msg_header.msg_type = UPDATE_CHAT_MSG_REQUEST;
//    msg->msg_header.total_count = 1;
//    msg->msg_header.msg_length = sizeof(uint32_t) + (msg->chat_msgs.size() * sizeof(long long));
//    msg->msg_header.timestamp = time(NULL);

//    const char *data = reinterpret_cast<const char*>(msg);
//    return data;
//}

const char *Business::construct_update_chatmsg_message(UPDATE_CHAT_MSG *msg, int &total_size)
{
    msg->msg_header.msg_type = UPDATE_CHAT_MSG_REQUEST;
    msg->msg_header.total_count = 1;

    // 计算消息体长度：消息数量(4字节) + 所有消息ID
    msg->msg_header.msg_length = sizeof(uint32_t) + (msg->chat_msgs.size() * sizeof(long long));
    msg->msg_header.timestamp = time(NULL);

    // 计算总发送长度：消息头 + 消息体
    total_size = sizeof(MSG_HEADER) + msg->msg_header.msg_length;

    // 分配缓冲区并构建完整消息
    char *buffer = new char[total_size];

    // 复制消息头
    memcpy(buffer, &msg->msg_header, sizeof(MSG_HEADER));

    // 构建消息体：先写消息数量，再写所有消息ID
    char *ptr = buffer + sizeof(MSG_HEADER);

    // 写入消息数量
    uint32_t msg_count = msg->chat_msgs.size();
    memcpy(ptr, &msg_count, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    // 写入所有消息ID
    if (msg_count > 0) {
        memcpy(ptr, msg->chat_msgs.data(), msg_count * sizeof(long long));
    }

    return buffer;
}

const char *Business::construct_update_group_chatmsg_message(UPDATE_CHAT_MSG *msg, int &total_size)
{
    msg->msg_header.msg_type = UPDATE_GROUP_CHAT_MSG_REQUEST;
    msg->msg_header.total_count = 1;

    // 计算消息体长度：消息数量(4字节) + 所有消息ID
    msg->msg_header.msg_length = sizeof(uint32_t) + (msg->chat_msgs.size() * sizeof(long long));
    msg->msg_header.timestamp = time(NULL);

    // 计算总发送长度：消息头 + 消息体
    total_size = sizeof(MSG_HEADER) + msg->msg_header.msg_length;

    // 分配缓冲区并构建完整消息
    char *buffer = new char[total_size];

    // 复制消息头
    memcpy(buffer, &msg->msg_header, sizeof(MSG_HEADER));

    // 构建消息体：先写消息数量，再写所有消息ID
    char *ptr = buffer + sizeof(MSG_HEADER);

    // 写入消息数量
    uint32_t msg_count = msg->chat_msgs.size();
    memcpy(ptr, &msg_count, sizeof(uint32_t));
    ptr += sizeof(uint32_t);

    // 写入所有消息ID
    if (msg_count > 0) {
        memcpy(ptr, msg->chat_msgs.data(), msg_count * sizeof(long long));
    }

    return buffer;
}

// 使用后记得释放内存
void Business::free_message_buffer(const char *buffer)
{
    delete[] buffer;
}

QPixmap Business::getRoundedPixmap(const QPixmap& src, int radius)
{
    if (src.isNull()) {
        return QPixmap();
    }

    QPixmap result(src.size());
    result.fill(Qt::transparent);

    QPainter painter(&result);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

    // 创建圆角路径
    QRect rect = QRect(0, 0, src.width(), src.height());
    QPainterPath path;
    path.addRoundedRect(rect, radius, radius);

    // 设置裁剪区域
    painter.setClipPath(path);
    painter.drawPixmap(rect, src);

    return result;
}

const char *Business::construct_create_group_message(QString name, QString group_account, const QByteArray &avatarData, QString user_account, CREATE_GROUP_MSG *msg)
{
    memset(msg, 0, sizeof(CREATE_GROUP_MSG));
    msg->msg_header.msg_type = CREATE_GROUP_REQUEST;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(CREATE_GROUP_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->group_name, name.toUtf8().constData());
    qstrcpy(msg->group_account, group_account.toUtf8().constData());
    qstrcpy(msg->creator_account, user_account.toUtf8().constData());

    // 处理头像数据
    if (!avatarData.isEmpty() && avatarData.size() <= MAX_AVATAR_SIZE) {
        msg->avatar_size = avatarData.size();
        memcpy(msg->avatar_data, avatarData.constData(), msg->avatar_size);
    } else {
        msg->avatar_size = 0;
    }

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_add_group_message(QString user_account, QString add_account, ADD_GROUP_MSG *msg)
{
    memset(msg, 0, sizeof(ADD_GROUP_MSG));
    msg->msg_header.msg_type = ADD_GROUP_REQUEST;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(ADD_GROUP_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, user_account.toUtf8().constData());
    qstrcpy(msg->group_account, add_account.toUtf8().constData());

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}

const char *Business::construct_group_ask_response_message(QString user_account, QString add_account, ADD_GROUP_MSG *msg, int choice)
{
    memset(msg, 0, sizeof(ADD_GROUP_MSG));
    msg->msg_header.msg_type = choice;
    msg->msg_header.total_count = 1;
    msg->msg_header.msg_length = sizeof(ADD_GROUP_MSG) - sizeof(MSG_HEADER);
    msg->msg_header.timestamp = time(NULL);

    qstrcpy(msg->user_account, user_account.toUtf8().constData());
    qstrcpy(msg->group_account, add_account.toUtf8().constData());

    const char *data = reinterpret_cast<const char*>(msg);
    return data;
}
