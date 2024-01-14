#include "mytcpserver.h"
#include <QDebug>

myTcpServer::myTcpServer()
{

}

// Sigle instance: Only one instance exist and can be accessed
myTcpServer &myTcpServer::instance()
{
    static myTcpServer instance;
    return instance;
}


// Overwrite: Process when connection is constructed
void myTcpServer::incomingConnection(qintptr handle)
{
    qDebug() << "new client connected";

    srvSocket *socket = new srvSocket;
    socket->setSocketDescriptor(handle);
    socketList.append(socket);

}
