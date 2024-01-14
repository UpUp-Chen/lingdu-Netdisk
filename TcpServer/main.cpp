#include "tcpserver.h"

#include <QApplication>
#include "DataBase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataBase::instance().init();

    TcpServer w;
    w.show();

    return a.exec();
}
