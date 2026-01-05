#include "userinfopopup.h"
#include "ui_userinfopopup.h"

UserInfoPopup::UserInfoPopup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserInfoPopup)
{
    ui->setupUi(this);
    // 设置窗口标志
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    // 设置样式
    setStyleSheet(
        "UserInfoPopup {"
        "    background-color: white;"
        "    border: 1px solid #dcdfe6;"
        "    border-radius: 8px;"
        "}"
    );
}

UserInfoPopup::~UserInfoPopup()
{
    delete ui;
}

void UserInfoPopup:: set_userinfo(QPixmap &avatarPixmap, QString name, QString account)
{
    QString name_text = "昵称：";
    name_text.append(name);

    QString account_text = "账号：";
    account_text.append(account);

    ui->headimg->clear();
//    ui->headimg->setStyleSheet(imagepath);
    ui->name->setText(name_text);
    ui->account->setText(account_text);

    // 缩放头像到合适尺寸
    QPixmap scaledAvatar = avatarPixmap.scaled(ui->headimg->width(), ui->headimg->height(),
                                        Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPixmap roundedAvatar = Business::getRoundedPixmap(scaledAvatar, 12); // 12是圆角半径
    ui->headimg->setPixmap(roundedAvatar);

}

void UserInfoPopup::showAtWidgetSide(QWidget *widget)
{
    if (!widget) return;

    // 获取按钮的全局位置和大小
    QRect widgetRect = widget->rect();
    QPoint widgetTopLeft = widget->mapToGlobal(widgetRect.topLeft());
    QPoint widgetBottomRight = widget->mapToGlobal(widgetRect.bottomRight());

    // 获取屏幕信息
    QScreen *screen = QApplication::screenAt(widgetTopLeft);
    if (!screen) {
        screen = QApplication::primaryScreen();
    }

    int x = widgetBottomRight.x() + 5;
    int y = widgetTopLeft.y();

    move(x, y);
    show();
    raise();
}


bool UserInfoPopup::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

        // 检查点击位置是否在弹窗外
        if (!geometry().contains(mouseEvent->globalPos())) {
            // 获取点击的控件
            QWidget *clickedWidget = QApplication::widgetAt(mouseEvent->globalPos());

            // 如果点击的是触发按钮，不关闭（避免立即关闭）
            if (clickedWidget && clickedWidget->objectName() != "headimgBtn") {
                hide();
                return true;
            }
        }
    }

    return QWidget::eventFilter(obj, event);
}

void UserInfoPopup::showEvent(QShowEvent *event)
{
    // 安装事件过滤器到应用程序级别
    qApp->installEventFilter(this);
    QWidget::showEvent(event);
}

void UserInfoPopup::hideEvent(QHideEvent *event)
{
    // 移除事件过滤器
    qApp->removeEventFilter(this);
    QWidget::hideEvent(event);
}
