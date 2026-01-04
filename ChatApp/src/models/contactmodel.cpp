// src/models/contactmodel.cpp
#include "contactmodel.h"
#include "src/utils/logger.h"
#include <QIcon>

ContactModel::ContactModel(QObject* parent)
    : QAbstractListModel(parent)
{
    // 初始化角色名称
    m_roleNames[IdRole] = "contactId";
    m_roleNames[NameRole] = "name";
    m_roleNames[GroupRole] = "group";
    m_roleNames[AvatarRole] = "avatar";
    m_roleNames[StatusRole] = "status";
    m_roleNames[UnreadCountRole] = "unreadCount";
    m_roleNames[LastMessageRole] = "lastMessage";
    m_roleNames[LastMessageTimeRole] = "lastMessageTime";
    m_roleNames[IsOnlineRole] = "isOnline";

    LOG_DEBUG("ContactModel", "ContactModel initialized");
}

ContactModel::~ContactModel()
{
    clear();
}

int ContactModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_contacts.size();
}

QVariant ContactModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_contacts.size()) {
        return QVariant();
    }

    const ContactInfo& contact = m_contacts.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return contact.name;
    case Qt::ToolTipRole:
        return QString("%1\n状态: %2\n未读消息: %3")
            .arg(contact.name)
            .arg(getStatusText(contact.status))
            .arg(contact.unreadCount);
    case Qt::DecorationRole:
        return getStatusIcon(contact.status);
    case IdRole:
        return contact.id;
    case NameRole:
        return contact.name;
    case GroupRole:
        return contact.group;
    case AvatarRole:
        return contact.avatar.isEmpty() ? ":/images/default_avatar.png" : contact.avatar;
    case StatusRole:
        return static_cast<int>(contact.status);
    case UnreadCountRole:
        return contact.unreadCount;
    case LastMessageRole:
        return contact.unreadCount > 0 ? contact.name + QString(" (%1条未读)").arg(contact.unreadCount) : contact.name;
    case LastMessageTimeRole:
        return contact.lastMessageTime;
    case IsOnlineRole:
        return contact.status == UserStatus_Online;
    }

    return QVariant();
}

QHash<int, QByteArray> ContactModel::roleNames() const
{
    return m_roleNames;
}

void ContactModel::addContact(const ContactInfo& contact)
{
    beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
    m_contacts.append(contact);
    endInsertRows();

    LOG_DEBUG("ContactModel", QString("Contact added: %1 (ID: %2)")
                                  .arg(contact.name).arg(contact.id));

    emit rowCountChanged();
}

void ContactModel::updateContact(const ContactInfo& contact)
{
    for (int i = 0; i < m_contacts.size(); ++i) {
        if (m_contacts[i].id == contact.id) {
            m_contacts[i] = contact;

            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);

            LOG_DEBUG("ContactModel", QString("Contact updated: %1").arg(contact.name));
            return;
        }
    }

    // 如果没找到，就添加
    addContact(contact);
}

void ContactModel::removeContact(int contactId)
{
    for (int i = 0; i < m_contacts.size(); ++i) {
        if (m_contacts[i].id == contactId) {
            beginRemoveRows(QModelIndex(), i, i);
            m_contacts.removeAt(i);
            endRemoveRows();

            LOG_DEBUG("ContactModel", QString("Contact removed: ID=%1").arg(contactId));
            emit rowCountChanged();
            return;
        }
    }

    LOG_WARNING("ContactModel", QString("Contact not found for removal: ID=%1").arg(contactId));
}

void ContactModel::updateContactStatus(int contactId, UserStatus status)
{
    for (int i = 0; i < m_contacts.size(); ++i) {
        if (m_contacts[i].id == contactId) {
            m_contacts[i].status = status;

            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, {StatusRole, IsOnlineRole, Qt::DecorationRole});

            LOG_DEBUG("ContactModel", QString("Contact status updated: ID=%1, status=%2")
                                          .arg(contactId).arg(static_cast<int>(status)));
            return;
        }
    }
}

void ContactModel::updateUnreadCount(int contactId, int unreadCount)
{
    for (int i = 0; i < m_contacts.size(); ++i) {
        if (m_contacts[i].id == contactId) {
            m_contacts[i].unreadCount = unreadCount;

            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, {UnreadCountRole, LastMessageRole});

            LOG_DEBUG("ContactModel", QString("Unread count updated: ID=%1, count=%2")
                                          .arg(contactId).arg(unreadCount));
            return;
        }
    }
}

void ContactModel::setContacts(const QList<ContactInfo>& contacts)
{
    if (!m_contacts.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_contacts.size() - 1);
        m_contacts.clear();
        endRemoveRows();
    }

    if (!contacts.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, contacts.size() - 1);
        m_contacts = contacts;
        endInsertRows();

        LOG_DEBUG("ContactModel", QString("%1 contacts loaded").arg(contacts.size()));
    }

    emit rowCountChanged();
}

void ContactModel::clear()
{
    if (!m_contacts.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_contacts.size() - 1);
        m_contacts.clear();
        endRemoveRows();

        LOG_DEBUG("ContactModel", "All contacts cleared");
        emit rowCountChanged();
    }
}

int ContactModel::getContactIndex(int contactId) const
{
    for (int i = 0; i < m_contacts.size(); ++i) {
        if (m_contacts[i].id == contactId) {
            return i;
        }
    }
    return -1;
}

ContactInfo ContactModel::getContact(int contactId) const
{
    for (const auto& contact : m_contacts) {
        if (contact.id == contactId) {
            return contact;
        }
    }
    return ContactInfo();
}

QString ContactModel::getStatusText(UserStatus status) const
{
    switch (status) {
    case UserStatus_Online:
        return "在线";
    case UserStatus_Offline:
        return "离线";
    case UserStatus_Busy:
        return "忙碌";
    case UserStatus_Away:
        return "离开";
    default:
        return "未知";
    }
}

QIcon ContactModel::getStatusIcon(UserStatus status) const
{
    QString iconPath;
    switch (status) {
    case UserStatus_Online:
        iconPath = ":/images/online.png";
        break;
    case UserStatus_Offline:
        iconPath = ":/images/offline.png";
        break;
    case UserStatus_Busy:
        iconPath = ":/images/busy.png";
        break;
    case UserStatus_Away:
        iconPath = ":/images/away.png";
        break;
    default:
        iconPath = ":/images/offline.png";
    }

    return QIcon(iconPath);
}
