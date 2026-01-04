// main.cpp
#include "mainwindow.h"
#include "src/utils/logger.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 设置应用程序信息
    QApplication::setApplicationName("ChatApp");
    QApplication::setOrganizationName("QtCourse");

    // 设置样式
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // 初始化日志系统
    Logger::instance()->init();
    LOG_INFO("Main", "Application starting...");

    // 创建并显示主窗口
    MainWindow w;
    w.show();

    LOG_INFO("Main", "Application started successfully");

    int ret = a.exec();

    // 清理
    Logger::instance()->uninit();

    return ret;
}
