#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QDebug>
#include <QMouseEvent>
#include "networkmanager.h"
#include "business.h"
#include "custommessagebox.h"
#include "registerdialog.h"
#include "datamanager.h"
#include "mainwindow.h"

// 前向声明主窗口类
class mainWindow;

QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

    void createMainWindow();

signals:
    void loginSuccess();      // 登录成功信号

private slots:
    void on_loginBtn_clicked();
    void on_closeBtn_clicked();
    void onConnectionStatusChanged(bool connected, const QString& message);
    void onConnectionTimeout();  // 连接超时处理
    void onDataReach();

    void on_registerBtn_clicked();
    void onloginSuccess();
    void onMainWindowReady();  // 主窗口准备完成

private:
    Ui::login *ui;
    NetworkManager* m_NetworkManager;
    QTimer* m_connectionTimer;   // 连接超时定时器
    DataManager& m_dataManager; // 引用单例实例
    mainWindow *m_mainWindow;
//    bool m_isConnecting;         // 是否正在连接中
//    bool m_isConnected;

    enum ConnectionState {
        Disconnected,      // 未连接
        Connecting,        // 连接中
        Connected,         // 已连接但未认证
        Authenticated      // 已连接并认证（登录成功）
    };

    enum PendingAction {
        None,
        Login,
        Register
    };

    ConnectionState m_connectionState;
    PendingAction m_pendingAction;
    QString m_pendingRegisterName;
    QString m_pendingRegisterAccount;
    QString m_pendingRegisterPassword;
    QByteArray m_pendingRegister_avatarData;

    UserBasicInfo m_currentUser;

    void setupConnections();
    void onTextChanged();
    void resetLoginButton();
    void resetUIState();
    void sendLoginRequest();
    void attemptConnection(PendingAction action);
    void sendRegisterRequest();
};

class DragWidgetFilter : public QObject
{
public:
    DragWidgetFilter(QObject *parent = nullptr)
        : QObject(parent) {}

protected:
    bool eventFilter(QObject* object, QEvent* event) override
    {
        auto w = dynamic_cast<QWidget*>(object);
        if(!w) {
            return false;
        }

        if(event->type() == QEvent::MouseButtonPress) {
            auto ev = dynamic_cast<QMouseEvent*>(event);
            if(!ev || ev->button() != Qt::LeftButton) {
                return false;
            }

            // 使用Qt 5的API
            m_dragPosition = ev->globalPos() - w->frameGeometry().topLeft();
            return true;
        }
        else if(event->type() == QEvent::MouseMove) {
            auto ev = dynamic_cast<QMouseEvent*>(event);
            if(!ev || !(ev->buttons() & Qt::LeftButton)) {
                return false;
            }

            if (w->isWindow()) {
                // 使用Qt 5的API
                QPoint newPos = ev->globalPos() - m_dragPosition;
                w->move(newPos);
            }
            return true;
        }

        return false;
    }

private:
    QPoint m_dragPosition;
};

#endif // LOGIN_H
