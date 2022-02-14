#include "util.h"
#include <QtSql>
#include <QtWidgets>
#include <QDesktopWidget>

QSqlDatabase dbconn::getDbOpenConn(QWidget *parent, QString name)
{
    Config config;
    QSqlDatabase db;
    if (!name.isEmpty())
        db = QSqlDatabase::addDatabase("QMYSQL", name);
    else
        db = QSqlDatabase::addDatabase("QMYSQL");

    db.setDatabaseName(config.dbDatabaseName);
    db.setHostName(config.dbHostName);
    db.setUserName(config.dbUserName);
    db.setPassword(config.dbPassword);
    if (!db.open()) {
        if (parent)
            QMessageBox::information(parent, "DB Connection Fail", "无法连接数据库");
    }
    return db;
}

// user表初始化
QSqlError dbconn::dbUserInit()
{
    QSqlDatabase db = getDbOpenConn(nullptr, "user");
    if (db.lastError().type() != QSqlError::NoError)
        return db.lastError();

    if (db.tables().contains("user", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery query;
    // 创建user表
    QString userCreationSql = "create table user(id integer primary key auto_increment, "
                              "name varchar(20), password char(32))";
    if (!query.exec(userCreationSql))
        return query.lastError();

    return QSqlError();
}

// product表初始化
QSqlError dbconn::dbProductInit()
{
    QSqlDatabase db = getDbOpenConn(nullptr, "product");
    if (db.lastError().type() != QSqlError::NoError)
        return db.lastError();

    QSqlQuery query;

    if (!db.tables().contains("product", Qt::CaseInsensitive)) {
        // 创建product表
        QString productCreationSql = "create table product(id integer primary key auto_increment, "
                                     "name varchar(30), "
                                     "product_type varchar(30), "
                                     "node_type varchar(30), "
                                     "protocol varchar(20), "
                                     "connect_method varchar(30), "
                                     "data_format varchar(30), "
                                     "description varchar(200), "
                                     "create_time datetime default now())";
        if (!query.exec(productCreationSql))
            return query.lastError();
    }

    if (!db.tables().contains("attr", Qt::CaseInsensitive)) {
        // 创建attr表
        QString attrCreationSql = "create table attr(id integer primary key auto_increment, "
                                  "product_id integer,"
                                  "func_name varchar(20),"
                                  "identifier varchar(20),"
                                  "data_type varchar(20),"
                                  "unit varchar(20),"
                                  "description varchar(100),"
                                  "foreign key (product_id) references product(id))";
        if (!query.exec(attrCreationSql))
            return query.lastError();
    }

    return QSqlError();

}

QSqlError dbconn::dbDeviceInit()
{
    QSqlDatabase db = getDbOpenConn(nullptr, "device");
    if (db.lastError().type() != QSqlError::NoError)
        return db.lastError();

    if (db.tables().contains("device", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery query;
    // 创建device表
    QString deviceCreationSql = "create table device(id integer primary key auto_increment, "
               "name varchar(20), "
               "remark varchar(20), "
               "product_id integer, "
               "ip varchar(20), "
               "create_time datetime default now(), "
               "activated_time datetime, "
               "last_time datetime, "
               "current_state varchar(20) default '未激活', "
               "foreign key (product_id) references product(id) "
               "on delete cascade on update cascade)";
    if (!query.exec(deviceCreationSql))
        return query.lastError();

    db.close();
    return QSqlError();
}



CenterAlignSqlModel::CenterAlignSqlModel(QObject *parent) :
    QSqlQueryModel(parent)
{

}

CenterAlignSqlModel::~CenterAlignSqlModel()
{

}

QVariant CenterAlignSqlModel::data(const QModelIndex &item, int role) const
{
    QVariant value = QSqlQueryModel::data(item, role);
    if (role == Qt::TextAlignmentRole) {
        value = Qt::AlignCenter;
        return value;
    }
    return value;
}

BlueBt::BlueBt(const QString &text, QWidget *parent) :
    QPushButton(text, parent)
{
    this->setStyleSheet("background-color: #0066CC; color: white; "
                        "border: none");
}

BlueBt::~BlueBt()
{

}

int msg::getCancelMsgBox()
{
    QMessageBox cancelMsgBox;
    cancelMsgBox.setWindowFlag(Qt::WindowStaysOnTopHint);
    cancelMsgBox.resize(200, 100);
    cancelMsgBox.setText("取消将舍弃当前操作");
    cancelMsgBox.setInformativeText("确认取消？");
    cancelMsgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    cancelMsgBox.setDefaultButton(QMessageBox::Ok);
    return cancelMsgBox.exec();
}



void util::setCenterShow(QWidget *widget)
{
    QDesktopWidget * desktop = QApplication::desktop();
    widget->setGeometry((desktop->width()-widget->width())/2,
        (desktop->height()-widget->height())/2, widget->width(), widget->height());
}

void util::setBgColor(QWidget *widget, const QString &color)
{
    QPalette mainPalette = widget->palette();
    mainPalette.setColor(QPalette::Background, QColor(color));
    widget->setAutoFillBackground(true);
    widget->setPalette(mainPalette);
}


ImgSwitchBt::ImgSwitchBt(const QString &img1, const QString &img2, int w, int h, QWidget *parent) :
    QPushButton(parent),
    width(w),
    height(h),
    normalImgPath(img1),
    pressedImgPath(img2)
{
    QPixmap pix;
    pix.load(normalImgPath);
    this->setIcon(pix);
    this->setIconSize(QSize(width, height));
    this->setFixedSize(width, height);
    this->setStyleSheet("border: 0px;");

    if (pressedImgPath.isEmpty()) return;
    connect(this, &QPushButton::clicked, [=]{
        currentState = !currentState;
        if (currentState)
            this->setIcon(QPixmap(pressedImgPath));
        else
            this->setIcon(QPixmap(normalImgPath));
        emit this->stateSwitched(currentState);
    });
}

ImgSwitchBt::~ImgSwitchBt()
{

}

bool ImgSwitchBt::getCurrentState()
{
    return currentState;
}

int util::getProductID(int deviceID)
{
    QSqlQuery query;
    query.exec(QString("select product_id from device where id=%1").arg(deviceID));
    query.first();
    return query.value(0).toInt();
}
