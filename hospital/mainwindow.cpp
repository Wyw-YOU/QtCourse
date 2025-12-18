#include "mainwindow.h"
#include "dbmanager.h"
#include "loginwidget.h"
#include "patientdialog.h"

#include <QStackedWidget>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QTableView>
#include <QLineEdit>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QAction>
#include <QHeaderView>
#include <QCoreApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_dbManager(new DbManager(this))
    , m_stack(new QStackedWidget(this))
    , m_loginWidget(nullptr)
    , m_patientPage(nullptr)
    , m_patientModel(nullptr)
    , m_patientView(nullptr)
    , m_searchEdit(nullptr)
    , m_addAction(nullptr)
    , m_editAction(nullptr)
    , m_delAction(nullptr)
    , m_refreshAction(nullptr)
    , m_devLabel(new QLabel(this))
    , m_toolbarInitialized(false)
{
    const QString dbPath = QCoreApplication::applicationDirPath() + "/hospital.db";
    if (!m_dbManager->openDatabase(dbPath)) {
        QMessageBox::critical(this, tr("错误"), tr("无法打开数据库！"));
    } else {
        m_dbManager->initDatabase();
    }

    setupLoginPage();
    setupPatientPage();
    setupStatusBar();

    setCentralWidget(m_stack);
    setWindowTitle(tr("医院诊疗系统 - 患者信息管理"));
    resize(1000, 640);
}

MainWindow::~MainWindow() {}

void MainWindow::setupLoginPage()
{
    m_loginWidget = new LoginWidget(this);
    connect(m_loginWidget, &LoginWidget::loginRequested,
            this, &MainWindow::handleLogin);

    m_stack->addWidget(m_loginWidget);
    m_stack->setCurrentWidget(m_loginWidget);
}

void MainWindow::setupPatientPage()
{
    m_patientPage = new QWidget(this);

    m_patientModel = new QSqlTableModel(this, m_dbManager->database());
    m_patientModel->setTable("patients");
    m_patientModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    m_patientModel->select();

    // 动态设置表头，避免字段顺序不一致导致错位
    QSqlRecord rec = m_patientModel->record();
    auto setHeaderIfExists = [&](const QString &fieldName, const QString &headerText){
        int idx = rec.indexOf(fieldName);
        if (idx >= 0) m_patientModel->setHeaderData(idx, Qt::Horizontal, headerText);
    };

    setHeaderIfExists("id",        tr("ID"));
    setHeaderIfExists("name",      tr("姓名"));
    setHeaderIfExists("idcard",    tr("身份证"));
    setHeaderIfExists("gender",    tr("性别"));
    setHeaderIfExists("birthdate", tr("出生日期"));
    setHeaderIfExists("height",    tr("身高(cm)"));
    setHeaderIfExists("weight",    tr("体重(kg)"));
    setHeaderIfExists("phone",     tr("手机号"));
    setHeaderIfExists("diagnosis", tr("诊断"));

    m_patientView = new QTableView(this);
    m_patientView->setModel(m_patientModel);
    m_patientView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_patientView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_patientView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_patientView->horizontalHeader()->setStretchLastSection(true);
    m_patientView->setAlternatingRowColors(true);
    m_patientView->verticalHeader()->setVisible(false);
    m_patientView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("按姓名搜索..."));

    auto *searchLabel = new QLabel(tr("查询："), this);

    auto *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchLabel);
    searchLayout->addWidget(m_searchEdit);
    searchLayout->addStretch();

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_patientView);

    m_patientPage->setLayout(mainLayout);

    m_stack->addWidget(m_patientPage);

    connect(m_searchEdit, &QLineEdit::returnPressed,
            this, &MainWindow::searchPatients);
}

void MainWindow::setupToolbar()
{
    if (m_toolbarInitialized) return;

    auto *toolBar = addToolBar(tr("操作"));

    m_addAction = new QAction(tr("新增"), this);
    m_editAction = new QAction(tr("编辑"), this);
    m_delAction = new QAction(tr("删除"), this);
    m_refreshAction = new QAction(tr("刷新"), this);

    toolBar->addAction(m_addAction);
    toolBar->addAction(m_editAction);
    toolBar->addAction(m_delAction);
    toolBar->addSeparator();
    toolBar->addAction(m_refreshAction);

    connect(m_addAction, &QAction::triggered, this, &MainWindow::addPatient);
    connect(m_editAction, &QAction::triggered, this, &MainWindow::editPatient);
    connect(m_delAction, &QAction::triggered, this, &MainWindow::deletePatient);
    connect(m_refreshAction, &QAction::triggered, this, &MainWindow::refreshPatients);

    m_toolbarInitialized = true;
}

void MainWindow::setupStatusBar()
{
    m_devLabel->setText(tr("开发者：吴煜文  学号：2023414290132"));
    statusBar()->addPermanentWidget(m_devLabel);
}

void MainWindow::handleLogin(const QString &username, const QString &password)
{
    if (m_dbManager->checkUserLogin(username, password)) {
        // 直接切换页面，不弹提示框
        setupToolbar();
        m_stack->setCurrentWidget(m_patientPage);
    } else {
        QMessageBox::warning(this, tr("警告"), tr("用户名或密码错误"));
    }
}

