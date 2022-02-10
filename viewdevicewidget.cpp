#include "viewdevicewidget.h"
#include "util.h"
#include "datawidget.h"
#include <QtWidgets>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

ViewDeviceWidget::ViewDeviceWidget(int deviceID, QWidget *parent) :
    QWidget(parent), deviceID(deviceID)
{
    sqlModel = new QSqlQueryModel(this);
    this->refresh();
    this->setStyleSheet("QPushButton {background-color: white;}");
    util::setBgColor(this);

    // 设备标题
    QLabel * headTitle = new QLabel("物联网平台 / 设备管理 / 设备/ 设备详情");
    QLabel * title = new QLabel(sqlModel->record(0).value(2).toString());
    title->setStyleSheet("font: bold 16pt ""宋体""; padding: 10px;");

    // 导航按钮
    infoBt = new QPushButton("设备信息");
    dataBt = new QPushButton("物模型数据");
    QHBoxLayout * guideLayout = new QHBoxLayout;
    guideLayout->addWidget(infoBt);
    guideLayout->addWidget(dataBt);
    guideLayout->setSpacing(0);
    guideLayout->addStretch();

    // 栈窗口
    stackWidget = new QStackedWidget;
    stackWidget->insertWidget(infoPage, infoWidget = generateTableWidget());
    //stackWidget->insertWidget(dataPage, dataWidget = new DataWidget(deviceID));
    stackWidget->setCurrentIndex(infoPage);

    this->setMapper();

    // 主窗口布局
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headTitle);
    mainLayout->addWidget(title);
    mainLayout->addLayout(guideLayout);
    mainLayout->addWidget(stackWidget);
    mainLayout->addStretch();

    mainLayout->setStretchFactor(stackWidget, 20);

    this->setLayout(mainLayout);

    connect(infoBt, &QPushButton::clicked, this, [=]{
        this->refresh();
        stackWidget->setCurrentIndex(infoPage);
    });

    connect(dataBt, &QPushButton::clicked, this, [=]{
        if (stackWidget->widget(dataPage)) {
            stackWidget->removeWidget(dataWidget);
            delete dataWidget;
        }
        stackWidget->insertWidget(dataPage, dataWidget = new DataWidget(deviceID));
        stackWidget->setCurrentIndex(dataPage);
    });


}

QWidget *ViewDeviceWidget::generateTableWidget()
{
    QWidget * tableWidget = new QWidget;
    tableWidget->setStyleSheet("QLabel{font: 16px;} QWidget{background-color: white;}");
    // 表窗口标题
    QLabel * infoLabel = new QLabel("设备信息");
    infoLabel->setStyleSheet("font: bold");
    // info表
    QGridLayout * infoLayout = new QGridLayout;
    infoLayout->addWidget(new QLabel("产品名称:"), 0, 0, 3, 1);
    infoLayout->addWidget(productNameLabel = new QLabel, 0, 1, 3, 1);
    infoLayout->addWidget(new QLabel("节点类型:"), 0, 2, 3, 1);
    infoLayout->addWidget(nodeTypeLabel = new QLabel, 0, 3, 3, 1);
    infoLayout->addWidget(new QLabel("DeviceName:"), 0, 4, 3, 1);
    infoLayout->addWidget(deviceNameLabel = new QLabel, 0, 5, 3, 1);
    infoLayout->addWidget(new QLabel("备注名称:"), 3, 0, 3, 1);
    infoLayout->addWidget(remarkLabel = new QLabel, 3, 1, 3, 1);
    infoLayout->addWidget(new QLabel("IP地址:"), 3, 2, 3, 1);
    infoLayout->addWidget(ipLabel = new QLabel, 3, 3, 3, 1);
    infoLayout->addWidget(new QLabel("当前状态:"), 3, 4, 3, 1);
    infoLayout->addWidget(currentStateLabel = new QLabel, 3, 5, 3, 1);
    infoLayout->addWidget(new QLabel("创建时间:"), 6, 0, 3, 1);
    infoLayout->addWidget(createTimeLabel = new QLabel, 6, 1, 3, 1);
    infoLayout->addWidget(new QLabel("激活时间:"), 6, 2, 3, 1);
    infoLayout->addWidget(activatedTimeLabel = new QLabel, 6, 3, 3, 1);
    infoLayout->addWidget(new QLabel("最后上线时间:"), 6, 4, 3, 1);
    infoLayout->addWidget(lastTimeLabel = new QLabel, 6, 5, 3, 1);

    // 布局
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(infoLabel);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
    tableWidget->setLayout(mainLayout);

    return tableWidget;
}

void ViewDeviceWidget::refresh()
{
    QString selectSql = "select b.name, "
                        "b.node_type, "
                        "a.name, "
                        "a.remark, "
                        "a.ip, "
                        "a.current_state, "
                        "a.create_time, "
                        "a.activated_time, "
                        "a.last_time from device a "
                        "inner join product b "
                        "on a.product_id=b.id where a.id=%1";
    sqlModel->setQuery(selectSql.arg(deviceID));
}

void ViewDeviceWidget::setMapper()
{
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);
    mapper->addMapping(productNameLabel, 0, "text");
    mapper->addMapping(nodeTypeLabel, 1, "text");
    mapper->addMapping(deviceNameLabel, 2, "text");
    mapper->addMapping(remarkLabel, 3, "text");
    mapper->addMapping(ipLabel, 4, "text");
    mapper->addMapping(currentStateLabel, 5, "text");
    mapper->addMapping(createTimeLabel, 6, "text");
    mapper->addMapping(activatedTimeLabel, 7, "text");
    mapper->addMapping(lastTimeLabel, 8, "text");
    mapper->toFirst();

}


