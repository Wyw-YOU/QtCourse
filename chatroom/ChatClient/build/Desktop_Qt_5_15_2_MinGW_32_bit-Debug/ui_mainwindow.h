/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *ipEdit;
    QLabel *label_2;
    QLineEdit *nicknameEdit;
    QPushButton *loginBtn;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QTextBrowser *msgBrowser;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *msgEdit;
    QPushButton *sendBtn;
    QVBoxLayout *verticalLayout_4;
    QListWidget *userListWidget;
    QPushButton *exitBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QString::fromUtf8("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QString::fromUtf8("page"));
        verticalLayout_2 = new QVBoxLayout(page);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        label = new QLabel(page);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        ipEdit = new QLineEdit(page);
        ipEdit->setObjectName(QString::fromUtf8("ipEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, ipEdit);

        label_2 = new QLabel(page);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        nicknameEdit = new QLineEdit(page);
        nicknameEdit->setObjectName(QString::fromUtf8("nicknameEdit"));

        formLayout->setWidget(1, QFormLayout::FieldRole, nicknameEdit);


        verticalLayout_2->addLayout(formLayout);

        loginBtn = new QPushButton(page);
        loginBtn->setObjectName(QString::fromUtf8("loginBtn"));

        verticalLayout_2->addWidget(loginBtn);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QString::fromUtf8("page_2"));
        horizontalLayout = new QHBoxLayout(page_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        msgBrowser = new QTextBrowser(page_2);
        msgBrowser->setObjectName(QString::fromUtf8("msgBrowser"));

        verticalLayout_3->addWidget(msgBrowser);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        msgEdit = new QLineEdit(page_2);
        msgEdit->setObjectName(QString::fromUtf8("msgEdit"));

        horizontalLayout_2->addWidget(msgEdit);

        sendBtn = new QPushButton(page_2);
        sendBtn->setObjectName(QString::fromUtf8("sendBtn"));

        horizontalLayout_2->addWidget(sendBtn);


        verticalLayout_3->addLayout(horizontalLayout_2);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        userListWidget = new QListWidget(page_2);
        userListWidget->setObjectName(QString::fromUtf8("userListWidget"));

        verticalLayout_4->addWidget(userListWidget);

        exitBtn = new QPushButton(page_2);
        exitBtn->setObjectName(QString::fromUtf8("exitBtn"));

        verticalLayout_4->addWidget(exitBtn);


        horizontalLayout->addLayout(verticalLayout_4);

        stackedWidget->addWidget(page_2);

        verticalLayout->addWidget(stackedWidget);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\345\220\264\347\205\234\346\226\207\347\232\204\350\201\212\345\244\251\345\256\244", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\346\234\215\345\212\241\345\231\250IP\345\234\260\345\235\200\357\274\232", nullptr));
        ipEdit->setText(QCoreApplication::translate("MainWindow", "127.0.0.1", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\346\230\265\347\247\260\357\274\232", nullptr));
        loginBtn->setText(QCoreApplication::translate("MainWindow", "\347\231\273\345\275\225\350\201\212\345\244\251\345\256\244", nullptr));
        sendBtn->setText(QCoreApplication::translate("MainWindow", "\345\217\221\351\200\201", nullptr));
        exitBtn->setText(QCoreApplication::translate("MainWindow", "\351\200\200\345\207\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
