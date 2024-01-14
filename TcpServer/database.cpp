#include "DataBase.h"
#include <QMessageBox>
#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QMYSQL");
}

// single instance
DataBase &DataBase::instance()
{
    static DataBase instance;
    return instance;
}

// Connect to my database
void DataBase::init()
{
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("netdisk");
    db.setUserName("root");
    db.setPassword("@dhsieHs54/");
    if(db.open())
    {
        QSqlQuery query;
        query.exec("select * from account");
        while(query.next())
        {
            QString data = QString("%1,%2,%3").arg(query.value(0).toString()).arg(query.value(1).toString()).arg(query.value(2).toString());
            qDebug() << data;
        }
    }
    else
    {
        QMessageBox::critical(NULL,"Error","Failed to open the database!");
    }
}


// destructor to close the database
DataBase::~DataBase()
{
    db.close();
}


// Execute function
// Here we let the database function return the message QString


// User task1: Register
// Insert the account to the database
QString DataBase::Register(const char *name, const char *pwd)
{
    QString sql = QString("insert into account(username,password) values(\'%1\',\'%2\')").arg(name).arg(pwd);
    QSqlQuery query;
    if (query.exec(sql))
        return REGISTER_SUCCESS;
    else
        return REGISTER_FAIL;
}

// User task2: Login
// Query the record
QString DataBase::Login(const char *name, const char *pwd)
{
    QString sql = QString("select * from account where username = \'%1\'").arg(name);
    QSqlQuery query;
    query.exec(sql);

    if(query.next())
    {
        QString sql = QString("select * from account where username = \'%1\' and password = \'%2\'").arg(name).arg(pwd);
        query.exec(sql);
        if(query.next())
            return LOGIN_SUCCESS;
        else
            return LOGIN_FAIL1;
    }
    else
        return LOGIN_FAIL2;

}

// User task3: Logout
// Query the record
QString DataBase::Logout(const char *name, const char *pwd)
{
    QString sql = QString("select * from account where username = \'%1\'").arg(name);
    QSqlQuery query;
    query.exec(sql);

    if(query.next())
    {
        QString sql = QString("select * from account where username = \'%1\' and password = \'%2\'").arg(name).arg(pwd);
        query.exec(sql);
        if(query.next())
        {
            QString sql = QString("delete from account where username = \'%1\' and password = \'%2\'").arg(name).arg(pwd);
            query.exec(sql);
            return LOGOUT_SUCCESS;
        }
        else
            return LOGOUT_FAIL1;
    }
    else
        return LOGOUT_FAIL2;

}
// Folder task1: Query Folder
// Query the record
QStringList DataBase::RefreshFolder(const char *userName, const char *parent)
{
    QStringList strList;
    QString sql = QString("SELECT name,ID FROM folder WHERE user = '%1' AND parent = '%2'").arg(userName).arg(parent);

    QSqlQuery query;
    query.exec(sql);
    qDebug() << "SQL--Refresh: "<<sql;
    while (query.next())
    {
        // unit: name "//" ID
        QString unit = query.value("name").toString() + "//" + query.value("ID").toString();
        strList.append(unit);
    }

    return strList;
}

// Folder task2: ADD Folder / File
// Insert the record
QString DataBase::AddFolder(const char *userName, const char *parent, const char *folderName)
{
    QString sql = QString("INSERT INTO folder (user, parent, name) VALUES (\'%1\', \'%2\', \'%3\')").arg(userName).arg(parent).arg(folderName);
    QSqlQuery query;
    qDebug() << "SQL: "<<sql;

    if(query.exec(sql))
        return ADD_FOLDER_SUCCESS;
    else
        return ADD_FOLDER_FAIL;

}

QString DataBase::AddFile(const char *userName, const char *parent, const char *fileName)
{
    char* format = std::strrchr(fileName, '.') + 1;
    QString sql = QString("INSERT INTO file (user, parent, name, format) VALUES (\'%1\', \'%2\', \'%3\', \'%4\')").arg(userName).arg(parent).arg(fileName).arg(format);
    QSqlQuery query;
    qDebug() << "SQL: "<<sql;

    if(query.exec(sql))
        return ADD_FILE_SUCCESS;
    else
        return ADD_FILE_FAIL1;

}


// Folder task3: DELETE Folder
// Delete the record
QString DataBase::DeleteFolder(const char *userName, const char *parent, QStringList idList)
{

    QString sql;
    for (QString id : idList)
        sql += QString("DELETE FROM folder WHERE user = \'%1\' AND parent = \'%2\' AND ID = \'%3\' ;").arg(userName).arg(parent).arg(id);

    QSqlQuery query;
    qDebug() << "SQL: "<<sql;

    if(query.exec(sql))
        return DELETE_FOLDER_SUCCESS;
    else
        return DELETE_FOLDER_FAIL;

}

// Folder task4: UP Folder
// SELECT the record
QString DataBase::UPFolder(const char *userName, const char *ID)
{

    QString sql = QString("SELECT parent FROM folder WHERE user = \'%1\' AND  ID = \'%2\'").arg(userName).arg(ID);
    qDebug() << "SQL: "<<sql;
    QSqlQuery query;
    query.exec(sql);
    query.next();

    return query.value("parent").toString();
}
