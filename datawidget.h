#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QtWidgets>
#include "util.h"
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QTimer>

class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(int deviceID, QWidget *parent = nullptr);
    QWidget * getShowWidget(int deviceID, const QString &identifier);

public slots:
    void refresh();

private:
    int deviceID;
    int maxcolumn = 5;

    QSqlQueryModel * sqlModel;
    QDataWidgetMapper * mapper;

    QTimer * timer;


signals:

};


class DataBox : public QWidget
{
    Q_OBJECT
public:
    explicit DataBox(const QString &text, const QString &data,
                     const QString &unit, QWidget *parent = nullptr);
    Q_PROPERTY(QString data READ data WRITE setData);

    QString data();
    void setData(const QString &text);

private:
    QLabel * dataLabel;

signals:
    void labelClicked();
};

#endif // DATAWIDGET_H
