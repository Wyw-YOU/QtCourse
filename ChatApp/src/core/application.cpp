// src/core/application.cpp
#include "application.h"
#include "src/utils/logger.h"
#include "src/database/dbmanager.h"
#include "src/network/networkmanager.h"
#include <QApplication>
#include <QMessageBox>
#include <QStandardPaths>
#include <QDir>
#include <QSettings>

Application* Application::m_instance = nullptr;

Application::Application(QObject* parent)
    : QObject(parent)
    , m_dbManager(nullptr)
    , m_networkManager(nullptr)
    , m_mainWindow(nullptr)
    , m_settings(nullptr)
{
    // 初始化数据路径
    m_dataPath = Utils::getDataPath();

    LOG_INFO("Application", QString("Application initialized. Data path: %1").arg(m_dataPath));
}

Application::~Application()
{
    cleanup();
}

Application* Application::instance()
{
    if (!m_instance) {
        m_instance = new Application();
    }
    return m_instance;
}

bool Application::initialize(QApplication* app)
{
    if (!app) {
        LOG_ERROR("Application", "QApplication is null");
        return false;
    }

    LOG_INFO("Application", "Initializing application...");

    // 加载设置
    loadSettings();

    // 初始化数据库管理器
    m_dbManager = new DatabaseManager(this);
    if (!m_dbManager->initialize()) {
        LOG_ERROR("Application", "Failed to initialize database");
        QMessageBox::critical(nullptr, "错误", "数据库初始化失败");
        return false;
    }

    // 初始化网络管理器
    m_networkManager = NetworkManager::instance();

    // 连接网络管理器的信号
    connect(m_networkManager, &NetworkManager::loginSuccess,
            this, &Application::onLoginSuccess);
    connect(m_networkManager, &NetworkManager::loginFailed,
            this, &Application::onLoginFailed);
    connect(m_networkManager, &NetworkManager::registerSuccess,
            this, &Application::onRegisterSuccess);
    connect(m_networkManager, &NetworkManager::registerFailed,
            this, &Application::onRegisterFailed);
    connect(m_networkManager, &NetworkManager::messageReceived,
            this, &Application::onMessageReceived);
    connect(m_networkManager, &NetworkManager::userStatusUpdated,
            this, &Application::onUserStatusUpdated);
    connect(m_networkManager, &NetworkManager::connectionChanged,
            this, &Application::connectionStatusChanged);

    LOG_INFO("Application", "Application initialized successfully");
    return true;
}

void Application::cleanup()
{
    LOG_INFO("Application", "Cleaning up application...");

    // 保存设置
    saveSettings();

    // 清理网络管理器
    if (m_networkManager) {
        // 注意：NetworkManager是单例，由自己管理生命周期
        // 或者在这里调用它的清理函数
    }

    // 清理数据库管理器
    if (m_dbManager) {
        m_dbManager->onAppExit();
        delete m_dbManager;
        m_dbManager = nullptr;
    }

    // 清理设置
    if (m_settings) {
        delete m_settings;
        m_settings = nullptr;
    }

    LOG_INFO("Application", "Application cleanup completed");
}

void Application::setCurrentUser(const UserInfo& user)
{
    m_currentUser = user;

    // 保存到设置
    setSetting("current_user/id", user.id);
    setSetting("current_user/username", user.username);
    setSetting("current_user/nickname", user.nickname);
    setSetting("current_user/avatar", user.avatar);

    emit userLoggedIn(user);
}

void Application::login(const QString& username, const QString& password)
{
    LOG_INFO("Application", QString("Login attempt: %1").arg(username));

    // 保存登录信息（如果需要记住密码）
    setSetting("last_login/username", username);

    // 调用网络管理器登录
    m_networkManager->login(username, password);
}

void Application::logout()
{
    LOG_INFO("Application", QString("Logout: %1").arg(m_currentUser.username));

    // 清理当前用户信息
    m_currentUser = UserInfo();

    // 清除保存的密码
    setSetting("last_login/password", "");

    // 断开网络连接
    m_networkManager->disconnectFromServer();

    emit userLoggedOut();
}

void Application::registerUser(const UserInfo& userInfo, const QString& password)
{
    LOG_INFO("Application", QString("Register attempt: %1").arg(userInfo.username));

    m_networkManager->registerUser(userInfo, password);
}

QVariant Application::getSetting(const QString& key, const QVariant& defaultValue) const
{
    if (!m_settings) {
        return defaultValue;
    }

    return m_settings->value(key, defaultValue);
}

void Application::setSetting(const QString& key, const QVariant& value)
{
    if (!m_settings) {
        return;
    }

    m_settings->setValue(key, value);
    m_settings->sync();
}

void Application::loadSettings()
{
    if (!m_settings) {
        QString settingsPath = m_dataPath + "/settings.ini";
        m_settings = new QSettings(settingsPath, QSettings::IniFormat);
    }

    LOG_INFO("Application", QString("Settings loaded from: %1").arg(m_settings->fileName()));
}

void Application::saveSettings()
{
    if (m_settings) {
        m_settings->sync();
        LOG_INFO("Application", "Settings saved");
    }
}

void Application::onLoginSuccess(const UserInfo& userInfo)
{
    LOG_INFO("Application", QString("Login success: %1 (ID: %2)")
                 .arg(userInfo.username).arg(userInfo.id));

    // 设置当前用户
    setCurrentUser(userInfo);

    // 保存到本地数据库
    // 注意：这里应该检查用户是否已存在，如果不存在则创建
    if (m_dbManager) {
        // 获取用户信息，如果不存在则创建
        UserInfo existingUser = m_dbManager->getUserById(userInfo.id);
        if (existingUser.id == -1) {
            // 用户不存在，创建本地记录
            // 注意：这里需要密码，但我们现在没有保存密码
            // 可以创建一个虚拟密码，或者不保存密码
            m_dbManager->createUser(userInfo, "dummy_password");
        } else {
            // 更新用户信息
            m_dbManager->updateUserInfo(userInfo);
        }
    }
}

void Application::onLoginFailed(const QString& error)
{
    LOG_ERROR("Application", QString("Login failed: %1").arg(error));

    // 可以在这里显示错误消息
}

void Application::onRegisterSuccess(const UserInfo& userInfo)
{
    LOG_INFO("Application", QString("Register success: %1").arg(userInfo.username));

    // 注册成功后自动登录
    setCurrentUser(userInfo);
}

void Application::onRegisterFailed(const QString& error)
{
    LOG_ERROR("Application", QString("Register failed: %1").arg(error));
}

void Application::onMessageReceived(const ChatMessage& message)
{
    LOG_DEBUG("Application", QString("Message received from %1: %2")
                  .arg(message.senderId).arg(message.content.left(50)));

    // 保存消息到数据库
    if (m_dbManager) {
        m_dbManager->saveMessage(message);
    }

    emit newMessageReceived(message);
}

void Application::onUserStatusUpdated(const UserStatusUpdate& update)
{
    LOG_DEBUG("Application", QString("User status updated: user=%1, status=%2")
                  .arg(update.userId).arg(update.status));

    // 更新本地数据库中的用户状态
    if (m_dbManager) {
        m_dbManager->updateUserStatus(update.userId, update.status);
    }
}
