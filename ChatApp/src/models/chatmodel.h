// src/models/chatmodel.h
#ifndef CHATMODEL_H
#define CHATMODEL_H

#include <QAbstractTableModel>
#include <QHash>
#include <QList>
#include "src/utils/global.h"

class ChatModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(int currentUserId READ currentUserId WRITE setCurrentUserId NOTIFY currentUserIdChanged)
    Q_PROPERTY(ChatType chatType READ chatType WRITE setChatType NOTIFY chatTypeChanged)
    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

public:
    enum Role {
        IdRole = Qt::UserRole + 1,
        SenderIdRole,
        ReceiverIdRole,
        ContentRole,
        TimestampRole,
        MessageTypeRole,
        StatusRole,
        IsMeRole,
        ChatTypeRole
    };

    explicit ChatModel(QObject* parent = nullptr);
    ~ChatModel();

    // QAbstractTableModel接口实现
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // 自定义方法
    void addMessage(const ChatMessage& message);
    void addMessages(const QList<ChatMessage>& messages);
    void updateMessageStatus(int messageId, MessageStatus status);
    void setCurrentUserId(int userId);
    void setChatType(ChatType type);
    void clear();

    // Getter方法
    int currentUserId() const { return m_currentUserId; }
    ChatType chatType() const { return m_chatType; }
    QList<ChatMessage> messages() const { return m_messages; }

signals:
    void currentUserIdChanged();
    void chatTypeChanged();
    void rowCountChanged();

private:
    QList<ChatMessage> m_messages;
    QHash<int, QByteArray> m_roleNames;
    int m_currentUserId;
    ChatType m_chatType;

    QString getStatusText(MessageStatus status) const;
};

#endif // CHATMODEL_H
