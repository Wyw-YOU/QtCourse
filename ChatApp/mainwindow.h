// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "src/gui/loginwidget.h"
#include "src/gui/contactwidget.h"
#include "src/gui/chatwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // 登录相关
    void onLoginRequested(const QString &username, const QString &password);
    void onRegisterRequested();

    // 联系人相关
    void onContactSelected(int contactId);
    void onAddContactRequested();

    // 聊天相关
    void onSendMessageRequested(int receiverId, const QString &content, ChatType type = ChatType_Single);

    // 应用状态
    void onConnectionStatusChanged(bool connected);
    void onUserLoggedIn(const UserInfo &user);
    void onUserLoggedOut();

    // 菜单栏
    void onAbout();

private:
    Ui::MainWindow *ui;
    LoginWidget *m_loginWidget;
    ContactWidget *m_contactWidget;
    ChatWidget *m_currentChatWidget;
    QStackedWidget *m_stackedWidget;

    // 聊天窗口映射
    QMap<int, ChatWidget*> m_chatWindows;

    // 添加菜单栏
    void createMenuBar();
    // 添加状态栏
    void createStatusBar();
    // 初始化界面
    void initUI();
};

#endif // MAINWINDOW_H