void MainWindow::addPatient()
{
    PatientDialog dlg(this);
    dlg.setPatientData(-1, "", "", "男", QDate::fromString("2000-01-01", "yyyy-MM-dd"), 0.0, 0.0, "", "");
    if (dlg.exec() == QDialog::Accepted) {
        QSqlDatabase db = m_dbManager->database();
        if (!db.isOpen()) {
            QMessageBox::warning(this, tr("错误"), tr("数据库未打开"));
            return;
        }

        QSqlQuery q(db);
        q.prepare("INSERT INTO patients (name, idcard, gender, birthdate, height, weight, phone, diagnosis) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        q.addBindValue(dlg.name());
        q.addBindValue(dlg.idCard());
        q.addBindValue(dlg.gender());
        q.addBindValue(dlg.birthDate().toString("yyyy-MM-dd"));
        q.addBindValue(dlg.height());
        q.addBindValue(dlg.weight());
        q.addBindValue(dlg.phone());
        q.addBindValue(dlg.diagnosis());

        if (!q.exec()) {
            QMessageBox::warning(this, tr("错误"), tr("插入失败：%1").arg(q.lastError().text()));
            return;
        }

        // 获取 last_insert_rowid 并刷新 model
        QSqlQuery q2(db);
        q2.exec("SELECT last_insert_rowid()");
        qint64 newId = -1;
        if (q2.next()) newId = q2.value(0).toLongLong();

        m_patientModel->select();

        // 定位并选中新插入行（按 id）
        QSqlRecord rec = m_patientModel->record();
        int idIdx = rec.indexOf("id");
        if (idIdx >= 0 && newId > 0) {
            for (int r = 0; r < m_patientModel->rowCount(); ++r) {
                QVariant idv = m_patientModel->data(m_patientModel->index(r, idIdx));
                if (idv.isValid() && idv.toLongLong() == newId) {
                    m_patientView->selectRow(r);
                    m_patientView->scrollTo(m_patientModel->index(r, idIdx));
                    break;
                }
            }
        }
    }
}

void MainWindow::editPatient()
{
    QModelIndex index = m_patientView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::information(this, tr("提示"), tr("请先选择要编辑的患者记录"));
        return;
    }

    int row = index.row();
    QSqlRecord rec = m_patientModel->record();

    int idIdx = rec.indexOf("id");
    int nameIdx = rec.indexOf("name");
    int idcardIdx = rec.indexOf("idcard");
    int genderIdx = rec.indexOf("gender");
    int birthIdx = rec.indexOf("birthdate");
    int heightIdx = rec.indexOf("height");
    int weightIdx = rec.indexOf("weight");
    int phoneIdx = rec.indexOf("phone");
    int diagIdx = rec.indexOf("diagnosis");

    int currentId = (idIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, idIdx)).toInt() : -1;
    QString name = (nameIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, nameIdx)).toString() : QString();
    QString idcard = (idcardIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, idcardIdx)).toString() : QString();
    QString gender = (genderIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, genderIdx)).toString() : QString();
    QString birthStr = (birthIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, birthIdx)).toString() : QString();
    double height = (heightIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, heightIdx)).toDouble() : 0.0;
    double weight = (weightIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, weightIdx)).toDouble() : 0.0;
    QString phone = (phoneIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, phoneIdx)).toString() : QString();
    QString diag = (diagIdx >= 0) ? m_patientModel->data(m_patientModel->index(row, diagIdx)).toString() : QString();

    QDate birth = QDate::fromString(birthStr, "yyyy-MM-dd");
    if (!birth.isValid()) birth = QDate::fromString(birthStr, "yyyy/MM/dd");

    PatientDialog dlg(this);
    dlg.setPatientData(currentId, name, idcard, gender, birth, height, weight, phone, diag);

    if (dlg.exec() == QDialog::Accepted) {
        QSqlDatabase db = m_dbManager->database();
        QSqlQuery q(db);
        q.prepare("UPDATE patients SET name = ?, idcard = ?, gender = ?, birthdate = ?, height = ?, weight = ?, phone = ?, diagnosis = ? WHERE id = ?");
        q.addBindValue(dlg.name());
        q.addBindValue(dlg.idCard());
        q.addBindValue(dlg.gender());
        q.addBindValue(dlg.birthDate().toString("yyyy-MM-dd"));
        q.addBindValue(dlg.height());
        q.addBindValue(dlg.weight());
        q.addBindValue(dlg.phone());
        q.addBindValue(dlg.diagnosis());
        q.addBindValue(currentId);

        if (!q.exec()) {
            QMessageBox::warning(this, tr("错误"), tr("更新失败：%1").arg(q.lastError().text()));
            return;
        }

        m_patientModel->select();
    }
}

void MainWindow::deletePatient()
{
    QModelIndex index = m_patientView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::information(this, tr("提示"), tr("请先选择要删除的患者记录"));
        return;
    }

    int row = index.row();
    QSqlRecord rec = m_patientModel->record();
    int idIdx = rec.indexOf("id");
    if (idIdx < 0) return;

    QVariant idv = m_patientModel->data(m_patientModel->index(row, idIdx));
    if (!idv.isValid()) return;

    if (QMessageBox::question(this, tr("确认"), tr("确定要删除选中的患者记录吗？")) == QMessageBox::Yes) {
        QSqlDatabase db = m_dbManager->database();
        QSqlQuery q(db);
        q.prepare("DELETE FROM patients WHERE id = ?");
        q.addBindValue(idv);
        if (!q.exec()) {
            QMessageBox::warning(this, tr("错误"), tr("删除失败：%1").arg(q.lastError().text()));
            return;
        }
        m_patientModel->select();
    }
}

void MainWindow::refreshPatients()
{
    m_patientModel->select();
}

void MainWindow::searchPatients()
{
    QString keyword = m_searchEdit->text().trimmed();
    if (keyword.isEmpty()) {
        m_patientModel->setFilter(QString());
    } else {
        keyword.replace("'", "''");
        QString filter = QString("name LIKE '%%1%%'").arg(keyword);
        m_patientModel->setFilter(filter);
    }
    m_patientModel->select();
}
