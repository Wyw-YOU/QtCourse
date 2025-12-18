#ifndef PATIENTDIALOG_H
#define PATIENTDIALOG_H

#include <QDialog>
#include <QDate>

class QLabel;
class QLineEdit;
class QComboBox;
class QDateEdit;
class QDoubleSpinBox;
class QPushButton;

class PatientDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PatientDialog(QWidget *parent = nullptr);

    void setPatientData(int id,
                        const QString &name,
                        const QString &idCard,
                        const QString &gender,
                        const QDate &birth,
                        double height,
                        double weight,
                        const QString &phone,
                        const QString &diagnosis = QString());

    int patientId() const;
    QString name() const;
    QString idCard() const;
    QString gender() const;
    QDate birthDate() const;
    double height() const;
    double weight() const;
    QString phone() const;
    QString diagnosis() const;

private:
    QLabel *m_idLabel;
    QLineEdit *m_nameEdit;
    QLineEdit *m_idCardEdit;
    QComboBox *m_genderCombo;
    QDateEdit *m_birthEdit;
    QDoubleSpinBox *m_heightSpin;
    QDoubleSpinBox *m_weightSpin;
    QLineEdit *m_phoneEdit;
    QLineEdit *m_diagEdit;

    QPushButton *m_saveBtn;
    QPushButton *m_cancelBtn;

    int m_idValue;
};

#endif // PATIENTDIALOG_H
