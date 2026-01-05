#include "mybubble.h"
#include "ui_mybubble.h"

myBubble::myBubble(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myBubble)
{
    ui->setupUi(this);
}

myBubble::~myBubble()
{
    delete ui;
}

void myBubble::setHeadImage(QString imgpath)
{
    ui->headimg_label->setStyleSheet(imgpath);
}

void myBubble::setContent(const QString &text)
{
    ui->content_label->setText(text);

    ui->content_label->setMaximumWidth(500);
    ui->content_label->setFont(QFont("Microsoft YaHei", 10)); // 明确指定字体

    // 计算精确的文本宽度
    QFontMetrics fm(ui->content_label->font());
    int textWidth = fm.horizontalAdvance(text);

    // 设置合适的宽度：文本实际宽度和最大宽度的较小值
    int optimalWidth = qMin(textWidth + 20, 500); // 加上内边距，不超过500

    ui->content_label->setFixedWidth(optimalWidth);
    ui->content_label->setWordWrap(textWidth + 20 > 500);

    // 强制更新布局
    ui->content_label->adjustSize();

    // 获取QLabel自己计算的大小（最准确）
    QSize labelSize = ui->content_label->sizeHint();

    // 计算合适的大小并调整
    QSize newBubbleSize = calculateBubbleSize(labelSize);
    this->setFixedSize(newBubbleSize);
    ui->content_label->setFixedSize(labelSize); // 根据布局调整
}


QSize myBubble::calculateBubbleSize(QSize &text_size) const
{
//    int textWidth = text_size.width();
    int textHeight = text_size.height();

    // 设置最大最小限制
    int maxWidth = 890;  // 最大宽度
//    int minWidth = 200;  // 最小宽度
    int minHeight = 111;  // 最小高度

    // 计算合适的宽度（考虑换行）
    int bubbleWidth = maxWidth;

    int bubbleHeight = qMax(textHeight + 70, minHeight);

    return QSize(bubbleWidth, bubbleHeight);
}
