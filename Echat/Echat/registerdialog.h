#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include "custommessagebox.h"
#include <QBuffer>
#include <QDebug>
#include "Protocol.h"


namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

    void set_group_register();

    QPixmap m_currentAvatar; // 存储当前头像
    QString m_name;
    QString m_account;
    QString m_password;
    QByteArray m_avatarData;
    int m_choice = 0;

private slots:

    void on_confirmButton_clicked();

    void on_headimg_btn_clicked();

private:
    Ui::RegisterDialog *ui;

    // 头像处理相关方法
    void updateAvatarDisplay();
    QPixmap optimizeAvatar(const QPixmap &original);
    QByteArray getOptimizedAvatarData();
    bool validateAvatarSize(const QByteArray &avatarData);
    bool showSizeWarning(const QByteArray &avatarData);

    // 工具方法
    QString formatFileSize(qint64 bytes);
};

#endif // REGISTERDIALOG_H
