#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel.h>
#include <QStack>
#include <QKeyEvent>
#include <QMap>
#include <QPushButton>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool op_safety;
    QString operand;
    QString opcode;
    QStack<QString> operands;
    QStack<QString> opcodes;
    QMap<int, QPushButton*> digitBTNS;
    QVector<QPushButton*> operatorBTNS1;
    QVector<QPushButton*> operatorBTNS2;

    QString calculation();

private slots:
    void btnNumClicked();
    void btnBinaryOperatorClicked();
    void btnUnaryOperatorClicked();

    void on_btnPeriod_clicked();

    void on_btnDel_clicked();

    void on_btnClear_clicked();

    void on_btnEqual_clicked();

    void on_btnClearAll_clicked();

    void on_btnSign_clicked();

    virtual void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;

    QLabel *statusMsgLabel;
};
#endif // MAINWINDOW_H
