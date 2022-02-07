#ifndef EDITPRODUCTWIDGET_H
#define EDITPRODUCTWIDGET_H

#include <QtWidgets>
#include <QSqlQueryModel>
#include <QSqlError>

class EditProductWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EditProductWidget(int productID = 0, QWidget *parent = nullptr);

public slots:
    void onConfirmBtClicked();
    void onCancelBtClicked();

private:
    void comboBoxSetModel();
    void setBtGroup();
    bool setSqlModel();
    void setMapper();

private:
    int productID;
    QVBoxLayout * mainLayout;
    QLineEdit * nameEdit;
//    QRadioButton * standardTypeBt;
//    QRadioButton * customTypeBt;
    QComboBox * typeComboBox;
    QButtonGroup * typeBtGroup;
    QButtonGroup * nodeBtGroup;
    QComboBox * connWayComboBox;
    QComboBox * protocolComboBox;
    QComboBox * dataFormatComboBox;
    QTextEdit * descriptionEdit;
    QSqlQueryModel * sqlModel;
    QDataWidgetMapper * mapper;

    QPushButton * confirmBt;
    QPushButton * cancelBt;

signals:
    void switchSignal();

};

#endif // EDITPRODUCTWIDGET_H
