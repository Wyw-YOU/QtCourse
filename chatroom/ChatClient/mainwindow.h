#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loginBtn_clicked();
    void on_sendBtn_clicked();
    void on_exitBtn_clicked();

    // 响应Client的信号
    void onConnected();
    void onDisconnected();
    void onReceivedMessage(QString sender, QString text);
    void onUserListUpdated(QStringList userList); // 核心槽函数
    void onLoginFailed(QString reason);
    void onSocketError(QString err);

private:
    Ui::MainWindow *ui;
    Client* m_client;
    QString m_username;
};

#endif // MAINWINDOW_H
