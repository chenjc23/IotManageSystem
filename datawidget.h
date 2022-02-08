#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QtWidgets>
#include "util.h"
#include <QSqlQueryModel>

class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(int deviceID, QWidget *parent = nullptr);

    void getProductID();

    void refresh();
private:
    int deviceID;
    int productID;

    int maxcolumn = 5;

    QSqlQueryModel * sqlModel;

signals:

};


class DataBox : public QWidget
{
    Q_OBJECT
public:
    explicit DataBox(const QString &text, const QString &data,
                     const QString &unit, QWidget *parent = nullptr);
//    Q_PROPERTY(QString data READ data WRITE setData);

//    QString data();
//    void setData(const QString &text);
};

#endif // DATAWIDGET_H
