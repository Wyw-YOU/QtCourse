#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"  // 包含主窗口头文件

login::login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::login)
    , m_NetworkManager(nullptr)
    , m_dataManager(DataManager::instance()) // 初始化引用
    , m_mainWindow(nullptr)
    , m_connectionState(Disconnected)
    , m_pendingAction(None)

{
    ui->setupUi(this);

    m_NetworkManager = new NetworkManager(this);

    // 创建连接超时定时器
    m_connectionTimer = new QTimer(this);
    m_connectionTimer->setSingleShot(true);

    setWindowFlag(Qt::FramelessWindowHint);

    setupConnections();

    this->installEventFilter(new DragWidgetFilter(this));

    this->setFocus();

    ui->loginBtn->setEnabled(false);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
}

login::~login()
{
    delete ui;
}

void login::setupConnections()
{
    // 连接文本变化信号
    connect(ui->accountEdit, &QLineEdit::textChanged, this, &login::onTextChanged);
    connect(ui->passwordEdit, &QLineEdit::textChanged, this, &login::onTextChanged);

    connect(m_NetworkManager, &NetworkManager::connectionStatusChanged, this, &login::onConnectionStatusChanged);
    connect(m_connectionTimer, &QTimer::timeout, this, &login::onConnectionTimeout);
    connect(m_NetworkManager, &NetworkManager::DataReach, this, &login::onDataReach);

    // 连接登录成功信号到槽函数
    connect(this, &login::loginSuccess, this, &login::onloginSuccess);
}

void login::onTextChanged()
{
    // 获取去除空白字符后的文本
    QString account = ui->accountEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    // 判断是否都有内容
    bool hasUsername = !account.isEmpty();
    bool hasPassword = !password.isEmpty();

    // 启用/禁用登录按钮
    if(hasUsername && hasPassword){
        ui->loginBtn->setStyleSheet("background-color: rgb(0, 141, 235); color:rgb(255, 255, 255); border:none; border-radius:10px");
        ui->loginBtn->setEnabled(true);
    }else{
        ui->loginBtn->setStyleSheet("background-color: rgb(158, 218, 255); color:rgb(255, 255, 255); border:none; border-radius:10px");
        ui->loginBtn->setEnabled(false);
    }
}

void login::on_loginBtn_clicked()
{
    if(m_connectionState == Connecting){
        return;
    }

    if(m_connectionState == Connected || m_connectionState == Authenticated){
        qDebug() << "网络已经连接，再次发送登录请求";
        sendLoginRequest();
    }else{
        // 未连接，开始连接流程
        attemptConnection(Login);
    }
}

void login::attemptConnection(PendingAction action)
{
    if (m_connectionState != Disconnected) {
        qDebug() << "连接状态异常，当前状态:" << m_connectionState;
        return;
    }

    m_pendingAction = action;
    m_connectionState = Connecting;

    if(action == Login){
        // 更新UI状态
        ui->loginBtn->setEnabled(false);
        ui->loginBtn->setStyleSheet("background-color: rgb(158, 218, 255); color:rgb(255, 255, 255); border:none; border-radius:10px");
        ui->loginBtn->setText("连接中...");
        ui->accountEdit->setReadOnly(true);
        ui->passwordEdit->setReadOnly(true);
        ui->registerBtn->setEnabled(false);
    }else if(action == Register){
        // 更新UI状态
        ui->loginBtn->setEnabled(false);
        ui->loginBtn->setStyleSheet("background-color: rgb(158, 218, 255); color:rgb(255, 255, 255); border:none; border-radius:10px");
        ui->loginBtn->setText("注册中...");
        ui->accountEdit->clear();
        ui->passwordEdit->clear();
        ui->accountEdit->setReadOnly(true);
        ui->passwordEdit->setReadOnly(true);
        ui->registerBtn->setEnabled(false);
    }

    // 启动连接超时检测
    m_connectionTimer->start(5000);
    m_NetworkManager->start();
}


void login::sendLoginRequest()
{
    if (m_connectionState != Connected) {
        qDebug() << "发送登录请求失败：未连接";
        resetUIState();
        return;
    }

    QString account = ui->accountEdit->text();
    QString password = ui->passwordEdit->text();

    LOGIN_MSG login_msg;
    const char* data = Business::construct_login_message(account, password, &login_msg);

    if(!m_NetworkManager->send_message(data, sizeof(LOGIN_MSG))){
        qDebug() << "发送登录请求错误";
        resetUIState();
    } else {
        ui->loginBtn->setText("登录中...");
        qDebug() << "登录请求已发送";
    }
}

void login::sendRegisterRequest()
{
    if (m_connectionState != Connected) {
        qDebug() << "发送注册请求失败：未连接";
        // 注册失败不需要重置UI状态，因为注册是独立流程
        return;
    }

    REGISTET_MSG register_msg;
    const char* data = Business::construct_register_message(
        m_pendingRegisterName,
        m_pendingRegisterAccount,
        m_pendingRegisterPassword,
        m_pendingRegister_avatarData,
        &register_msg
    );

    if(!m_NetworkManager->send_message(data, sizeof(REGISTET_MSG))){
        qDebug() << "发送注册请求错误";
    } else {
        qDebug() << "注册请求已发送";
    }
}


