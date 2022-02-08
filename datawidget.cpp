#include "datawidget.h"
#include "labels.h"
#include "util.h"
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QtWidgets>
#include <QDebug>

DataWidget::DataWidget(int deviceID, QWidget *parent) :
    QWidget(parent), deviceID(deviceID)
{
    this->getProductID();
    util::setBgColor(this, "#FFFFFF");
    sqlModel = new QSqlQueryModel(this);
    this->refresh();
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
    // 删除原布局
    //delete this->layout();
    // 在对应产品的属性表中获取该设备的属性值
    QString sql = tr("select * from product_%1_attr where device_id=%2 "
                     "order by id desc limit 1").arg(productID).arg(deviceID);
    sqlModel->setQuery(sql);

    QSqlRecord topRecord = sqlModel->record(0);

    // 设置属性布局
    QGridLayout * mainLayout = new QGridLayout;
    mainLayout->setSpacing(15);

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
        // 根据属性个数依次创建DataBox
        mainLayout->addWidget(new DataBox(query.value(0).toString(),
                              data, query.value(1).toString()),
                              (attrNum-3)/maxcolumn, (attrNum-3)%maxcolumn);
    }

    QHBoxLayout * hLayout = new QHBoxLayout;
    hLayout->addLayout(mainLayout);
    hLayout->addStretch();
    QVBoxLayout * vLayout = new QVBoxLayout;
    QLabel * subTitle = new QLabel("最近数据");
    subTitle->setStyleSheet("font: bold 14px; padding: 5px");
    vLayout->addWidget(subTitle);
    vLayout->addLayout(hLayout);
    vLayout->addStretch();

    this->setLayout(vLayout);
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
    dataLayout->addWidget(new QLabel(data));
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
