/********************************************************************************
** Form generated from reading UI file 'advancedbubble.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDBUBBLE_H
#define UI_ADVANCEDBUBBLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdvancedBubble
{
public:
    QLabel *headimg_label;
    QLabel *name_label;
    QLabel *content_label;

    void setupUi(QWidget *AdvancedBubble)
    {
        if (AdvancedBubble->objectName().isEmpty())
            AdvancedBubble->setObjectName(QString::fromUtf8("AdvancedBubble"));
        AdvancedBubble->resize(386, 128);
        headimg_label = new QLabel(AdvancedBubble);
        headimg_label->setObjectName(QString::fromUtf8("headimg_label"));
        headimg_label->setGeometry(QRect(10, 10, 51, 51));
        name_label = new QLabel(AdvancedBubble);
        name_label->setObjectName(QString::fromUtf8("name_label"));
        name_label->setGeometry(QRect(80, 10, 68, 21));
        content_label = new QLabel(AdvancedBubble);
        content_label->setObjectName(QString::fromUtf8("content_label"));
        content_label->setGeometry(QRect(80, 40, 291, 71));

        retranslateUi(AdvancedBubble);

        QMetaObject::connectSlotsByName(AdvancedBubble);
    } // setupUi

    void retranslateUi(QWidget *AdvancedBubble)
    {
        AdvancedBubble->setWindowTitle(QCoreApplication::translate("AdvancedBubble", "Form", nullptr));
        headimg_label->setText(QCoreApplication::translate("AdvancedBubble", "\345\244\264\345\203\217", nullptr));
        name_label->setText(QCoreApplication::translate("AdvancedBubble", "TextLabel", nullptr));
        content_label->setText(QCoreApplication::translate("AdvancedBubble", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdvancedBubble: public Ui_AdvancedBubble {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDBUBBLE_H
