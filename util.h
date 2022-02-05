#ifndef UTIL_H
#define UTIL_H

#include <../config.h>
#include <QtSql>
#include <QMessageBox>

namespace dbconn{

QSqlDatabase getDbOpenConn(QWidget *parent = nullptr, QString name = "");

QSqlError dbUserInit();

QSqlError dbProductInit();

}

class ProductSqlModel : public QSqlQueryModel
{
public:
    explicit ProductSqlModel(QObject *parent = nullptr);
    ~ProductSqlModel();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};




#endif // UTIL_H


