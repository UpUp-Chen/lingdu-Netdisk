// The sub widget

#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QToolButton>
#include <QDir>
#include <QFileInfo>
#include <QDebug>
#include "protocol.h"
#include <QButtonGroup>
#include <QTimer>
namespace Ui {
class File;
}
class FolderIcon
{
public:
    QToolButton* button;
    QString name;
    QString ID;
public:
    FolderIcon(QString Name, QString ID);
    ~FolderIcon();
};

class File : public QWidget
{
    Q_OBJECT

public:
    explicit File(QWidget *parent = nullptr);
    ~File();
    static File& instance();

public:
    Ui::File *ui;
    QTimer clickTimer; // Monitor Double-Clicked Event
    QButtonGroup* buttonGroup; // group of icons
    QList<FolderIcon*> icons;
    QGridLayout* iconLayout;  // Lay out icon

    QString curDirect; // Current directory
    //to pass to UploadData() from Upload
    QTimer* timer;
    QString filePath;

private slots:
    void showFolder(QStringList nameList);
    void on_AddFolder_Button_clicked();
    void onFolderIconDoubleClicked(FolderIcon* icon);
    void onFolderIconChecked();
    void on_Delete_Button_clicked();
    void on_UP_BUTTON_clicked();
    void on_DOWN_BUTTON_clicked();
    void on_REFRESH_BUTTON_clicked();
    void on_Upload_Button_clicked();
    void UploadData();

public:
    void refresh();
    void recvMsg(PDU* pdu);
};

#endif // FILE_H
