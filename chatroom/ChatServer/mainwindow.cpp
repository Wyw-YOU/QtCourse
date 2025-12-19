// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("服务器日志");

    m_server = new Server(this);
    connect(m_server, &Server::log, this, &MainWindow::appendLog);
    if (m_server->listen(QHostAddress::LocalHost, 8888)) {
        appendLog("服务器启动，监听端口8888");
    } else {
        appendLog("服务器启动失败：" + m_server->errorString());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stopBtn_clicked()
{
    // 新增：停止服务前断开所有客户端
    ChatServer::getInstance()->disconnectAllClients();

    m_server->close();
    appendLog("服务器已停止");
}

void MainWindow::appendLog(const QString& msg)
{
    ui->logTextEdit->append(msg); // 现在控件已定义，不会报错
}
