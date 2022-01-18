#include "entrywidget.h"
#include "ui_entrywidget.h"
#include "util.h"
#include "homewidget.h"
#include <QtSql>
#include <QCryptographicHash>
#include <QDebug>


EntryWidget::EntryWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EntryWidget)
{
    // ui界面样式补充设置
    ui->setupUi(this);
    ui->loginBt->setStyleSheet("background-color: #007d65; color: white; font: 18px");
    ui->signupBt->setStyleSheet("font: 18px");
    ui->titleLabel->setStyleSheet("font: bold 26px");
    ui->userNameEdit->setPlaceholderText("请输入");
    ui->userNameEdit->setFocus(Qt::OtherFocusReason);
    ui->passwordEdit->setPlaceholderText("请输入");
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    this->setStyleSheet("background-color: #cde6c7");

    // 创建user表
    QSqlError err = dbconn::dbUserInit();
    if (err.type() != QSqlError::NoError) {
        QMessageBox::critical(this, "Unable to init database", err.text());
    }

    //
    connect(ui->loginBt, SIGNAL(clicked()), this, SLOT(login()));
    connect(ui->signupBt, SIGNAL(clicked()), this, SLOT(signup()));
}

EntryWidget::~EntryWidget()
{
    delete ui;
}

void EntryWidget::login()
{
    if (!inputCheck()) return;
    // 连接数据库
    QSqlDatabase db = dbconn::getDbOpenConn(this);
    if (!db.isOpen()) return;
    QSqlQuery query;
    if (!query.prepare(tr("select password from user where name = ?"))) {
        showError(query.lastError());
        return;
    }
    query.addBindValue(userName);
    if (!query.exec()) {
        showError(query.lastError());
        return;
    }

    if (query.next()) {
        if (query.value(0).toString() == passwordMd5) {
            // QMessageBox::information(this, "Sucess", "登录成功");
            homeWidget = new HomeWidget;
            this -> hide();
            homeWidget -> show();
        } else {
            QMessageBox::warning(this, "Unvalid", "密码错误，请重新输入");
            return;
        }
    } else {
        QMessageBox::information(this, "Unvalid", "此账号尚未注册，请确认并注册");
        return;
    }
    db.close();

}

void EntryWidget::signup()
{
    if (!inputCheck()) return;
    // 连接数据库
    QSqlDatabase db = dbconn::getDbOpenConn(this);
    if (!db.isOpen()) return;

    // 查询数据库是否已存在该账号
    QSqlQuery query;
    if (!query.prepare(tr("select name from user where name = ?"))) {
        showError(query.lastError());
        return;
    }
    query.addBindValue(userName);
    if (!query.exec()) {
        showError(query.lastError());
        return;
    }
    if (query.size() > 0) {
        QMessageBox::warning(this, "User exists", "该账号已存在，请直接登录");
        return;
    }
    // 若未存在，存入账号密码信息
    if (!query.prepare(tr("insert into user(name, password) values(?, ?)"))) {
        showError(query.lastError());
        return;
    }
    query.addBindValue(userName);
    query.addBindValue(passwordMd5);
    if (!query.exec()) {
        showError(query.lastError());
        return;
    }
    QMessageBox::information(this, "Signup sucess", "注册成功");
    db.close();
}

bool EntryWidget::inputCheck()
{
    userName = ui->userNameEdit->text();
    QString password = ui->passwordEdit->text();
    if (userName.isEmpty()) {
        QMessageBox::information(this, "Non-standard input", "账号不能为空");
        return false;
    }

    if (password.isEmpty()) {
        QMessageBox::information(this, "Non-standard input", "密码不能为空");
        return false;
    }
    // 密码MD5加密
    QCryptographicHash hashMd5(QCryptographicHash::Md5);
    hashMd5.addData(password.toLatin1());
    passwordMd5 = hashMd5.result().toHex().data();

    return true;
}

void EntryWidget::showError(const QSqlError &err)
{
    QMessageBox::critical(this, "DB operation fail", err.text());
}

void EntryWidget::enterEvent(QEvent *event)
{
    qDebug() << event->type();
}

