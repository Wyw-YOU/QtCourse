// src/database/dbmanager.cpp
#include "dbmanager.h"
#include "src/utils/logger.h"
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QCryptographicHash>

DatabaseManager::DatabaseManager(QObject* parent)
    : QObject(parent)
    , m_initialized(false)
{
    // 设置缓存清理定时器
    m_cacheTimer.setInterval(300000); // 5分钟清理一次缓存
    connect(&m_cacheTimer, &QTimer::timeout, this, &DatabaseManager::clearCache);
    m_cacheTimer.start();
}

DatabaseManager::~DatabaseManager()
{
    onAppExit();
}

bool DatabaseManager::initialize(const QString& dbPath)
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized) {
        return true;
    }

    // 确定数据库路径
    if (dbPath.isEmpty()) {
        m_databasePath = Utils::getDataPath() + "/" + DATABASE_NAME;
    } else {
        m_databasePath = dbPath;
    }

    LOG_INFO("Database", QString("Initializing database: %1").arg(m_databasePath));

    // 确保目录存在
    QFileInfo fileInfo(m_databasePath);
    QDir dir = fileInfo.dir();
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            LOG_ERROR("Database", "Failed to create database directory");
            return false;
        }
    }

    // 创建数据库连接
    m_database = QSqlDatabase::addDatabase("QSQLITE", "main_connection");
    m_database.setDatabaseName(m_databasePath);

    if (!m_database.open()) {
        LOG_ERROR("Database", QString("Failed to open database: %1").arg(m_database.lastError().text()));
        return false;
    }

    // 设置数据库参数
    QSqlQuery query(m_database);
    query.exec("PRAGMA foreign_keys = ON");
    query.exec("PRAGMA journal_mode = WAL");
    query.exec("PRAGMA synchronous = NORMAL");
    query.exec("PRAGMA cache_size = 10000");

    // 创建表
    if (!createTables()) {
        LOG_ERROR("Database", "Failed to create tables");
        m_database.close();
        return false;
    }

    // 创建索引
    if (!createIndexes()) {
        LOG_WARNING("Database", "Failed to create indexes, but continuing");
    }

    // 创建触发器
    if (!createTriggers()) {
        LOG_WARNING("Database", "Failed to create triggers, but continuing");
    }

    m_initialized = true;
    LOG_INFO("Database", "Database initialized successfully");

    return true;
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(m_database);

    // 创建用户表
    QString createUserTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username VARCHAR(50) UNIQUE NOT NULL,
            password_hash VARCHAR(64) NOT NULL,
            nickname VARCHAR(50),
            avatar TEXT,
            signature TEXT,
            status INTEGER DEFAULT 0,
            last_online TIMESTAMP,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    )";

    if (!query.exec(createUserTable)) {
        LOG_ERROR("Database", QString("Failed to create users table: %1").arg(query.lastError().text()));
        return false;
    }

    // 创建联系人表
    QString createContactTable = R"(
        CREATE TABLE IF NOT EXISTS contacts (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            contact_id INTEGER NOT NULL,
            name VARCHAR(50),
            group_name VARCHAR(50) DEFAULT '默认分组',
            avatar TEXT,
            status INTEGER DEFAULT 0,
            unread_count INTEGER DEFAULT 0,
            last_message TEXT,
            last_message_time TIMESTAMP,
            is_favorite BOOLEAN DEFAULT 0,
            remark TEXT,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(user_id, contact_id),
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY(contact_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";

    if (!query.exec(createContactTable)) {
        LOG_ERROR("Database", QString("Failed to create contacts table: %1").arg(query.lastError().text()));
        return false;
    }

    // 创建消息表
    QString createMessageTable = R"(
        CREATE TABLE IF NOT EXISTS messages (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            session_id VARCHAR(100) NOT NULL,
            sender_id INTEGER NOT NULL,
            receiver_id INTEGER NOT NULL,
            chat_type INTEGER NOT NULL,
            content TEXT NOT NULL,
            message_type INTEGER NOT NULL,
            status INTEGER DEFAULT 0,
            timestamp TIMESTAMP NOT NULL,
            local_id VARCHAR(100),
            server_id VARCHAR(100),
            is_sent BOOLEAN DEFAULT 0,
            is_read BOOLEAN DEFAULT 0,
            extra_data TEXT,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(sender_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY(receiver_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";

    if (!query.exec(createMessageTable)) {
        LOG_ERROR("Database", QString("Failed to create messages table: %1").arg(query.lastError().text()));
        return false;
    }

    // 创建聊天会话表
    QString createSessionTable = R"(
        CREATE TABLE IF NOT EXISTS chat_sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            contact_id INTEGER NOT NULL,
            chat_type INTEGER NOT NULL,
            last_message TEXT,
            last_message_time TIMESTAMP,
            unread_count INTEGER DEFAULT 0,
            is_pinned BOOLEAN DEFAULT 0,
            is_muted BOOLEAN DEFAULT 0,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
            UNIQUE(user_id, contact_id, chat_type),
            FOREIGN KEY(user_id) REFERENCES users(id) ON DELETE CASCADE,
            FOREIGN KEY(contact_id) REFERENCES users(id) ON DELETE CASCADE
        )
    )";

    if (!query.exec(createSessionTable)) {
        LOG_ERROR("Database", QString("Failed to create chat_sessions table: %1").arg(query.lastError().text()));
        return false;
    }

    LOG_INFO("Database", "All tables created successfully");
    return true;
}

bool DatabaseManager::createIndexes()
{
    QSqlQuery query(m_database);

    // 为消息表创建索引
    QStringList indexes = {
        "CREATE INDEX IF NOT EXISTS idx_messages_session_id ON messages(session_id)",
        "CREATE INDEX IF NOT EXISTS idx_messages_sender_id ON messages(sender_id)",
        "CREATE INDEX IF NOT EXISTS idx_messages_receiver_id ON messages(receiver_id)",
        "CREATE INDEX IF NOT EXISTS idx_messages_timestamp ON messages(timestamp)",
        "CREATE INDEX IF NOT EXISTS idx_messages_status ON messages(status)",
        "CREATE INDEX IF NOT EXISTS idx_contacts_user_id ON contacts(user_id)",
        "CREATE INDEX IF NOT EXISTS idx_contacts_contact_id ON contacts(contact_id)",
        "CREATE INDEX IF NOT EXISTS idx_chat_sessions_user_id ON chat_sessions(user_id)",
        "CREATE INDEX IF NOT EXISTS idx_chat_sessions_contact_id ON chat_sessions(contact_id)"
    };

    for (const QString& sql : indexes) {
        if (!query.exec(sql)) {
            LOG_WARNING("Database", QString("Failed to create index: %1").arg(query.lastError().text()));
            // 继续执行其他索引创建
        }
    }

    return true;
}

bool DatabaseManager::createTriggers()
{
    QSqlQuery query(m_database);

    // 触发器：更新会话的最后消息
    QString triggerSessionUpdate = R"(
        CREATE TRIGGER IF NOT EXISTS trg_update_session_after_message
        AFTER INSERT ON messages
        BEGIN
            INSERT OR REPLACE INTO chat_sessions
            (user_id, contact_id, chat_type, last_message, last_message_time, unread_count, updated_at)
            VALUES
            (
                NEW.receiver_id,
                NEW.sender_id,
                NEW.chat_type,
                NEW.content,
                NEW.timestamp,
                CASE WHEN NEW.is_read = 0 THEN 1 ELSE 0 END,
                CURRENT_TIMESTAMP
            );

            -- 同时更新联系人的最后消息
            UPDATE contacts
            SET last_message = NEW.content,
                last_message_time = NEW.timestamp,
                unread_count = unread_count + CASE WHEN NEW.is_read = 0 THEN 1 ELSE 0 END,
                updated_at = CURRENT_TIMESTAMP
            WHERE user_id = NEW.receiver_id AND contact_id = NEW.sender_id;
        END
    )";

    if (!query.exec(triggerSessionUpdate)) {
        LOG_WARNING("Database", QString("Failed to create trigger: %1").arg(query.lastError().text()));
    }

    return true;
}

// 用户相关操作
bool DatabaseManager::createUser(const UserInfo& user, const QString& password)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    QString passwordHash = QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());

    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT INTO users (username, password_hash, nickname, avatar, signature, status, last_online)
        VALUES (:username, :password_hash, :nickname, :avatar, :signature, :status, :last_online)
    )");

    query.bindValue(":username", user.username);
    query.bindValue(":password_hash", passwordHash);
    query.bindValue(":nickname", user.nickname.isEmpty() ? user.username : user.nickname);
    query.bindValue(":avatar", user.avatar);
    query.bindValue(":signature", user.signature);
    query.bindValue(":status", static_cast<int>(user.status));
    query.bindValue(":last_online", user.lastOnline.isValid() ? user.lastOnline : QDateTime::currentDateTime());

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to create user: %1").arg(query.lastError().text()));
        return false;
    }

    LOG_INFO("Database", QString("User created: %1").arg(user.username));
    return true;
}

