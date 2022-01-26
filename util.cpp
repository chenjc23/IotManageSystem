#include "util.h"
#include <QtSql>

QSqlDatabase dbconn::getDbOpenConn(QWidget *parent)
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

// user表初始化
QSqlError dbconn::dbUserInit()
{
    QSqlDatabase db = getDbOpenConn();
    if (db.lastError().type() != QSqlError::NoError)
        return db.lastError();

    if (db.tables().contains("user", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery query;
    // 创建user表
    QString userCreationSql = "create table user(id integer primary key auto_increment, "
                              "name varchar(20), password char(32))";
    if (!query.exec(userCreationSql))
        return query.lastError();

    return QSqlError();
}

// product表初始化
QSqlError dbconn::dbProductInit()
{
    QSqlDatabase db = getDbOpenConn();
    if (db.lastError().type() != QSqlError::NoError)
        return db.lastError();

    if (db.tables().contains("product", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery query;
    // 创建product表
    QString productCreationSql = "create table product(id integer primary key auto_increment, "
                                 "name varchar(30), product_type varchar(30), node_type varchar(30), "
                                 "connect_method varchar(30), data_format varchar(30), description varchar(200), "
                                 "create_time datetime default now())";
    if (!query.exec(productCreationSql))
        return query.lastError();

    return QSqlError();
}

ProductSqlModel::ProductSqlModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

ProductSqlModel::~ProductSqlModel()
{

}

QVariant ProductSqlModel::data(const QModelIndex &item, int role) const
{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::TextAlignmentRole) {
        value = Qt::AlignCenter;
        return value;
    }
    return value;
}
