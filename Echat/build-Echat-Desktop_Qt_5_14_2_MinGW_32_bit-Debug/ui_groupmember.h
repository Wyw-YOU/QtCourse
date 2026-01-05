/********************************************************************************
** Form generated from reading UI file 'groupmember.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GROUPMEMBER_H
#define UI_GROUPMEMBER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GroupMember
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *headimg_label;
    QLabel *name_label;

    void setupUi(QWidget *GroupMember)
    {
        if (GroupMember->objectName().isEmpty())
            GroupMember->setObjectName(QString::fromUtf8("GroupMember"));
        GroupMember->resize(236, 52);
        horizontalLayout = new QHBoxLayout(GroupMember);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 1, 1, 1);
        headimg_label = new QLabel(GroupMember);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setMinimumSize(QSize(35, 35));
        headimg_label->setMaximumSize(QSize(35, 35));

        horizontalLayout->addWidget(headimg_label);

        name_label = new QLabel(GroupMember);
        name_label->setObjectName(QString::fromUtf8("name_label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        name_label->setFont(font);

        horizontalLayout->addWidget(name_label);


        retranslateUi(GroupMember);

        QMetaObject::connectSlotsByName(GroupMember);
    } // setupUi

    void retranslateUi(QWidget *GroupMember)
    {
        GroupMember->setWindowTitle(QCoreApplication::translate("GroupMember", "Form", nullptr));
        headimg_label->setText(QString());
        name_label->setText(QCoreApplication::translate("GroupMember", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GroupMember: public Ui_GroupMember {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GROUPMEMBER_H
