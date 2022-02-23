#ifndef UTIL_H
#define UTIL_H

#include <../config.h>
#include <QtSql>
#include <QtWidgets>
#include <QMessageBox>
#include <QPushButton>
#include <QDesktopWidget>

namespace dbconn{

QSqlDatabase getDbOpenConn(QWidget *parent = nullptr, QString name = "");

QSqlError dbUserInit();

QSqlError dbProductInit();

QSqlError dbDeviceInit();

}

namespace msg{

int getCancelMsgBox();

}

namespace util {

enum TimeSlot {Hour=1, Day=24, Week=24*7, Month=24*30};

enum ChartType {Area, Line, Spline, Scatter};

void setCenterShow(QWidget * widget);

void setBgColor(QWidget * widget, const QString &color = "#99CCCC");

int getProductID(int device_id);
}

class CenterAlignSqlModel : public QSqlQueryModel
{
public:
    explicit CenterAlignSqlModel(QObject *parent = nullptr);
    ~CenterAlignSqlModel();
    QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const override;
};

class BlueBt : public QPushButton
{
public:
    explicit BlueBt(const QString &text, QWidget *parent = nullptr);
    ~BlueBt();
};

class ImgSwitchBt : public QPushButton
{
    Q_OBJECT
public:
    explicit ImgSwitchBt(QWidget *parent = nullptr);
    explicit ImgSwitchBt(const QString &img1, const QString &img2 = "",
                         int w = 40, int h = 20, QWidget *parent = nullptr);
    ~ImgSwitchBt();
    bool getCurrentState();

signals:
    void stateSwitched(bool state);

private:
    int width;
    int height;
    QString normalImgPath;
    QString pressedImgPath;
    bool currentState = false;
    void init();
};


#endif // UTIL_H


