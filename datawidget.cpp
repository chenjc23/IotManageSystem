#include "datawidget.h"
#include "labels.h"
#include "util.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtWidgets>
#include <QTimer>

DataWidget::DataWidget(int deviceID, QWidget *parent) :
    QWidget(parent), deviceID(deviceID)
{
    this->getProductID();
    util::setBgColor(this, "#FFFFFF");

    // 模型与映射创建
    sqlModel = new QSqlQueryModel(this);
    // 在对应产品属性表中提取所有属性值
    QString sql = tr("select * from product_%1_attr where device_id=%2 "
                     "order by id desc limit 1").arg(productID).arg(deviceID);
    sqlModel->setQuery(sql);
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);

    // 得到最近的记录
    QSqlRecord topRecord = sqlModel->record(0);

    // 设置属性布局
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setSpacing(15);
    // 根据属性个数依次创建DataBox
    for (int attrNum = 3; attrNum < sqlModel->columnCount(); ++attrNum) {
        QSqlQuery query;
        query.prepare("select func_name, unit from attr "
                      "where product_id=? and identifier=?");
        query.addBindValue(productID);
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
                              (attrNum-3)/maxcolumn, (attrNum-3)%maxcolumn);
        mapper->addMapping(dataBox, attrNum, "data");
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

void DataWidget::getProductID()
{
    QSqlQuery query;
    // 获取设备相应产品ID
    query.exec(tr("select product_id from device where id=%1").arg(deviceID));
    query.first();
    productID = query.value(0).toInt();
}

void DataWidget::refresh()
{
    QString sql = tr("select * from product_%1_attr where device_id=%2 "
                     "order by id desc limit 1").arg(productID).arg(deviceID);
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
}

QString DataBox::data()
{
    return dataLabel->text();
}

void DataBox::setData(const QString &text)
{
    dataLabel->setText(text);
}