void login::on_closeBtn_clicked()
{
    // 停止超时计时器
    if (m_connectionTimer->isActive()) {
        m_connectionTimer->stop();
    }

    if (m_NetworkManager) {
        m_NetworkManager->stop();
    }

    reject(); // 关闭对话框
}

void login::onConnectionStatusChanged(bool connected, const QString& message)
{
    // 停止超时计时器
    if (m_connectionTimer->isActive()) {
        m_connectionTimer->stop();
    }

    if (connected) {
        m_connectionState = Connected;
        qDebug() << "连接服务器成功";

        // 根据待处理的操作发送相应的请求
        if (m_pendingAction == Login) {
            sendLoginRequest();
        } else if (m_pendingAction == Register) {
            sendRegisterRequest();
        }
        m_dataManager.set_networkmanager(m_NetworkManager);
    } else {
        m_connectionState = Disconnected;
        m_pendingAction = None;
        qDebug() << "连接服务器失败:" << message;

        resetUIState();

        CustomMessageBox::showWarning(
            this,
            "连接失败",
            "无法连接到服务器，请检查网络连接"
        );
    }
}

void login::resetUIState()
{
    if(!ui->accountEdit->text().isEmpty() && !ui->accountEdit->text().isEmpty()){
        ui->loginBtn->setEnabled(true);
    }
    ui->loginBtn->setStyleSheet("background-color: rgb(0, 141, 235); color:rgb(255, 255, 255); border:none; border-radius:10px");
    ui->loginBtn->setText("登录");
    ui->accountEdit->setReadOnly(false);
    ui->passwordEdit->setReadOnly(false);
    ui->registerBtn->setEnabled(true);

    // 更新连接状态
    m_connectionState = Disconnected;
    m_pendingAction = None;
}

void login::onConnectionTimeout()
{
    if (m_connectionState == Connecting) {
        qDebug() << "连接超时";

        CustomMessageBox::showWarning(
            this,
            "网络连接超时",
            "• 网络连接是否正常\n"
            "• 服务器可能不在线\n"
        );

        // 停止网络连接尝试
        if (m_NetworkManager) {
            m_NetworkManager->stop();
        }

        // 恢复状态
        resetUIState();
    }
}

