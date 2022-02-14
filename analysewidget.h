#ifndef ANALYSEWIDGET_H
#define ANALYSEWIDGET_H

#include <QtWidgets>
#include "devicedatachartview.h"
#include "devicedataformview.h"
#include "util.h"

class AnalyseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyseWidget(QWidget *parent = nullptr);

public slots:
    void setAttrBox();
    void updateTheme();
    void updateChartView();
    void updateFormView();
    void unifiedUpdate();
    void refresh(); // 外部窗口调用

private:
    QComboBox * deviceBox;
    QComboBox * attrBox;
    QComboBox * timeBox;
    QComboBox * themeBox;
    QComboBox * chartTypeBox;

    QHBoxLayout * navLayout;
    QVBoxLayout * mainLayout;
    QList<QChartView *> chartViews;

    QLabel * chartTypeLabel;
    QLabel * showAllAttrLabel;
    ImgSwitchBt * showAllAttrBt;
    QLabel * timeLabel;
    QPushButton * chartBt;
    QPushButton * formBt;

    QStackedWidget * stackWidget;
    QWidget * chartWidget;
    DeviceDataFormView * formWidget;

    QComboBox * createDeviceBox();
    QComboBox * createAttrBox();
    QComboBox * createTimeBox();
    QComboBox * createThemeBox();
    QComboBox * createChartTypeBox();

    void connectSignals();
    void deleteChartLayout();
    void setPartVisible(bool sign);


signals:

};

#endif // ANALYSEWIDGET_H
