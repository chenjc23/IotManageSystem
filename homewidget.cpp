#include "homewidget.h"
#include "productwidget.h"
#include "ui_homewidget.h"
#include "devicewidget.h"
#include "groupingwidget.h"
#include "serverwidget.h"
#include <QDesktopWidget>
#include <QDebug>

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    // product表初始化
    QSqlError err = dbconn::dbProductInit();
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Unable to create product table", err.text());
    }
    // device表初始化
    err = dbconn::dbDeviceInit();
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Unable to create device table", err.text());
    }

    ui->setupUi(this);
    this->setFixedSize(1300,750);
    this->setStyleSheet("QPushButton {padding: 10px; border-right-color: transparent}");

    productWidget = new ProductWidget;
    deviceWidget = new DeviceWidget;
    analyseWidget = new AnalyseWidget;
    groupingWidget = new GroupingWidget;
    serverWidget = new ServerWidget;

    ui->stackedWidget->insertWidget(1, productWidget);
    ui->stackedWidget->insertWidget(2, deviceWidget);
    ui->stackedWidget->insertWidget(3, analyseWidget);
    ui->stackedWidget->insertWidget(4, groupingWidget);
    ui->stackedWidget->insertWidget(5, serverWidget);

    ui->stackedWidget->setCurrentWidget(productWidget);

    connect(ui->headBt, SIGNAL(clicked()), this, SLOT(switchVGroup()));

    connect(ui->productBt, SIGNAL(clicked()), this, SLOT(showProductWidget()));
    connect(ui->deviceBt, SIGNAL(clicked()), this, SLOT(showDeviceWidget()));
    connect(ui->dataAnalyseBt, SIGNAL(clicked()), this, SLOT(showAnalyseWidget()));
    connect(ui->groupBt, SIGNAL(clicked()), this, SLOT(showGroupingWidget()));
    connect(ui->serverBt, SIGNAL(clicked()), this, SLOT(showServerWidget()));
}

HomeWidget::~HomeWidget()
{
    delete ui;
}

void HomeWidget::switchVGroup()
{
    if (ui->groupBox->isVisible()) {
        ui->groupBox->hide();
    } else {
        ui->groupBox->show();
    }
}

void HomeWidget::showProductWidget()
{
    productWidget->refresh();
    ui->stackedWidget->setCurrentWidget(productWidget);
}

void HomeWidget::showDeviceWidget()
{
    deviceWidget->refresh();
    ui->stackedWidget->setCurrentWidget(deviceWidget);
}

void HomeWidget::showAnalyseWidget()
{
    analyseWidget->refresh();
    ui->stackedWidget->setCurrentWidget(analyseWidget);
}

void HomeWidget::showGroupingWidget()
{
    ui->stackedWidget->setCurrentWidget(groupingWidget);
}

void HomeWidget::showServerWidget()
{
    ui->stackedWidget->setCurrentWidget(serverWidget);
}


