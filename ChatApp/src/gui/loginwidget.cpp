// src/gui/loginwidget.cpp
#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "src/core/application.h"
#include "src/utils/logger.h"
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

LoginWidget::LoginWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LoginWidget)
    , m_autoLogin(false)
{
    ui->setupUi(this);

    // 加载设置
    loadSettings();

    // 连接信号槽
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginWidget::on_btnLogin_clicked);
    connect(ui->btnRegister, &QPushButton::clicked, this, &LoginWidget::on_btnRegister_clicked);
    connect(ui->lineEditUsername, &QLineEdit::returnPressed, this, &LoginWidget::on_lineEditUsername_returnPressed);
    connect(ui->lineEditPassword, &QLineEdit::returnPressed, this, &LoginWidget::on_lineEditPassword_returnPressed);

    LOG_DEBUG("LoginWidget", "LoginWidget initialized");
}

LoginWidget::~LoginWidget()
{
    saveSettings();
    delete ui;
}

void LoginWidget::onLoginSuccess()
{
    ui->labelStatus->setText("登录成功！");
    ui->labelStatus->setStyleSheet("color: #4CAF50;");

    LOG_INFO("LoginWidget", "Login success UI updated");
}

void LoginWidget::onLoginFailed(const QString& error)
{
    ui->labelStatus->setText("登录失败：" + error);
    ui->labelStatus->setStyleSheet("color: #F44336;");

    // 启用登录按钮
    ui->btnLogin->setEnabled(true);

    LOG_ERROR("LoginWidget", QString("Login failed: %1").arg(error));
}

void LoginWidget::onConnectionStatusChanged(bool connected)
{
    if (connected) {
        ui->labelStatus->setText("已连接到服务器");
        ui->labelStatus->setStyleSheet("color: #4CAF50;");
        ui->btnLogin->setEnabled(true);
    } else {
        ui->labelStatus->setText("连接断开，正在重连...");
        ui->labelStatus->setStyleSheet("color: #FF9800;");
        ui->btnLogin->setEnabled(false);
    }
}

void LoginWidget::on_btnLogin_clicked()
{
    QString username = ui->lineEditUsername->text().trimmed();
    QString password = ui->lineEditPassword->text();

    if (username.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入用户名");
        ui->lineEditUsername->setFocus();
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "请输入密码");
        ui->lineEditPassword->setFocus();
        return;
    }

    // 显示登录状态
    ui->labelStatus->setText("正在登录...");
    ui->labelStatus->setStyleSheet("color: #2196F3;");

    // 禁用登录按钮
    ui->btnLogin->setEnabled(false);

    // 发出登录请求信号
    emit loginRequested(username, password);

    LOG_INFO("LoginWidget", QString("Login button clicked: username=%1").arg(username));
}

void LoginWidget::on_btnRegister_clicked()
{
    emit registerRequested();
    LOG_DEBUG("LoginWidget", "Register button clicked");
}

void LoginWidget::on_lineEditUsername_returnPressed()
{
    ui->lineEditPassword->setFocus();
}

void LoginWidget::on_lineEditPassword_returnPressed()
{
    on_btnLogin_clicked();
}

void LoginWidget::loadSettings()
{
    QSettings settings;

    // 加载用户名
    QString lastUsername = settings.value("last_login/username").toString();
    if (!lastUsername.isEmpty()) {
        ui->lineEditUsername->setText(lastUsername);
    }

    // 加载记住密码
    bool rememberPassword = settings.value("login/remember_password", false).toBool();
    ui->checkBoxRemember->setChecked(rememberPassword);

    if (rememberPassword) {
        QString lastPassword = settings.value("last_login/password").toString();
        if (!lastPassword.isEmpty()) {
            ui->lineEditPassword->setText(lastPassword);
        }
    }

    // 加载自动登录
    m_autoLogin = settings.value("login/auto_login", false).toBool();
    ui->checkBoxAutoLogin->setChecked(m_autoLogin);

    // 如果启用了自动登录，且保存了密码，则自动触发登录
    if (m_autoLogin && !ui->lineEditUsername->text().isEmpty() &&
        !ui->lineEditPassword->text().isEmpty()) {
        QTimer::singleShot(1000, this, &LoginWidget::autoLoginRequested);
    }

    LOG_DEBUG("LoginWidget", "Settings loaded");
}

void LoginWidget::saveSettings()
{
    QSettings settings;

    // 保存记住密码设置
    bool rememberPassword = ui->checkBoxRemember->isChecked();
    settings.setValue("login/remember_password", rememberPassword);

    if (rememberPassword) {
        // 保存密码（注意：实际应用中应该加密存储）
        settings.setValue("last_login/password", ui->lineEditPassword->text());
    } else {
        // 清除保存的密码
        settings.remove("last_login/password");
    }

    // 保存自动登录设置
    m_autoLogin = ui->checkBoxAutoLogin->isChecked();
    settings.setValue("login/auto_login", m_autoLogin);

    LOG_DEBUG("LoginWidget", "Settings saved");
}
