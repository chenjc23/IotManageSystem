#include "productwidget.h"
#include "ui_productwidget.h"
#include "util.h"
#include "labels.h"
#include "editproductwidget.h"
#include "viewproductwidget.h"
#include <QtWidgets>
#include <QtSql>
#include <QDebug>

ProductWidget::ProductWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget)
{
    // 创建产品界面控件
    ui->setupUi(this);
    QLabel * headTitle = new QLabel("物联网平台 / 设备管理 / 产品");
    QLabel * title = new QLabel("产品（设备模型）");
    createProductBt = new QPushButton("创建产品");
    QLineEdit * searchEdit = new QLineEdit(this);

    // 部分控件样式设置
    title -> setStyleSheet("font: bold 16pt ""宋体""; padding: 10px;");
    createProductBt -> setStyleSheet("background-color: #0066CC; color: white");
    searchEdit -> setPlaceholderText("输入产品名称查询");
    QPalette mainPalette = this -> palette();
    mainPalette.setColor(QPalette::Background, QColor("#99CCCC"));
    this -> setAutoFillBackground(true);
    this -> setPalette(mainPalette);


    // 控件布局设置
    QHBoxLayout * funcLayout = new QHBoxLayout;
    QVBoxLayout * mainLayout = new QVBoxLayout;

    funcLayout -> addWidget(createProductBt);
    funcLayout -> addWidget(searchEdit);
    funcLayout -> addStretch();

    mainLayout -> addWidget(headTitle);
    mainLayout -> addWidget(title);
    mainLayout -> addLayout(funcLayout);
    this -> setLayout(mainLayout);

    // 模型与视图
    productModel = new CenterAlignSqlModel(this);
    productView = new QTableView(this);
    // 视图添加模型
    productView->setModel(productModel);
    // 设置视图样式
    productView->verticalHeader()->hide();
    productView->setShowGrid(false);
    productView->setStyleSheet("QTableView {font: 16px;} "
                "QTableView::item:hover{background-color: #6699CC; color: white}");
    productView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    this->refresh();

    mainLayout -> addWidget(productView);
    mainLayout -> setStretchFactor(productView, 20);

    connect(createProductBt, SIGNAL(clicked()), this, SLOT(onCreateBtClicked()));


}

ProductWidget::~ProductWidget()
{
    delete ui;
}

void ProductWidget::refresh()
{
    productModel->setQuery("select name, protocol, node_type, create_time, id from product");
    productModel->setHeaderData(0, Qt::Horizontal, tr("产品名称"));
    productModel->setHeaderData(1, Qt::Horizontal, tr("传输协议"));
    productModel->setHeaderData(2, Qt::Horizontal, tr("节点类型"));
    productModel->setHeaderData(3, Qt::Horizontal, tr("创建时间"));
    productModel->setHeaderData(4, Qt::Horizontal, tr("操作"));
    // 调整视图最后一列的尺寸策略
    productView->horizontalHeader()->setSectionResizeMode(productModel->columnCount()-1, QHeaderView::ResizeToContents);
    // 视图添加控件
    for (int rowNum = 0; rowNum < productModel->rowCount(); ++rowNum) {
        int productId = productModel->data(productModel->index(rowNum, productModel->columnCount()-1)).toInt();
        // 创建产品编辑和删除标签
        StdComboLabels * productBt = new StdComboLabels(productId, this);
        // 视图中添加产品编辑和删除标签
        productView->setIndexWidget(productModel->index(rowNum, productModel->columnCount()-1), productBt);
        // 连接标签与相应槽函数1
        connect(productBt, SIGNAL(viewLabelClicked(int)), this, SLOT(viewProduct(int)));
        connect(productBt, SIGNAL(editLabelClicked(int)), this, SLOT(editProduct(int)));
        connect(productBt, SIGNAL(deleteLabelClicked(int)), this, SLOT(deleteProduct(int)));
    }
}

void ProductWidget::onCreateBtClicked()
{
    // 进入创建产品界面，由id为0指示为创建页面
    editProduct(0);
}

void ProductWidget::viewProduct(int itemId)
{
    QStackedWidget * stackWidget = static_cast<QStackedWidget *>(this->parentWidget());
    ViewProductWidget * viewProductWidget = new ViewProductWidget(itemId, stackWidget);
    stackWidget->addWidget(viewProductWidget);
    stackWidget->setCurrentWidget(viewProductWidget);
}

void ProductWidget::editProduct(int itemId)
{
    QStackedWidget * stackWidget = static_cast<QStackedWidget *>(this->parentWidget());
    EditProductWidget * editProductWidget = new EditProductWidget(itemId, stackWidget);
    stackWidget->addWidget(editProductWidget);
    stackWidget->setCurrentWidget(editProductWidget);
    connect(editProductWidget, &EditProductWidget::switchSignal, [=]{
        this->refresh();
        stackWidget->setCurrentWidget(this);
        stackWidget->removeWidget(editProductWidget);
    });
}

void ProductWidget::deleteProduct(int itemId)
{
    if (msg::getCancelMsgBox() == QMessageBox::Ok) {
        QSqlQuery query;
        // 删除product表
        query.exec(tr("delete from product where id=%1").arg(itemId));
        // 删除product下所有设备属性表
        query.exec(tr("select id from device where product_id=%1").arg(itemId));
        QSqlQuery dropQuery;
        while (query.next()) {
            dropQuery.exec(tr("drop table device_%1_attr").arg(query.value(0).toInt()));
        }
        this->refresh();
    }
}

