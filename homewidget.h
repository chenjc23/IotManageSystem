#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include "productwidget.h"
#include "devicewidget.h"
#include "analysewidget.h"
#include "groupingwidget.h"
#include "serverwidget.h"

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
    void showAnalyseWidget();
    void showGroupingWidget();
    void showServerWidget();


private:
    Ui::HomeWidget *ui;
    ProductWidget * productWidget;
    DeviceWidget * deviceWidget;
    AnalyseWidget * analyseWidget;
    GroupingWidget * groupingWidget;
    ServerWidget * serverWidget;
};

#endif // HOMEWIDGET_H
