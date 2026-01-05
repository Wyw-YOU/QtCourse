/********************************************************************************
** Form generated from reading UI file 'mybubble.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYBUBBLE_H
#define UI_MYBUBBLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_myBubble
{
public:
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *content_label;
    QLabel *headimg_label;

    void setupUi(QWidget *myBubble)
    {
        if (myBubble->objectName().isEmpty())
            myBubble->setObjectName(QString::fromUtf8("myBubble"));
        myBubble->resize(419, 111);
        horizontalLayout = new QHBoxLayout(myBubble);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        content_label = new QLabel(myBubble);
        content_label->setObjectName(QString::fromUtf8("content_label"));
        content_label->setLayoutDirection(Qt::LeftToRight);
        content_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	background-color: rgb(42, 198, 255);\n"
"	border-radius:13px;\n"
"	padding:10px;\n"
"}"));
        content_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(content_label);

        headimg_label = new QLabel(myBubble);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setMinimumSize(QSize(51, 51));
        headimg_label->setMaximumSize(QSize(51, 51));

        horizontalLayout->addWidget(headimg_label);


        retranslateUi(myBubble);

        QMetaObject::connectSlotsByName(myBubble);
    } // setupUi

    void retranslateUi(QWidget *myBubble)
    {
        myBubble->setWindowTitle(QCoreApplication::translate("myBubble", "Form", nullptr));
        content_label->setText(QCoreApplication::translate("myBubble", "\344\275\240\345\234\250\345\271\262\345\225\245\345\221\200\357\274\237", nullptr));
        headimg_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class myBubble: public Ui_myBubble {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYBUBBLE_H
