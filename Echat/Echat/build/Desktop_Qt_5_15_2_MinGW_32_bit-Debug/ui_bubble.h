/********************************************************************************
** Form generated from reading UI file 'bubble.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BUBBLE_H
#define UI_BUBBLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Bubble
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer_2;
    QLabel *headimg_label;
    QLabel *label;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout;
    QLabel *name_label;
    QLabel *content_label;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *Bubble)
    {
        if (Bubble->objectName().isEmpty())
            Bubble->setObjectName(QString::fromUtf8("Bubble"));
        Bubble->resize(419, 111);
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        Bubble->setFont(font);
        Bubble->setLayoutDirection(Qt::LeftToRight);
        horizontalLayout = new QHBoxLayout(Bubble);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);

        headimg_label = new QLabel(Bubble);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setMinimumSize(QSize(51, 51));
        headimg_label->setMaximumSize(QSize(51, 51));
        headimg_label->setLayoutDirection(Qt::LeftToRight);
        headimg_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);

        verticalLayout_3->addWidget(headimg_label);

        label = new QLabel(Bubble);
        label->setObjectName(QString::fromUtf8("label"));

        verticalLayout_3->addWidget(label);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);


        horizontalLayout->addLayout(verticalLayout_3);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        name_label = new QLabel(Bubble);
        name_label->setObjectName(QString::fromUtf8("name_label"));

        verticalLayout->addWidget(name_label);

        content_label = new QLabel(Bubble);
        content_label->setObjectName(QString::fromUtf8("content_label"));
        content_label->setStyleSheet(QString::fromUtf8("QLabel{\n"
"	background-color: rgb(255, 255, 255);\n"
"	border-radius:13px;\n"
"	padding:10px;\n"
"}"));
        content_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout->addWidget(content_label);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        retranslateUi(Bubble);

        QMetaObject::connectSlotsByName(Bubble);
    } // setupUi

    void retranslateUi(QWidget *Bubble)
    {
        Bubble->setWindowTitle(QCoreApplication::translate("Bubble", "Form", nullptr));
        headimg_label->setText(QString());
        label->setText(QString());
        name_label->setText(QCoreApplication::translate("Bubble", "windy", nullptr));
        content_label->setText(QCoreApplication::translate("Bubble", "\344\275\240\345\234\250\345\271\262\345\225\245\345\221\200\357\274\237", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Bubble: public Ui_Bubble {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BUBBLE_H
