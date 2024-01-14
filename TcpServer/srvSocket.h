#ifndef SRVSOCKET_H
#define SRVSOCKET_H
#include <QTcpSocket>
#include "protocol.h"
#include "Database.h"

class srvSocket : public QTcpSocket
{
    Q_OBJECT;
public:
    srvSocket();
public slots:
    void recvMsg();
};

#endif