UserInfo DatabaseManager::getUserById(int userId)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return UserInfo();

    // 检查缓存
    if (m_userCache.contains(userId)) {
        return m_userCache[userId];
    }

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT id, username, nickname, avatar, signature, status, last_online
        FROM users WHERE id = :id
    )");
    query.bindValue(":id", userId);

    if (!query.exec() || !query.next()) {
        return UserInfo();
    }

    UserInfo user;
    user.id = query.value("id").toInt();
    user.username = query.value("username").toString();
    user.nickname = query.value("nickname").toString();
    user.avatar = query.value("avatar").toString();
    user.signature = query.value("signature").toString();
    user.status = static_cast<UserStatus>(query.value("status").toInt());
    user.lastOnline = query.value("last_online").toDateTime();

    // 更新缓存
    m_userCache[userId] = user;

    return user;
}

// 联系人相关操作
bool DatabaseManager::addContact(const ContactInfo& contact)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT OR REPLACE INTO contacts
        (user_id, contact_id, name, group_name, avatar, status, remark)
        VALUES (:user_id, :contact_id, :name, :group_name, :avatar, :status, :remark)
    )");

    query.bindValue(":user_id", contact.id); // 这里假设contact.id是用户ID
    query.bindValue(":contact_id", contact.id); // 需要从参数传递
    query.bindValue(":name", contact.name);
    query.bindValue(":group_name", contact.group);
    query.bindValue(":avatar", contact.avatar);
    query.bindValue(":status", static_cast<int>(contact.status));
    query.bindValue(":remark", contact.name); // 可以使用remark字段

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to add contact: %1").arg(query.lastError().text()));
        return false;
    }

    // 清除缓存
    m_contactCache.remove(contact.id);

    LOG_INFO("Database", QString("Contact added: %1").arg(contact.name));
    emit contactUpdated(contact);

    return true;
}

