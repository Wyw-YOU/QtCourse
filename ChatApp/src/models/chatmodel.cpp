// src/models/chatmodel.cpp
#include "chatmodel.h"
#include "src/utils/logger.h"
#include <QDateTime>
#include <QBrush>
#include <QFont>

ChatModel::ChatModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_chatType(ChatType_Single)
{
    // 初始化角色名称
    m_roleNames[IdRole] = "messageId";
    m_roleNames[SenderIdRole] = "senderId";
    m_roleNames[ReceiverIdRole] = "receiverId";
    m_roleNames[ContentRole] = "content";
    m_roleNames[TimestampRole] = "timestamp";
    m_roleNames[MessageTypeRole] = "messageType";
    m_roleNames[StatusRole] = "status";
    m_roleNames[IsMeRole] = "isMe";
    m_roleNames[ChatTypeRole] = "chatType";

    LOG_DEBUG("ChatModel", "ChatModel initialized");
}

ChatModel::~ChatModel()
{
    clear();
}

int ChatModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_messages.size();
}

int ChatModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1; // 只有一列，显示消息内容
}

QVariant ChatModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_messages.size()) {
        return QVariant();
    }

    const ChatMessage& message = m_messages.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return message.content;
    case Qt::ToolTipRole:
        return QString("发送时间: %1\n状态: %2")
            .arg(message.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
            .arg(getStatusText(message.status));
    case IdRole:
        return message.id;
    case SenderIdRole:
        return message.senderId;
    case ReceiverIdRole:
        return message.receiverId;
    case ContentRole:
        return message.content;
    case TimestampRole:
        return message.timestamp;
    case MessageTypeRole:
        return static_cast<int>(message.messageType);
    case StatusRole:
        return static_cast<int>(message.status);
    case IsMeRole:
        return m_currentUserId == message.senderId;
    case ChatTypeRole:
        return static_cast<int>(message.chatType);
    case Qt::TextAlignmentRole:
        return (m_currentUserId == message.senderId) ?
                   Qt::AlignRight : Qt::AlignLeft;
    case Qt::ForegroundRole:
        if (m_currentUserId == message.senderId) {
            return QBrush(QColor("#0066CC")); // 自己发的消息颜色
        } else {
            return QBrush(QColor("#333333")); // 别人发的消息颜色
        }
    case Qt::FontRole:
        if (message.messageType == MessageType_System) {
            QFont font;
            font.setItalic(true);
            return font;
        }
        break;
    }

    return QVariant();
}

QVariant ChatModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return "消息";
        default:
            return QVariant();
        }
    } else {
        return QString::number(section + 1);
    }
}

QHash<int, QByteArray> ChatModel::roleNames() const
{
    return m_roleNames;
}

void ChatModel::addMessage(const ChatMessage& message)
{
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append(message);
    endInsertRows();

    LOG_DEBUG("ChatModel", QString("Message added: ID=%1").arg(message.id));
}

void ChatModel::addMessages(const QList<ChatMessage>& messages)
{
    if (messages.isEmpty()) {
        return;
    }

    beginInsertRows(QModelIndex(), m_messages.size(),
                    m_messages.size() + messages.size() - 1);
    m_messages.append(messages);
    endInsertRows();

    LOG_DEBUG("ChatModel", QString("%1 messages added").arg(messages.size()));
}

void ChatModel::updateMessageStatus(int messageId, MessageStatus status)
{
    for (int i = 0; i < m_messages.size(); ++i) {
        if (m_messages[i].id == messageId) {
            m_messages[i].status = status;

            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index, {StatusRole});

            LOG_DEBUG("ChatModel", QString("Message status updated: ID=%1, status=%2")
                                       .arg(messageId).arg(static_cast<int>(status)));
            return;
        }
    }

    LOG_WARNING("ChatModel", QString("Message not found for status update: ID=%1")
                                 .arg(messageId));
}

void ChatModel::setCurrentUserId(int userId)
{
    if (m_currentUserId != userId) {
        m_currentUserId = userId;

        // 通知视图数据已更改
        if (!m_messages.isEmpty()) {
            QModelIndex first = createIndex(0, 0);
            QModelIndex last = createIndex(m_messages.size() - 1, 0);
            emit dataChanged(first, last, {IsMeRole, Qt::TextAlignmentRole, Qt::ForegroundRole});
        }
    }
}

void ChatModel::setChatType(ChatType type)
{
    if (m_chatType != type) {
        m_chatType = type;
        emit chatTypeChanged();
    }
}

void ChatModel::clear()
{
    if (!m_messages.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_messages.size() - 1);
        m_messages.clear();
        endRemoveRows();

        LOG_DEBUG("ChatModel", "All messages cleared");
    }
}

QString ChatModel::getStatusText(MessageStatus status) const
{
    switch (status) {
    case MessageStatus_Sending:
        return "发送中";
    case MessageStatus_Sent:
        return "已发送";
    case MessageStatus_Received:
        return "已送达";
    case MessageStatus_Read:
        return "已读";
    default:
        return "未知";
    }
}
