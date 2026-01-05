#include "custommessagebox.h"
#include <QPainter>
#include <QApplication>
#include <QScreen>
#include <QScrollBar>

CustomMessageBox::CustomMessageBox(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setAttribute(Qt::WA_TranslucentBackground);
}

void CustomMessageBox::setupUI()
{
    // 创建背景容器
    QWidget *bgWidget = new QWidget(this);
    bgWidget->setObjectName("bgWidget");

    // 设置样式
    bgWidget->setStyleSheet(
        "#bgWidget {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                               stop:0 #ffffff, stop:1 #f8f9fa);"
        "   border-radius: 15px;"
        "   border: 1px solid #e0e0e0;"
        "   font-family: 'Microsoft YaHei';"
        "}"
        "QLabel {"
        "   color: #495057;"
        "   font-size: 14px;"
        "   background: transparent;"
        "   font-family: 'Microsoft YaHei';"
        "}"
        "QTextEdit {"
        "   color: #666;"
        "   font-size: 13px;"
        "   background: transparent;"
        "   border: none;"
        "   padding: 0px;"
        "   font-family: 'Microsoft YaHei';"
        "}"
        "QTextEdit:focus {"
        "   border: none;"
        "   outline: none;"
        "}"
        "QPushButton {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                               stop:0 #4299e1, stop:1 #3182ce);"
        "   color: white;"
        "   border: none;"
        "   padding: 8px 20px;"
        "   border-radius: 8px;"
        "   font-size: 12px;"
        "   min-width: 70px;"
        "   font-family: 'Microsoft YaHei';"
        "}"
        "QPushButton:hover {"
        "   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                               stop:0 #87CEFA, stop:1 #87CEFA);"
        "}"
    );

    // 创建控件
    m_iconLabel = new QLabel(bgWidget);
    m_iconLabel->setFixedSize(40, 40);
    m_iconLabel->setAlignment(Qt::AlignCenter);

    m_titleLabel = new QLabel(bgWidget);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333; margin-bottom: 5px;");

    // 使用 QTextEdit 替代 QLabel，支持更好的文本显示
    m_textEdit = new QTextEdit(bgWidget);
    m_textEdit->setReadOnly(true);
    m_textEdit->setFrameStyle(QFrame::NoFrame);
    m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse);

    // 设置 QTextEdit 的样式
    m_textEdit->setStyleSheet(
        "QTextEdit {"
        "   background: transparent;"
        "   border: none;"
        "   color: #666;"
        "   font-size: 13px;"
        "   line-height: 140%;"
        "}"
        "QScrollBar:vertical {"
        "   border: none;"
        "   background: #f0f0f0;"
        "   width: 8px;"
        "   border-radius: 4px;"
        "}"
        "QScrollBar::handle:vertical {"
        "   background: #c0c0c0;"
        "   border-radius: 4px;"
        "   min-height: 20px;"
        "}"
        "QScrollBar::handle:vertical:hover {"
        "   background: #a0a0a0;"
        "}"
    );

    m_okButton = new QPushButton("确定", bgWidget);
    connect(m_okButton, &QPushButton::clicked, this, &CustomMessageBox::onOkClicked);

    // 布局
    QVBoxLayout *bgLayout = new QVBoxLayout(bgWidget);
    QHBoxLayout *iconLayout = new QHBoxLayout();
    iconLayout->addStretch();
    iconLayout->addWidget(m_iconLabel);
    iconLayout->addStretch();

    bgLayout->addLayout(iconLayout);
    bgLayout->addWidget(m_titleLabel);
    bgLayout->addWidget(m_textEdit);
    bgLayout->addWidget(m_okButton, 0, Qt::AlignCenter);
    bgLayout->setSpacing(10);
    bgLayout->setContentsMargins(25, 20, 25, 20);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(bgWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // 设置初始最小大小
    bgWidget->setMinimumSize(350, 180);
    setMinimumSize(350, 180);

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(25);
    shadow->setColor(QColor(0, 0, 0, 60));
    shadow->setOffset(0, 5);
    bgWidget->setGraphicsEffect(shadow);
}

void CustomMessageBox::setMessage(const QString &title, const QString &text, IconType icon)
{
    m_titleLabel->setText(title);

    // 使用 HTML 格式化文本，支持更好的换行和格式
    QString formattedText = text;
    formattedText.replace("\n", "<br>");
    m_textEdit->setHtml(QString("<div style='text-align: center; line-height: 140%;'>%1</div>").arg(formattedText));

    setIcon(icon);

    // 根据内容调整大小
    adjustSizeToContent();
}

void CustomMessageBox::adjustSizeToContent()
{
    // 计算文本所需高度
    m_textEdit->document()->setTextWidth(m_textEdit->width());
    int textHeight = m_textEdit->document()->size().height();

    // 限制最大高度为屏幕高度的 60%
    QScreen *screen = QApplication::primaryScreen();
    int maxHeight = screen->availableGeometry().height() * 0.6;

    // 设置文本编辑框的高度
    int textEditHeight = qMin(textHeight + 10, 200); // 最大200像素，超过显示滚动条
    m_textEdit->setFixedHeight(textEditHeight);

    // 计算总高度
    int totalHeight = 180 + (textEditHeight - 60); // 基础高度180 + 文本额外高度

    // 设置对话框大小
    setFixedSize(350, qMin(totalHeight, maxHeight));
    findChild<QWidget*>("bgWidget")->setFixedSize(350, qMin(totalHeight, maxHeight));
}

void CustomMessageBox::setIcon(IconType type)
{
    QPixmap pixmap(40, 40);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QColor iconColor;
    QString iconText;

    switch(type) {
    case Information:
        iconColor = QColor(23, 162, 184);  // 蓝色
        iconText = "i";
        break;
    case Warning:
        iconColor = QColor(255, 193, 7);   // 黄色
        iconText = "!";
        break;
    case Critical:
        iconColor = QColor(220, 53, 69);   // 红色
        iconText = "×";
        break;
    case Question:
        iconColor = QColor(40, 167, 69);   // 绿色
        iconText = "?";
        break;
    }

    // 绘制圆形背景
    painter.setBrush(iconColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(2, 2, 36, 36);

    // 绘制图标文字
    painter.setPen(QPen(Qt::white, 2));
    painter.setFont(QFont("Arial", 18, QFont::Bold));
    painter.drawText(pixmap.rect(), Qt::AlignCenter, iconText);

    m_iconLabel->setPixmap(pixmap);
}

void CustomMessageBox::onOkClicked()
{
    accept();
}

// 静态方法实现
void CustomMessageBox::showInformation(QWidget *parent, const QString &title, const QString &text)
{
    CustomMessageBox msgBox(parent);
    msgBox.setMessage(title, text, Information);
    msgBox.exec();
}

void CustomMessageBox::showWarning(QWidget *parent, const QString &title, const QString &text)
{
    CustomMessageBox msgBox(parent);
    msgBox.setMessage(title, text, Warning);
    msgBox.exec();
}

void CustomMessageBox::showCritical(QWidget *parent, const QString &title, const QString &text)
{
    CustomMessageBox msgBox(parent);
    msgBox.setMessage(title, text, Critical);
    msgBox.exec();
}