QList<ContactInfo> DatabaseManager::getAllContacts()
{
    QMutexLocker locker(&m_mutex);

    QList<ContactInfo> contacts;

    if (!m_initialized) return contacts;

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT c.id, c.contact_id, c.name, c.group_name, c.avatar, c.status,
               c.unread_count, c.last_message_time, u.username, u.nickname
        FROM contacts c
        LEFT JOIN users u ON c.contact_id = u.id
        ORDER BY c.is_favorite DESC, c.last_message_time DESC, c.name ASC
    )");

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to get contacts: %1").arg(query.lastError().text()));
        return contacts;
    }

    while (query.next()) {
        ContactInfo contact;
        contact.id = query.value("contact_id").toInt();
        contact.name = query.value("name").toString();
        if (contact.name.isEmpty()) {
            contact.name = query.value("nickname").toString();
            if (contact.name.isEmpty()) {
                contact.name = query.value("username").toString();
            }
        }
        contact.group = query.value("group_name").toString();
        contact.avatar = query.value("avatar").toString();
        if (contact.avatar.isEmpty()) {
            contact.avatar = ":/images/default_avatar.png";
        }
        contact.status = static_cast<UserStatus>(query.value("status").toInt());
        contact.unreadCount = query.value("unread_count").toInt();
        contact.lastMessageTime = query.value("last_message_time").toDateTime();

        contacts.append(contact);

        // 更新缓存
        m_contactCache[contact.id] = contact;
    }

    return contacts;
}

