#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class QLabel;

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);

signals:
    void loginRequested(const QString &username, const QString &password);
    void registerRequested();

private slots:
    void onLoginButtonClicked();
    void onRegisterButtonClicked();

private:
    QLabel *m_titleLabel;
    QLineEdit *m_userEdit;
    QLineEdit *m_passEdit;
    QPushButton *m_registerBtn;
    QPushButton *m_loginBtn;
};

#endif // LOGINWIDGET_H
