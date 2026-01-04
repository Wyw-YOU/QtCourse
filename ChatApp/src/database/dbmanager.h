// src/database/dbmanager.h
#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMutex>
#include <QThread>
#include <QTimer>
#include "src/utils/global.h"

class DatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseManager(QObject* parent = nullptr);
    ~DatabaseManager();

    // 初始化数据库
    bool initialize(const QString& dbPath = QString());

    // 用户相关操作
    bool createUser(const UserInfo& user, const QString& password);
    UserInfo getUserById(int userId);
    UserInfo getUserByUsername(const QString& username);
    bool updateUserInfo(const UserInfo& user);
    bool updateUserStatus(int userId, UserStatus status);

    // 联系人相关操作
    bool addContact(const ContactInfo& contact);
    bool removeContact(int contactId);
    bool updateContact(const ContactInfo& contact);
    QList<ContactInfo> getAllContacts();
    QList<ContactInfo> getContactsByGroup(const QString& group);
    ContactInfo getContactById(int contactId);

    // 消息相关操作
    bool saveMessage(const ChatMessage& message);
    bool updateMessageStatus(int messageId, MessageStatus status);
    QList<ChatMessage> getMessages(int userId, int contactId, ChatType type, int limit = 100);
    QList<ChatMessage> getUnreadMessages(int userId);
    int getUnreadCount(int contactId);
    bool markMessagesAsRead(int contactId);

    // 会话相关操作
    int getChatSessionId(int userId, int contactId, ChatType type);
    bool updateChatSession(int sessionId, const QString& lastMessage, const QDateTime& timestamp);
    QList<QPair<int, ChatMessage>> getRecentChatSessions(int userId);

    // 数据库维护
    bool clearOldMessages(int days = 30);
    bool compactDatabase();
    qint64 getDatabaseSize() const;

signals:
    void messageSaved(const ChatMessage& message);
    void contactUpdated(const ContactInfo& contact);
    void databaseError(const QString& error);

public slots:
    void onAppExit();

private:
    bool createTables();
    bool createIndexes();
    bool createTriggers();

    bool executeTransaction(const std::function<bool(QSqlDatabase&)>& operation);

    QSqlDatabase m_database;
    QString m_databasePath;
    QMutex m_mutex;
    bool m_initialized;

    // 数据库连接池（简化版）
    QList<QSqlDatabase> m_connectionPool;
    QMutex m_poolMutex;

    // 缓存机制
    QMap<int, UserInfo> m_userCache;
    QMap<int, ContactInfo> m_contactCache;
    QMap<QString, QList<ChatMessage>> m_messageCache;
    QTimer m_cacheTimer;

private slots:
    void clearCache();
};

#endif // DBMANAGER_H
