#include "file.h"
#include "ui_file.h"
#include <QInputDialog>
#include <QMessageBox>
#include "tcpclient.h"
#include "protocol.h"
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>
#include <QFileDialog>
#include <QThread>

File::File(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::File)
{
    timer = new QTimer;

    ui->setupUi(this);

    ui->Download_Button->hide();
    ui->Share_Button->hide();
    ui->Delete_Button->hide();
    ui->UP_BUTTON->setEnabled(false);

    iconLayout = new QGridLayout(ui->widget);
    iconLayout->setRowStretch(5, 1);
    iconLayout->setColumnStretch(6, 1);

    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, QOverload<QAbstractButton*,bool>::of(&QButtonGroup::buttonToggled),
            this, &File::onFolderIconChecked);  // button checked change: update shown buttons in file widget
    connect(timer,SIGNAL(timeout()),this,SLOT(UploadData()));
    curDirect = "5";

}

File::~File()
{
    delete ui;
}

File& File::instance()
{
    static File file;
    return file;
}

FolderIcon::FolderIcon(QString Name, QString id)
{
    QToolButton* Tb = new QToolButton();
    Tb->setIcon(QIcon("D:\\QT\\Project\\NetDisk\\TcpClient\\folder.jpg"));

    Tb->setStyleSheet("border: none;");
    Tb->setIconSize(QSize(80, 80));
    Tb->setFixedSize(100, 100);
    Tb->raise();
    Tb->setText(Name);
    Tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    Tb->setCheckable(true);
    Tb->setStyleSheet("QToolButton { border: none; }"
                      "QToolButton:checked { background-color: lightblue; }");
    button = Tb;
    name = Name;
    ID = id;
}

FolderIcon::~FolderIcon()
{
    delete(button);
}

// UI updata

// Show the folder widget of current parent directory
void File::refresh()
{
    PDU *pdu = mkPDU(0);
    pdu->type = MSGTYPE::REFRESH_FOLDER;
    strncpy(pdu->meta, TcpClient::instance().userName.toStdString().c_str(), 32);
    strncpy(pdu->meta+32, curDirect.toStdString().c_str(), 32);

    qDebug()<<"Client Emitted PDU -- Refresh: "<< "len"<<pdu->len<<"datalen"<<pdu->dataLen<<"meta"<<pdu->meta;

    TcpClient::instance().socket().write((char*)pdu, pdu->len);

    delete pdu;
}

void File::showFolder(QStringList nameList)
{
    int ColumnSize = 4;

    // Delete previous button or label
    icons.clear();
    for (QAbstractButton *button : buttonGroup->buttons())
        buttonGroup->removeButton(button);
    while (iconLayout->count() > 0) {
        QLayoutItem *item = iconLayout->takeAt(0);

        if (item->widget()) {
            delete item->widget();
        }

        delete item;
    }

    // Add current button
    if(nameList.empty())
    {
        iconLayout->setRowStretch(0, 1);
        iconLayout->setColumnStretch(0, 1);

        QLabel* label = new QLabel(ui->widget);
        QFont font("Arial", 20);
        font.setItalic(true);
        label->setFont(font);
        label->setStyleSheet("color: red");
        label->setAlignment(Qt::AlignCenter);
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        label->setText("Empty Folder");

        iconLayout->addWidget(label);
        ui->DOWN_BUTTON->setEnabled(false);
    }

    else
    {
        iconLayout->setRowStretch(5, 1);
        iconLayout->setColumnStretch(6, 1);

        for (int i = 0; i < nameList.size(); ++i)
        {
            // nameList: [name + "//" + ID,...]
            QString nameUnit = nameList.at(i);
            QStringList nameUnitList = nameUnit.split("//");
            QString name = nameUnitList.at(0);
            QString ID = nameUnitList.at(1);

            FolderIcon* icon= new FolderIcon(name, ID);
            buttonGroup->addButton(icon->button);
            icons.append(icon);

            int row = i/ColumnSize;
            int column = i%ColumnSize;

            iconLayout->addWidget(icon->button, row, column);

            // connect(icon->button, &QToolButton::clicked, this, [=]() {
            //     onFolderIconClicked(icon);
            // }); // Folder ICon click() --- onFolderIconClicked(icon)
            connect(icon->button, &QToolButton::clicked, this, [=]() {
                if (clickTimer.isActive() && clickTimer.remainingTime() < QApplication::doubleClickInterval()) {
                    clickTimer.stop();
                    onFolderIconDoubleClicked(icon);
                } else {

                    clickTimer.start();
                }
            }); // Folder Icon DoubleClick -- on FolderIconDoubleClicked
        }
    }
    ui->widget->update();
}


