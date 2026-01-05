// advancedbubble.cpp
#include "advancedbubble.h"
#include <QPainter>
#include <QPainterPath>
#include <QDateTime>

AdvancedBubble::AdvancedBubble(QWidget *parent, BubbleType type, bool isOwn, const QString &content)
    : QWidget(parent)
    , m_type(type)
    , m_isOwn(isOwn)
{
    // 设置气泡颜色
    if (m_isOwn) {
        m_bubbleColor = QColor(149, 236, 105);  // 自己消息 - 绿色
    } else {
        m_bubbleColor = QColor(255, 255, 255);  // 对方消息 - 白色
    }

    setMessage(content);

    // 设置时间
    setTime(QDateTime::currentDateTime().toString("hh:mm"));
}


void AdvancedBubble::setMessage(const QString &message)
{
    m_contentLabel->setText(message);

    // 根据文本内容调整大小
    QFontMetrics fm(m_contentLabel->font());
    int textWidth = fm.horizontalAdvance(message);
    int maxWidth = 280;

    if (textWidth > maxWidth) {
        m_contentLabel->setFixedWidth(maxWidth);
    } else {
        m_contentLabel->setFixedWidth(qMin(textWidth + 30, maxWidth));
    }

    adjustSize();
}

//void AdvancedBubble::setAvatar(const QPixmap &avatar)
//{
//    m_avatar = avatar;
//    if (!avatar.isNull()) {
//        QPixmap scaledAvatar = avatar.scaled(38, 38, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        m_avatarLabel->setPixmap(scaledAvatar);
//    }
//}

void AdvancedBubble::setTime(const QString &time)
{
    m_timeLabel->setText(time);
}

void AdvancedBubble::setUserName(const QString &name)
{
    if (m_nameLabel && !m_isOwn) {
        m_nameLabel->setText(name);
    }
}
