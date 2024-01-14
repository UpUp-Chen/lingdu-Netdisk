#include "log.h"
#include "friend.h"
#include <QApplication>
#include "mainwidget.h"
#include "tcpClient.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    TcpClient::instance();

    // Log::instance().show();
    TcpClient::instance().userName = "pwenss";
    MainWidget::instance().show();

    return a.exec();
}
