#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    digitBTNS = { {Qt::Key_0, ui->btnNum0},
                {Qt::Key_1, ui->btnNum1},
                {Qt::Key_2, ui->btnNum2},
                {Qt::Key_3, ui->btnNum3},
                {Qt::Key_4, ui->btnNum4},
                {Qt::Key_5, ui->btnNum5},
                {Qt::Key_6, ui->btnNum6},
                {Qt::Key_7, ui->btnNum7},
                {Qt::Key_8, ui->btnNum8},
                {Qt::Key_0, ui->btnNum9} };

    for(auto& digitBtn : digitBTNS)
    {
        connect(digitBtn, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    }

    // connect(ui->btnNum0, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum1, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum2, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum3, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum4, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum5, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum6, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum7, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum8, SIGNAL(clicked()), this, SLOT(btnNumClicked()));
    // connect(ui->btnNum9, SIGNAL(clicked()), this, SLOT(btnNumClicked()));


    // connect(ui->btnPlus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnMinus, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnMultiple, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    // connect(ui->btnDivide, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));

    // connect(ui->btnPercentage, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnInverse, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnSquare, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    // connect(ui->btnSqrt, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));

    operatorBTNS1 = { ui->btnPlus, ui->btnMinus, ui->btnMultiple, ui->btnDivide };
    for(auto& op_btn : operatorBTNS1)
    {
        connect(op_btn, SIGNAL(clicked()), this, SLOT(btnBinaryOperatorClicked()));
    }

    operatorBTNS2 = { ui->btnPercentage, ui->btnInverse, ui->btnSquare, ui->btnSqrt };
    for(auto& op_btn : operatorBTNS2)
    {
        connect(op_btn, SIGNAL(clicked()), this, SLOT(btnUnaryOperatorClicked()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::calculation()
{
    if(op_safety == false)
        ui->display->setText("除0错误！！！");

    double result = 0;
    if(operands.size() == 2 && opcodes.size() > 0)
    {
        // 获取操作数
        double operand1 = operands.front().toDouble();
        operands.pop_front();
        double operand2 = operands.front().toDouble();
        operands.pop_front();

        // 取操作符
        QString op = opcodes.front();
        opcodes.pop_front();

        // 开始计算
        if(op == '+')
        {
            result = operand1 + operand2;
        }
        else if(op == '-')
        {
            result = operand1 - operand2;
        }
        else if(op == "×")
        {
            result = operand1 * operand2;
        }
        else if(op == "÷")
        {
            // if(operand2 != 0)
                result = operand1 / operand2;
            // else
                op_safety = false;
        }

        operands.push_back(QString::number(result));

        ui->statusbar->showMessage(QString("calculationing... operands size: %1, opcodes size: %2").arg(operands.size()).arg(opcodes.size()));
    }
    else
    {
        ui->statusbar->showMessage(QString("operands size: %1, opcodes size: %2").arg(operands.size()).arg(opcodes.size()));
    }

    return QString::number(result);
}

// 数字
void MainWindow::btnNumClicked()
{
    QString digit = qobject_cast<QPushButton*>(sender())->text();

    // 防止0023
    if(operand == '0' && digit == '0')
        digit = "";

    // 防止023
    if(operand == '0' && digit != '0')
        operand = "";

    operand += digit;
    ui->display->setText(operand);

    // QString str = ui->display->text();
    // str += qobject_cast<QPushButton*>(sender())->text();

    // ui->display->setText(str);
    // ui->statusbar->showMessage("btn clicked");
}

// .
void MainWindow::on_btnPeriod_clicked()
{
    if(!operand.contains('.'))
        operand += qobject_cast<QPushButton*>(sender())->text();

    ui->display->setText(operand);
}

// del
void MainWindow::on_btnDel_clicked()
{
    operand = operand.left(operand.length() - 1);

    ui->display->setText(operand);
}

// 双目运算符
void MainWindow::btnBinaryOperatorClicked()
{
    ui->statusbar->showMessage("last operand" + operand);
    QString opcode = qobject_cast<QPushButton*>(sender())->text();

    if(operand != "")
    {
        operands.push_back(operand);
        operand = "";

        opcodes.push_back(opcode);
    }
    else if(operands.size() == 1)
    {
        opcodes.push_back(opcode);
    }

    QString result = calculation();
    ui->display->setText(result);
}

// 单目运算符
void MainWindow::btnUnaryOperatorClicked()
{
    QString op = qobject_cast<QPushButton*>(sender())->text();
    double result = operand.toDouble();

    if(operand != "")
    {
        operand = "";
    }
    else if(operands.size() == 1)
    {
        result = operands.front().toDouble();
        operands.pop_front();
    }

    if(op == '%')
        result /= 100.0;
    else if(op == "1/X")
        result = 1 / result;
    else if(op == "X²")
        result *= result;
    else if(op == "√x")
        result = sqrt(result);

    operands.push_back(QString::number(result));
    ui->display->setText(QString::number(result));
}

// =
void MainWindow::on_btnEqual_clicked()
{
    if(operand != "")
    {
        operands.push_back(operand);
        operand = "";
    }

    QString result = calculation();

    ui->display->setText(result);
}

// 取反
void MainWindow::on_btnSign_clicked()
{
    double result = operands.front().toDouble();
    operands.pop_front();

    if(result != 0)
    {
        result *= -1;
    }

    operands.push_back(QString::number(result));
    ui->display->setText(QString::number(result));
}

// --------------------清除----------------------
// C
void MainWindow::on_btnClear_clicked()
{
    operand.clear();
    operands.clear();
    opcodes.clear();

    ui->display->setText(operand);
}

// CE
void MainWindow::on_btnClearAll_clicked()
{
    operand.clear();

    ui->display->setText(operand);
}


// ------------键盘输入事件-------------
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    for(auto& btnKey : digitBTNS.keys())
    {
        if(event->key() == btnKey)
        {
            digitBTNS[btnKey]->animateClick();
        }
    }
}































