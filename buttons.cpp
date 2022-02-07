#include "buttons.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>
#include <QDebug>

ProductButtons::ProductButtons(int id, QWidget *parent) :
    QWidget(parent), itemId(id)
{
    // label style 设置
    this->setStyleSheet("QLabel {color: blue; font: 16px;} "
                        "QLabel:hover{color: #003399}");
    // 避免内部控件尺寸小于窗体最小尺寸
    this->setMinimumSize(120, 35);
    // 创建label
    viewLabel = new QLabel("查看");
    editLabel = new QLabel("编辑");
    deleteLabel = new QLabel("删除");
    viewLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    editLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    deleteLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    viewLabel->installEventFilter(this);
    editLabel->installEventFilter(this);
    deleteLabel->installEventFilter(this);

    QHBoxLayout * mainLayout = new QHBoxLayout;

    mainLayout->addWidget(viewLabel);
    mainLayout->addWidget(editLabel);
    mainLayout->addWidget(deleteLabel);
    this->setLayout(mainLayout);
    this->setAutoFillBackground(true);
}

ProductButtons::~ProductButtons()
{

}

// 对编辑label和删除label的鼠标点击事件进行过滤
bool ProductButtons::eventFilter(QObject *obj, QEvent *ev)
{
    if (obj == viewLabel) {
        if (ev->type() == QEvent::MouseButtonPress) {
            emit this->viewLabelClicked(itemId);
        }
    }
    if (obj == editLabel) {
        if (ev->type() == QEvent::MouseButtonPress) {
            emit this->editLabelClicked(itemId);
        }
    }
    if (obj == deleteLabel) {
        if (ev->type() == QEvent::MouseButtonPress) {
            emit this->deleteLabelClicked(itemId);
        }
    }
    return QWidget::eventFilter(obj, ev);
}

FuncLabel::FuncLabel(const QString &text, int id, QWidget *parent) :
    QLabel(text, parent), id(id)
{
    this->setStyleSheet("QLabel {color: blue; font: 16px;} "
                        "QLabel:hover{color: #003399}");
    this->setAutoFillBackground(true);

}

FuncLabel::~FuncLabel()
{

}

void FuncLabel::mousePressEvent(QMouseEvent *ev)
{
    if (id)
        emit funcLabelClicked(id);
    else
        emit funcLabelClicked();
    return QLabel::mousePressEvent(ev);
}

