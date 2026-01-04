#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include "src/utils/global.h"

namespace Ui {
class LoginWidget;
}

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    void loginRequested(const QString &username, const QString &password);
    void registerRequested();
    void autoLoginRequested();

public slots:
    void onLoginSuccess();
    void onLoginFailed(const QString &error);
    void onConnectionStatusChanged(bool connected);

private slots:
    void on_btnLogin_clicked();
    void on_btnRegister_clicked();
    void on_lineEditUsername_returnPressed();
    void on_lineEditPassword_returnPressed();

private:
    Ui::LoginWidget *ui;
    void loadSettings();
    void saveSettings();
    bool m_autoLogin;
};

#endif // LOGINWIDGET_H
