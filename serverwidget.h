#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

public slots:
    void serverTrig(QProcess *process, const QString &arg, QPushButton *bt, QTextEdit *edit, int state = -1);


private:
    Ui::ServerWidget *ui;
    QProcess * udpProcess;
    QProcess * tcpProcess;
    QProcess * mqttProcess;
};

#endif // SERVERWIDGET_H
