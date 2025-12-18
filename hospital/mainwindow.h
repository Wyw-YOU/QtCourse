#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class DbManager;
class LoginWidget;
class QStackedWidget;
class QSqlTableModel;
class QTableView;
class QLineEdit;
class QLabel;
class QAction;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLogin(const QString &username, const QString &password);

    void addPatient();
    void editPatient();
    void deletePatient();
    void refreshPatients();
    void searchPatients();

private:
    void setupLoginPage();
    void setupPatientPage();
    void setupToolbar();
    void setupStatusBar();

private:
    DbManager *m_dbManager;

    QStackedWidget *m_stack;
    LoginWidget *m_loginWidget;

    QWidget *m_patientPage;
    QSqlTableModel *m_patientModel;
    QTableView *m_patientView;
    QLineEdit *m_searchEdit;

    QAction *m_addAction;
    QAction *m_editAction;
    QAction *m_delAction;
    QAction *m_refreshAction;

    QLabel *m_devLabel;

    bool m_toolbarInitialized;
};

#endif // MAINWINDOW_H
