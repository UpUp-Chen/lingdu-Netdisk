#include "log.h"
#include "ui_log.h"
#include "protocol.h"
#include "tcpclient.h"
#include <QMessageBox>
#include "mainwidget.h"

Log::Log(QWidget *parent) : QWidget(parent), ui(new Ui::Log)
{
    ui->setupUi(this);
}

Log::~Log()
{
    delete ui;
}

Log& Log::instance()
{
    static Log logW;
    return logW;
}

// User Task 1: Register ---- MSGTYPE::Login
void Log::on_Register_Button_clicked()
{
    QString name = ui->UserName_lineEdit->text();
    QString pwd = ui->Password_lineEdit->text();

    // illeagl input
    if(name.isEmpty())
        QMessageBox::critical(this,"Register Error","UserName cannot be empty!");
    else if(pwd.isEmpty())
        QMessageBox::critical(this,"Register Error","Password cannot be empty!");\

    // send register message to server
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->type = REGISTER;
        strncpy(pdu->meta,name.toStdString().c_str(),32);
        strncpy(pdu->meta + 32,pwd.toStdString().c_str(),32);

        TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

        delete pdu;
    }

}

// User Task 2:  Login---- MSGTYPE::Login
void Log::on_Login_Button_clicked()
{
    QString name = ui->UserName_lineEdit->text();
    TcpClient::instance().userName = name; // Update the user name
    QString pwd = ui->Password_lineEdit->text();

    // illeagl input
    if(name.isEmpty())
        QMessageBox::critical(this,"Login Error","UserName cannot be empty!");
    else if(pwd.isEmpty())
        QMessageBox::critical(this,"Login Error","Password cannot be empty!");

    // send register message to server
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->type = LOGIN;
        strncpy(pdu->meta,name.toStdString().c_str(),32);
        strncpy(pdu->meta + 32,pwd.toStdString().c_str(),32);

        TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

        delete pdu;
    }
}

// User Task3: Log out
void Log::on_Logout_Button_clicked()
{
    QString name = ui->UserName_lineEdit->text();
    QString pwd = ui->Password_lineEdit->text();

    // illeagl input
    if(name.isEmpty())
        QMessageBox::critical(this,"Logout Error","UserName cannot be empty!");
    else if(pwd.isEmpty())
        QMessageBox::critical(this,"Logout Error","Password cannot be empty!");

    // send register message to server
    else
    {
        PDU *pdu = mkPDU(0);
        pdu->type = LOGOUT;
        strncpy(pdu->meta,name.toStdString().c_str(),32);
        strncpy(pdu->meta + 32,pwd.toStdString().c_str(),32);

        TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

        delete pdu;
    }
}



// Response Message Execute Function
void Log::recvMsg(PDU* pdu)
{

    // Distinguished the message type and Execute
    switch (pdu->type)
    {
    // User Task1: Register
    case REGISTER:
    {
        if(0 == strcmp(pdu->meta,REGISTER_SUCCESS))
            QMessageBox::information(this,"Redister",REGISTER_SUCCESS);
        else if(0 == strcmp(pdu->meta,REGISTER_FAIL))
            QMessageBox::warning(this,"Register",REGISTER_FAIL);

        break;
    }
    // User Task2: Login
    case LOGIN:
    {
        if(0 == strcmp(pdu->meta,LOGIN_SUCCESS))
        {
            QMessageBox::information(this,"Login",LOGIN_SUCCESS);
            MainWidget::instance().show();
            this->hide();
        }
        else if(0 == strcmp(pdu->meta,LOGIN_FAIL1))
            QMessageBox::warning(this,"Login","Password error!\nPlease check again!");
        else
            QMessageBox::warning(this,"Login","User not exist!\nPlease check again!");
        break;
    }
    // User Task3: Logout
    case LOGOUT:
    {
        if(0 == strcmp(pdu->meta,LOGOUT_SUCCESS))
            QMessageBox::information(this,"Login",LOGOUT_SUCCESS);
        else if(0 == strcmp(pdu->meta,LOGOUT_FAIL1))
            QMessageBox::warning(this,"Login","Password error!\nPlease check again!");
        else
            QMessageBox::warning(this,"Login","User not exist!\nPlease check again!");
        break;
    }
    default:
    {
        break;
    }
    }

}
