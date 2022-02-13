#include "analysewidget.h"
#include "util.h"
#include "devicedatachartview.h"
#include <QSqlQuery>
#include <QtWidgets>
#include <QChartView>
#include <QTableView>

AnalyseWidget::AnalyseWidget(QWidget *parent) : QWidget(parent),
    deviceBox(createDeviceBox()),
    attrBox(new QComboBox),
    timeBox(createTimeBox()),
    themeBox(createThemeBox()),
    chartTypeBox(createChartTypeBox()),
    mainLayout(new QVBoxLayout),
    chartBt(new QPushButton("图表")),
    formBt(new QPushButton("表格"))
{
    QLabel * title = new QLabel("数据分析");
    title -> setStyleSheet("font: bold 16pt ""宋体""; padding: 10px;");

    navLayout = new QHBoxLayout;
    navLayout->setSpacing(10);
    navLayout->addWidget(new QLabel("Device:"));
    navLayout->addWidget(deviceBox);
    navLayout->addWidget(new QLabel("属性:"));
    navLayout->addWidget(attrBox);
    navLayout->addWidget(timeLabel = new QLabel("时间段:"));
    navLayout->addWidget(timeBox);
    navLayout->addWidget(new QLabel("Theme:"));
    navLayout->addWidget(themeBox);
    navLayout->addStretch();

    QHBoxLayout * subLayout = new QHBoxLayout;
    subLayout->setSpacing(0);
    subLayout->addWidget(chartTypeLabel = new QLabel("ChartType:"));
    subLayout->addWidget(chartTypeBox);
    subLayout->addStretch();
    subLayout->addWidget(chartBt);
    subLayout->addWidget(formBt);
    subLayout->addSpacing(20);

    mainLayout->addWidget(new QLabel("物联网平台 / 数据分析"));
    mainLayout->addWidget(title);
    mainLayout->addLayout(navLayout);
    mainLayout->addSpacing(15);
    mainLayout->addLayout(subLayout);

    // 栈窗口显示图表或表格
    stackWidget = new QStackedWidget;
    mainLayout->addWidget(stackWidget);
    mainLayout->setStretchFactor(stackWidget, 30);


    this->setAttrBox();
    this->updateChartView();
    this->setLayout(mainLayout);
    this->connectSignals();
}

void AnalyseWidget::setAttrBox()
{
    //attrBox->clear();
    QSqlQuery query;
    // 先获取设备对应产品id
    query.exec(tr("select product_id from device where id=%1").arg(deviceBox->currentData().toInt()));
    query.first();
    // 查看产品属性
    query.exec(tr("select identifier from attr where product_id=%1").arg(query.value(0).toInt()));

    QComboBox * tempBox = new QComboBox;
    navLayout->replaceWidget(attrBox, tempBox);
    delete attrBox;
    attrBox = tempBox;

    while (query.next()) {
        attrBox->addItem(query.value(0).toString());
    }
    connect(attrBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &AnalyseWidget::unifiedUpdate);
}

