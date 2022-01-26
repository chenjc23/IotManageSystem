#include "buttons.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QEvent>

ProductButtons::ProductButtons(int id, QWidget *parent) :
    QWidget(parent), itemId(id)
{
    // label style 设置
    this->setStyleSheet("QLabel {color: blue; font: 16px;} "
                        "QLabel:hover{color: #003399}");
    // 避免内部控件尺寸小于窗体最小尺寸
    this->setMinimumSize(120, 35);
    // 创建编辑lebel和删除label
    editLabel = new QLabel("编辑");
    deleteLabel = new QLabel("删除");
    editLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    deleteLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    editLabel->installEventFilter(this);
    deleteLabel->installEventFilter(this);

    QHBoxLayout * mainLayout = new QHBoxLayout;
    // mainLayout->setMargin(5);
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
