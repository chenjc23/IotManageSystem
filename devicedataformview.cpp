#include "devicedataformview.h"
#include "util.h"
#include <QTableView>
#include <QSqlQuery>

DeviceDataFormView::DeviceDataFormView(int deviceID, const QString &identifier, QWidget *parent) :
    QTableView(parent),
    deviceID(deviceID),
    identifier(identifier)
{
    sqlModel = new CenterAlignSqlModel;
    this->setModel(sqlModel);
    this->setStyleSheet("QTableView {font: 16px;} "
                "QTableView::item:hover{background-color: #6699CC; color: white}");
    if (deviceID != 0)
        this->refresh();
}

void DeviceDataFormView::refresh()
{
    // 标识符为空意味着需要显示设备的所有属性
    if (identifier.isNull()) {
        QSqlQuery query;
        QString attrStr;
        query.exec(tr("select a.identifier from attr a, device b "
                      "where b.id=%1 and b.product_id=a.product_id").arg(deviceID));
        while (query.next()){
            attrStr += query.value(0).toString() + ", ";
        }
        // 若不存在属性则清空model，否则查询所有属性
        if (attrStr.isEmpty()) {
            sqlModel->clear();
        } else {
            this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
            sqlModel->setQuery(tr("select %1 "
                                  "from device_%2_attr "
                                  "order by time desc").arg(attrStr+"time").arg(deviceID));
            for (int num(0); num < sqlModel->columnCount(); ++num) {
                sqlModel->setHeaderData(num, Qt::Horizontal, sqlModel->record().fieldName(num));
            }
        }
    } else {
        this->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        sqlModel->setQuery(tr("select %1, time from device_%2_attr "
                              "where %1 order by time desc").arg(identifier).arg(deviceID));
        sqlModel->setHeaderData(0, Qt::Horizontal, identifier);
        sqlModel->setHeaderData(1, Qt::Horizontal, "时间");
        this->horizontalHeader()->resizeSection(0, 150);
        this->horizontalHeader()->resizeSection(1, 200);
    }
}

void DeviceDataFormView::refresh(int deviceID, const QString &identifier)
{
    this->deviceID = deviceID;
    this->identifier = identifier;
    this->refresh();
}

void DeviceDataFormView::clearModel()
{
    sqlModel->clear();
}
