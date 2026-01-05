#include "datamanager.h"
#include "networkmanager.h"

DataManager::DataManager(QObject *parent) : QObject(parent)
{

}

DataManager::~DataManager()
{
    if(m_friend_add_ask != NULL){
        free(m_friend_add_ask);
    }
}

void DataManager::setCurrentUser(const UserBasicInfo& user)
{
    m_currentUser = user;
    emit currentUserChanged();
}

UserBasicInfo DataManager::getCurrentUser() const
{
    return m_currentUser;
}

void DataManager::setCurrentfriend_add_asks(FRIEND_ADD_ASK asks[], int count)
{
    for (int i = 0; i < count; i++)
    {
        m_friend_add_asks.append(asks[i]);
    }
}

void DataManager::update_friend_add_asks(FRIEND_ADD_ASK &ask)
{

    for(int i = 0; i < m_friend_add_asks.size(); i++){
        if(strcmp(ask.user_account, m_friend_add_asks[i].user_account) == 0 and strcmp(ask.friend_acccount, m_friend_add_asks[i].friend_acccount) == 0){
            m_friend_add_asks[i] = ask;
            return;
        }
    }

    m_friend_add_asks.append(ask);
}

QList<FRIEND_ADD_ASK> DataManager::getCurrentfriend_add_asks() const
{
    return m_friend_add_asks;
}

void DataManager::set_networkmanager(NetworkManager* networkmanager)
{
    m_networkmanager = networkmanager;
}

void DataManager::setCurrent_friendlist(USER_INFO friends[], int count)
{
    for (int i = 0; i < count; i++)
    {
        m_friends.append(friends[i]);
    }
}

void DataManager::update_friendlist(USER_INFO &friend_info)
{
    for(int i = 0; i < m_friends.size(); i++){
        if(strcmp(friend_info.user_account, m_friends[i].user_account) == 0){
            m_friends[i] = friend_info;
            return;
        }
    }

    m_friends.append(friend_info);
}

void DataManager::update_users_in_group_list(GROUP_MEMBER_QUERY_RESPONSE_MSG* msg)
{
    if(msg->msg_header.total_count > 1){
        QString group_account = QString::fromUtf8(msg->group_account);
        QVector<USER_INFO> members;
        for(int i = 0; i < msg->msg_header.total_count; i++) {
            members.append(msg->users[i]);
        }

        m_groupMembers[group_account] = members;
        qDebug() << "成功拉取群成员信息，群号:" << group_account << "成员数量:" << members.size();
    }else if(msg->msg_header.total_count == 1){
        QString group_account = QString::fromUtf8(msg->group_account);
        USER_INFO add_user = msg->users[0];

        m_groupMembers[group_account].append(add_user);
        qDebug() << "成功更新群成员信息，群号:" << group_account << "增加的群成员:" << add_user.user_account;
    }
}

QList<USER_INFO> DataManager::getCurrentfriend_friendlist() const
{
    return m_friends;
}

void DataManager::setCurrentgroup_add_asks(GROUP_ADD_ASK asks[], int count)
{
    for (int i = 0; i < count; i++)
    {
        m_group_add_asks.append(asks[i]);
    }
}

QList<GROUP_ADD_ASK> DataManager::getCurrentgroup_add_asks() const
{
    return m_group_add_asks;
}

void DataManager::update_group_add_asks(GROUP_ADD_ASK &ask)
{
    for(int i = 0; i < m_group_add_asks.size(); i++){
        if(strcmp(ask.user_account, m_group_add_asks[i].user_account) == 0 and strcmp(ask.group_acccount, m_group_add_asks[i].group_acccount) == 0){
            m_group_add_asks[i] = ask;
            return;
        }
    }

    m_group_add_asks.append(ask);
}

void DataManager::setCurrent_grouplist(GROUP_INFO groups[], int count)
{
    for (int i = 0; i < count; i++)
    {
        m_groups.append(groups[i]);
    }
}

QList<GROUP_INFO> DataManager::getCurrent_grouplist() const
{
    return m_groups;
}

void DataManager::update_grouplist(GROUP_INFO &group_info)
{
    for(int i = 0; i < m_groups.size(); i++){
        if(strcmp(group_info.group_account, m_groups[i].group_account) == 0){
            m_groups[i] = group_info;
            return;
        }
    }

    m_groups.append(group_info);
}



