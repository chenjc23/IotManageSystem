#include "viewproductwidget.h"
#include "editproductwidget.h"
#include "util.h"
#include "labels.h"
#include "editfuncwidget.h"
#include <QtWidgets>
#include <QtSql>


ViewProductWidget::ViewProductWidget(int productID, QWidget *parent) :
    QWidget(parent), productID(productID)
{
    sqlModel = new QSqlQueryModel(this);
    this->refresh();
    this->setStyleSheet("QPushButton {background-color: white;}");
    QPalette mainPalette = this->palette();
    mainPalette.setColor(QPalette::Background, QColor("#99CCCC"));
    this -> setAutoFillBackground(true);
    this -> setPalette(mainPalette);


    // 产品标题
    QLabel * headTitle = new QLabel("物联网平台 / 设备管理 / 产品/ 产品详情");
    QLabel * title = new QLabel(sqlModel->record(0).value(0).toString());
    title->setStyleSheet("font: bold 16pt ""宋体""; padding: 10px;");

    // 导航按钮
    QPushButton * infoBt = new QPushButton("产品信息");
    QPushButton * funcBt = new QPushButton("功能定义");
    QHBoxLayout * guideLayout = new QHBoxLayout;
    guideLayout->addWidget(infoBt);
    guideLayout->addWidget(funcBt);
    guideLayout->setSpacing(0);
    guideLayout->addStretch();

    // 栈窗口
    stackWidget = new QStackedWidget;
    stackWidget->insertWidget(infoPage, generateTableWidget());
    stackWidget->insertWidget(funcPage, new AttrWidget(productID));
    stackWidget->setCurrentIndex(infoPage);

    this->setMapper();

    // 主窗口布局
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(headTitle);
    mainLayout->addWidget(title);
    mainLayout->addLayout(guideLayout);
    mainLayout->addWidget(stackWidget);
    mainLayout->addStretch();

    mainLayout->setStretchFactor(stackWidget, 20);

    this->setLayout(mainLayout);

    connect(infoBt, &QPushButton::clicked, this, [=]{
        this->refresh();
        stackWidget->setCurrentIndex(infoPage);
    });
    connect(funcBt, &QPushButton::clicked, this, [=]{
        static_cast<AttrWidget *>(stackWidget->widget(funcPage))->refresh();
        stackWidget->setCurrentIndex(funcPage);
    });
}

QWidget *ViewProductWidget::generateTableWidget()
{
    QWidget * tableWidget = new QWidget;

    tableWidget->setStyleSheet("QLabel{font: 16px;} QWidget{background-color: white;}");
    // 表窗口标题
    QLabel * infoLabel = new QLabel("产品信息");
    editLabel = new QLabel("编辑");
    infoLabel->setStyleSheet("font: bold");
    editLabel->setStyleSheet("color: blue");
    editLabel->installEventFilter(this);
    QHBoxLayout * headLayout = new QHBoxLayout;
    headLayout->addWidget(infoLabel);
    headLayout->addSpacing(10);
    headLayout->addWidget(editLabel);
    headLayout->addStretch();

    // info表
    QGridLayout * infoLayout = new QGridLayout;
    infoLayout->addWidget(new QLabel("产品名称:"), 0, 0, 2, 1);
    infoLayout->addWidget(nameLabel = new QLabel, 0, 1, 2, 2);
    infoLayout->addWidget(new QLabel("节点类型:"), 0, 2, 2, 1);
    infoLayout->addWidget(nodeLabel = new QLabel, 0, 3, 2, 2);
    infoLayout->addWidget(new QLabel("创建时间:"), 0, 4, 2, 1);
    infoLayout->addWidget(timeLabel = new QLabel, 0, 5, 2, 2);
    infoLayout->addWidget(new QLabel("所属品类:"), 2, 0, 2, 1);
    infoLayout->addWidget(typeLabel = new QLabel, 2, 1, 2, 2);
    infoLayout->addWidget(new QLabel("数据格式:"), 2, 2, 2, 1);
    infoLayout->addWidget(dataFormatLabel = new QLabel, 2, 3, 2, 2);
    infoLayout->addWidget(new QLabel("连网方式:"), 2, 4, 2, 1);
    infoLayout->addWidget(connLabel = new QLabel, 2, 5, 2, 2);
    infoLayout->addWidget(new QLabel("传输协议:"), 4, 0, 2, 1);
    infoLayout->addWidget(protocolLabel = new QLabel, 4, 1, 2, 2);
    infoLayout->addWidget(new QLabel("产品描述:"), 4, 2, 2, 1);
    infoLayout->addWidget(desLabel = new QLabel, 4, 3, 2, 4);

    // 布局
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addLayout(headLayout);
    mainLayout->addSpacing(20);
    mainLayout->addLayout(infoLayout);
    mainLayout->addStretch();
    tableWidget->setLayout(mainLayout);

    return tableWidget;
}

