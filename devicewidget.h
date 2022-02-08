#ifndef DEVICEWIDGET_H
#define DEVICEWIDGET_H

#include <QtWidgets>
#include "util.h"
#include <QSqlQueryModel>
#include <QTableView>

namespace Ui {
class DeviceWidget;
}

class DeviceWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceWidget(QWidget *parent = nullptr);
    ~DeviceWidget();

    void productBoxRefresh();

public slots:

    void refresh(const QString &text = QString());
    void showEditDeviceWidget(int id = 0);
    void showViewDeviceWidget(int id);
    void deleteDevice(int id);


private:
    Ui::DeviceWidget *ui;

    QComboBox * productChooseComboBox;
    BlueBt * addDeviceBt;
    QLineEdit * searchEdit;

    QStringList productList;
    QStringListModel * productModel;

    QSqlQueryModel * sqlModel;
    QTableView * deviceView;

private:


};

#endif // DEVICEWIDGET_H
