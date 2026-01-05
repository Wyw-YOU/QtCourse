#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QTextEdit>  // 改为 QTextEdit 以支持更好的文本显示

class CustomMessageBox : public QDialog
{
    Q_OBJECT

public:
    enum IconType {
        Information,
        Warning,
        Critical,
        Question
    };

    explicit CustomMessageBox(QWidget *parent = nullptr);
    void setMessage(const QString &title, const QString &text, IconType icon = Information);

    // 静态方法，方便直接调用
    static void showInformation(QWidget *parent, const QString &title, const QString &text);
    static void showWarning(QWidget *parent, const QString &title, const QString &text);
    static void showCritical(QWidget *parent, const QString &title, const QString &text);

private slots:
    void onOkClicked();

private:
    void setupUI();
    void setIcon(IconType type);
    void adjustSizeToContent();  // 新增：根据内容调整大小

    QLabel *m_iconLabel;
    QLabel *m_titleLabel;
    QTextEdit *m_textEdit;  // 改为 QTextEdit
    QPushButton *m_okButton;
};

#endif // CUSTOMMESSAGEBOX_H
