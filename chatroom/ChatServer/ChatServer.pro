QT       += core gui network  # 新增network模块（TCP通信必需）

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 适配Qt5.15，禁用过时API（可选，根据需求开启）
# DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050F00    # 禁用Qt5.15之前的过时API

SOURCES += \
    chatserver.cpp \
    clienthandler.cpp \
    main.cpp \
    mainwindow.cpp \
    protocol.cpp \
    server.cpp

HEADERS += \
    chatserver.h \
    clienthandler.h \
    mainwindow.h \
    protocol.h \
    server.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# 可选：添加调试信息（方便开发阶段排查问题）
CONFIG += debug