// Update shown buttons
void File::onFolderIconChecked()
{
    int checkedCount = buttonGroup->checkedButton() ? 1 : 0;
    if (checkedCount)
    {
        ui->Upload_Button->hide();
        ui->AddFolder_Button->hide();

        ui->Download_Button->show();
        ui->Share_Button->show();
        ui->Delete_Button->show();
    }
    else
    {
        ui->Upload_Button->show();
        ui->AddFolder_Button->show();

        ui->Download_Button->hide();
        ui->Share_Button->hide();
        ui->Delete_Button->hide();
    }
}

// CD the target directory
void File::onFolderIconDoubleClicked(FolderIcon* icon)
{
    qDebug()<<"Double Clicked";
    ui->UP_BUTTON->setEnabled(true);
    curDirect = icon->ID;
    refresh();
}



// Response Message Execute Function
void File::recvMsg(PDU* pdu)
{
    // Distinguished the message type and Execute
    switch (pdu->type)
    {
    // Folder Task1: Add folder
    case ADD_FOLDER:
    {
        if(0 == strcmp(pdu->meta,ADD_FOLDER_SUCCESS))
        {
            QMessageBox::information(this,"Add Folder",ADD_FOLDER_SUCCESS);
            refresh();
        }
        else if(0 == strcmp(pdu->meta,ADD_FOLDER_FAIL))
            QMessageBox::warning(this,"Add Folder","Folder has already existed!");

        break;
    }
    case REFRESH_FOLDER:
    {
        QStringList nameList; // list: name'//'nameID
        if(pdu->dataLen == 1)   // dataLen = combinedString.size() + 1
        {
            nameList= QStringList();
        }
        else
        {
            // char[] to QstringList
            const char* charData = pdu->data;
            QString combinedString = QString::fromUtf8(charData);
            nameList = combinedString.split('#');
        }
        qDebug() << "NameList:" <<nameList;
        showFolder(nameList);

        break;
    }
    case DELETE_FOLDER:
    {
        if(0 == strcmp(pdu->meta,DELETE_FOLDER_SUCCESS))
        {
            QMessageBox::information(this,"Delete Folder",DELETE_FOLDER_SUCCESS);
            refresh();
        }
        else if(0 == strcmp(pdu->meta,DELETE_FOLDER_FAIL))
            QMessageBox::warning(this,"Delete Folder","Unexpected Error!");

        break;
    }
    case UP_FOLDER:
    {
        curDirect = QString::fromUtf8(pdu->meta);
        qDebug()<<"UP directory: "<<curDirect;

        if(curDirect == "root")
             ui->UP_BUTTON->setEnabled(false);
        ui->DOWN_BUTTON->setEnabled(true);

        refresh();
        break;
    }
    case UPLOAD:
    {
        if(0 == strcmp(pdu->meta,ADD_FILE_SUCCESS))
        {
            QMessageBox::information(this,"Upload", "Upload File Success!");
            refresh();
        }
        else if(0 == strcmp(pdu->meta,ADD_FILE_FAIL1))
        {
            QMessageBox::warning(this,"Upload", "File has already existed!");
        }
        else if(0 == strcmp(pdu->meta,ADD_FILE_FAIL2))
        {
            QMessageBox::warning(this,"Upload", "Read file error!");
        }
        break;
    }
    default:
    {
        break;
    }
    }

}



