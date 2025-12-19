#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTime>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_client = new Client(this);

    // 关键：信号槽绑定（必须确保这行存在）
    connect(m_client, &Client::connectedToServer, this, &MainWindow::onConnected);
    connect(m_client, &Client::disconnectedFromServer, this, &MainWindow::onDisconnected);
    connect(m_client, &Client::receivedMessage, this, &MainWindow::onReceivedMessage);
    connect(m_client, &Client::userListUpdated, this, &MainWindow::onUserListUpdated); // 核心绑定
    connect(m_client, &Client::loginFailed, this, &MainWindow::onLoginFailed);
    connect(m_client, &Client::socketError, this, &MainWindow::onSocketError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginBtn_clicked()
{
    QString ip = ui->ipEdit->text().trimmed();
    m_username = ui->nicknameEdit->text().trimmed();

    if (ip.isEmpty() || m_username.isEmpty()) {
        QMessageBox::warning(this, "提示", "IP和昵称不能为空！");
        return;
    }

    m_client->connectToServer(ip, 8888);
}

void MainWindow::on_sendBtn_clicked()
{
    QString text = ui->msgEdit->text().trimmed();
    if (text.isEmpty()) return;

    m_client->sendMessage(text);
    ui->msgEdit->clear();
}

void MainWindow::on_exitBtn_clicked()
{
    m_client->disconnectFromServer();
    ui->stackedWidget->setCurrentIndex(0);
    ui->userListWidget->clear(); // 退出时清空列表
}

void MainWindow::onConnected()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->msgBrowser->clear();
    ui->userListWidget->clear();
    ui->msgBrowser->append(QString("[%1] 已连接聊天室").arg(QTime::currentTime().toString("HH:mm:ss")));
    m_client->sendLogin(m_username); // 连接成功后发送登录
}

void MainWindow::onDisconnected()
{
    QMessageBox::information(this, "提示", "已断开服务器连接");
    ui->stackedWidget->setCurrentIndex(0);
    ui->userListWidget->clear();
}

void MainWindow::onReceivedMessage(QString sender, QString text)
{
    ui->msgBrowser->append(QString("[%1] %2: %3").arg(QTime::currentTime().toString("HH:mm:ss")).arg(sender).arg(text));
}

// 核心：更新用户列表UI（必现！）
void MainWindow::onUserListUpdated(QStringList userList)
{
    qDebug() << "[MainWindow] 收到用户列表，准备更新UI：" << userList;
    ui->userListWidget->clear(); // 先清空旧列表
    ui->userListWidget->addItems(userList); // 添加新列表
    qDebug() << "[MainWindow] 列表项数：" << ui->userListWidget->count(); // 打印项数，验证是否添加成功
}

void MainWindow::onLoginFailed(QString reason)
{
    QMessageBox::warning(this, "登录失败", reason);
    m_client->disconnectFromServer();
}

void MainWindow::onSocketError(QString err)
{
    QMessageBox::critical(this, "连接错误", err);
}
