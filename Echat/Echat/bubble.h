#ifndef BUBBLE_H
#define BUBBLE_H

#include <QWidget>
#include <QTextDocument>
#include <QTimer>
#include <QHBoxLayout>
#include <QDebug>
#include "Protocol.h"
#include "datamanager.h"
#include "business.h"

namespace Ui {
class Bubble;
}

class Bubble : public QWidget
{
    Q_OBJECT

public:
    explicit Bubble(QWidget *parent = nullptr);
    ~Bubble();

    void setHeadImage(USER_INFO &user_info);
    QSize calculateTextSize() const;
    QSize calculateBubbleSize(QSize &text_size, int parent_width) const; // 新增：计算气泡大小
    void setContent(const QString &text, int parent_width);
    QString getContent();
    void setHeadImage(QPixmap &avatarPixmap);
    void setHeadImage(GROUP_INFO &group_info);

    void setBubble(int parent_width, QString name="");

    enum BubbleType {
        Other,  // 对方消息
        Self    // 自己消息
    };
    void setBubbleType(BubbleType type);  // 新增：设置消息类型
    void updateLayout();

    ChatMessage m_chat_msg;

private:
    Ui::Bubble *ui;
    BubbleType m_bubbleType;  // 记录消息类型
    DataManager& m_dataManager;
};

#endif // BUBBLE_H
