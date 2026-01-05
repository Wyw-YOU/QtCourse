/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_login
{
public:
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QPushButton *closeBtn;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *verticalSpacer_2;
    QLineEdit *accountEdit;
    QLineEdit *passwordEdit;
    QSpacerItem *verticalSpacer;
    QPushButton *loginBtn;
    QPushButton *registerBtn;
    QSpacerItem *verticalSpacer_3;

    void setupUi(QDialog *login)
    {
        if (login->objectName().isEmpty())
            login->setObjectName(QString::fromUtf8("login"));
        login->resize(478, 522);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icons/echat.png"), QSize(), QIcon::Normal, QIcon::Off);
        login->setWindowIcon(icon);
        login->setStyleSheet(QString::fromUtf8("QLineEdit{\n"
"   border:none;\n"
"   border-radius:11px;\n"
"}\n"
"\n"
"QPushButton{\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"}"));
        menubar = new QMenuBar(login);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 502, 25));
        statusbar = new QStatusBar(login);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        statusbar->setGeometry(QRect(0, 0, 3, 22));
        closeBtn = new QPushButton(login);
        closeBtn->setObjectName(QString::fromUtf8("closeBtn"));
        closeBtn->setGeometry(QRect(450, 10, 20, 20));
        closeBtn->setMinimumSize(QSize(20, 20));
        closeBtn->setMaximumSize(QSize(20, 20));
        closeBtn->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"	border-image: url(:/new/prefix1/icons/close.png);\n"
"	background-color: rgba(0, 0, 0, 0);\n"
"    border:none;\n"
"}\n"
"\n"
"QPushButton:hover {\n"
"	background-color: rgb(255, 0, 0);\n"
"	border-image: url(:/new/prefix1/icons/close2.png);\n"
"}\n"
""));
        closeBtn->setIconSize(QSize(10, 10));
        layoutWidget = new QWidget(login);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(90, 60, 303, 421));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(100, 100));
        label->setMaximumSize(QSize(100, 100));
        label->setLayoutDirection(Qt::LeftToRight);
        label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	background-color: rgb(255, 255, 255);\n"
"     border-radius: 50px;\n"
"}"));
        label->setPixmap(QPixmap(QString::fromUtf8(":/new/prefix1/icons/echat.png")));
        label->setAlignment(Qt::AlignCenter);
        label->setMargin(5);

        horizontalLayout->addWidget(label);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer_2 = new QSpacerItem(20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        accountEdit = new QLineEdit(layoutWidget);
        accountEdit->setObjectName(QString::fromUtf8("accountEdit"));
        accountEdit->setMinimumSize(QSize(301, 51));
        accountEdit->setMaximumSize(QSize(301, 51));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        accountEdit->setFont(font);
        accountEdit->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(accountEdit);

        passwordEdit = new QLineEdit(layoutWidget);
        passwordEdit->setObjectName(QString::fromUtf8("passwordEdit"));
        passwordEdit->setMinimumSize(QSize(301, 51));
        passwordEdit->setMaximumSize(QSize(301, 51));
        passwordEdit->setFont(font);
        passwordEdit->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(passwordEdit);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        loginBtn = new QPushButton(layoutWidget);
        loginBtn->setObjectName(QString::fromUtf8("loginBtn"));
        loginBtn->setEnabled(true);
        loginBtn->setMinimumSize(QSize(301, 41));
        loginBtn->setMaximumSize(QSize(301, 41));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font1.setPointSize(12);
        loginBtn->setFont(font1);
        loginBtn->setCursor(QCursor(Qt::ArrowCursor));
        loginBtn->setTabletTracking(false);
        loginBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(158, 218, 255);\n"
"	color: rgb(255, 255, 255);\n"
"    border:none;\n"
"    border-radius:10px;\n"
"}"));

        verticalLayout->addWidget(loginBtn);

        registerBtn = new QPushButton(layoutWidget);
        registerBtn->setObjectName(QString::fromUtf8("registerBtn"));
        registerBtn->setMinimumSize(QSize(301, 41));
        registerBtn->setMaximumSize(QSize(301, 41));
        registerBtn->setFont(font);

        verticalLayout->addWidget(registerBtn);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);


        retranslateUi(login);

        QMetaObject::connectSlotsByName(login);
    } // setupUi

    void retranslateUi(QDialog *login)
    {
        login->setWindowTitle(QCoreApplication::translate("login", "login", nullptr));
        closeBtn->setText(QString());
        label->setText(QString());
        accountEdit->setPlaceholderText(QCoreApplication::translate("login", "\350\276\223\345\205\245Echat\350\264\246\345\217\267", nullptr));
        passwordEdit->setPlaceholderText(QCoreApplication::translate("login", "\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        loginBtn->setText(QCoreApplication::translate("login", "\347\231\273\345\275\225", nullptr));
        registerBtn->setText(QCoreApplication::translate("login", "\346\263\250\345\206\214\350\264\246\345\217\267", nullptr));
    } // retranslateUi

};

namespace Ui {
    class login: public Ui_login {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
