/********************************************************************************
** Form generated from reading UI file 'frienditem.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FRIENDITEM_H
#define UI_FRIENDITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_friendItem
{
public:
    QHBoxLayout *horizontalLayout_4;
    QGroupBox *groupBox;
    QLabel *headimg_label;
    QLabel *ret_dot_label;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *friend_name;
    QSpacerItem *horizontalSpacer;
    QLabel *time_label;
    QHBoxLayout *horizontalLayout_2;
    QLabel *msg_label;
    QSpacerItem *horizontalSpacer_2;
    QLabel *unread_label;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *friendItem)
    {
        if (friendItem->objectName().isEmpty())
            friendItem->setObjectName(QString::fromUtf8("friendItem"));
        friendItem->resize(305, 113);
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        friendItem->setFont(font);
        horizontalLayout_4 = new QHBoxLayout(friendItem);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        groupBox = new QGroupBox(friendItem);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(55, 60));
        groupBox->setMaximumSize(QSize(55, 60));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox{\n"
"	border:none;\n"
"}"));
        headimg_label = new QLabel(groupBox);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setGeometry(QRect(0, 8, 51, 51));
        headimg_label->setMinimumSize(QSize(51, 51));
        headimg_label->setMaximumSize(QSize(51, 51));
        ret_dot_label = new QLabel(groupBox);
        ret_dot_label->setObjectName(QString::fromUtf8("ret_dot_label"));
        ret_dot_label->setGeometry(QRect(42, 2, 12, 12));
        ret_dot_label->setMinimumSize(QSize(12, 12));
        ret_dot_label->setMaximumSize(QSize(12, 12));
        ret_dot_label->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 0, 0);\n"
"border-radius:6px;"));

        horizontalLayout_4->addWidget(groupBox);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        friend_name = new QLabel(friendItem);
        friend_name->setObjectName(QString::fromUtf8("friend_name"));

        horizontalLayout->addWidget(friend_name);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        time_label = new QLabel(friendItem);
        time_label->setObjectName(QString::fromUtf8("time_label"));

        horizontalLayout->addWidget(time_label);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        msg_label = new QLabel(friendItem);
        msg_label->setObjectName(QString::fromUtf8("msg_label"));
        QFont font1;
        font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font1.setBold(false);
        font1.setWeight(50);
        msg_label->setFont(font1);
        msg_label->setStyleSheet(QString::fromUtf8("color: rgb(98, 98, 98);"));

        horizontalLayout_2->addWidget(msg_label);

        horizontalSpacer_2 = new QSpacerItem(80, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        unread_label = new QLabel(friendItem);
        unread_label->setObjectName(QString::fromUtf8("unread_label"));
        unread_label->setMinimumSize(QSize(30, 20));
        unread_label->setMaximumSize(QSize(30, 20));
        QFont font2;
        font2.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font2.setBold(true);
        font2.setWeight(75);
        unread_label->setFont(font2);
        unread_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	border-radius:5px;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(150, 158, 204);\n"
"}"));
        unread_label->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(unread_label);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);


        horizontalLayout_4->addLayout(verticalLayout);


        retranslateUi(friendItem);

        QMetaObject::connectSlotsByName(friendItem);
    } // setupUi

    void retranslateUi(QWidget *friendItem)
    {
        friendItem->setWindowTitle(QCoreApplication::translate("friendItem", "Form", nullptr));
        groupBox->setTitle(QString());
        headimg_label->setText(QString());
        ret_dot_label->setText(QString());
        friend_name->setText(QCoreApplication::translate("friendItem", "\346\234\213\345\217\2131", nullptr));
        time_label->setText(QCoreApplication::translate("friendItem", "19\357\274\23230", nullptr));
        msg_label->setText(QCoreApplication::translate("friendItem", "\344\275\240\345\245\275,\347\216\260\345\234\250\345\234\250\345\271\262\345\225\245\345\221\200", nullptr));
        unread_label->setText(QCoreApplication::translate("friendItem", "15", nullptr));
    } // retranslateUi

};

namespace Ui {
    class friendItem: public Ui_friendItem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FRIENDITEM_H
