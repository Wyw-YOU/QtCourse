// src/models/contactmodel.h
#ifndef CONTACTMODEL_H
#define CONTACTMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QList>
#include <QIcon>
#include "src/utils/global.h"

class ContactModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
    enum Role {
        IdRole = Qt::UserRole + 1,
        NameRole,
        GroupRole,
        AvatarRole,
        StatusRole,
        UnreadCountRole,
        LastMessageRole,
        LastMessageTimeRole,
        IsOnlineRole
    };

    explicit ContactModel(QObject* parent = nullptr);
    ~ContactModel();

    // QAbstractListModel接口实现
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 自定义方法
    void addContact(const ContactInfo& contact);
    void updateContact(const ContactInfo& contact);
    void removeContact(int contactId);
    void updateContactStatus(int contactId, UserStatus status);
    void updateUnreadCount(int contactId, int unreadCount);
    void setContacts(const QList<ContactInfo>& contacts);
    void clear();

    // 辅助方法
    int getContactIndex(int contactId) const;
    ContactInfo getContact(int contactId) const;
    QList<ContactInfo> contacts() const { return m_contacts; }

signals:
    void rowCountChanged();

private:
    QList<ContactInfo> m_contacts;
    QHash<int, QByteArray> m_roleNames;

    QString getStatusText(UserStatus status) const;
    QIcon getStatusIcon(UserStatus status) const;
};

#endif // CONTACTMODEL_H