bool ViewProductWidget::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == editLabel) {
        if (ev->type() == QEvent::MouseButtonPress) {
            QStackedWidget * stackWidget = static_cast<QStackedWidget *>(this->parentWidget());
            EditProductWidget * editProductWidget = new EditProductWidget(productID, stackWidget);
            stackWidget->addWidget(editProductWidget);
            stackWidget->setCurrentWidget(editProductWidget);
            connect(editProductWidget, &EditProductWidget::switchSignal, [=]{
                this->refresh();
                stackWidget->setCurrentWidget(this);
                stackWidget->removeWidget(editProductWidget);
            });
        }
    }
    return QWidget::eventFilter(obj, ev);
}

void ViewProductWidget::refresh()
{
    QString selectSql = "select name, "
                        "product_type, "
                        "node_type, "
                        "connect_method, "
                        "protocol, "
                        "data_format, "
                        "description, "
                        "create_time from product where id=%1";
    sqlModel->setQuery(selectSql.arg(productID));
}

void ViewProductWidget::setMapper()
{
    QDataWidgetMapper * tableMapper = new QDataWidgetMapper(this);
    tableMapper->setModel(sqlModel);
    tableMapper->addMapping(nameLabel, 0, "text");
    tableMapper->addMapping(typeLabel, 1, "text");
    tableMapper->addMapping(nodeLabel, 2, "text");
    tableMapper->addMapping(connLabel, 3, "text");
    tableMapper->addMapping(protocolLabel, 4, "text");
    tableMapper->addMapping(dataFormatLabel, 5, "text");
    tableMapper->addMapping(desLabel, 6, "text");
    tableMapper->addMapping(timeLabel, 7, "text");
    tableMapper->toFirst();
}


// 属性窗口
AttrWidget::AttrWidget(int productID, QWidget *parent) :
    QWidget(parent), productID(productID)
{
    funcBt = new BlueBt("添加功能");
    funcBt->setFixedWidth(80);
    attrModel = new CenterAlignSqlModel(this);
    attrView = new QTableView(this);
    attrView->setModel(attrModel);
    // 设置表样式
    attrView->verticalHeader()->hide();
    attrView->setStyleSheet("QTableView {font: 16px;} "
                "QTableView::item:hover{background-color: #6699CC; color: white}");
    attrView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->refresh();

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(funcBt);
    mainLayout->addWidget(attrView);

    mainLayout->setStretchFactor(attrView, 20);
    this->setLayout(mainLayout);

    connect(funcBt, &QPushButton::clicked, this, [=]{
        showFuncWidget(0);
    });
}

AttrWidget::~AttrWidget()
{

}

void AttrWidget::showFuncWidget(int id)
{
    EditFuncWidget * funcWidget = new EditFuncWidget(productID, id);
    funcWidget->show();
    connect(funcWidget, &EditFuncWidget::editFinished, [=]{
        this->refresh();
    });
}

void AttrWidget::refresh()
{
    attrModel->setQuery(tr("select func_name, identifier, "
                        "data_type, unit, id from attr where product_id=%1").arg(productID));
    attrModel->setHeaderData(0, Qt::Horizontal, tr("功能名称"));
    attrModel->setHeaderData(1, Qt::Horizontal, tr("标识符"));
    attrModel->setHeaderData(2, Qt::Horizontal, tr("数据类型"));
    attrModel->setHeaderData(3, Qt::Horizontal, tr("单位"));
    attrModel->setHeaderData(4, Qt::Horizontal, tr("操作"));
    // 添加操作控件
    for (int rowNum = 0; rowNum < attrModel->rowCount(); ++rowNum) {
        int id = attrModel->record(rowNum).value(attrModel->columnCount()-1).toInt();
        FuncLabel * editLabel = new FuncLabel("编辑", id);
        attrView->setIndexWidget(attrModel->index(rowNum, attrModel->columnCount()-1), editLabel);
        connect(editLabel, SIGNAL(funcLabelClicked(int)), this, SLOT(showFuncWidget(int)));
    }
}
