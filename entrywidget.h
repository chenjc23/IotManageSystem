#ifndef ENTRYWIDGET_H
#define ENTRYWIDGET_H

#include <QWidget>
#include <homewidget.h>
#include <QSqlError>

QT_BEGIN_NAMESPACE
namespace Ui { class EntryWidget; }
QT_END_NAMESPACE

class EntryWidget : public QWidget
{
    Q_OBJECT

public:
    EntryWidget(QWidget *parent = nullptr);
    ~EntryWidget();

    bool inputCheck();
    void showError(const QSqlError &err);

public slots:
    void login();
    void signup();

protected:
    //virtual void enterEvent(QEvent *event) override;

private:
    Ui::EntryWidget *ui;
    QString userName;
    QString passwordMd5;
    HomeWidget * homeWidget;
};
#endif // ENTRYWIDGET_H
