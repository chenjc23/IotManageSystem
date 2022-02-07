#ifndef UTIL_H
#define UTIL_H

#include <../config.h>
#include <QtSql>
#include <QMessageBox>
#include <QPushButton>

namespace dbconn{

QSqlDatabase getDbOpenConn(QWidget *parent = nullptr, QString name = "");

QSqlError dbUserInit();

QSqlError dbProductInit();

}

namespace msg{

int getCancelMsgBox();

}

class CenterAlignSqlModel : public QSqlQueryModel
{
public:
    explicit CenterAlignSqlModel(QObject *parent = nullptr);
    ~CenterAlignSqlModel();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

class BlueBt : public QPushButton
{
public:
    explicit BlueBt(const QString &text, QWidget *parent = nullptr);
    ~BlueBt();

};




#endif // UTIL_H


