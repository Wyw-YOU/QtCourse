#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DbManager::DbManager(QObject *parent)
    : QObject(parent)
{
}

bool DbManager::openDatabase(const QString &dbFilePath)
{
    if (QSqlDatabase::contains("hospital_connection")) {
        m_db = QSqlDatabase::database("hospital_connection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "hospital_connection");
        m_db.setDatabaseName(dbFilePath);
    }

    if (!m_db.open()) {
        qDebug() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    return true;
}

void DbManager::initDatabase()
{
    QSqlQuery query(m_db);

    // users 表
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "username TEXT UNIQUE NOT NULL,"
            "password TEXT NOT NULL"
            ")"
            )) {
        qDebug() << "Create users failed:" << query.lastError().text();
    }

    // 插入默认用户（若无）
    if (query.exec("SELECT COUNT(*) FROM users")) {
        if (query.next()) {
            int cnt = query.value(0).toInt();
            if (cnt == 0) {
                QSqlQuery ins(m_db);
                ins.prepare("INSERT INTO users (username, password) VALUES (?, ?)");
                ins.addBindValue("admin");
                ins.addBindValue("123456");
                if (!ins.exec()) qDebug() << "Insert default user failed:" << ins.lastError().text();
            }
        }
    }

    // patients 表（包含扩展字段）
    if (!query.exec(
            "CREATE TABLE IF NOT EXISTS patients ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "name TEXT NOT NULL,"
            "idcard TEXT,"
            "gender TEXT,"
            "birthdate TEXT,"   // yyyy-MM-dd
            "height REAL,"      // cm
            "weight REAL,"      // kg
            "phone TEXT,"
            "diagnosis TEXT"    // 可扩展字段：诊断信息
            ")"
            )) {
        qDebug() << "Create patients failed:" << query.lastError().text();
    }

    // 插入示例数据（若为空）
    if (query.exec("SELECT COUNT(*) FROM patients")) {
        if (query.next()) {
            int cnt = query.value(0).toInt();
            if (cnt == 0) {
                QSqlQuery ins(m_db);
                ins.prepare("INSERT INTO patients (name, idcard, gender, birthdate, height, weight, phone, diagnosis) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
                ins.addBindValue("张三");
                ins.addBindValue("110101199001010001");
                ins.addBindValue("男");
                ins.addBindValue("1990-01-01");
                ins.addBindValue(170.0);
                ins.addBindValue(65.0);
                ins.addBindValue("13800000001");
                ins.addBindValue("感冒");
                if (!ins.exec()) qDebug() << "Insert sample patient1 failed:" << ins.lastError().text();

                ins.clear();
                ins.addBindValue("李四");
                ins.addBindValue("110101199502020002");
                ins.addBindValue("女");
                ins.addBindValue("1995-02-02");
                ins.addBindValue(160.0);
                ins.addBindValue(52.0);
                ins.addBindValue("13800000002");
                ins.addBindValue("过敏");
                if (!ins.exec()) qDebug() << "Insert sample patient2 failed:" << ins.lastError().text();
            }
        }
    }
}

bool DbManager::checkUserLogin(const QString &username, const QString &password)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM users WHERE username = ? AND password = ?");
    query.addBindValue(username);
    query.addBindValue(password);

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError().text();
        return false;
    }
    return query.next();
}
