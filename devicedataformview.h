#ifndef DEVICEDATAFORMVIEW_H
#define DEVICEDATAFORMVIEW_H

#include <QWidget>
#include <QTableView>
#include "util.h"

class DeviceDataFormView : public QTableView
{
    Q_OBJECT
public:
    explicit DeviceDataFormView(int deviceID=0, const QString &identifier=QString(), QWidget *parent = nullptr);

    void refresh();
    void refresh(int deviceID, const QString &identifier=QString());
    void clearModel();

private:
    int deviceID;
    QString identifier;

    CenterAlignSqlModel * sqlModel;

signals:

};

#endif // DEVICEDATAFORMVIEW_H
