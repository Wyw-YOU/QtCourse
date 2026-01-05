/********************************************************************************
** Form generated from reading UI file 'addfriend.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFRIEND_H
#define UI_ADDFRIEND_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddFriend
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QLineEdit *account_edit;
    QPushButton *searchBtn;
    QHBoxLayout *horizontalLayout_2;
    QLabel *headimg_label;
    QVBoxLayout *verticalLayout;
    QLabel *account_label;
    QLabel *name_label;
    QPushButton *addBtn;

    void setupUi(QWidget *AddFriend)
    {
        if (AddFriend->objectName().isEmpty())
            AddFriend->setObjectName(QString::fromUtf8("AddFriend"));
        AddFriend->resize(505, 300);
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        AddFriend->setFont(font);
        widget = new QWidget(AddFriend);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 10, 471, 281));
        verticalLayout_2 = new QVBoxLayout(widget);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        account_edit = new QLineEdit(widget);
        account_edit->setObjectName(QString::fromUtf8("account_edit"));
        account_edit->setMinimumSize(QSize(300, 35));
        account_edit->setMaximumSize(QSize(300, 35));

        horizontalLayout->addWidget(account_edit);

        searchBtn = new QPushButton(widget);
        searchBtn->setObjectName(QString::fromUtf8("searchBtn"));
        searchBtn->setMinimumSize(QSize(60, 35));
        searchBtn->setMaximumSize(QSize(60, 35));
        searchBtn->setFont(font);
        searchBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-radius:8px;\n"
"	background-color: rgb(11, 184, 243);\n"
"}"));

        horizontalLayout->addWidget(searchBtn);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        headimg_label = new QLabel(widget);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setMinimumSize(QSize(120, 120));
        headimg_label->setMaximumSize(QSize(120, 120));

        horizontalLayout_2->addWidget(headimg_label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        account_label = new QLabel(widget);
        account_label->setObjectName(QString::fromUtf8("account_label"));
        account_label->setFont(font);
        account_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        verticalLayout->addWidget(account_label);

        name_label = new QLabel(widget);
        name_label->setObjectName(QString::fromUtf8("name_label"));
        name_label->setFont(font);
        name_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-color: rgb(0, 0, 0);\n"
"	border: 2px solid black;\n"
"	border-radius:12px;\n"
"}"));

        verticalLayout->addWidget(name_label);


        horizontalLayout_2->addLayout(verticalLayout);

        addBtn = new QPushButton(widget);
        addBtn->setObjectName(QString::fromUtf8("addBtn"));
        addBtn->setMinimumSize(QSize(60, 35));
        addBtn->setMaximumSize(QSize(60, 35));
        addBtn->setFont(font);
        addBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(11, 184, 243);\n"
"	border-radius:12px;\n"
"}"));

        horizontalLayout_2->addWidget(addBtn);


        verticalLayout_2->addLayout(horizontalLayout_2);


        retranslateUi(AddFriend);

        QMetaObject::connectSlotsByName(AddFriend);
    } // setupUi

    void retranslateUi(QWidget *AddFriend)
    {
        AddFriend->setWindowTitle(QCoreApplication::translate("AddFriend", "Form", nullptr));
        searchBtn->setText(QCoreApplication::translate("AddFriend", "\346\220\234\347\264\242", nullptr));
        headimg_label->setText(QCoreApplication::translate("AddFriend", "\345\244\264\345\203\217", nullptr));
        account_label->setText(QCoreApplication::translate("AddFriend", "\350\264\246\345\217\267\357\274\232", nullptr));
        name_label->setText(QCoreApplication::translate("AddFriend", "\346\230\265\347\247\260\357\274\232", nullptr));
        addBtn->setText(QCoreApplication::translate("AddFriend", "\346\267\273\345\212\240", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddFriend: public Ui_AddFriend {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFRIEND_H