void AnalyseWidget::updateTheme()
{
    QChart::ChartTheme theme = static_cast<QChart::ChartTheme>(
                themeBox->currentData().toInt());
    // 设置主窗口主题
    QPalette pal = window()->palette();
    if (theme == QChart::ChartThemeLight) {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeBlueCerulean) {
        pal.setColor(QPalette::Window, QRgb(0x40434a));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    } else if (theme == QChart::ChartThemeBrownSand) {
        pal.setColor(QPalette::Window, QRgb(0x9e8965));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeBlueNcs) {
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else if (theme == QChart::ChartThemeHighContrast) {
        pal.setColor(QPalette::Window, QRgb(0xffab03));
        pal.setColor(QPalette::WindowText, QRgb(0x181818));
    } else if (theme == QChart::ChartThemeBlueIcy) {
        pal.setColor(QPalette::Window, QRgb(0xcee7f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    } else {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    window()->setPalette(pal);

    // 设置图像主题
    for (QChartView * chartView : chartViews)
        chartView->chart()->setTheme(theme);
}

QComboBox * AnalyseWidget::createDeviceBox()
{
    QComboBox * deviceBox = new QComboBox;
    QSqlQuery query;
    query.exec("select name, id from device");
    while (query.next()) {
        deviceBox->addItem(query.value(0).toString(), query.value(1).toInt());
    }
    return deviceBox;
}

QComboBox *AnalyseWidget::createTimeBox()
{
    QComboBox * timeBox = new QComboBox;
    timeBox->addItem("1小时", util::Hour);
    timeBox->addItem("24小时", util::Day);
    timeBox->addItem("7天", util::Week);
    timeBox->addItem("30天", util::Month);
    return timeBox;
}

QComboBox *AnalyseWidget::createThemeBox()
{
    QComboBox * themeBox = new QComboBox;
    themeBox->addItem("Light", QChart::ChartThemeLight);
    themeBox->addItem("Blue Cerulean", QChart::ChartThemeBlueCerulean);
    themeBox->addItem("Brown Sand", QChart::ChartThemeBrownSand);
    themeBox->addItem("Blue NCS", QChart::ChartThemeBlueNcs);
    themeBox->addItem("High Contrast", QChart::ChartThemeHighContrast);
    themeBox->addItem("Blue Icy", QChart::ChartThemeBlueIcy);
    themeBox->addItem("Qt", QChart::ChartThemeQt);
    return themeBox;
}

QComboBox *AnalyseWidget::createChartTypeBox()
{
    QComboBox * chartTypeBox = new QComboBox;
    chartTypeBox->addItem("All");
    chartTypeBox->addItem("Area", util::Area);
    chartTypeBox->addItem("Line", util::Line);
    chartTypeBox->addItem("Spline", util::Spline);
    chartTypeBox->addItem("Scatter", util::Scatter);
    return chartTypeBox;
}

void AnalyseWidget::connectSignals()
{
    connect(deviceBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=]{
        this->setAttrBox();
        this->unifiedUpdate();
    });

    connect(timeBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &AnalyseWidget::updateChartView);

    connect(chartTypeBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &AnalyseWidget::updateChartView);

    connect(themeBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &AnalyseWidget::updateTheme);

    connect(chartBt, &QPushButton::clicked, [=]{
        if (chartTypeLabel->isVisible()) return;
        this->setPartVisible(true);
        this->updateChartView();
    });

    connect(formBt, &QPushButton::clicked, [=]{
        if (!chartTypeLabel->isVisible()) return;
        this->setPartVisible(false);
        this->updateFormView();
    });

}

void AnalyseWidget::unifiedUpdate()
{
    if (stackWidget->currentWidget() == chartWidget)
        this->updateChartView();
    else
        this->updateFormView();
}

void AnalyseWidget::updateChartView()
{
    // 清空图表列
    chartViews.clear();

    // 创建新布局
    QGridLayout * chartLayout = new QGridLayout;
    chartLayout->setSpacing(10);

    // 若属性非空
    QString identifier = attrBox->currentText();
    if (!identifier.isEmpty()) {
        int deviceID = deviceBox->currentData().toInt();
        util::TimeSlot timeSlot = static_cast<util::TimeSlot>(timeBox->currentData().toInt());
        DeviceDataChartView * view;

        if (chartTypeBox->currentText() == "All") {

            chartLayout->addWidget(view = new DeviceDataChartView(
                                       deviceID, identifier, timeSlot, util::Area), 0, 0);
            chartViews.append(view);
            chartLayout->addWidget(view = new DeviceDataChartView(
                                       deviceID, identifier, timeSlot, util::Line), 0, 1);
            chartViews.append(view);
            chartLayout->addWidget(view = new DeviceDataChartView(
                                       deviceID, identifier, timeSlot, util::Spline), 1, 0);
            chartViews.append(view);
            chartLayout->addWidget(view = new DeviceDataChartView(
                                       deviceID, identifier, timeSlot, util::Scatter), 1, 1);
            chartViews.append(view);
        } else {
            util::ChartType chartType = static_cast<util::ChartType>(chartTypeBox->currentData().toInt());
            chartLayout->addWidget(view = new DeviceDataChartView(
                                       deviceID, identifier, timeSlot, chartType));
            chartViews.append(view);
        }
    }

    this->updateTheme();

    QWidget * newWidget = new QWidget;
    newWidget->setLayout(chartLayout);
    stackWidget->addWidget(newWidget);
    stackWidget->setCurrentWidget(newWidget);
    if (stackWidget->indexOf(chartWidget) > 0) {
        stackWidget->removeWidget(chartWidget);
        delete chartWidget;
    }
    chartWidget = newWidget;
}

void AnalyseWidget::updateFormView()
{
    if (stackWidget->indexOf(formWidget) < 0) {
        sqlModel = new CenterAlignSqlModel;
        formView = new QTableView;
        formView->setModel(sqlModel);
        // 设置视图样式
        //formView->verticalHeader()->hide();
        //formView->setShowGrid(false);
        formView->setStyleSheet("QTableView {font: 16px;} "
                    "QTableView::item:hover{background-color: #6699CC; color: white}");
        formWidget = formView;
        stackWidget->addWidget(formWidget);
    }
    this->formRefresh();
    stackWidget->setCurrentWidget(formWidget);
}

void AnalyseWidget::formRefresh()
{
    // 若属性为空
    QString identifier = attrBox->currentText();
    if (identifier.isEmpty()) {
        sqlModel->clear();
        return;
    }
    // 若属性非空
    int deviceID = deviceBox->currentData().toInt();
    QString sql = tr("select %1, time from product_%2_attr "
                     "where %1 and device_id=%3 "
                     "order by time desc").arg(identifier).arg(
                util::getProductID(deviceID)).arg(deviceID);
    sqlModel->setQuery(sql);
    sqlModel->setHeaderData(0, Qt::Horizontal, attrBox->currentText());
    sqlModel->setHeaderData(1, Qt::Horizontal, "时间");
    formView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    formView->horizontalHeader()->resizeSection(0, 150);
    formView->horizontalHeader()->resizeSection(1, 200);
}

void AnalyseWidget::setPartVisible(bool sign)
{
    chartTypeLabel->setVisible(sign);
    chartTypeBox->setVisible(sign);
    timeLabel->setVisible(sign);
    timeBox->setVisible(sign);
}

