QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriend.cpp \
    bubble.cpp \
    business.cpp \
    custommessagebox.cpp \
    datamanager.cpp \
    frienditem.cpp \
    groupmember.cpp \
    main.cpp \
    login.cpp \
    mainwindow.cpp \
    networkmanager.cpp \
    noticeitem.cpp \
    registerdialog.cpp \
    userinfopopup.cpp

HEADERS += \
    Protocol.h \
    addfriend.h \
    bubble.h \
    business.h \
    custommessagebox.h \
    datamanager.h \
    frienditem.h \
    groupmember.h \
    login.h \
    mainwindow.h \
    networkmanager.h \
    noticeitem.h \
    registerdialog.h \
    userinfopopup.h

FORMS += \
    addfriend.ui \
    bubble.ui \
    frienditem.ui \
    groupmember.ui \
    login.ui \
    mainwindow.ui \
    noticeitem.ui \
    registerdialog.ui \
    userinfopopup.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    headImg.qrc \
    icons.qrc
RC_ICONS = main_icon/echat.ico
