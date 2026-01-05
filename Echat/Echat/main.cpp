#include "login.h"
#include "mainwindow.h"
//#include <QScreen>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    login loginWindow;

    // 显示登录窗口
    loginWindow.show();

    // 直接进入事件循环，等待登录窗口的信号
    return a.exec();
}
