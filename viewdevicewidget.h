#ifndef VIEWDEVICEWIDGET_H
#define VIEWDEVICEWIDGET_H

#include <QtWidgets>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

class ViewDeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewDeviceWidget(int deviceID, QWidget *parent = nullptr);
    QWidget * generateTableWidget();


private:
    int deviceID;

    QPushButton * infoBt;
    QPushButton * dataBt;

    enum page{infoPage, dataPage};
    QStackedWidget * stackWidget;

    QLabel *productNameLabel, *nodeTypeLabel, *deviceNameLabel,
        *remarkLabel, *ipLabel, *createTimeLabel, *activatedTimeLabel,
        *lastTimeLabel, *currentStateLabel;

    QSqlQueryModel * sqlModel;
    QDataWidgetMapper * mapper;

private:
    void refresh();
    void setMapper();

signals:

};

#endif // VIEWDEVICEWIDGET_H