void login::onDataReach()
{
    // 如果已经登录成功，忽略后续数据
    if (m_connectionState == Authenticated && m_mainWindow) {
        return;
    }

    while(true){
        MSG_HEADER msg_header;
        // 尝试读取消息头
        if (!m_NetworkManager->read_message(msg_header)) {
            qDebug() << "没有更多消息或读取消息头失败";
            break;
        }

        RESPONSE_MSG response_msg;
        USER_QUERY_RESPONSE_MSG user_query_response_msg;
        FRIEND_ASK_NOTICE_MSG *friend_ask_notice_msg = NULL;
        FRIEND_LIST_MSG *friend_list_msg = NULL;
        GROUP_ASK_NOTICE_MSG *group_ask_notice_msg = NULL;
        GROUP_LIST_MSG* group_list_msg =NULL;
        int total_size = sizeof(MSG_HEADER) + msg_header.msg_length;

        switch(msg_header.msg_type)
        {
        case ADD_FRIEND_LIST_RESPONSE:
            friend_ask_notice_msg = (FRIEND_ASK_NOTICE_MSG*)malloc(total_size);
            m_NetworkManager->read_add_friend_list(msg_header, friend_ask_notice_msg);
            m_dataManager.setCurrentfriend_add_asks(friend_ask_notice_msg->asks, friend_ask_notice_msg->msg_header.total_count);
            // 释放内存
            free(friend_ask_notice_msg);
            break;

        case FRIEND_LIST_RESPONSE:
            friend_list_msg = (FRIEND_LIST_MSG*)malloc(total_size);
            m_NetworkManager->read_friend_list_msg(msg_header, friend_list_msg);
            m_dataManager.setCurrent_friendlist(friend_list_msg->friends, friend_list_msg->msg_header.total_count);
            // 释放内存
            free(friend_list_msg);
            break;

        case ADD_GROUP_LIST_RESPONSE:
            group_ask_notice_msg = (GROUP_ASK_NOTICE_MSG*)malloc(total_size);
            m_NetworkManager->read_add_group_list(msg_header, group_ask_notice_msg);
            m_dataManager.setCurrentgroup_add_asks(group_ask_notice_msg->asks, group_ask_notice_msg->msg_header.total_count);
            // 释放内存
            free(group_ask_notice_msg);
            break;

        case GROUP_LIST_RESPONSE:
            group_list_msg = (GROUP_LIST_MSG*)malloc(total_size);
            m_NetworkManager->read_group_list_msg(msg_header, group_list_msg);
            m_dataManager.setCurrent_grouplist(group_list_msg->groups, group_list_msg->msg_header.total_count);
            // 释放内存
            free(group_list_msg);
            break;

        case LOGIN_RESPONSE:
             m_NetworkManager->read_login_message(msg_header, user_query_response_msg);
             qDebug() << "接收到登陆回应";
             if (user_query_response_msg.success_flag == 0)
             {
                 qDebug() <<  user_query_response_msg.response;
                 // 使用自定义消息框
                 CustomMessageBox::showWarning(
                     this,  // 父窗口
                     "登陆失败",
                     user_query_response_msg.response
                 );
                 // 登录失败，保持连接但重置认证状态
                 resetUIState();
                 m_connectionState = Connected;
             }else{
                 qDebug() << user_query_response_msg.response;
                 m_connectionState = Authenticated;

                 m_currentUser.nickname = user_query_response_msg.user_info.user_name;
                 m_currentUser.account = user_query_response_msg.user_info.user_account;
                 m_currentUser.onlineStatus = user_query_response_msg.user_info.status;
                 QByteArray avatarData(user_query_response_msg.user_info.avatar_data,
                                     user_query_response_msg.user_info.avatar_size);
                 m_currentUser.avatarData = avatarData;

                 // 将头像数据转换为QPixmap用于显示
                 QPixmap avatarPixmap;
                 if (avatarPixmap.loadFromData(avatarData)) {
                     m_currentUser.avatarPixmap = avatarPixmap;
                     qDebug() << "头像加载成功，大小:" << avatarData.size() << "字节";
                 } else {
                     qDebug() << "头像数据加载失败";
                     // 加载默认头像
                     m_currentUser.avatarPixmap = QPixmap(":/new/prefix1/HeadImages/1.png");
                 }

                 m_dataManager.setCurrentUser(m_currentUser);
     //            qDebug() << "nickname: " << m_currentUser.nickname << "account: " << m_currentUser.account << "onlineStatus: " << m_currentUser.onlineStatus;
                 // 发出登录成功信号，开始创建主窗口
                 emit loginSuccess();
                 return;
             }
            break;
        case REGISTER_RESPONSE:
            m_NetworkManager->read_remain_message(msg_header, response_msg);
            qDebug() << "接收到注册回应";

            resetUIState();
            // 注册完成后重置连接状态
            m_connectionState = Connected;

            if (response_msg.success_flag == 0)
            {
                qDebug() <<  response_msg.response;
                // 使用自定义消息框
                CustomMessageBox::showWarning(
                    this,  // 父窗口
                    "注册失败",
                    response_msg.response
                );
            }else{
                qDebug() <<  response_msg.response;
                // 注册成功后自动填充账号
                ui->accountEdit->setText(m_pendingRegisterAccount);
                ui->passwordEdit->setFocus();
                ui->passwordEdit->clear();
                CustomMessageBox::showInformation(this, "注册成功", "账号注册成功，请登录");
            }
            break;

        default:
            break;
        }
    }
//   MSG_HEADER msg_header;
//   m_NetworkManager->read_message(msg_header);
//   qDebug() <<  "msg_length:"  << msg_header.msg_length;
//   qDebug() << "---登录界面打印----";
}

void login::onloginSuccess()
{
    // 在后台线程中创建主窗口，避免阻塞UI
    QTimer::singleShot(0, this, &login::createMainWindow);
}

void login::createMainWindow()
{
    qDebug() << "开始创建主窗口...";

    // 创建主窗口实例
    m_mainWindow = new mainWindow();

    // 连接主窗口的初始化完成信号
    connect(m_mainWindow, &mainWindow::initializationComplete, this, &login::onMainWindowReady);
}

void login::onMainWindowReady()
{
    // 显示主窗口
    m_mainWindow->show();

    // 关闭登录窗口
    this->close();
}


void login::on_registerBtn_clicked()
{
    if (m_connectionState == Connecting) {
        // 正在连接中，忽略点击
        return;
    }

    // 创建注册对话框实例
    RegisterDialog registerDialog(this);
    registerDialog.setModal(true);
    registerDialog.setWindowTitle("注册");

    // 显示注册对话框并等待用户操作
    if (registerDialog.exec() == QDialog::Accepted) {
        // 获取注册信息
        QString name = registerDialog.m_name;
        QString account = registerDialog.m_account;
        QString password = registerDialog.m_password;

        if (!name.isEmpty() && !account.isEmpty() && !password.isEmpty()) {
            // 保存注册信息
            m_pendingRegisterName = name;
            m_pendingRegisterAccount = account;
            m_pendingRegisterPassword = password;
            m_pendingRegister_avatarData = registerDialog.m_avatarData;
            if (m_connectionState == Connected || m_connectionState == Authenticated) {
                // 已经连接，直接发送注册请求
                sendRegisterRequest();
            } else {
                // 未连接，开始连接流程
                attemptConnection(Register);
            }
        }
    }
}
