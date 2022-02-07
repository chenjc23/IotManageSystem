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
    void viewLabelClicked(int itemId);
    void editLabelClicked(int itemId);
    void deleteLabelClicked(int itemId);

private:
    QLabel * viewLabel;
    QLabel * editLabel;
    QLabel * deleteLabel;
    int itemId;

};

class FuncLabel : public QLabel
{
    Q_OBJECT
public:
    explicit FuncLabel(const QString &text, int id = 0, QWidget *parent = nullptr);
    ~FuncLabel();

protected:
    void mousePressEvent(QMouseEvent *ev) override;

private:
    int id;

signals:
    void funcLabelClicked();
    void funcLabelClicked(int id);

};




#endif // BUTTONS_H
