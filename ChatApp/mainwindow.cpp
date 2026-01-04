// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "src/core/application.h"
#include "src/utils/logger.h"
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_loginWidget(nullptr)
    , m_contactWidget(nullptr)
    , m_currentChatWidget(nullptr)
    , m_stackedWidget(nullptr)
{
    ui->setupUi(this);

    // 初始化UI
    initUI();

    // 创建菜单栏和状态栏
    createMenuBar();
    createStatusBar();

    // 连接网络状态信号
    Application* app = Application::instance();
    connect(app, &Application::connectionStatusChanged,
            this, &MainWindow::onConnectionStatusChanged);
    connect(app, &Application::userLoggedIn,
            this, &MainWindow::onUserLoggedIn);
    connect(app, &Application::userLoggedOut,
            this, &MainWindow::onUserLoggedOut);

    LOG_INFO("MainWindow", "MainWindow initialized");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUI()
{
    // 获取stackedWidget
    m_stackedWidget = ui->stackedWidget;

    // 获取登录页面中的loginWidget
    m_loginWidget = ui->loginWidget;

    // 连接登录信号
    if (m_loginWidget) {
        connect(m_loginWidget, &LoginWidget::loginRequested,
                this, &MainWindow::onLoginRequested);
        connect(m_loginWidget, &LoginWidget::registerRequested,
                this, &MainWindow::onRegisterRequested);
    }

    // 获取联系人页面中的contactWidget
    m_contactWidget = ui->contactWidget;

    // 连接联系人信号
    if (m_contactWidget) {
        connect(m_contactWidget, &ContactWidget::contactSelected,
                this, &MainWindow::onContactSelected);
        connect(m_contactWidget, &ContactWidget::addContactRequested,
                this, &MainWindow::onAddContactRequested);
    }

    // 初始显示登录页面
    m_stackedWidget->setCurrentIndex(0);
}

void MainWindow::createMenuBar()
{
    // 文件菜单
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    // 关于菜单
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);
    connect(ui->actionAbout_Qt, &QAction::triggered, qApp, &QApplication::aboutQt);
}

void MainWindow::createStatusBar()
{
    // 初始状态
    ui->statusbar->showMessage("准备就绪");
}

void MainWindow::onLoginRequested(const QString& username, const QString& password)
{
    Application* app = Application::instance();
    app->login(username, password);

    // 更新状态栏
    ui->statusbar->showMessage("正在登录...");

    LOG_INFO("MainWindow", QString("Login requested for: %1").arg(username));
}

void MainWindow::onRegisterRequested()
{
    // 这里可以打开注册对话框
    QMessageBox::information(this, "注册", "注册功能暂未实现");

    LOG_DEBUG("MainWindow", "Register requested");
}

void MainWindow::onContactSelected(int contactId)
{
    // 创建或获取聊天窗口
    if (m_chatWindows.contains(contactId)) {
        m_currentChatWidget = m_chatWindows[contactId];
    } else {
        m_currentChatWidget = new ChatWidget();
        m_chatWindows[contactId] = m_currentChatWidget;

        // 设置聊天信息
        m_currentChatWidget->setChatInfo(contactId, "联系人" + QString::number(contactId));

        // 连接信号
        connect(m_currentChatWidget, &ChatWidget::sendMessageRequested,
                this, &MainWindow::onSendMessageRequested);
    }

    // 添加到聊天堆栈并显示
    QStackedWidget* chatStack = ui->chatStackedWidget;
    int index = chatStack->indexOf(m_currentChatWidget);
    if (index == -1) {
        index = chatStack->addWidget(m_currentChatWidget);
    }
    chatStack->setCurrentIndex(index);

    LOG_DEBUG("MainWindow", QString("Contact selected: ID=%1").arg(contactId));
}

void MainWindow::onAddContactRequested()
{
    // 添加联系人对话框
    QMessageBox::information(this, "添加联系人", "添加联系人功能暂未实现");

    LOG_DEBUG("MainWindow", "Add contact requested");
}

void MainWindow::onSendMessageRequested(int receiverId, const QString& content, ChatType type)
{
    Application* app = Application::instance();

    // 这里应该通过网络发送消息
    // 简化实现：直接添加到聊天窗口
    if (m_chatWindows.contains(receiverId)) {
        ChatWidget* chatWidget = m_chatWindows[receiverId];

        // 创建消息
        ChatMessage message;
        message.senderId = app->currentUser().id;
        message.receiverId = receiverId;
        message.content = content;
        message.messageType = MessageType_Text;
        message.chatType = type;
        message.status = MessageStatus_Sent;
        message.timestamp = QDateTime::currentDateTime();

        // 添加到聊天窗口
        chatWidget->addMessage(message, true);
    }

    LOG_DEBUG("MainWindow", QString("Message sent to %1: %2").arg(receiverId).arg(content));
}

void MainWindow::onConnectionStatusChanged(bool connected)
{
    if (connected) {
        ui->statusbar->showMessage("已连接到服务器");
    } else {
        ui->statusbar->showMessage("连接断开");
    }

    // 通知登录窗口
    if (m_loginWidget) {
        m_loginWidget->onConnectionStatusChanged(connected);
    }
}

void MainWindow::onUserLoggedIn(const UserInfo& user)
{
    // 切换到主页面
    m_stackedWidget->setCurrentIndex(1);

    // 设置联系人窗口的用户信息
    if (m_contactWidget) {
        m_contactWidget->setUserInfo(user);
    }

    // 更新状态栏
    ui->statusbar->showMessage(QString("欢迎，%1").arg(user.nickname.isEmpty() ? user.username : user.nickname));

    // 添加一些测试联系人
    QList<ContactInfo> testContacts;

    ContactInfo contact1;
    contact1.id = 2;
    contact1.name = "张三";
    contact1.group = "好友";
    contact1.status = UserStatus_Online;
    contact1.unreadCount = 3;
    testContacts.append(contact1);

    ContactInfo contact2;
    contact2.id = 3;
    contact2.name = "李四";
    contact2.group = "好友";
    contact2.status = UserStatus_Online;
    contact2.unreadCount = 0;
    testContacts.append(contact2);

    ContactInfo contact3;
    contact3.id = 4;
    contact3.name = "王五";
    contact3.group = "同事";
    contact3.status = UserStatus_Offline;
    contact3.unreadCount = 1;
    testContacts.append(contact3);

    // 更新联系人列表
    if (m_contactWidget) {
        m_contactWidget->updateContactList(testContacts);
    }

    LOG_INFO("MainWindow", QString("User logged in: %1").arg(user.username));
}

void MainWindow::onUserLoggedOut()
{
    // 切换回登录页面
    m_stackedWidget->setCurrentIndex(0);

    // 清空聊天窗口
    for (ChatWidget* chatWidget : m_chatWindows) {
        delete chatWidget;
    }
    m_chatWindows.clear();

    // 更新状态栏
    ui->statusbar->showMessage("已退出登录");

    LOG_INFO("MainWindow", "User logged out");
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "关于 ChatApp",
                       "<h3>ChatApp 即时通讯系统</h3>"
                       "<p>版本: 1.0.0</p>"
                       "<p>这是一个基于 Qt 的即时通讯系统，支持用户注册登录、联系人管理、单聊/群聊等功能。</p>"
                       "<p>开发: QT期末大实验项目</p>");
}
