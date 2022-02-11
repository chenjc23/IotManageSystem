#include "analysewidget.h"
#include "util.h"
#include "devicedatachartview.h"

AnalyseWidget::AnalyseWidget(QWidget *parent) : QWidget(parent)
{
    util::setBgColor(this);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new DeviceDataChartView(5, "length", util::Hour, util::Line));
    this->setLayout(mainLayout);

}
