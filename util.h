#ifndef UTIL_H
#define UTIL_H

#include <../config.h>
#include <QtSql>
#include <QtWidgets>
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopWidget>

namespace dbconn{

QSqlDatabase getDbOpenConn(QWidget *parent = nullptr, QString name = "");

QSqlError dbUserInit();

QSqlError dbProductInit();

QSqlError dbDeviceInit();

}

namespace msg{

int getCancelMsgBox();

}

namespace util {

void setCenterShow(QWidget * widget);

void setBgColor(QWidget * widget, const QString &color = "#99CCCC");

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


