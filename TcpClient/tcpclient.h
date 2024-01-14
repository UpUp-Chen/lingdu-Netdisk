//TcpClient:: connect to server, construct the socket, process the message from server

#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>


class TcpClient : public QWidget
{
    Q_OBJECT

public:
    TcpClient(QWidget *parent = nullptr);
    ~TcpClient();

    void loadConfig(); // read Ip/Port from the config file
    static TcpClient& instance();
    QTcpSocket& socket();
    static TcpClient &getInstance();
    QTcpSocket &getTcpSocket();


private slots:
    void recvMsg();    // receive response message from the server

private:
    QString IP;   // IP string
    quint16 port; // Port usigned short
public:
    QString userName; // user name

    QTcpSocket cliSocket; // Tcp socket, used to connect to the server and transmit data
};
#endif

