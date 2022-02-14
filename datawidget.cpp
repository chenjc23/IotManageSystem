#include "datawidget.h"
#include "labels.h"
#include "util.h"
#include "devicedatachartview.h"
#include "devicedataformview.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtWidgets>
#include <QTimer>
#include <QDebug>

DataWidget::DataWidget(int deviceID, QWidget *parent) :
    QWidget(parent), deviceID(deviceID)
{
    util::setBgColor(this, "#FFFFFF");

    // 模型与映射创建
    sqlModel = new QSqlQueryModel(this);
    // 在设备属性表中提取最近的属性值
    QString sql = tr("select * from device_%1_attr "
                     "order by time desc limit 1").arg(deviceID);
    sqlModel->setQuery(sql);
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);

    // 得到最近的记录
    QSqlRecord topRecord = sqlModel->record(0);

    // 设置属性布局
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setSpacing(15);
    // 根据属性个数依次创建DataBox
    for (int attrNum = 2; attrNum < sqlModel->columnCount(); ++attrNum) {
        QSqlQuery query;
        query.prepare("select a.func_name, a.unit from attr a, device b "
                      "where a.product_id=b.product_id and "
                      "b.id=? and "
                      "a.identifier=?");

        query.addBindValue(deviceID);
        query.addBindValue(topRecord.fieldName(attrNum));
        query.exec();
        query.first();
        //设置数据为空时的显示
        QString data = topRecord.value(attrNum).toString();
        if (data.isEmpty())
            data = "--";
        DataBox * dataBox;
        mainLayout->addWidget(dataBox = new DataBox(query.value(0).toString(),
                              data, query.value(1).toString()),
                              (attrNum-2)/maxcolumn, (attrNum-2)%maxcolumn);
        mapper->addMapping(dataBox, attrNum, "data");
        connect(dataBox, &DataBox::labelClicked, [=] {
            QWidget * tempWidget = this->getShowWidget(deviceID, topRecord.fieldName(attrNum));
            tempWidget->setAttribute(Qt::WA_DeleteOnClose);
            tempWidget->setWindowModality(Qt::ApplicationModal);
            tempWidget->show();
        });
    }

    // 主窗口布局
    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addLayout(mainLayout);
    hLayout->addStretch();
    QVBoxLayout * vLayout = new QVBoxLayout;
    QLabel * subTitle = new QLabel("最近数据");
    subTitle->setStyleSheet("font: bold 14px; padding: 5px");

    // 创建小标题以及实时刷新按钮
    QHBoxLayout * subLayout = new QHBoxLayout;
    ImgSwitchBt * refreshBt = new ImgSwitchBt(":/picture/stateBt1.png",
                                              ":/picture/stateBt2.png");
    subLayout->addWidget(subTitle);
    subLayout->addStretch();
    subLayout->addWidget(new QLabel("实时刷新"));
    subLayout->addWidget(refreshBt);
    subLayout->addSpacing(15);

    vLayout->addLayout(subLayout);
    vLayout->addLayout(hLayout);
    vLayout->addStretch();

    this->setLayout(vLayout);

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DataWidget::refresh);

    // 连接更新按钮的信号和槽
    connect(refreshBt, &ImgSwitchBt::stateSwitched, [=](bool state){
        if (state)
            timer->start(3000);
        else
            timer->stop();
    });
}

QWidget *DataWidget::getShowWidget(int deviceID, const QString &identifier)
{
    QWidget * showWidget = new QWidget;
    // 创建时间拉选框
    QComboBox * timeBox = new QComboBox;
    timeBox->addItem("1小时", util::Hour);
    timeBox->addItem("24小时", util::Day);
    timeBox->addItem("7天", util::Week);
    timeBox->addItem("30天", util::Month);
    QPushButton * chartBt = new QPushButton("图表");
    QPushButton * formBt = new QPushButton("表格");

    QHBoxLayout * headLayout = new QHBoxLayout;
    headLayout->setSpacing(0);
    headLayout->addWidget(timeBox);
    headLayout->addStretch();
    headLayout->addWidget(chartBt);
    headLayout->addWidget(formBt);

    QStackedWidget * stackWidget = new QStackedWidget;
    DeviceDataChartView * chartView = new DeviceDataChartView(deviceID,
                   identifier, static_cast<util::TimeSlot>(timeBox->currentData().toInt()), util::Line);
    DeviceDataFormView * formView = new DeviceDataFormView(deviceID, identifier);
    stackWidget->addWidget(chartView);
    stackWidget->addWidget(formView);
    stackWidget->setCurrentWidget(chartView);

    QVBoxLayout * mainLayout = new  QVBoxLayout;
    mainLayout->addLayout(headLayout);
    mainLayout->addWidget(stackWidget);
    mainLayout->setStretchFactor(stackWidget, 20);

    showWidget->setLayout(mainLayout);

    connect(timeBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=,&chartView](){
        DeviceDataChartView * tempView = new DeviceDataChartView(deviceID,
                       identifier, static_cast<util::TimeSlot>(timeBox->currentData().toInt()), util::Line);
        stackWidget->addWidget(tempView);
        stackWidget->setCurrentWidget(tempView);
        stackWidget->removeWidget(chartView);
        delete chartView;
        chartView = tempView;
    });

    connect(chartBt, &QPushButton::clicked, [=]{
        timeBox->setVisible(true);
        stackWidget->setCurrentWidget(chartView);
    });

    connect(formBt, &QPushButton::clicked, [=]{
        timeBox->setVisible(false);
        stackWidget->setCurrentWidget(formView);
    });

    return showWidget;
}

void DataWidget::refresh()
{
    QString sql = tr("select * from device_%1_attr "
                     "order by id desc limit 1").arg(deviceID);
    sqlModel->setQuery(sql);
    mapper->toFirst();
}

DataBox::DataBox(const QString &text, const QString &data, const QString &unit, QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("QLabel {font: bold 20px;}");
    FuncLabel * viewDataLabel = new FuncLabel("查看数据");
    viewDataLabel->setAutoFillBackground(false);

    QHBoxLayout * titleLayout = new QHBoxLayout;
    titleLayout->addWidget(new QLabel(text));
    titleLayout->addStretch();
    titleLayout->addWidget(viewDataLabel);

    QHBoxLayout * dataLayout = new QHBoxLayout;
    dataLayout->addStretch();
    dataLayout->addWidget(dataLabel =  new QLabel(data));
    dataLayout->addWidget(new QLabel(unit));
    dataLayout->addStretch();

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(titleLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(dataLayout);
    mainLayout->addStretch();
    util::setBgColor(this, "#99CCCC");
    this->setFixedSize(200, 130);
    this->setLayout(mainLayout);

    connect(viewDataLabel, SIGNAL(funcLabelClicked()), this, SIGNAL(labelClicked()));
}

QString DataBox::data()
{
    return dataLabel->text();
}

void DataBox::setData(const QString &text)
{
    dataLabel->setText(text);
}
