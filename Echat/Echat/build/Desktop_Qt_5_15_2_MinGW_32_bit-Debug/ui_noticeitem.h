/********************************************************************************
** Form generated from reading UI file 'noticeitem.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NOTICEITEM_H
#define UI_NOTICEITEM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_noticeitem
{
public:
    QHBoxLayout *horizontalLayout_2;
    QLabel *headimg_label;
    QVBoxLayout *verticalLayout;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *friend_name;
    QSpacerItem *horizontalSpacer_2;
    QLabel *time_label;
    QSpacerItem *horizontalSpacer;
    QPushButton *acceptBtn;
    QPushButton *rejectBtn;
    QLabel *msg_label;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *noticeitem)
    {
        if (noticeitem->objectName().isEmpty())
            noticeitem->setObjectName(QString::fromUtf8("noticeitem"));
        noticeitem->resize(456, 113);
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        noticeitem->setFont(font);
        horizontalLayout_2 = new QHBoxLayout(noticeitem);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        headimg_label = new QLabel(noticeitem);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setMinimumSize(QSize(51, 51));
        headimg_label->setMaximumSize(QSize(51, 51));

        horizontalLayout_2->addWidget(headimg_label);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        friend_name = new QLabel(noticeitem);
        friend_name->setObjectName(QString::fromUtf8("friend_name"));

        horizontalLayout->addWidget(friend_name);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        time_label = new QLabel(noticeitem);
        time_label->setObjectName(QString::fromUtf8("time_label"));

        horizontalLayout->addWidget(time_label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        acceptBtn = new QPushButton(noticeitem);
        acceptBtn->setObjectName(QString::fromUtf8("acceptBtn"));
        acceptBtn->setMinimumSize(QSize(65, 40));
        acceptBtn->setMaximumSize(QSize(65, 40));
        acceptBtn->setFont(font);
        acceptBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	background-color: rgb(1, 188, 250);\n"
"	border-radius:6px;\n"
"	color: rgb(255, 255, 255);\n"
"}"));

        horizontalLayout->addWidget(acceptBtn);

        rejectBtn = new QPushButton(noticeitem);
        rejectBtn->setObjectName(QString::fromUtf8("rejectBtn"));
        rejectBtn->setMinimumSize(QSize(65, 40));
        rejectBtn->setMaximumSize(QSize(65, 40));
        rejectBtn->setFont(font);
        rejectBtn->setStyleSheet(QString::fromUtf8("QPushButton{\n"
"	border-radius:6px;\n"
"	color: rgb(255, 255, 255);\n"
"	background-color: rgb(252, 14, 0);\n"
"}"));

        horizontalLayout->addWidget(rejectBtn);


        verticalLayout->addLayout(horizontalLayout);

        msg_label = new QLabel(noticeitem);
        msg_label->setObjectName(QString::fromUtf8("msg_label"));

        verticalLayout->addWidget(msg_label);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(noticeitem);

        QMetaObject::connectSlotsByName(noticeitem);
    } // setupUi

    void retranslateUi(QWidget *noticeitem)
    {
        noticeitem->setWindowTitle(QCoreApplication::translate("noticeitem", "Form", nullptr));
        headimg_label->setText(QString());
        friend_name->setText(QCoreApplication::translate("noticeitem", "\346\234\213\345\217\2131", nullptr));
        time_label->setText(QCoreApplication::translate("noticeitem", "[19\357\274\23230]", nullptr));
        acceptBtn->setText(QCoreApplication::translate("noticeitem", "\346\216\245\345\217\227", nullptr));
        rejectBtn->setText(QCoreApplication::translate("noticeitem", "\346\213\222\347\273\235", nullptr));
        msg_label->setText(QCoreApplication::translate("noticeitem", "\350\257\267\346\261\202\346\267\273\345\212\240\346\202\250\344\270\272\345\245\275\345\217\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class noticeitem: public Ui_noticeitem {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NOTICEITEM_H
