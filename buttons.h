#ifndef BUTTONS_H
#define BUTTONS_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ProductButtons : public QWidget
{
    Q_OBJECT
public:
    explicit ProductButtons(int id, QWidget *parent = nullptr);
    ~ProductButtons();

protected:
    bool eventFilter(QObject *obj, QEvent *ev);


signals:
    void editLabelClicked(int itemId);
    void deleteLabelClicked(int itemId);

private:
    QLabel * editLabel;
    QLabel * deleteLabel;
    int itemId;

};




#endif // BUTTONS_H
