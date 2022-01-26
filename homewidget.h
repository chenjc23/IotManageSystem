#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "productwidget.h"
#include "devicewidget.h"

namespace Ui {
class HomeWidget;
}

class HomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

public slots:
    void switchVGroup();
    void showProductWidget();
    void showDeviceWidget();


private:
    Ui::HomeWidget *ui;
    ProductWidget * productWidget;
    DeviceWidget * deviceWidget;

};

#endif // HOMEWIDGET_H
