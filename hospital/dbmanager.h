#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>
#include <QSqlDatabase>

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);

    bool openDatabase(const QString &dbFilePath);
    void initDatabase();

    bool checkUserLogin(const QString &username, const QString &password);

    QSqlDatabase database() const { return m_db; }

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
