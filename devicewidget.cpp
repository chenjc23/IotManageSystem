#include "devicewidget.h"
#include "ui_devicewidget.h"
#include "util.h"
#include "labels.h"
#include "editdevicewidget.h"
#include "viewdevicewidget.h"
#include <QtWidgets>
#include <QSqlQueryModel>

DeviceWidget::DeviceWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceWidget)
{
    ui->setupUi(this);

    // 控件创建
    QLabel * headTitle = new QLabel("物联网平台 / 设备管理 / 设备");
    QLabel * title = new QLabel("设备");
    title -> setStyleSheet("font: bold 16pt ""宋体""; padding: 10px;");

    // 顶部产品组合框创建
    productChooseComboBox = new QComboBox;
    productModel = new QStringListModel(this);
    productChooseComboBox->setModel(productModel);
    this->productBoxRefresh();


    addDeviceBt = new BlueBt("添加设备");
    searchEdit = new QLineEdit;
    searchEdit->setPlaceholderText("请输入DeviceName");

    // 模型与视图
    sqlModel = new QSqlQueryModel(this);
    deviceView = new QTableView(this);
    deviceView->setModel(sqlModel);
    deviceView->verticalHeader()->hide();
    deviceView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    deviceView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    deviceView->setStyleSheet("QTableView {font: 16px;} "
                  "QTableView::item:hover{background-color: #6699CC; color: white}");
    //deviceView->setShowGrid(false);
    this->refresh();

    // 布局设置
    QHBoxLayout * headLayout = new QHBoxLayout;
    headLayout->addWidget(productChooseComboBox);
    headLayout->addWidget(new QLabel("选择产品"));
    headLayout->addStretch();

    QHBoxLayout * subLayout = new QHBoxLayout;
    subLayout->addWidget(addDeviceBt);
    subLayout->addWidget(searchEdit);
    subLayout->addStretch();

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headTitle);
    mainLayout->addWidget(title);
    mainLayout->addLayout(headLayout);
    mainLayout->addLayout(subLayout);
    mainLayout->addWidget(deviceView);
    mainLayout->setStretchFactor(deviceView, 20);
    this->setLayout(mainLayout);

    // 背景色设置
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor("#99CCCC"));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

    // 产品选择信号与槽函数连接
    connect(productChooseComboBox, &QComboBox::currentTextChanged,
            this, &DeviceWidget::refresh);

    connect(addDeviceBt, &QPushButton::clicked, [=]{
        this->showEditDeviceWidget();
    });

}




DeviceWidget::~DeviceWidget()
{
    delete ui;
}

void DeviceWidget::refresh(const QString &text)
{
    QString sql = "select a.name, "
                  "b.name, "
                  "b.node_type, "
                  "a.current_state, "
                  "a.last_time, "
                  "a.id from device a inner join product b on a.product_id = b.id";
    if (!text.isEmpty()) {
        sql += " where b.name=?";
        QSqlQuery query;
        query.prepare(sql);
        query.addBindValue(text);
        query.exec();
        sqlModel->setQuery(query);
    } else {
        sqlModel->setQuery(sql);
    }

    sqlModel->setHeaderData(0, Qt::Horizontal, "DeviceName");
    sqlModel->setHeaderData(1, Qt::Horizontal, "设备所属产品");
    sqlModel->setHeaderData(2, Qt::Horizontal, "节点类型");
    sqlModel->setHeaderData(3, Qt::Horizontal, "设备状态");
    sqlModel->setHeaderData(4, Qt::Horizontal, "最后上线时间");
    sqlModel->setHeaderData(5, Qt::Horizontal, "操作");

    deviceView->horizontalHeader()->setSectionResizeMode(sqlModel->columnCount()-1,
                                           QHeaderView::ResizeToContents);
    for (int rowNum = 0; rowNum < sqlModel->rowCount(); ++rowNum) {
        int deviceID = sqlModel->record(rowNum).value(sqlModel->columnCount()-1).toInt();
        StdComboLabels * stdLabels = new StdComboLabels(deviceID);
        deviceView->setIndexWidget(sqlModel->index(rowNum, sqlModel->columnCount()-1), stdLabels);
        connect(stdLabels, SIGNAL(viewLabelClicked(int)), this, SLOT(showViewDeviceWidget(int)));
        connect(stdLabels, SIGNAL(editLabelClicked(int)), this, SLOT(showEditDeviceWidget(int)));
        connect(stdLabels, SIGNAL(deleteLabelClicked(int)), this, SLOT(deleteDevice(int)));

    }
    if (text.isEmpty())
        this->productBoxRefresh();
}

void DeviceWidget::showEditDeviceWidget(int id)
{
    EditDeviceWidget * widget = new EditDeviceWidget(id);
    widget->setAttribute(Qt::WA_DeleteOnClose);
    widget->setWindowModality(Qt::ApplicationModal);
    widget->show();
    connect(widget, &EditDeviceWidget::editFinished, [=]{
        this->refresh();
    });

}

void DeviceWidget::showViewDeviceWidget(int id)
{
    QStackedWidget * stackWidget = static_cast<QStackedWidget *>(this->parentWidget());
    ViewDeviceWidget * viewDeviceWidget = new ViewDeviceWidget(id, stackWidget);
    stackWidget->addWidget(viewDeviceWidget);
    stackWidget->setCurrentWidget(viewDeviceWidget);
}

void DeviceWidget::deleteDevice(int id)
{
    if (msg::getCancelMsgBox() == QMessageBox::Ok) {
        QSqlQuery query;
        query.exec(tr("delete from device where id=%1").arg(id));
        query.exec(tr("drop table device_%1_attr").arg(id));
        this->refresh();
    }
}


void DeviceWidget::productBoxRefresh()
{
    productList.clear();
    QSqlQuery query;
    query.exec("select name from product");

    while (query.next()) {
        productList << query.record().value(0).toString();
    }
    productModel->setStringList(productList);
    productChooseComboBox->setCurrentIndex(-1);
}







