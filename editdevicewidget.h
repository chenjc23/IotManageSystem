#ifndef EDITDEVICEWIDGET_H
#define EDITDEVICEWIDGET_H

#include <QtWidgets>
#include "util.h"
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

class EditDeviceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditDeviceWidget(int deviceID = 0, QWidget *parent = nullptr);

public slots:
    void onConfirmBtClicked();

private:

    int deviceID;

    QComboBox * productComboBox;
    QLineEdit * nameEdit;
    QLineEdit * remarkEdit;

    BlueBt * confirmBt;
    QPushButton * cancelBt;

    QSqlQueryModel * sqlModel;
    QDataWidgetMapper * mapper;

private:
    void setProductComboBoxModel();
    void setMapperWithModel();

signals:
    void editFinished();


};

#endif // EDITDEVICEWIDGET_H
