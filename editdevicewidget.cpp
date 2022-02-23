#include "editdevicewidget.h"
#include "util.h"
#include <QtWidgets>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>

EditDeviceWidget::EditDeviceWidget(int deviceID, QWidget *parent) :
    QWidget(parent), deviceID(deviceID)
{
    QLabel * title = new QLabel("添加设备");
    title->setStyleSheet("font: bold 14pt ""宋体""; padding: 10px;");

    productComboBox = new QComboBox;
    this->setProductComboBox();
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("请输入DeviceName");
    remarkEdit = new QLineEdit;
    remarkEdit->setPlaceholderText("请输入备注名称");
    ipEdit = new QLineEdit;
    ipEdit->setPlaceholderText("请输入设备IP地址");
    confirmBt = new BlueBt("确认");
    confirmBt->setFixedSize(50, 20);
    cancelBt = new QPushButton("取消");
    cancelBt->setFixedSize(50, 20);

    // 模型与映射
    if (deviceID)
        this->setMapperWithModel();

    // 布局设置
    QHBoxLayout * bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(confirmBt);
    bottomLayout->addWidget(cancelBt);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addWidget(new QLabel("产品"));
    mainLayout->addWidget(productComboBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("DeviceName"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("备注名称"));
    mainLayout->addWidget(remarkEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("IP地址"));
    mainLayout->addWidget(ipEdit);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    // 主窗口的属性设置
    this->setFixedSize(300, 350);
    this->setAttribute(Qt::WA_DeleteOnClose);
    util::setCenterShow(this);
    this->setWindowFlag(Qt::FramelessWindowHint);
    this->setLayout(mainLayout);

    // 控件信号连接
    connect(confirmBt, &QPushButton::clicked, this, &EditDeviceWidget::onConfirmBtClicked);
    connect(cancelBt, &QPushButton::clicked, [=]{
        if (msg::getCancelMsgBox() == QMessageBox::Ok)
            this->close();
    });

}

void EditDeviceWidget::onConfirmBtClicked()
{
    if (!this->validityCheck()) return;
    // sql操作
    QString updateSql;
    QSqlQuery query;
    // 针对添加设备
    if (!deviceID) {
        int productID = productComboBox->currentData().toInt();
        updateSql = "insert into device (product_id,"
                    "name,"
                    "remark, ip) values (?,?,?,?)";
        query.prepare(updateSql);
        query.addBindValue(productID);
        query.addBindValue(nameEdit->text());
        query.addBindValue(remarkEdit->text());
        query.addBindValue(ipEdit->text());
        query.exec();
        // 若是添加页，创建新的设备属性表
        query.exec("select last_insert_id()");
        query.first();
        QString lastDeviceID = query.value(0).toString();
        // 寻找对应产品的属性字段
        QString attrStr;
        QSqlQuery funcQuery;
        funcQuery.exec(tr("select identifier, data_type "
                          "from attr where product_id=%1").arg(productID));
        while (funcQuery.next()) {
            attrStr += tr(",%1 %2").arg(funcQuery.value(0).toString()).arg(
                        funcQuery.value(1).toString());
        }
        // 根据提取的属性字段创建设备属性表
        query.exec(tr("create table %1 (id integer primary key auto_increment, "
                      "time datetime default now() %2)").arg(
                       "device_"+lastDeviceID+"_attr").arg(attrStr));

    } else {
        updateSql = "update device set name=?,"
                    "remark=?, ip=? where id=?";
        query.prepare(updateSql);
        query.addBindValue(nameEdit->text());
        query.addBindValue(remarkEdit->text());
        query.addBindValue(ipEdit->text());
        query.addBindValue(deviceID);
        query.exec();
    }

    emit editFinished();
    this->close();
}

void EditDeviceWidget::setProductComboBox()
{
    QSqlQuery query;
    query.exec("select name, id from product");
    while (query.next()) {
        productComboBox->addItem(query.value(0).toString(), query.value(1).toInt());
    }

    // 若添加页，默认未选
    if (!deviceID)
        productComboBox->setCurrentIndex(-1);
}

void EditDeviceWidget::setMapperWithModel()
{
    sqlModel = new QSqlQueryModel(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);
    sqlModel->setQuery(tr("select b.name, a.name, a.remark, a.ip "
                      "from device a inner join product b "
                      "on a.product_id = b.id where a.id=%1").arg(deviceID));
    mapper->addMapping(productComboBox, 0, "currentText");
    mapper->addMapping(nameEdit, 1);
    mapper->addMapping(remarkEdit, 2);
    mapper->addMapping(ipEdit, 3);
    mapper->toFirst();
    productComboBox->setDisabled(true);
}

bool EditDeviceWidget::validityCheck()
{
    QString err;
    if (productComboBox->currentIndex() < 0) {
        err = "请先选择产品。";
    } else if (nameEdit->text().isEmpty()) {
        err = "设备名称不能为空。";
    } else if (ipEdit->text().isEmpty()) {
        err = "设备IP地址不能为空。";
    } else {
        QSqlQuery query;
        query.prepare("select id from device where name=?");
        query.addBindValue(nameEdit->text());
        query.exec();
        if (query.first() and (!deviceID or (deviceID and query.value(0).toInt() !=deviceID)))
            err = "设备名称已存在。";
    }
    if (err.isEmpty()) return true;
    else {
        QMessageBox::critical(this, "IotManageSystem", err);
        return false;
    }
}