// 消息相关操作
bool DatabaseManager::saveMessage(const ChatMessage& message)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    // 生成会话ID
    QString sessionId = QString("%1_%2_%3")
                            .arg(qMin(message.senderId, message.receiverId))
                            .arg(qMax(message.senderId, message.receiverId))
                            .arg(static_cast<int>(message.chatType));

    QSqlQuery query(m_database);
    query.prepare(R"(
        INSERT INTO messages
        (session_id, sender_id, receiver_id, chat_type, content, message_type,
         status, timestamp, is_sent, is_read)
        VALUES
        (:session_id, :sender_id, :receiver_id, :chat_type, :content, :message_type,
         :status, :timestamp, :is_sent, :is_read)
    )");

    query.bindValue(":session_id", sessionId);
    query.bindValue(":sender_id", message.senderId);
    query.bindValue(":receiver_id", message.receiverId);
    query.bindValue(":chat_type", static_cast<int>(message.chatType));
    query.bindValue(":content", message.content);
    query.bindValue(":message_type", static_cast<int>(message.messageType));
    query.bindValue(":status", static_cast<int>(message.status));
    query.bindValue(":timestamp", message.timestamp.isValid() ? message.timestamp : QDateTime::currentDateTime());
    query.bindValue(":is_sent", message.status >= MessageStatus_Sent);
    query.bindValue(":is_read", message.status >= MessageStatus_Read);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to save message: %1").arg(query.lastError().text()));
        return false;
    }

    // 获取插入的ID
    int messageId = query.lastInsertId().toInt();

    // 更新缓存
    QString cacheKey = QString("%1_%2").arg(message.senderId).arg(message.receiverId);
    if (!m_messageCache.contains(cacheKey)) {
        m_messageCache[cacheKey] = QList<ChatMessage>();
    }

    ChatMessage savedMessage = message;
    savedMessage.id = messageId;
    m_messageCache[cacheKey].append(savedMessage);

    // 限制缓存大小
    if (m_messageCache[cacheKey].size() > 100) {
        m_messageCache[cacheKey].removeFirst();
    }

    LOG_DEBUG("Database", QString("Message saved: ID=%1, Content=%2").arg(messageId).arg(message.content.left(50)));
    emit messageSaved(savedMessage);

    return true;
}

