/********************************************************************************
** Form generated from reading UI file 'userinfopopup.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_USERINFOPOPUP_H
#define UI_USERINFOPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_UserInfoPopup
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *headimg;
    QVBoxLayout *verticalLayout;
    QLabel *name;
    QLabel *account;

    void setupUi(QWidget *UserInfoPopup)
    {
        if (UserInfoPopup->objectName().isEmpty())
            UserInfoPopup->setObjectName(QString::fromUtf8("UserInfoPopup"));
        UserInfoPopup->resize(290, 160);
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        UserInfoPopup->setFont(font);
        UserInfoPopup->setStyleSheet(QString::fromUtf8("QWidget{\n"
"	border-radius:12px;\n"
"	background-color: rgb(255, 255, 255);\n"
"}"));
        horizontalLayout = new QHBoxLayout(UserInfoPopup);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        headimg = new QLabel(UserInfoPopup);
        headimg->setObjectName(QString::fromUtf8("headimg"));
        headimg->setMinimumSize(QSize(100, 100));
        headimg->setMaximumSize(QSize(100, 100));
        headimg->setFont(font);

        horizontalLayout->addWidget(headimg);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        name = new QLabel(UserInfoPopup);
        name->setObjectName(QString::fromUtf8("name"));
        name->setFont(font);

        verticalLayout->addWidget(name);

        account = new QLabel(UserInfoPopup);
        account->setObjectName(QString::fromUtf8("account"));
        account->setFont(font);

        verticalLayout->addWidget(account);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(UserInfoPopup);

        QMetaObject::connectSlotsByName(UserInfoPopup);
    } // setupUi

    void retranslateUi(QWidget *UserInfoPopup)
    {
        UserInfoPopup->setWindowTitle(QCoreApplication::translate("UserInfoPopup", "Form", nullptr));
        headimg->setText(QCoreApplication::translate("UserInfoPopup", "\345\244\264\345\203\217", nullptr));
        name->setText(QCoreApplication::translate("UserInfoPopup", "windy", nullptr));
        account->setText(QCoreApplication::translate("UserInfoPopup", "\350\264\246\345\217\267\357\274\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class UserInfoPopup: public Ui_UserInfoPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_USERINFOPOPUP_H
