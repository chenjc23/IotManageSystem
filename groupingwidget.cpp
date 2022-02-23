#include "groupingwidget.h"
#include <QtWidgets>
#include <QTreeWidget>


GroupingWidget::GroupingWidget(QWidget *parent) : QWidget(parent)
{
    treeWidget = new  QTreeWidget;
    treeWidget->header()->hide();
    treeWidget->setFixedWidth(170);
    treeWidget->setFocusPolicy(Qt::NoFocus);
    treeWidget->setIndentation(0);



    QTreeWidgetItem * topItem = new QTreeWidgetItem(treeWidget);
    topItem->setText(0, "设备管理");
    //topItem->setBackgroundColor(0, QColor("#CCCCCC"));
    //topItem->setTextAlignment(0, Qt::AlignCenter);
    topItem->setFont(0, QFont("宋体", 16, QFont::Bold));

    QTreeWidgetItem * aItem = new QTreeWidgetItem(treeWidget);
    aItem->setText(0, "数据分析");
    aItem->setFont(0, QFont("宋体", 16, QFont::Bold));

    QTreeWidgetItem * productItem = new QTreeWidgetItem(topItem);
    productItem->setText(0, "产品");
    productItem->setTextAlignment(0, Qt::AlignCenter);

    QTreeWidgetItem * deviceItem = new QTreeWidgetItem(topItem);
    deviceItem->setText(0, "设备");

    treeWidget->setStyleSheet("QTreeWidget{font: 20px ""宋体""; } QTreeWidget::item {padding: 10px;"
                              "} QTreeWidget::item:selected{background-color: #CCCCCC; color: black;} "
                              "");

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeWidget);
    this->setLayout(mainLayout);
    this->setStyleSheet("background-color: transparent;");



}
