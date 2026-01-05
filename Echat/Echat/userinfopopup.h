#ifndef USERINFOPOPUP_H
#define USERINFOPOPUP_H

#include <QWidget>
#include <QScreen>
#include <QMouseEvent>
#include <QPixmap>
#include "business.h"

namespace Ui {
class UserInfoPopup;
}

class UserInfoPopup : public QWidget
{
    Q_OBJECT

public:
    explicit UserInfoPopup(QWidget *parent = nullptr);
    ~UserInfoPopup();

    void set_userinfo(QPixmap &avatarPixmap, QString name, QString account);
    void showAtWidgetSide(QWidget *widget);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

private:
    Ui::UserInfoPopup *ui;
};

#endif // USERINFOPOPUP_H
