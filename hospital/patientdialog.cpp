#include "patientdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>

PatientDialog::PatientDialog(QWidget *parent)
    : QDialog(parent)
    , m_idLabel(new QLabel(this))
    , m_nameEdit(new QLineEdit(this))
    , m_idCardEdit(new QLineEdit(this))
    , m_genderCombo(new QComboBox(this))
    , m_birthEdit(new QDateEdit(this))
    , m_heightSpin(new QDoubleSpinBox(this))
    , m_weightSpin(new QDoubleSpinBox(this))
    , m_phoneEdit(new QLineEdit(this))
    , m_diagEdit(new QLineEdit(this))
    , m_saveBtn(new QPushButton(tr("保存"), this))
    , m_cancelBtn(new QPushButton(tr("取消"), this))
    , m_idValue(-1)
{
    setWindowTitle(tr("编辑患者信息"));
    setModal(true);
    setMinimumWidth(520);

    m_idLabel->setText(tr(""));
    m_idLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_idLabel->setMinimumWidth(80);

    m_genderCombo->addItem(tr("男"));
    m_genderCombo->addItem(tr("女"));
    m_genderCombo->addItem(tr("其他"));

    m_birthEdit->setCalendarPopup(true);
    m_birthEdit->setDisplayFormat("yyyy/MM/dd");
    m_birthEdit->setDate(QDate::fromString("2000/01/01", "yyyy/MM/dd"));

    m_heightSpin->setRange(0.0, 300.0);
    m_heightSpin->setDecimals(1);
    m_heightSpin->setSuffix(tr(" cm"));
    m_heightSpin->setSingleStep(0.5);

    m_weightSpin->setRange(0.0, 500.0);
    m_weightSpin->setDecimals(1);
    m_weightSpin->setSuffix(tr(" kg"));
    m_weightSpin->setSingleStep(0.1);

    m_phoneEdit->setPlaceholderText(tr("手机号"));
    m_idCardEdit->setPlaceholderText(tr("身份证号"));
    m_diagEdit->setPlaceholderText(tr("诊断信息"));

    QFormLayout *form = new QFormLayout;
    form->addRow(tr("ID:"), m_idLabel);
    form->addRow(tr("姓名:"), m_nameEdit);
    form->addRow(tr("身份证:"), m_idCardEdit);
    form->addRow(tr("性别:"), m_genderCombo);
    form->addRow(tr("出生日期:"), m_birthEdit);
    form->addRow(tr("身高:"), m_heightSpin);
    form->addRow(tr("体重:"), m_weightSpin);
    form->addRow(tr("手机号:"), m_phoneEdit);
    form->addRow(tr("诊断:"), m_diagEdit);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(m_saveBtn);
    btnLayout->addWidget(m_cancelBtn);

    QVBoxLayout *main = new QVBoxLayout;
    main->addLayout(form);
    main->addSpacing(8);
    main->addLayout(btnLayout);
    main->setContentsMargins(12, 12, 12, 12);

    setLayout(main);

    connect(m_saveBtn, &QPushButton::clicked, this, &PatientDialog::accept);
    connect(m_cancelBtn, &QPushButton::clicked, this, &PatientDialog::reject);
}

void PatientDialog::setPatientData(int id,
                                   const QString &name,
                                   const QString &idCard,
                                   const QString &gender,
                                   const QDate &birth,
                                   double height,
                                   double weight,
                                   const QString &phone,
                                   const QString &diagnosis)
{
    m_idValue = id;
    if (id >= 0) m_idLabel->setText(QString::number(id));
    else m_idLabel->setText(tr(""));

    m_nameEdit->setText(name);
    m_idCardEdit->setText(idCard);

    int idx = m_genderCombo->findText(gender);
    if (idx >= 0) m_genderCombo->setCurrentIndex(idx);
    else m_genderCombo->setCurrentIndex(0);

    if (birth.isValid()) m_birthEdit->setDate(birth);
    m_heightSpin->setValue(height);
    m_weightSpin->setValue(weight);
    m_phoneEdit->setText(phone);
    m_diagEdit->setText(diagnosis);
}

int PatientDialog::patientId() const { return m_idValue; }
QString PatientDialog::name() const { return m_nameEdit->text().trimmed(); }
QString PatientDialog::idCard() const { return m_idCardEdit->text().trimmed(); }
QString PatientDialog::gender() const { return m_genderCombo->currentText(); }
QDate PatientDialog::birthDate() const { return m_birthEdit->date(); }
double PatientDialog::height() const { return m_heightSpin->value(); }
double PatientDialog::weight() const { return m_weightSpin->value(); }
QString PatientDialog::phone() const { return m_phoneEdit->text().trimmed(); }
QString PatientDialog::diagnosis() const { return m_diagEdit->text().trimmed(); }
