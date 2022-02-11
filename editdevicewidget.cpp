#include "editdevicewidget.h"
#include "util.h"
#include <QtWidgets>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QSqlQuery>

EditDeviceWidget::EditDeviceWidget(int deviceID, QWidget *parent) :
    QWidget(parent), deviceID(deviceID)
{
    QLabel * title = new QLabel("添加自定义功能");
    title->setStyleSheet("font: bold 14pt ""宋体""; padding: 10px;");

    productComboBox = new QComboBox;
    this->setProductComboBoxModel();
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("请输入DeviceName");
    remarkEdit = new QLineEdit;
    remarkEdit->setPlaceholderText("请输入备注名称");
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
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);

    // 主窗口的属性设置
    this->setFixedSize(300, 300);
    this->setAttribute(Qt::WA_DeleteOnClose);
    util::setCenterShow(this);
    this->setWindowFlags(windowFlags() |
               Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
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
    // sql操作
    QString updateSql;
    QSqlQuery query;
    // 针对添加设备
    if (!deviceID) {
//        query.exec(tr("select id from "
//             "product where name=%1").arg(productComboBox->currentText()));
        query.prepare("select id from product where name=?");
        query.addBindValue(productComboBox->currentText());
        query.exec();
        query.first();
        int productID = query.value(0).toInt();
        updateSql = "insert into device (product_id,"
                    "name,"
                    "remark) values (?,?,?)";
        query.prepare(updateSql);
        query.addBindValue(productID);
        query.addBindValue(nameEdit->text());
        query.addBindValue(remarkEdit->text());
        query.exec();
//        // 更新product表
//        query.exec("select last_insert_id()");
//        query.first();
//        QString newID = query.value(0).toString();
//        query.prepare("update product "
//                      "set device_ids=concat(device_ids, ?) where id=?");
//        query.addBindValue(newID+",");
//        query.addBindValue(productID);
//        query.exec();
    } else {
        updateSql = "update device set name=?,"
                    "remark=? where id=?";
        query.prepare(updateSql);
        query.addBindValue(nameEdit->text());
        query.addBindValue(remarkEdit->text());
        query.addBindValue(deviceID);
        query.exec();
    }

    emit editFinished();
    this->close();
}

void EditDeviceWidget::setProductComboBoxModel()
{
    QSqlQuery query;
    query.exec("select name from product");

    QStringList products;
    while (query.next()) {
        products << query.record().value(0).toString();
    }
    QStringListModel * model = new QStringListModel(products, this);
    productComboBox->setModel(model);
    if (!deviceID)
        productComboBox->setCurrentIndex(-1);
}

void EditDeviceWidget::setMapperWithModel()
{
    sqlModel = new QSqlQueryModel(this);
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);
    sqlModel->setQuery(tr("select b.name, a.name, a.remark "
                  "from device a inner join product b "
                  "on a.product_id = b.id where a.id=%1").arg(deviceID));
    mapper->addMapping(productComboBox, 0, "currentText");
    mapper->addMapping(nameEdit, 1);
    mapper->addMapping(remarkEdit, 2);
    mapper->toFirst();
    productComboBox->setDisabled(true);
}



