#include "devicedatachartview.h"
#include "util.h"
#include <QtWidgets>
#include <QtCharts>
#include <QtSql>
#include <QDebug>



DeviceDataChartView::DeviceDataChartView(int deviceID, const QString &identifier,
                util::TimeSlot timeSlot, util::ChartType chartType, QWidget *parent) :
    QChartView(parent),
    deviceID(deviceID),
    identifier(identifier),
    timeSlot(timeSlot),
    chartType(chartType)
{
    this->setProductID();
    // 根据类型参数设置chart
    switch (chartType) {
    case util::Area :
        chart = createAreaChart();
        break;
    case util::Line :
        chart = createLineChart();
        break;
    case util::Spline :
        chart = createSplineChart();
        break;
    case util::Scatter :
        chart = createScatterChart();
        break;
    }
    this->setChart(chart);
    this->setRenderHint(QPainter::Antialiasing);
}

void DeviceDataChartView::setProductID()
{
    QSqlQuery query;
    query.exec(tr("select product_id from device where id=%1").arg(deviceID));
    query.first();
    productID = query.value(0).toInt();
}

void DeviceDataChartView::setXYSeries()
{
    QString sql = tr("select time, %1 from product_%2_attr "
            "where device_id=%3 and "
            "time >= (now() - interval %4 hour) "
            "order by time").arg(identifier).arg(productID).arg(deviceID).arg(timeSlot);
    QSqlQuery query;
    query.exec(sql);
    while (query.next()) {
        series->append(query.value(0).toDateTime().toMSecsSinceEpoch(), query.value(1).toFloat());
    }
}

void DeviceDataChartView::setAxis()
{
    QDateTimeAxis * axisX = new QDateTimeAxis;
    QValueAxis * axisY = new QValueAxis;
    switch (timeSlot) {
    case util::Week :
        axisX->setTickCount(8);
        axisX->setFormat("MM/dd");
        break;
    case util::Month :
        axisX->setTickCount(13);
        axisX->setFormat("MM/dd");
        break;
    default:
        axisX->setTickCount(13);
        axisX->setFormat("HH:mm");
        break;
    }
    axisY->setLabelFormat("%.2f");
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
}

QChart *DeviceDataChartView::createAreaChart()
{
    chart = new QChart;
    return chart;
}

QChart *DeviceDataChartView::createLineChart()
{
    chart = new QChart;
    series = new QLineSeries;
    this->setXYSeries();
    chart->addSeries(series);
    chart->legend()->hide();
    this->setAxis();
    return chart;
}

QChart *DeviceDataChartView::createSplineChart()
{
    chart = new QChart;
    return chart;
}

QChart *DeviceDataChartView::createScatterChart()
{
    chart = new QChart;
    return chart;
}
