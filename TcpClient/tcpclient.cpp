#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include "protocol.h"
#include "mainwidget.h"
#include "tcpclient.h"
#include "log.h"

TcpClient::TcpClient(QWidget *parent) : QWidget(parent)
{
    loadConfig();

    // when receive the response message, execute
    connect(&cliSocket, SIGNAL(readyRead()),this,SLOT(recvMsg()));  //QTconnect:  signal-->slot
    cliSocket.connectToHost(QHostAddress(IP),port); // subjectively Connect to server
}

TcpClient::~TcpClient()
{
}

TcpClient& TcpClient::instance()
{
    static TcpClient tcpClient;
    return tcpClient;
}

QTcpSocket& TcpClient::socket()
{
    return cliSocket;
}

void TcpClient::loadConfig()
{
    QFile file(":/client.config");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray baData = file.readAll();
        QString strData = baData.toStdString().c_str();

        strData.replace("\r\n"," ");
        QStringList strList = strData.split(" ");
        IP = strList.at(0);
        port = strList.at(1).toUShort();
        qDebug() << "IP地址为：" << IP << "端口为：" << port;

        file.close();
    }
    else
    {
        QMessageBox::critical(this,"open config","open config failed");
    }
}



// Response Message Execute Function
void TcpClient::recvMsg()
{
    // Read the PDU
    uint len = 0;
    cliSocket.read((char*)&len,sizeof(uint));
    uint msgLen = len - sizeof(PDU);
    PDU *pdu = mkPDU(msgLen);
    cliSocket.read((char*)pdu+sizeof(uint),len-sizeof(uint));

    qDebug()<<"Client Received PDU:  "<<"len: "<<pdu->len<<"type: "<<pdu->type<<"dataLen: "<<pdu->dataLen<<"meta: "<<pdu->meta;

    // Distinguished the message type and Execute
    switch (pdu->type)
    {
    // User Task1: Register
    case REGISTER:
    {
        Log::instance().recvMsg(pdu);
        break;
    }
    // User Task2: Login
    case LOGIN:
    {
        Log::instance().recvMsg(pdu);
        break;
    }
    // User Task3: Logout
    case LOGOUT:
    {
        Log::instance().recvMsg(pdu);
        break;
    }
    // Folder Task:
    case REFRESH_FOLDER:
    {
        File::instance().recvMsg(pdu);
        break;
    }
    case ADD_FOLDER:
    {
        File::instance().recvMsg(pdu);
        break;
    }
    case DELETE_FOLDER:
    {
        File::instance().recvMsg(pdu);
        break;
    }
    case UP_FOLDER:
    {
        File::instance().recvMsg(pdu);
        break;
    }
    case UPLOAD:
    {
        File::instance().recvMsg(pdu);
        break;
    }


    default:
    {
        break;
    }
    }

}
// 返回TcpClient实例对象
TcpClient &TcpClient::getInstance()
{
    static TcpClient instance;
    return instance;
}

// 获取TcpSocket
QTcpSocket &TcpClient::getTcpSocket()
{
    return cliSocket;
}