QList<ChatMessage> DatabaseManager::getMessages(int userId, int contactId, ChatType type, int limit)
{
    QMutexLocker locker(&m_mutex);

    QList<ChatMessage> messages;

    if (!m_initialized) return messages;

    // 检查缓存
    QString cacheKey = QString("%1_%2").arg(userId).arg(contactId);
    if (m_messageCache.contains(cacheKey) && limit <= 100) {
        // 从缓存中获取最新的limit条消息
        QList<ChatMessage> cached = m_messageCache[cacheKey];
        int start = qMax(0, cached.size() - limit);
        for (int i = start; i < cached.size(); i++) {
            messages.append(cached[i]);
        }
        return messages;
    }

    QString sessionId = QString("%1_%2_%3")
                            .arg(qMin(userId, contactId))
                            .arg(qMax(userId, contactId))
                            .arg(static_cast<int>(type));

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT id, sender_id, receiver_id, chat_type, content, message_type,
               status, timestamp, is_sent, is_read
        FROM messages
        WHERE session_id = :session_id
        ORDER BY timestamp DESC
        LIMIT :limit
    )");

    query.bindValue(":session_id", sessionId);
    query.bindValue(":limit", limit);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to get messages: %1").arg(query.lastError().text()));
        return messages;
    }

    while (query.next()) {
        ChatMessage message;
        message.id = query.value("id").toInt();
        message.senderId = query.value("sender_id").toInt();
        message.receiverId = query.value("receiver_id").toInt();
        message.chatType = static_cast<ChatType>(query.value("chat_type").toInt());
        message.content = query.value("content").toString();
        message.messageType = static_cast<MessageType>(query.value("message_type").toInt());
        message.status = static_cast<MessageStatus>(query.value("status").toInt());
        message.timestamp = query.value("timestamp").toDateTime();

        messages.append(message);
    }

    // 反转顺序，使时间顺序为正序
    std::reverse(messages.begin(), messages.end());

    return messages;
}

// 会话相关操作
QList<QPair<int, ChatMessage>> DatabaseManager::getRecentChatSessions(int userId)
{
    QMutexLocker locker(&m_mutex);

    QList<QPair<int, ChatMessage>> sessions;

    if (!m_initialized) return sessions;

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT cs.contact_id, cs.last_message, cs.last_message_time, cs.unread_count,
               cs.chat_type, u.nickname, u.avatar, u.status
        FROM chat_sessions cs
        LEFT JOIN users u ON cs.contact_id = u.id
        WHERE cs.user_id = :user_id
        ORDER BY cs.last_message_time DESC
        LIMIT 50
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to get chat sessions: %1").arg(query.lastError().text()));
        return sessions;
    }

    while (query.next()) {
        ChatMessage lastMessage;
        lastMessage.senderId = query.value("contact_id").toInt();
        lastMessage.receiverId = userId;
        lastMessage.content = query.value("last_message").toString();
        lastMessage.timestamp = query.value("last_message_time").toDateTime();
        lastMessage.chatType = static_cast<ChatType>(query.value("chat_type").toInt());

        int contactId = query.value("contact_id").toInt();
        int unreadCount = query.value("unread_count").toInt();

        sessions.append(qMakePair(contactId, lastMessage));
    }

    return sessions;
}

// 数据库维护
bool DatabaseManager::clearOldMessages(int days)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    QSqlQuery query(m_database);
    query.prepare(R"(
        DELETE FROM messages
        WHERE timestamp < datetime('now', '-' || :days || ' days')
        AND id NOT IN (
            SELECT id FROM messages
            WHERE timestamp >= datetime('now', '-' || :days || ' days')
            ORDER BY timestamp DESC
            LIMIT 1000
        )
    )");

    query.bindValue(":days", days);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to clear old messages: %1").arg(query.lastError().text()));
        return false;
    }

    int deletedRows = query.numRowsAffected();
    LOG_INFO("Database", QString("Cleared %1 old messages").arg(deletedRows));

    // 清理缓存
    clearCache();

    return true;
}

bool DatabaseManager::compactDatabase()
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    QSqlQuery query(m_database);
    if (!query.exec("VACUUM")) {
        LOG_ERROR("Database", QString("Failed to compact database: %1").arg(query.lastError().text()));
        return false;
    }

    LOG_INFO("Database", "Database compacted successfully");
    return true;
}

qint64 DatabaseManager::getDatabaseSize() const
{
    QFileInfo fileInfo(m_databasePath);
    return fileInfo.size();
}

void DatabaseManager::onAppExit()
{
    QMutexLocker locker(&m_mutex);

    if (m_initialized) {
        // 执行清理操作
        m_cacheTimer.stop();
        clearCache();

        // 关闭数据库连接
        if (m_database.isOpen()) {
            m_database.close();
        }

        m_initialized = false;
        LOG_INFO("Database", "Database closed");
    }
}

