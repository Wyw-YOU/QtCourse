/********************************************************************************
** Form generated from reading UI file 'avatarcropdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AVATARCROPDIALOG_H
#define UI_AVATARCROPDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_AvatarCropDialog
{
public:
    QLabel *label;
    QPushButton *confirmButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *AvatarCropDialog)
    {
        if (AvatarCropDialog->objectName().isEmpty())
            AvatarCropDialog->setObjectName(QString::fromUtf8("AvatarCropDialog"));
        AvatarCropDialog->resize(389, 300);
        label = new QLabel(AvatarCropDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(90, 30, 200, 200));
        confirmButton = new QPushButton(AvatarCropDialog);
        confirmButton->setObjectName(QString::fromUtf8("confirmButton"));
        confirmButton->setGeometry(QRect(240, 253, 51, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Microsoft YaHei"));
        confirmButton->setFont(font);
        cancelButton = new QPushButton(AvatarCropDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(313, 253, 51, 31));
        cancelButton->setFont(font);

        retranslateUi(AvatarCropDialog);

        QMetaObject::connectSlotsByName(AvatarCropDialog);
    } // setupUi

    void retranslateUi(QDialog *AvatarCropDialog)
    {
        AvatarCropDialog->setWindowTitle(QCoreApplication::translate("AvatarCropDialog", "Dialog", nullptr));
        label->setText(QString());
        confirmButton->setText(QCoreApplication::translate("AvatarCropDialog", "\347\241\256\350\256\244", nullptr));
        cancelButton->setText(QCoreApplication::translate("AvatarCropDialog", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AvatarCropDialog: public Ui_AvatarCropDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AVATARCROPDIALOG_H
