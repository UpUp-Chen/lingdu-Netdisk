#include "tcpserver.h"
#include "ui_tcpserver.h"
#include "mytcpserver.h"
#include <QByteArray>
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
#include <QFile>

TcpServer::TcpServer(QWidget *parent): QWidget(parent), ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    loadConfig();
    myTcpServer::instance().listen(QHostAddress(IP),port); // listen the connection from clients
}

TcpServer::~TcpServer()
{
    delete ui;
}

// load configration from file
void TcpServer::loadConfig()
{
    QFile file(":/server.config");
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
