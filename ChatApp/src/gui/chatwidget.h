#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMap>
#include "src/utils/global.h"

namespace Ui {
class ChatWidget;
}

class ChatWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    void setChatInfo(int contactId, const QString &contactName,
                     const QString &avatar = QString(), ChatType type = ChatType_Single);
    void addMessage(const ChatMessage &message, bool isMe = false);
    void updateMessageStatus(int messageId, MessageStatus status);
    void loadHistory(const QList<ChatMessage> &messages);

signals:
    void sendMessageRequested(int receiverId, const QString &content, ChatType type);
    void fileSendRequested(int receiverId, const QString &filePath);
    void closeRequested();

public slots:
    void onMessageReceived(const ChatMessage &message);
    void onMessageSent(const ChatMessage &message);
    void onMessageSendFailed(int messageId, const QString &error);
    void onUserStatusUpdated(int userId, UserStatus status);

private slots:
    void on_btnSend_clicked();
    void on_textEditInput_textChanged();
    void on_btnFile_clicked();
    void on_btnEmoji_clicked();
    void on_btnHistory_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ChatWidget *ui;
    void setupMessageList();
    QListWidgetItem* createMessageItem(const ChatMessage &message, bool isMe);
    void scrollToBottom();

    int m_contactId;
    QString m_contactName;
    ChatType m_chatType;
    QMap<int, QListWidgetItem*> m_messageItems;
};

#endif // CHATWIDGET_H
