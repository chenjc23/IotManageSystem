#ifndef GROUPINGWIDGET_H
#define GROUPINGWIDGET_H

#include <QWidget>
#include <QTreeWidget>

class GroupingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GroupingWidget(QWidget *parent = nullptr);

private:
    QTreeWidget * treeWidget;

signals:

};

#endif // GROUPINGWIDGET_H
