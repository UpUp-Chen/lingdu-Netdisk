#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QList>
#include "srvSocket.h"

class myTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    myTcpServer();

    // singleton pattern
    static myTcpServer &instance();

    // overwrite to show new connection
    void incomingConnection(qintptr handle);
private:
    QList<srvSocket*> socketList;
};

#endif
