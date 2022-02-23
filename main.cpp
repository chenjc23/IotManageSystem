#include "entrywidget.h"

#include <QApplication>

#include "homewidget.h"
#include "labels.h"
#include "productwidget.h"
#include "editproductwidget.h"
#include "util.h"
#include <QProcess>
#include <QDebug>
#include "serverwidget.h"

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

//    QProcess udpProcess;
//    udpProcess.start("G:\\QtProject\\build-IotManageSystem-Desktop_Qt_5_9_9_MinGW_32bit-Debug\\udpserver\\debug\\udpserver.exe");

//    qDebug() << udpProcess.state();

//    EditProductWidget E(1);
//    E.show();



//    ProductButtons Bt(1);
//    Bt.show();

    return a.exec();
}
