#ifndef VIEWPRODUCTWIDGET_H
#define VIEWPRODUCTWIDGET_H

#include <QtWidgets>
#include <QSqlQueryModel>
#include "util.h"

class ViewProductWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewProductWidget(int productID, QWidget *parent = nullptr);
    QWidget * generateTableWidget();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

private:
    enum page{infoPage, funcPage};
    int productID;
    QSqlQueryModel * sqlModel;

    QPushButton * infoBt;
    QPushButton * funcBt;


    QStackedWidget * stackWidget;

    QLabel * editLabel;

    QLabel *nameLabel, *typeLabel, *nodeLabel, *protocolLabel,
        *connLabel, *dataFormatLabel, *desLabel, *timeLabel;

private:
    void refresh();
    void setMapper();

signals:

};


class AttrWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AttrWidget(int productID, QWidget *parent = nullptr);
    ~AttrWidget();
    void refresh();

public slots:
    void showFuncWidget(int id);

private:
    int productID;
    BlueBt * funcBt;

    CenterAlignSqlModel * attrModel;
    QTableView * attrView;

};


#endif // VIEWPRODUCTWIDGET_H
