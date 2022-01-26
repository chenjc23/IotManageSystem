#include "productwidget.h"
#include "ui_productwidget.h"
#include "util.h"
#include "buttons.h"
#include <QtWidgets>
#include <QtSql>
#include <QDebug>

ProductWidget::ProductWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProductWidget)
{
    // 检查数据库中product表是否存在，不存在则新建product表
    QSqlError err = dbconn::dbProductInit();
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Unable to create product table", err.text());
    }

    // 创建产品界面控件
    ui->setupUi(this);
    QLabel * headTitle = new QLabel("物联网平台 / 设备管理 / 产品");
    QLabel * title = new QLabel("产品（设备模型）");
    QPushButton * createProductBt = new QPushButton("创建产品");
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
    mainLayout -> setStretchFactor(headTitle, 1);
    mainLayout -> setStretchFactor(title, 1);
    mainLayout -> setStretchFactor(funcLayout, 1);
    this -> setLayout(mainLayout);
    // 模型与视图
    QSqlDatabase db = dbconn::getDbOpenConn(this);
    if (!db.isOpen()) return;
    this -> updateModel();
    this -> createView();

    mainLayout -> addWidget(productView);
    mainLayout -> setStretchFactor(productView, 30);


}

ProductWidget::~ProductWidget()
{
    delete ui;
}

void ProductWidget::updateModel()
{
    productModel = new ProductSqlModel(this);
    productModel->setQuery("select name, data_format, node_type, create_time, id from product");
    productModel->setHeaderData(0, Qt::Horizontal, tr("产品名称"));
    productModel->setHeaderData(1, Qt::Horizontal, tr("数据格式"));
    productModel->setHeaderData(2, Qt::Horizontal, tr("节点类型"));
    productModel->setHeaderData(3, Qt::Horizontal, tr("创建时间"));
    productModel->setHeaderData(4, Qt::Horizontal, tr("操作"));
}

void ProductWidget::createView()
{
    productView = new QTableView(this);
    // 视图添加模型
    productView->setModel(productModel);

    // 设置视图样式
    productView->verticalHeader()->hide();
    productView->setShowGrid(false);
    //productView->setSelectionBehavior(QAbstractItemView::SelectRows);
    productView->setStyleSheet("QTableView {font: 16px;} "
                "QTableView::item:hover{background-color: #6699CC; color: white}");
    productView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    productView->horizontalHeader()->setSectionResizeMode(productModel->columnCount()-1, QHeaderView::ResizeToContents);
    // 视图添加控件
    for (int rowNum = 0; rowNum < productModel->rowCount(); ++rowNum) {
        int productId = productModel->data(productModel->index(rowNum, productModel->columnCount()-1)).toInt();
        // 创建产品编辑和删除标签
        ProductButtons * productBt = new ProductButtons(productId, this);
        // 连接标签与相应槽函数
        connect(productBt, SIGNAL(editLabelClicked(int)), this, SLOT(editProduct(int)));
        connect(productBt, SIGNAL(deleteLabelClicked(int)), this, SLOT(deleteProduct(int)));
        // 视图中添加标签
        productView->setIndexWidget(productModel->index(rowNum, productModel->columnCount()-1), productBt);
    }

}

void ProductWidget::editProduct(int itemId)
{
    qDebug() << itemId;
}

void ProductWidget::deleteProduct(int itemId)
{
    qDebug() << itemId;
}
