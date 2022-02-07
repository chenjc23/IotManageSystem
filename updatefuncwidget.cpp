#include "updatefuncwidget.h"
#include "util.h"
#include <QDesktopWidget>
#include <QDebug>

UpdateFuncWidget::UpdateFuncWidget(int productID, int attrID, QWidget *parent) :
    QWidget(parent),
    productID(productID),
    attrID(attrID)
{
    QLabel * title = new QLabel("添加自定义功能");
    title -> setStyleSheet("font: bold 14pt ""宋体""; padding: 10px;");
    nameEdit = new QLineEdit;
    identifierEdit = new QLineEdit;
    dataFormatComboBox = new QComboBox;

    // combobox设置model
    QStringList items;
    items<< "int"<<"float"<<"double"<<"enum"<<"text"<<"date";
    QStringListModel * model = new QStringListModel(items);
    dataFormatComboBox->setModel(model);

    unitEdit = new QLineEdit;
    descriptionEdit = new QTextEdit;
    nameEdit->setPlaceholderText("请输入功能名称");
    identifierEdit->setPlaceholderText("请输入标识符");
    unitEdit->setPlaceholderText("请输入单位");
    descriptionEdit->setPlaceholderText("请输入描述");
    descriptionEdit->setFixedHeight(80);

    // 底部button
    confirmBt = new BlueBt("确认");
    confirmBt->setFixedSize(70, 20);
    cancelBt = new QPushButton("取消");
    QHBoxLayout * bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(confirmBt);
    bottomLayout->addWidget(cancelBt);
    bottomLayout->setSpacing(10);
    bottomLayout->addStretch();

    // 主窗口布局
    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(title);
    mainLayout->addWidget(new QLabel("*功能名称"));
    mainLayout->addWidget(nameEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("*标识符"));
    mainLayout->addWidget(identifierEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("*数据类型"));
    mainLayout->addWidget(dataFormatComboBox);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("单位"));
    mainLayout->addWidget(unitEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("描述"));
    mainLayout->addWidget(descriptionEdit);
    mainLayout->addStretch();
    mainLayout->addLayout(bottomLayout);
    this->setLayout(mainLayout);

    if (this->setSqlModel())
        this->setMapper();

    // 调整窗口属性
    QDesktopWidget * desktop = QApplication::desktop();
    this->setFixedSize(350, 450);
    this->setGeometry((desktop->width()-width())/2,
                      (desktop->height()-height())/2, width(), height());
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    connect(confirmBt, &QPushButton::clicked, this, &UpdateFuncWidget::onConfirmBtClicked);
    connect(cancelBt, &QPushButton::clicked, this, &UpdateFuncWidget::closeWidget);

}

void UpdateFuncWidget::onConfirmBtClicked()
{
    QString updateSql;
    if (attrID) {
        updateSql = "update attr set func_name=?,"
                    "identifier=?,"
                    "data_type=?,"
                    "unit=?,"
                    "description=? where id=?";
    } else {
        updateSql = "insert into attr (func_name,"
                    "identifier,"
                    "data_type,"
                    "unit,"
                    "description,"
                    "product_id) values (?,?,?,?,?,?)";
    }
    QSqlQuery query;
    query.prepare(updateSql);
    query.addBindValue(nameEdit->text());
    query.addBindValue(identifierEdit->text());
    query.addBindValue(dataFormatComboBox->currentText());
    query.addBindValue(unitEdit->text());
    query.addBindValue(descriptionEdit->toPlainText());

    if (attrID) {
        query.addBindValue(attrID);
    } else {
        query.addBindValue(productID);
    }
    // 若是插入功能，更新product表
    if (query.exec() && !attrID) {
        query.exec("select last_insert_id()");
        query.first();
        QString newID = query.value(0).toString();
        query.prepare("update product "
                      "set func_ids=concat(func_ids, ?) where id=?");
        query.addBindValue(newID+",");
        query.addBindValue(productID);
        query.exec();
    }
    emit updateFinished();
    this->close();
}

void UpdateFuncWidget::closeWidget()
{
    if (msg::getCancelMsgBox() == QMessageBox::Ok) {
        this->close();
    }
}

bool UpdateFuncWidget::setSqlModel()
{
    if (!attrID) return false;
    sqlModel = new QSqlQueryModel(this);
    QString selectSql = "select func_name, "
                        "identifier, "
                        "data_type, "
                        "unit, "
                        "description from attr where id=%1";
    sqlModel->setQuery(selectSql.arg(attrID));
    return true;
}

void UpdateFuncWidget::setMapper()
{
    mapper = new QDataWidgetMapper(this);
    mapper->setModel(sqlModel);
    mapper->addMapping(nameEdit, 0);
    mapper->addMapping(identifierEdit, 1);
    mapper->addMapping(dataFormatComboBox, 2, "currentText");
    mapper->addMapping(unitEdit, 3);
    mapper->addMapping(descriptionEdit, 4);
    mapper->toFirst();
}
