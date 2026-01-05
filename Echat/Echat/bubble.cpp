#include "bubble.h"
#include "ui_bubble.h"


Bubble::Bubble(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bubble),
    m_bubbleType(Other),  // 默认是对方消息
    m_dataManager(DataManager::instance())
{
    ui->setupUi(this);

}

Bubble::~Bubble()
{
    delete ui;
}

QString Bubble::getContent()
{
    return ui->content_label->text();
}

//void Bubble::setHeadImage(QString imgpath)
//{
//    ui->headimg_label->setStyleSheet(imgpath);
//}

void Bubble::setHeadImage(USER_INFO &user_info)
{
    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(user_info.avatar_data, user_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);
}

void Bubble::setHeadImage(GROUP_INFO &group_info)
{
    // 如果只有avatarData，先转换为QPixmap
    QByteArray avatarData(group_info.avatar_data, group_info.avatar_size);

    QPixmap avatarPixmap;
    avatarPixmap.loadFromData(avatarData);
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg_label->setPixmap(roundedAvatar);
}

void Bubble::setHeadImage(QPixmap &avatarPixmap)
{
    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg_label->width(), ui->headimg_label->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    ui->headimg_label->setPixmap(scaledAvatar);
    ui->headimg_label->setStyleSheet(
        "QLabel {"
        "   border-radius: 12px;"
        "}"
    );
}

void Bubble::setContent(const QString &text, int parent_width)
{
    ui->content_label->setText(text);

    int max_textwidth = parent_width - 300;
    ui->content_label->setMaximumWidth(max_textwidth);
    ui->content_label->setFont(QFont("Microsoft YaHei", 10)); // 明确指定字体

    // 计算精确的文本宽度
    QFontMetrics fm(ui->content_label->font());
    int textWidth = fm.horizontalAdvance(text);

    // 设置合适的宽度：文本实际宽度和最大宽度的较小值
    int optimalWidth = qMin(textWidth + 20, max_textwidth); // 加上内边距，不超过500

    // 重要：先重置尺寸限制，让标签自由计算
    ui->content_label->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    ui->content_label->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    ui->content_label->setFixedWidth(optimalWidth);
    ui->content_label->setWordWrap(textWidth + 20 > max_textwidth);

    // 强制更新布局
    ui->content_label->adjustSize();

    // 获取QLabel自己计算的大小（最准确）
    QSize labelSize = ui->content_label->sizeHint();
//    qDebug() << "最终气泡尺寸:" << labelSize;

    // 计算合适的大小并调整
    QSize newBubbleSize = calculateBubbleSize(labelSize, parent_width);
    this->setFixedSize(newBubbleSize);
    ui->content_label->setFixedSize(labelSize); // 根据布局调整
    ui->label->setFixedHeight(labelSize.height());
}

QSize Bubble::calculateBubbleSize(QSize &text_size, int parent_width) const
{
//    int textWidth = text_size.width();
    int textHeight = text_size.height();

    // 根据消息类型设置不同的最大宽度
    int maxWidth = (m_bubbleType == Self) ? parent_width-50 : parent_width-100;

//    int minWidth = 200;  // 最小宽度
    int minHeight = (m_bubbleType == Self) ? 90 : 111;   // 最小高度

    // 计算合适的宽度（考虑换行）
    int bubbleWidth = maxWidth;

    int bubbleHeight = (m_bubbleType == Self) ? qMax(textHeight + 50, minHeight) : qMax(textHeight + 70, minHeight);

    return QSize(bubbleWidth, bubbleHeight);
}

void Bubble::setBubbleType(BubbleType type)
{
    m_bubbleType = type;
    updateLayout();  // 更新布局
}

void Bubble::updateLayout()
{
    if (m_bubbleType == Self) {
        // 自己消息的布局：头像在右侧，消息在左侧
        // 调整水平布局的方向
        ui->horizontalLayout->setDirection(QHBoxLayout::RightToLeft);

        // 隐藏昵称（自己的消息不显示昵称）
        ui->name_label->setVisible(false);

        // 调整内容标签的对齐方式
//        ui->content_label->setAlignment(Qt::AlignLeft | Qt::AlignCenter);

        // 设置不同的背景色区分
        ui->content_label->setStyleSheet(
            "QLabel{"
                "background-color: rgb(42, 198, 255);"
                "border-radius:13px;"
                "padding:10px;"
            "}"
        );
    } else {
        // 对方消息的布局：头像在左侧，消息在右侧
        ui->horizontalLayout->setDirection(QHBoxLayout::LeftToRight);
        ui->name_label->setVisible(true);  // 显示昵称
//        ui->content_label->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
        ui->content_label->setStyleSheet(
            "QLabel{"
                "background-color: rgb(255, 255, 255);"
                "border-radius:13px;"
                "padding:10px;"
            "}"
        );
    }
}

void Bubble::setBubble(int parent_width, QString name)
{
    if(m_chat_msg.sender == m_dataManager.getCurrentUser().account){
        this->setBubbleType(BubbleType::Self);
    }else{
        ui->name_label->setText(name);
    }

    this->setContent(m_chat_msg.content, parent_width);
}
