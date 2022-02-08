#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QtWidgets>

class DataWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DataWidget(int deviceID, QWidget *parent = nullptr);

private:
    int deviceID;

signals:

};


class DataBox : public QWidget
{
    Q_OBJECT
public:
    explicit DataBox();
};

#endif // DATAWIDGET_H