// Folder Task
// Add folder
void File::on_AddFolder_Button_clicked()
{
    QString name = QInputDialog::getText(this,"Add folder","Folder name");

    if(!name.isEmpty())
    {
        if(name.size() > 32)
        {
            QMessageBox::warning(this,"Add Folder","Folder name cannot exceed 32 characters!");
        }
        else if(name.contains('#'))
        {
            QMessageBox::warning(this,"Add Folder","Folder name cannot contain '#'!");
        }
        else
        {
            QString userName = TcpClient::instance().userName;

            // Meta: username+current Direct   Data: name
            PDU *pdu = mkPDU(name.size()+1);
            pdu->type = MSGTYPE::ADD_FOLDER;
            strncpy(pdu->meta,userName.toStdString().c_str(),32);
            strncpy(pdu->meta + 32,curDirect.toStdString().c_str(),32);
            memcpy(pdu->data,name.toStdString().c_str(),name.size());

            TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

            delete pdu;
        }
    }
    else
    {
        QMessageBox::warning(this,"Add Folder","Foler name cannot be null!");
    }


}

// Delete folder
void File::on_Delete_Button_clicked()
{
    qDebug()<<"here";
    // Get all selected button
    QList<FolderIcon*> checkedIcons;
    QStringList IDs;
    for (FolderIcon* icon : icons)
    {
        if (icon->button->isChecked())
            checkedIcons.append(icon);
    }
    qDebug()<<"here";

    // Delete
    for (FolderIcon* icon: checkedIcons)
        IDs.append(icon->ID);
    QString combinedString = IDs.join("#");

    PDU *pdu = mkPDU(combinedString.size()+1);
    pdu->type = MSGTYPE::DELETE_FOLDER;
    strncpy(pdu->meta,TcpClient::instance().userName.toStdString().c_str(),32);
    strncpy(pdu->meta + 32,curDirect.toStdString().c_str(),32);
    memcpy(pdu->data,combinedString.toStdString().c_str(),combinedString.size());

    TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

    delete pdu;
}



void File::on_UP_BUTTON_clicked()
{
    qDebug()<<"UP folder";
    PDU *pdu = mkPDU(0);
    pdu->type = MSGTYPE::UP_FOLDER;
    strncpy(pdu->meta,TcpClient::instance().userName.toStdString().c_str(),32);
    strncpy(pdu->meta + 32,curDirect.toStdString().c_str(),32);

    TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

    delete pdu;
}


void File::on_DOWN_BUTTON_clicked()
{
    FolderIcon* icon = icons.at(0);
    curDirect = icon->ID;
    ui->UP_BUTTON->setEnabled(true);
    refresh();
}


void File::on_REFRESH_BUTTON_clicked()
{
    refresh();
}


void File::on_Upload_Button_clicked()
{
    filePath = QFileDialog::getOpenFileName();
    if(!filePath.isEmpty())
    {
        int index = filePath.lastIndexOf('/');
        QString fileName = filePath.mid(index + 1);

        QFile file(filePath);
        qint64 fileSize = file.size();

        PDU *pdu = mkPDU(21); // long long int
        // Write meta data, username, parent directory, file name
        pdu->type = UPLOAD;
        strncpy(pdu->meta,TcpClient::instance().userName.toStdString().c_str(), 32);
        strncpy(pdu->meta+32, curDirect.toStdString().c_str(), 32);
        strncpy(pdu->meta+64, fileName.toStdString().c_str(), 32);
        sprintf(pdu->data,"%lld",fileSize);
        if (!file.open(QIODevice::ReadOnly| QIODevice::Unbuffered))
        {
            QMessageBox::warning(this, "UPLOAD", "Cannot open the file!");
            return;
        }
        file.close();

        TcpClient::instance().cliSocket.write((char*)pdu, pdu->len);

        qDebug()<<"File Size: "<<fileSize;

        delete pdu;
        timer->start(1000); // Connect to UploadData()

    }
    else
    {
        QMessageBox::warning(this,"UPLOAD","File connot be null!");
    }
}

void File::UploadData()
{
    timer->stop();
    qDebug()<<"here";

    char *buffer = new char[4096];
    int readSize=0;
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,"UpLoad","Cannot open the file!");
        return;
    }

    while(true)
    {
        readSize = file.read(buffer,4096);
        qDebug()<<"Send FileData: "<<readSize;

        if(readSize>0&&readSize<=4096)
            TcpClient::instance().cliSocket.write(buffer, readSize);

        else if(readSize==0)
            break;

        else
        {
            QMessageBox::warning(this,"UPLOAD","Read file error!");
            break;
        }

    }

    delete[] buffer;

}
