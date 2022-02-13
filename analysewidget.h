#ifndef ANALYSEWIDGET_H
#define ANALYSEWIDGET_H

#include <QtWidgets>
#include "devicedatachartview.h"

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

private:
    QComboBox * deviceBox;
    QComboBox * attrBox;
    QComboBox * timeBox;
    QComboBox * themeBox;
    QComboBox * chartTypeBox;

    QHBoxLayout * navLayout;
    QGridLayout * chartLayout;
    QVBoxLayout * mainLayout;
    QList<QChartView *> chartViews;

    QLabel * chartTypeLabel;
    QPushButton * chartBt;
    QPushButton * formBt;

    QComboBox * createDeviceBox();
    QComboBox * createAttrBox();
    QComboBox * createTimeBox();
    QComboBox * createThemeBox();
    QComboBox * createChartTypeBox();

    void connectSignals();
    void deleteChartLayout();


signals:

};

#endif // ANALYSEWIDGET_H
