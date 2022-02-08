#include "entrywidget.h"

#include <QApplication>

#include "homewidget.h"
#include "labels.h"
#include "productwidget.h"
#include "editproductwidget.h"
#include "util.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    EntryWidget w;
//    w.show();
    // 数据库连接
    QSqlDatabase db = dbconn::getDbOpenConn();
    if (!db.isOpen()) return 0;

    HomeWidget H;
    H.show();
//    EditProductWidget E(1);
//    E.show();



//    ProductButtons Bt(1);
//    Bt.show();

    return a.exec();
}
