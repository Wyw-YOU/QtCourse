#include "loginwidget.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QWidget>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent)
    , m_titleLabel(new QLabel(this))
    , m_userEdit(new QLineEdit(this))
    , m_passEdit(new QLineEdit(this))
    , m_registerBtn(new QPushButton(tr("注册"), this))
    , m_loginBtn(new QPushButton(tr("登录"), this))
{
    m_titleLabel->setText(tr("欢迎使用诊疗测试系统"));
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("font-size:18px; font-weight:600;");

    m_userEdit->setPlaceholderText(tr("用户名"));
    m_passEdit->setPlaceholderText(tr("密码"));
    m_passEdit->setEchoMode(QLineEdit::Password);

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("用户名："), m_userEdit);
    form->addRow(tr("密码："), m_passEdit);

    m_registerBtn->setFixedWidth(100);
    m_loginBtn->setFixedWidth(100);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_registerBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_loginBtn);

    QWidget *container = new QWidget(this);
    QVBoxLayout *containerLayout = new QVBoxLayout(container);
    containerLayout->addWidget(m_titleLabel);
    containerLayout->addSpacing(12);
    containerLayout->addLayout(form);
    containerLayout->addSpacing(12);
    containerLayout->addLayout(btnLayout);
    containerLayout->setContentsMargins(20, 12, 20, 12);
    container->setLayout(containerLayout);
    container->setFixedSize(420, 240);

    QHBoxLayout *outerH = new QHBoxLayout;
    outerH->addStretch();
    outerH->addWidget(container, 0, Qt::AlignCenter);
    outerH->addStretch();

    QVBoxLayout *outerV = new QVBoxLayout(this);
    outerV->addStretch();
    outerV->addLayout(outerH);
    outerV->addStretch();
    outerV->setContentsMargins(12, 12, 12, 12);

    setLayout(outerV);

    connect(m_loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginButtonClicked);
    connect(m_registerBtn, &QPushButton::clicked, this, &LoginWidget::onRegisterButtonClicked);
}

void LoginWidget::onLoginButtonClicked()
{
    emit loginRequested(m_userEdit->text().trimmed(), m_passEdit->text());
}

void LoginWidget::onRegisterButtonClicked()
{
    emit registerRequested();
}
