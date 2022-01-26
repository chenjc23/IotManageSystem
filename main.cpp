#include "entrywidget.h"

#include <QApplication>

#include "homewidget.h"
#include "buttons.h"
#include "productwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    EntryWidget w;
//    w.show();
    HomeWidget H;
    H.show();
//    ProductButtons Bt(1);
//    Bt.show();

    return a.exec();
}
