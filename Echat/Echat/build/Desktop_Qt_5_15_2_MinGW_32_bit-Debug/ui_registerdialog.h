/********************************************************************************
** Form generated from reading UI file 'registerdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REGISTERDIALOG_H
#define UI_REGISTERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RegisterDialog
{
public:
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *headimg_btn;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout;
    QLabel *label_3;
    QLineEdit *name_edit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_4;
    QLineEdit *account_edit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_5;
    QLineEdit *password_edit;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *confirmButton;

    void setupUi(QDialog *RegisterDialog)
    {
        if (RegisterDialog->objectName().isEmpty())
            RegisterDialog->setObjectName(QString::fromUtf8("RegisterDialog"));
        RegisterDialog->resize(400, 424);
        widget = new QWidget(RegisterDialog);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(50, 20, 281, 391));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        font.setPointSize(12);
        font.setBold(false);
        font.setWeight(50);
        label->setFont(font);

        horizontalLayout_4->addWidget(label);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        headimg_btn = new QPushButton(widget);
        headimg_btn->setObjectName(QString::fromUtf8("headimg_btn"));
        headimg_btn->setMinimumSize(QSize(120, 120));
        headimg_btn->setMaximumSize(QSize(120, 120));
        headimg_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));

        horizontalLayout_4->addWidget(headimg_btn);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_3 = new QLabel(widget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setFont(font);

        horizontalLayout->addWidget(label_3);

        name_edit = new QLineEdit(widget);
        name_edit->setObjectName(QString::fromUtf8("name_edit"));
        name_edit->setMinimumSize(QSize(0, 40));

        horizontalLayout->addWidget(name_edit);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_4 = new QLabel(widget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setFont(font);

        horizontalLayout_2->addWidget(label_4);

        account_edit = new QLineEdit(widget);
        account_edit->setObjectName(QString::fromUtf8("account_edit"));
        account_edit->setMinimumSize(QSize(0, 40));

        horizontalLayout_2->addWidget(account_edit);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_5 = new QLabel(widget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setFont(font);

        horizontalLayout_3->addWidget(label_5);

        password_edit = new QLineEdit(widget);
        password_edit->setObjectName(QString::fromUtf8("password_edit"));
        password_edit->setMinimumSize(QSize(0, 40));

        horizontalLayout_3->addWidget(password_edit);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        confirmButton = new QPushButton(widget);
        confirmButton->setObjectName(QString::fromUtf8("confirmButton"));
        confirmButton->setMaximumSize(QSize(55, 40));

        horizontalLayout_5->addWidget(confirmButton);


        verticalLayout->addLayout(horizontalLayout_5);


        retranslateUi(RegisterDialog);

        QMetaObject::connectSlotsByName(RegisterDialog);
    } // setupUi

    void retranslateUi(QDialog *RegisterDialog)
    {
        RegisterDialog->setWindowTitle(QCoreApplication::translate("RegisterDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("RegisterDialog", "\345\244\264\345\203\217:", nullptr));
        headimg_btn->setText(QString());
        label_3->setText(QCoreApplication::translate("RegisterDialog", "\346\230\265\347\247\260\357\274\232", nullptr));
        label_4->setText(QCoreApplication::translate("RegisterDialog", "\350\264\246\345\217\267\357\274\232", nullptr));
        label_5->setText(QCoreApplication::translate("RegisterDialog", "\345\257\206\347\240\201\357\274\232", nullptr));
        confirmButton->setText(QCoreApplication::translate("RegisterDialog", "\346\217\220\344\272\244", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RegisterDialog: public Ui_RegisterDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REGISTERDIALOG_H
