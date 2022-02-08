
#include "editproductwidget.h"
#include "productwidget.h"
#include <QtWidgets>
#include "util.h"
#include <QtSql>
#include <QDebug>

EditProductWidget::EditProductWidget(int productID, QWidget *parent) :
    QWidget(parent),
    productID(productID)
{
    // 窗口标题设置
    QLabel *headTitle, *title;
    if (productID) {
        headTitle = new QLabel("物联网平台 / 设备管理 / 产品/ 编辑产品");
        title = new QLabel("编辑产品（设备模型）");
    } else {
        headTitle = new QLabel("物联网平台 / 设备管理 / 产品/ 新建产品");
        title = new QLabel("新建产品（设备模型）");
    }
    title->setStyleSheet("font: bold 16pt ""宋体""; padding: 10px;");

    // 产品名称编辑框
    nameEdit = new QLineEdit;
    nameEdit->setPlaceholderText("请输入产品名称");
    nameEdit->setFixedSize(450, 30);
    nameEdit->setStyleSheet("font: 14px ""宋体""");

    // 一系列组合框的创建和样式调整
    typeComboBox = new QComboBox;
    connWayComboBox = new QComboBox;
    protocolComboBox = new QComboBox;
    dataFormatComboBox = new QComboBox;
    typeComboBox->setFixedSize(450, 30);
    connWayComboBox->setFixedSize(450, 30);
    protocolComboBox->setFixedSize(450, 30);
    dataFormatComboBox->setFixedSize(450, 30);
    this->comboBoxSetModel();

    // 将按钮组添加到按钮布局
    this->setBtGroup();
    QHBoxLayout * typeBtLayout = new QHBoxLayout;
    for (QAbstractButton * bt: typeBtGroup->buttons()) {
        typeBtLayout->addWidget(bt);
    }
    QHBoxLayout * nodeBtLayout = new QHBoxLayout;
    for (QAbstractButton * bt: nodeBtGroup->buttons()) {
        nodeBtLayout->addWidget(bt);
    }
    typeBtLayout->addStretch();
    nodeBtLayout->addStretch();

    // 窗口子标题创建
    QLabel * subTitle = new QLabel("连网与数据");
    subTitle->setStyleSheet("font: bold 12pt ""宋体""; padding: 10px;");
    QLabel * subTitle2 = new QLabel("更多信息");
    subTitle2->setStyleSheet("font: bold 12pt ""宋体""; padding: 10px;");

    // 产品描述框创建
    descriptionEdit = new QTextEdit;
    descriptionEdit->setPlaceholderText("请输入产品描述");
    descriptionEdit->setFixedSize(450, 60);
    descriptionEdit->setStyleSheet("font: 14px ""宋体""");

    // 底部确认和取消按钮创建
    confirmBt = new QPushButton("确认");
    cancelBt = new QPushButton("取消");
    confirmBt->setStyleSheet("background-color: #0066CC; color: white; "
                         "width: 50px; margin-bottom: 10px; border: none");
    cancelBt->setStyleSheet("width: 50px; margin-bottom: 10px");
    QHBoxLayout * bottomBtLayout = new QHBoxLayout;
    bottomBtLayout->addWidget(confirmBt);
    bottomBtLayout->addWidget(cancelBt);
    bottomBtLayout->addStretch();


    // 窗口主布局
    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headTitle);
    mainLayout->addWidget(title);
    mainLayout->addWidget(new QLabel("*产品名称"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("*所属品类"));

    mainLayout->addLayout(typeBtLayout);
    mainLayout->addWidget(typeComboBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("*节点类型"));
    mainLayout->addLayout(nodeBtLayout);
    mainLayout->addSpacing(10);


    mainLayout->addWidget(subTitle);
    mainLayout->addWidget(new QLabel("*连网方式"));
    mainLayout->addWidget(connWayComboBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("*传输协议"));
    mainLayout->addWidget(protocolComboBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("*数据格式"));
    mainLayout->addWidget(dataFormatComboBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(subTitle2);
    mainLayout->addWidget(new QLabel("*产品描述"));
    mainLayout->addWidget(descriptionEdit);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomBtLayout);


    // 若是编辑操作，设置对应ID的product模型，并构建映射控件的mapper
    if (this->setSqlModel())
        this->setMapper();

    // 主窗口样式设置
    this->setStyleSheet("QComboBox {font: 14px ""宋体"";}");
    QPalette mainPalette = this->palette();
    mainPalette.setColor(QPalette::Background, QColor("#99CCCC"));
    this -> setAutoFillBackground(true);
    this -> setPalette(mainPalette);
    this->setLayout(mainLayout);

    // 连接控件信号与对应槽函数
    connect(confirmBt, SIGNAL(clicked()), this, SLOT(onConfirmBtClicked()));
    connect(cancelBt, SIGNAL(clicked()), this, SLOT(onCancelBtClicked()));
}

