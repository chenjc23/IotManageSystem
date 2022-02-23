#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include "util.h"

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductWidget(QWidget *parent = nullptr);
    ~ProductWidget();
    void updateModel();
    void updateView();
    void refresh();

public slots:
    void onCreateBtClicked();
    void viewProduct(int itemId);
    void editProduct(int itemId);
    void deleteProduct(int itemId);  

private:
    QPushButton * createProductBt;
    CenterAlignSqlModel * productModel;
    QTableView * productView;


};

#endif // PRODUCTWIDGET_H
