// src/core/application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <QObject>
#include <QApplication>
#include "src/database/dbmanager.h"
#include "src/network/networkmanager.h"
#include "src/utils/global.h"

class MainWindow;

class Application : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Application)

public:
    static Application* instance();

    bool initialize(QApplication* app);
    void cleanup();

    // 获取管理器实例
    DatabaseManager* dbManager() const { return m_dbManager; }
    NetworkManager* networkManager() const { return m_networkManager; }

    // 当前用户信息
    UserInfo currentUser() const { return m_currentUser; }
    void setCurrentUser(const UserInfo& user);

    // 应用状态
    bool isLoggedIn() const { return m_currentUser.id != -1; }
    QString dataPath() const { return m_dataPath; }

    // 全局设置
    QVariant getSetting(const QString& key, const QVariant& defaultValue = QVariant()) const;
    void setSetting(const QString& key, const QVariant& value);

signals:
    void userLoggedIn(const UserInfo& user);
    void userLoggedOut();
    void connectionStatusChanged(bool connected);
    void newMessageReceived(const ChatMessage& message);

public slots:
    void login(const QString& username, const QString& password);
    void logout();
    void registerUser(const UserInfo& userInfo, const QString& password);

private:
    explicit Application(QObject* parent = nullptr);
    ~Application();

    void loadSettings();
    void saveSettings();

    static Application* m_instance;
    DatabaseManager* m_dbManager;
    NetworkManager* m_networkManager;
    MainWindow* m_mainWindow;

    UserInfo m_currentUser;
    QString m_dataPath;
    QSettings* m_settings;
};

#endif // APPLICATION_H