void EditProductWidget::onConfirmBtClicked()
{
    QString updateSql;
    if (productID) {
        updateSql = "update product set name=?, "
                    "product_type=?, "
                    "node_type=?, "
                    "connect_method=?, "
                    "protocol=?, "
                    "data_format=?, "
                    "description=? where id=?";
    } else {
        updateSql = "insert into product (name, "
                    "product_type, "
                    "node_type, "
                    "connect_method, "
                    "protocol, "
                    "data_format, "
                    "description) values (?,?,?,?,?,?,?)";
    }

    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(nameEdit->text());
    // 产品品类值绑定
    if (typeBtGroup->checkedId() == 1) {
        query.addBindValue("自定义品类");
    } else {
        query.addBindValue(typeComboBox->currentText());
    }
    query.addBindValue(nodeBtGroup->checkedButton()->text());
    query.addBindValue(connWayComboBox->currentText());
    query.addBindValue(protocolComboBox->currentText());
    query.addBindValue(dataFormatComboBox->currentText());
    query.addBindValue(descriptionEdit->toPlainText());
    // 若是编辑页，绑定产品ID
    if (productID)
        query.addBindValue(productID);
    query.exec();

    // 若是添加页，创建新的属性表
    if (!productID) {
        query.exec("select last_insert_id()");
        query.first();
        QString newID = query.value(0).toString();
        query.exec(tr("create table %1 (id integer primary key auto_increment, "
                      "device_id integer, "
                      "time datetime default now())").arg("product_"+newID+"_attr"));
    }

    // 切换回产品页面
    emit switchSignal();
}

void EditProductWidget::onCancelBtClicked()
{
    if (msg::getCancelMsgBox() == QMessageBox::Ok)
        emit switchSignal();
        //this->switchToProductWidget();
//    QMessageBox cancelMsgBox;
//    cancelMsgBox.resize(200, 100);
//    cancelMsgBox.setText("取消将舍弃当前操作");
//    cancelMsgBox.setInformativeText("确认取消？");
//    cancelMsgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//    cancelMsgBox.setDefaultButton(QMessageBox::Ok);
//    int result = cancelMsgBox.exec();
//    if (result == QMessageBox::Ok)
//        this->switchToProductWidget();
}


void EditProductWidget::comboBoxSetModel()
{
    QStringList items;;
    QStringListModel * model;
    items<<"路灯照明"<<"车辆定位卡"<<"水浸检测"<<"井盖移位检测"
        <<"地磁检测器"<<"无人机"<<"流量计"<<"温湿器";
    model = new QStringListModel(items, this);
    typeComboBox->setModel(model);

    items.clear();
    items<<"WiFi"<<"蜂窝（2G/3G/4G/5G）"<<"以太网"<<"其他";
    model = new QStringListModel(items, this);
    connWayComboBox->setModel(model);

    items.clear();
    items<<"UDP"<<"TCP"<<"MQTT";
    model = new QStringListModel(items, this);
    protocolComboBox->setModel(model);

    items.clear();
    items<<"Alink JSON"<<"透传/自定义";
    model = new QStringListModel(items, this);
    dataFormatComboBox->setModel(model);
}

void EditProductWidget::setBtGroup()
{
    typeBtGroup = new QButtonGroup(this);
    typeBtGroup->addButton(new QRadioButton("标准品类", this), 0);
    typeBtGroup->addButton(new QRadioButton("自定义品类", this), 1);

    // 设置标准品类按钮控制品类组合框的显示和隐藏
    connect(typeBtGroup->button(0), &QAbstractButton::toggled, this, [=](bool checked){
        if (checked)
            typeComboBox->show();
        else
            typeComboBox->hide();
    });

    nodeBtGroup = new QButtonGroup(this);
    nodeBtGroup->addButton(new QRadioButton("直连设备" ,this), 0);
    nodeBtGroup->addButton(new QRadioButton("网关子设备", this), 1);
    nodeBtGroup->addButton(new QRadioButton("网关设备", this), 2);

    typeBtGroup->button(0)->setChecked(true);
    nodeBtGroup->button(0)->setChecked(true);
}

bool EditProductWidget::setSqlModel()
{
    // 判断是否为编辑窗口
    if (!productID) return false;
//    // 数据库连接
//    QSqlDatabase db = dbconn::getDbOpenConn(this);
//    if (!db.isOpen()) return false;
    sqlModel = new QSqlQueryModel(this);
    QString selectSql = "select name, "
                        "product_type, "
                        "node_type, "
                        "connect_method, "
                        "protocol, "
                        "data_format, "
                        "description from product where id=%1";
    sqlModel->setQuery(selectSql.arg(productID));
    return true;
}

void EditProductWidget::setMapper()
{
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);
    mapper->addMapping(nameEdit, 0);
    mapper->addMapping(connWayComboBox, 3, "currentText");
    mapper->addMapping(protocolComboBox, 4, "currentText");
    mapper->addMapping(dataFormatComboBox, 5, "currentText");
    mapper->addMapping(descriptionEdit, 6);
    // 产品所属品类的数据库与窗口的映射
    if (sqlModel->record(0).value(1).toString() == "自定义品类") {
        typeBtGroup->button(1)->setChecked(true);
    } else {
        mapper->addMapping(typeComboBox, 1, "currentText");
    }
    // 节点类型的数据库与窗口的映射
    for (QAbstractButton * bt : nodeBtGroup->buttons()) {
        if (bt->text() == sqlModel->record(0).value(2).toString()) {
            bt->setChecked(true);
            break;
        }
    }
    mapper->toFirst();
}

