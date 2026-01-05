// advancedbubble.h
#ifndef ADVANCEDBUBBLE_H
#define ADVANCEDBUBBLE_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

class AdvancedBubble : public QWidget
{
    Q_OBJECT

public:
    enum BubbleType {
        TextMessage,
        ImageMessage,
        FileMessage
    };

    explicit AdvancedBubble(QWidget *parent = nullptr,
                          BubbleType type = TextMessage,
                          bool isOwn = true,
                          const QString &content = "");

    void setMessage(const QString &message);
    void setAvatar(const QPixmap &avatar);
    void setTime(const QString &time);
    void setUserName(const QString &name);

private:
    BubbleType m_type;
    bool m_isOwn;  // 是否是自己发送的消息

//    QLabel *m_avatarLabel;
    QLabel *m_contentLabel;
    QLabel *m_timeLabel;
    QLabel *m_nameLabel;

    QColor m_bubbleColor;
    QPixmap m_avatar;
};

#endif // ADVANCEDBUBBLE_H
