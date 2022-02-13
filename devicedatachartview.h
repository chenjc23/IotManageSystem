#ifndef DEVICEDATACHARTVIEW_H
#define DEVICEDATACHARTVIEW_H

#include <QtWidgets>
#include <QtCharts>
#include "util.h"
#include <QList>


class DeviceDataChartView : public QChartView
{
    Q_OBJECT
public:
    explicit DeviceDataChartView(int deviceID, const QString &identifier,
                                 util::TimeSlot timeSlot, util::ChartType chartType, QWidget *parent = nullptr);


private:
    int productID;

    int deviceID;
    const QString &identifier;
    util::TimeSlot timeSlot;
    util::ChartType chartType;

    QChart * chart;
    QAbstractSeries * series;

    //QList<QPointF> dataList;

private:
    void setProductID();
    void setXYSeries();
    void setAxis();

    QChart *createAreaChart();
    QChart *createLineChart();
    QChart *createSplineChart();
    QChart *createScatterChart();

    void makeChart();


signals:

};

#endif // DEVICEDATACHARTVIEW_H
