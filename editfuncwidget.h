#ifndef ADDFUNCWIDGET_H
#define ADDFUNCWIDGET_H

#include <QtWidgets>
#include <util.h>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>

class EditFuncWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditFuncWidget(int productID, int attrID = 0, QWidget *parent = nullptr);

public slots:
    void onConfirmBtClicked();
    void closeWidget();

private:
    int productID;
    int attrID;
    QLineEdit * nameEdit;
    QLineEdit * identifierEdit;
    QComboBox * dataFormatComboBox;
    QLineEdit * unitEdit;
    QTextEdit * descriptionEdit;

    BlueBt * confirmBt;
    QPushButton * cancelBt;

    QSqlQueryModel * sqlModel;
    QDataWidgetMapper * mapper;

private:
    bool setSqlModel();
    void setMapper();

signals:
    void editFinished();

};

#endif // ADDFUNCWIDGET_H
