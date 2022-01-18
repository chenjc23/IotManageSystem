#ifndef UTIL_H
#define UTIL_H

#include <../config.h>
#include <QtSql>
#include <QMessageBox>

namespace dbconn{

QSqlDatabase getDbOpenConn(QWidget *parent = nullptr)
{
    Config config;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setDatabaseName(config.dbDatabaseName);
    db.setHostName(config.dbHostName);
    db.setUserName(config.dbUserName);
    db.setPassword(config.dbPassword);
    if (!db.open()) {
        if (parent)
            QMessageBox::information(parent, "DB Connection Fail", "无法连接数据库");
    }
    return db;
}

QSqlError dbUserInit()
{
    QSqlDatabase db = getDbOpenConn();
    if (db.lastError().type() != QSqlError::NoError)
        return db.lastError();

    if (db.tables().contains("user", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery query;
    QString userCreationSql = "create table user(id integer primary key auto_increment, "
                              "name varchar(20) not null, password char(32)) not null";
    if (!query.exec(userCreationSql))
        return query.lastError();

    return QSqlError();
}


}


#endif // UTIL_H