void DatabaseManager::clearCache()
{
    QMutexLocker locker(&m_mutex);

    m_userCache.clear();
    m_contactCache.clear();
    m_messageCache.clear();

    LOG_DEBUG("Database", "Cache cleared");
}

// 事务执行
bool DatabaseManager::executeTransaction(const std::function<bool(QSqlDatabase&)>& operation)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    bool success = false;

    if (m_database.transaction()) {
        try {
            success = operation(m_database);

            if (success) {
                if (!m_database.commit()) {
                    LOG_ERROR("Database", "Failed to commit transaction");
                    success = false;
                }
            } else {
                m_database.rollback();
            }
        } catch (...) {
            m_database.rollback();
            LOG_ERROR("Database", "Exception in transaction, rolled back");
            success = false;
        }
    } else {
        LOG_ERROR("Database", "Failed to start transaction");
        success = false;
    }

    return success;
}

// 其他方法的实现...
bool DatabaseManager::updateMessageStatus(int messageId, MessageStatus status)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    QSqlQuery query(m_database);
    query.prepare(R"(
        UPDATE messages
        SET status = :status,
            is_sent = CASE WHEN :status >= 1 THEN 1 ELSE is_sent END,
            is_read = CASE WHEN :status >= 3 THEN 1 ELSE is_read END
        WHERE id = :id
    )");

    query.bindValue(":status", static_cast<int>(status));
    query.bindValue(":id", messageId);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to update message status: %1").arg(query.lastError().text()));
        return false;
    }

    return true;
}

QList<ChatMessage> DatabaseManager::getUnreadMessages(int userId)
{
    QMutexLocker locker(&m_mutex);

    QList<ChatMessage> messages;

    if (!m_initialized) return messages;

    QSqlQuery query(m_database);
    query.prepare(R"(
        SELECT id, sender_id, receiver_id, chat_type, content, message_type,
               status, timestamp
        FROM messages
        WHERE receiver_id = :user_id AND is_read = 0
        ORDER BY timestamp ASC
    )");

    query.bindValue(":user_id", userId);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to get unread messages: %1").arg(query.lastError().text()));
        return messages;
    }

    while (query.next()) {
        ChatMessage message;
        message.id = query.value("id").toInt();
        message.senderId = query.value("sender_id").toInt();
        message.receiverId = query.value("receiver_id").toInt();
        message.chatType = static_cast<ChatType>(query.value("chat_type").toInt());
        message.content = query.value("content").toString();
        message.messageType = static_cast<MessageType>(query.value("message_type").toInt());
        message.status = static_cast<MessageStatus>(query.value("status").toInt());
        message.timestamp = query.value("timestamp").toDateTime();

        messages.append(message);
    }

    return messages;
}

bool DatabaseManager::markMessagesAsRead(int contactId)
{
    QMutexLocker locker(&m_mutex);

    if (!m_initialized) return false;

    QSqlQuery query(m_database);
    query.prepare(R"(
        UPDATE messages
        SET status = 3, is_read = 1
        WHERE receiver_id = :receiver_id AND sender_id = :sender_id AND is_read = 0
    )");

    // 这里需要当前用户ID，暂时使用参数
    query.bindValue(":receiver_id", 1); // 需要从应用管理器获取当前用户ID
    query.bindValue(":sender_id", contactId);

    if (!query.exec()) {
        LOG_ERROR("Database", QString("Failed to mark messages as read: %1").arg(query.lastError().text()));
        return false;
    }

    // 更新联系人未读计数
    query.prepare(R"(
        UPDATE contacts
        SET unread_count = 0
        WHERE contact_id = :contact_id
    )");
    query.bindValue(":contact_id", contactId);
    query.exec();

    return true;
}
