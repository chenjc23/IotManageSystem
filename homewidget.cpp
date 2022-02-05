#include "homewidget.h"
#include "productwidget.h"
#include "ui_homewidget.h"
#include "devicewidget.h"
#include <QDesktopWidget>

HomeWidget::HomeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);
    this->setFixedSize(1300,750);
    this->setStyleSheet("QPushButton {padding: 10px;}");

    productWidget = new ProductWidget;
    deviceWidget = new DeviceWidget;
    ui->stackedWidget->insertWidget(1, productWidget);
    ui->stackedWidget->insertWidget(2, deviceWidget);
//    ui->stackedWidget->addWidget(productWidget);
//    ui->stackedWidget->addWidget(deviceWidget);
    ui->stackedWidget->setCurrentWidget(productWidget);

    connect(ui->headBt, SIGNAL(clicked()), this, SLOT(switchVGroup()));
    connect(ui->productBt, SIGNAL(clicked()), this, SLOT(showProductWidget()));
    connect(ui->deviceBt, SIGNAL(clicked()), this, SLOT(showDeviceWidget()));

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
    ui->stackedWidget->setCurrentWidget(deviceWidget);
}


