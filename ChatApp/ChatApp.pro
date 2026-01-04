QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    src/core/application.cpp \
    src/gui/loginwidget.cpp \
    src/gui/chatwidget.cpp \
    src/gui/contactwidget.cpp \
    src/models/chatmodel.cpp \
    src/models/contactmodel.cpp \
    src/models/messageitemdelegate.cpp \
    src/network/networkmanager.cpp \
    src/network/tcpclient.cpp \
    src/database/dbmanager.cpp \
    src/utils/global.cpp \
    src/utils/logger.cpp

HEADERS += \
    mainwindow.h \
    src/core/application.h \
    src/gui/loginwidget.h \
    src/gui/chatwidget.h \
    src/gui/contactwidget.h \
    src/models/chatmodel.h \
    src/models/contactmodel.h \
    src/models/messageitemdelegate.h \
    src/network/networkmanager.h \
    src/network/tcpclient.h \
    src/database/dbmanager.h \
    src/utils/logger.h \
    src/utils/global.h

FORMS += \
    mainwindow.ui \
    src/gui/loginwidget.ui \
    src/gui/chatwidget.ui \
    src/gui/contactwidget.ui

# 资源文件
RESOURCES += \
    src/rescources/resources.qrc

# 包含路径
INCLUDEPATH += src

# 编译输出目录
DESTDIR = $$OUT_PWD/bin
OBJECTS_DIR = $$OUT_PWD/obj
MOC_DIR = $$OUT_PWD/moc
RCC_DIR = $$OUT_PWD/rcc
UI_DIR = $$OUT_PWD/ui

# 部署
win32: {
    target.path = $$[QT_INSTALL_EXAMPLES]/ChatApp
    INSTALLS += target
}

DISTFILES += \
    styles/default.qss
