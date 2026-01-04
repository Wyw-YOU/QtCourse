// src/gui/chatwidget.cpp
#include "chatwidget.h"
#include "ui_chatwidget.h"
#include "src/utils/logger.h"
#include <QDateTime>
#include <QFileDialog>
#include <QKeyEvent>
#include <QScrollBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

ChatWidget::ChatWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ChatWidget)
    , m_contactId(-1)
    , m_chatType(ChatType_Single)
{
    ui->setupUi(this);

    // 连接信号槽
    connect(ui->btnSend, &QPushButton::clicked, this, &ChatWidget::on_btnSend_clicked);
    connect(ui->btnFile, &QPushButton::clicked, this, &ChatWidget::on_btnFile_clicked);
    connect(ui->btnEmoji, &QPushButton::clicked, this, &ChatWidget::on_btnEmoji_clicked);
    connect(ui->btnHistory, &QPushButton::clicked, this, &ChatWidget::on_btnHistory_clicked);
    connect(ui->textEditInput, &QTextEdit::textChanged, this, &ChatWidget::on_textEditInput_textChanged);

    // 初始化输入状态标签为隐藏
    ui->labelInputStatus->setVisible(false);

    LOG_DEBUG("ChatWidget", "ChatWidget initialized");
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

void ChatWidget::setChatInfo(int contactId, const QString& contactName,
                             const QString& avatar, ChatType type)
{
    m_contactId = contactId;
    m_contactName = contactName;
    m_chatType = type;

    // 更新UI
    ui->chatTitleLabel->setText(contactName);

    if (!avatar.isEmpty()) {
        // 这里可以设置头像
    }

    // 清空消息列表
    ui->listWidgetMessages->clear();
    m_messageItems.clear();

    LOG_DEBUG("ChatWidget", QString("Chat info set: contactId=%1, name=%2, type=%3")
                                .arg(contactId).arg(contactName).arg(static_cast<int>(type)));
}

void ChatWidget::addMessage(const ChatMessage& message, bool isMe)
{
    QString timeStr = message.timestamp.toString("hh:mm");
    QString content = message.content;

    // 创建简单的消息显示
    QString displayText;
    if (isMe) {
        displayText = QString("[我 %1] %2").arg(timeStr).arg(content);
    } else {
        displayText = QString("[%1 %2] %3").arg(m_contactName).arg(timeStr).arg(content);
    }

    QListWidgetItem* item = new QListWidgetItem(displayText);

    // 设置对齐方式
    if (isMe) {
        item->setTextAlignment(Qt::AlignRight);
    } else {
        item->setTextAlignment(Qt::AlignLeft);
    }

    ui->listWidgetMessages->addItem(item);

    // 滚动到底部
    ui->listWidgetMessages->scrollToBottom();

    LOG_DEBUG("ChatWidget", QString("Message added: %1").arg(content.left(30)));
}

void ChatWidget::updateMessageStatus(int messageId, MessageStatus status)
{
    // 简化实现，暂时不处理消息状态更新
    Q_UNUSED(messageId);
    Q_UNUSED(status);
}

void ChatWidget::loadHistory(const QList<ChatMessage>& messages)
{
    // 清空当前消息
    ui->listWidgetMessages->clear();
    m_messageItems.clear();

    // 添加历史消息
    for (const ChatMessage& message : messages) {
        // 这里需要知道当前用户ID来判断是否是自己的消息
        // 简化处理，都显示为对方消息
        addMessage(message, false);
    }

    LOG_DEBUG("ChatWidget", QString("%1 history messages loaded").arg(messages.size()));
}

void ChatWidget::onMessageReceived(const ChatMessage& message)
{
    if (message.senderId == m_contactId) {
        addMessage(message, false);
    }
}

void ChatWidget::onMessageSent(const ChatMessage& message)
{
    // 消息发送成功，已经在发送时添加了
    // 这里可以更新消息状态
}

void ChatWidget::onMessageSendFailed(int messageId, const QString& error)
{
    Q_UNUSED(messageId);
    LOG_ERROR("ChatWidget", QString("Message send failed: %1").arg(error));
}

void ChatWidget::onUserStatusUpdated(int userId, UserStatus status)
{
    if (userId == m_contactId) {
        QString statusText;
        switch (status) {
        case UserStatus_Online:
            statusText = "在线";
            break;
        case UserStatus_Offline:
            statusText = "离线";
            break;
        case UserStatus_Busy:
            statusText = "忙碌";
            break;
        case UserStatus_Away:
            statusText = "离开";
            break;
        default:
            statusText = "未知";
        }

        ui->chatStatusLabel->setText(statusText);
    }
}

void ChatWidget::on_btnSend_clicked()
{
    QString content = ui->textEditInput->toPlainText().trimmed();

    if (content.isEmpty()) {
        return;
    }

    if (m_contactId == -1) {
        LOG_WARNING("ChatWidget", "Cannot send message: no contact selected");
        return;
    }

    // 创建本地消息
    ChatMessage message;
    message.senderId = 1; // 假设当前用户ID为1
    message.receiverId = m_contactId;
    message.content = content;
    message.messageType = MessageType_Text;
    message.chatType = m_chatType;
    message.status = MessageStatus_Sending;
    message.timestamp = QDateTime::currentDateTime();

    // 添加到UI（作为自己的消息）
    addMessage(message, true);

    // 发出发送消息的信号
    emit sendMessageRequested(m_contactId, content, m_chatType);

    // 清空输入框
    ui->textEditInput->clear();

    LOG_DEBUG("ChatWidget", QString("Message sent: %1").arg(content.left(50)));
}

void ChatWidget::on_textEditInput_textChanged()
{
    QString text = ui->textEditInput->toPlainText();

    // 更新输入状态
    if (text.isEmpty()) {
        ui->labelInputStatus->setVisible(false);
    } else {
        ui->labelInputStatus->setText(QString("已输入 %1 个字").arg(text.length()));
        ui->labelInputStatus->setVisible(true);
    }

    // 根据文本长度启用/禁用发送按钮
    ui->btnSend->setEnabled(!text.trimmed().isEmpty());
}

void ChatWidget::on_btnFile_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", QDir::homePath());

    if (!filePath.isEmpty()) {
        emit fileSendRequested(m_contactId, filePath);

        LOG_DEBUG("ChatWidget", QString("File selected: %1").arg(filePath));
    }
}

void ChatWidget::on_btnEmoji_clicked()
{
    // 在输入框中插入一个表情
    QTextCursor cursor = ui->textEditInput->textCursor();
    cursor.insertText("😊 ");
    ui->textEditInput->setTextCursor(cursor);
    ui->textEditInput->setFocus();

    LOG_DEBUG("ChatWidget", "Emoji button clicked");
}

void ChatWidget::on_btnHistory_clicked()
{
    LOG_DEBUG("ChatWidget", "History button clicked");
    // 这里可以显示聊天记录对话框
}

void ChatWidget::keyPressEvent(QKeyEvent* event)
{
    // 支持Ctrl+Enter发送消息
    if ((event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) &&
        (event->modifiers() & Qt::ControlModifier)) {
        on_btnSend_clicked();
        event->accept();
    } else {
        QWidget::keyPressEvent(event);
    }
}
