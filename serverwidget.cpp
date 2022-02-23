#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QSettings>
#include <QProcess>
#include <QCoreApplication>
#include "util.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget),
    udpProcess(new QProcess),
    tcpProcess(new QProcess),
    mqttProcess(new QProcess)
{
    // 于ui添加server信息
    ui->setupUi(this);
    QSettings settings("../config.ini", QSettings::IniFormat, this);
    ui->udpHostEdit->setText(settings.value("ip/udp").toString());
    ui->udpPortEdit->setText(settings.value("port/udp").toString());
    ui->tcpHostEdit->setText(settings.value("ip/tcp").toString());
    ui->tcpPortEdit->setText(settings.value("port/tcp").toString());
    ui->mqttHostEdit->setText(settings.value("ip/mqtt").toString());
    ui->mqttPortEdit->setText(settings.value("port/mqtt").toString());

    // server process信号连接
    connect(udpProcess, &QProcess::readyRead, [=]{
        ui->udpLogEdit->insertPlainText(udpProcess->readAll());
        ui->udpLogEdit->moveCursor(QTextCursor::End);
    });

    connect(tcpProcess, &QProcess::readyRead, [=]{
        ui->tcpLogEdit->insertPlainText(tcpProcess->readAll());
        ui->tcpLogEdit->moveCursor(QTextCursor::End);
    });

    connect(mqttProcess, &QProcess::readyRead, [=]{
        ui->mqttLogEdit->insertPlainText(mqttProcess->readAll());
        ui->mqttLogEdit->moveCursor(QTextCursor::End);
    });


    // 控件信号连接
    connect(ui->udpBt, &QPushButton::clicked, [=]{
        serverTrig(udpProcess, "-u", ui->udpBt, ui->udpLogEdit);
    });

    connect(ui->tcpBt, &QPushButton::clicked, [=]{
        serverTrig(tcpProcess, "-t", ui->tcpBt, ui->tcpLogEdit);
    });

    connect(ui->mqttBt, &QPushButton::clicked, [=]{
        serverTrig(mqttProcess, "-m", ui->mqttBt, ui->mqttLogEdit);
    });

    connect(ui->allActBt, &ImgSwitchBt::stateSwitched, [=](bool state){
        serverTrig(udpProcess, "-u", ui->udpBt, ui->udpLogEdit, state);
        serverTrig(tcpProcess, "-t", ui->tcpBt, ui->tcpLogEdit, state);
        serverTrig(mqttProcess, "-m", ui->mqttBt, ui->mqttLogEdit, state);
        if (state)
            ui->allBtLabel->setText("一键停止");
        else
            ui->allBtLabel->setText("一键启动");
    });
}

ServerWidget::~ServerWidget()
{
    delete ui;
    udpProcess->close();
    tcpProcess->close();
    mqttProcess->close();
    delete udpProcess;
    delete tcpProcess;
    delete mqttProcess;
}

void ServerWidget::serverTrig(QProcess *process, const QString &arg,
                              QPushButton *bt, QTextEdit *edit, int state)
{
    if (!process->state() or state == 1) {
        if (process->state() or state == 0) return;
        process->start(QCoreApplication::applicationDirPath()+"/../../ServerProcess/debug/ServerProcess.exe",
                       QStringList() << arg);
        bt->setText("停止");
    } else {
        edit->clear();
        process->close();
        bt->setText("启动");
    }
}
