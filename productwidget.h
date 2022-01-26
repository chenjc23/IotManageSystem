#ifndef PRODUCTWIDGET_H
#define PRODUCTWIDGET_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>
#include "util.h"

namespace Ui {
class ProductWidget;
}

class ProductWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProductWidget(QWidget *parent = nullptr);
    ~ProductWidget();
    void updateModel();
    void createView();

public slots:
    void editProduct(int itemId);
    void deleteProduct(int itemId);

private:
    Ui::ProductWidget *ui;
    // QSqlQueryModel * productModel;
    ProductSqlModel * productModel;
    QTableView * productView;


};

#endif // PRODUCTWIDGET_H
