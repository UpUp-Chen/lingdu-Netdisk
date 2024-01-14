#include "srvSocket.h"
#include <QDebug>
#include <QFile>
#include <QDataStream>

bool readFile=false;
QFile file;
qint64 totalSize;
qint64 readSize;

srvSocket::srvSocket()
{
    connect(this,SIGNAL(readyRead()),this,SLOT(recvMsg()));  // QTcpSocket::teadyRead() --> srvSocket::recvMsg()
}


// Receive the message form client
void srvSocket::recvMsg()
{
    if(!readFile)
    {
        // Read the PDU
        uint len = 0;
        read((char*)&len,sizeof(uint));
        uint msgLen = len - sizeof(PDU);
        PDU *pdu = mkPDU(msgLen);
        read((char*)pdu+sizeof(uint),len-sizeof(uint));

        qDebug()<<"Server Received PDU: "<<"len"<<pdu->len<<"type"<<pdu->type<<"dataLen"<<pdu->dataLen<<"meta"<<pdu->meta;

        switch (pdu->type)
        {
        // User Task1: Resgister
        case REGISTER:
        {
            // Read data
            char name[32] = {'\0'};
            char pwd[32] = {'\0'};
            strncpy(name,pdu->meta,32);
            strncpy(pwd,pdu->meta+32,32);

            // Process
            QString msg = DataBase::instance().Register(name,pwd);

            // Send response message to target client
            PDU *respdu = mkPDU(0);
            respdu->type = REGISTER;
            strcpy(respdu->meta,msg.toStdString().c_str());

            write((char*)respdu, respdu->len);

            free(respdu);
            break;
        }
        // User Task2: Login
        case LOGIN:
        {

            // Read data
            char name[32] = {'\0'};
            char pwd[32] = {'\0'};
            strncpy(name,pdu->meta,32);
            strncpy(pwd,pdu->meta+32,32);

            // Process
            QString msg = DataBase::instance().Login(name,pwd);

            // Send response message to target client
            PDU *respdu = mkPDU(0);
            respdu->type = LOGIN;
            strcpy(respdu->meta,msg.toStdString().c_str());

            write((char*)respdu, respdu->len);

            delete (respdu);
            break;
        }
        case LOGOUT:
        {

            // Read data
            char name[32] = {'\0'};
            char pwd[32] = {'\0'};
            strncpy(name,pdu->meta,32);
            strncpy(pwd,pdu->meta+32,32);

            // Process
            QString msg = DataBase::instance().Logout(name,pwd);

            // Send response message to target client
            PDU *respdu = mkPDU(0);
            respdu->type = LOGOUT;
            strcpy(respdu->meta,msg.toStdString().c_str());

            write((char*)respdu, respdu->len);

            delete (respdu);
            break;
        }

        // Folder task:
        case REFRESH_FOLDER:
        {

            // Read data
            char userName[32] = {'\0'};
            char parent[32] = {'\0'};
            strncpy(userName,pdu->meta, 32);
            strncpy(parent,pdu->meta+32, 32);

            // Process
            QStringList nameUnitList = DataBase::instance().RefreshFolder(userName, parent);
            qDebug() << "Folder List:" <<nameUnitList;

            // Send response message to target client
            QString combinedString = nameUnitList.join('#'); // We use # to split the different string

            PDU *respdu = mkPDU(combinedString.size()+1);
            respdu->type = REFRESH_FOLDER;
            memcpy(respdu->data,combinedString.toStdString().c_str(),combinedString.size());

            qDebug()<<"Server Emitted PDU--Refresh:  "<<"len: "<<respdu->len<<"type: "<<respdu->type<<"dataLen: "<<respdu->dataLen<<"meta: "<<respdu->meta;

            write((char*)respdu, respdu->len);

            delete (respdu);
            break;
        }

        // Folder task:
        case ADD_FOLDER:
        {

            // Read data
            char userName[32] = {'\0'};
            char parent[32] = {'\0'};
            strncpy(userName,pdu->meta,32);
            strncpy(parent,pdu->meta+32,32);
            char folderName[32] = {'\0'};
            strncpy(folderName,pdu->data,pdu->dataLen);
            qDebug()<<"Folder name in server:" << folderName;

            // Process
            QString msg = DataBase::instance().AddFolder(userName,parent,folderName);

            // Send response message to target client
            PDU *respdu = mkPDU(0);
            respdu->type = ADD_FOLDER;
            strcpy(respdu->meta,msg.toStdString().c_str());

            write((char*)respdu, respdu->len);

            delete (respdu);
            break;
        }

        case DELETE_FOLDER:
        {

            // Read data
            char userName[32] = {'\0'};
            char parent[32] = {'\0'};
            strncpy(userName,pdu->meta,32);
            strncpy(parent,pdu->meta+32,32);
            const char* charData = pdu->data;
            QString combinedString(charData);
            QStringList idList = combinedString.split('#');

            // Process
            QString msg = DataBase::instance().DeleteFolder(userName,parent,idList);

            // Send response message to target client
            PDU *respdu = mkPDU(0);
            respdu->type = DELETE_FOLDER;
            strcpy(respdu->meta,msg.toStdString().c_str());

            write((char*)respdu, respdu->len);

            delete respdu;
            break;
        }

        case UP_FOLDER:
        {
            // Read data
            char userName[32] = {'\0'};
            char ID[32] = {'\0'};
            strncpy(userName,pdu->meta,32);
            strncpy(ID,pdu->meta+32,32);

            // Process
            QString parent = DataBase::instance().UPFolder(userName,ID);
            qDebug()<<"Parent: "<<parent;

            // Send response message to target client
            PDU *respdu = mkPDU(0);
            respdu->type = UP_FOLDER;
            strncpy(respdu->meta, parent.toStdString().c_str(), 32);

            write((char*)respdu, respdu->len);

            delete respdu;
            break;
        }

        case UPLOAD:
        {
            // Read data
            char userName[32] = {'\0'};
            char parent[32] = {'\0'};
            char fileName[32] = {'\0'};
            strncpy(userName,pdu->meta, 32);
            strncpy(parent,pdu->meta+32, 32);
            strncpy(fileName,pdu->meta+64, 32);
            sscanf(pdu->data,"%lld",&totalSize);

            QString filePath = "D:\\QT\\Project\\NetDisk\\Data\\"+QString(userName)+'\\'+QString(parent)+'#'+QString(fileName);

            // Process
            QString msg = DataBase::instance().AddFile(userName,parent,fileName);

            if (msg==ADD_FILE_SUCCESS)
            {
                file.setFileName(filePath);

                if(file.open(QIODevice::WriteOnly))
                {
                    readFile = true;
                    readSize = 0;
                }
                qDebug()<<"IsReadFile: "<<readFile<<"TotalSize: "<<totalSize;
            }
            else
            {
                // Error 1: File have already existed!
                PDU* respdu = mkPDU(0);
                respdu->type = UPLOAD;
                strcpy(respdu->meta,ADD_FILE_FAIL1);

                write((char*)respdu,respdu->len);

                delete(respdu);
                break;
            }
        }

        default:
        {
            break;
        }
        }
        delete pdu;
    }
    else
    {
        // The tcp cannot send all the filedata once, so we need to receive it multipul times, and each send isn't the type of PDU!
        qDebug()<<"Begin Read";

        // Read all send data
        QByteArray buff = readAll();
        file.write(buff);
        readSize += buff.size();
        qDebug()<<"Received the file : Currrent Received: "<<readSize<<"  TotalSize: "<<totalSize;

        if(readSize == totalSize)
        {
            file.close();
            readFile = false;

            PDU *respdu = mkPDU(0);
            respdu->type = UPLOAD;
            strcpy(respdu->meta,ADD_FILE_SUCCESS);

            write((char*)respdu,respdu->len);

            delete(respdu);
        }
        else if(readSize > totalSize)
        {
            // Error 2: ReadSize more than totalSize

            file.close();
            readFile = false;
            PDU *respdu = NULL;
            respdu = mkPDU(0);
            respdu->type = UPLOAD;
            strcpy(respdu->meta,ADD_FILE_FAIL2);

            write((char*)respdu,respdu->len);

            delete(respdu);
        }

    }

}
