#ifndef LOG_H
#define LOG_H

#include <QWidget>
#include <QFile>
#include <QTcpSocket>
#include "protocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Log; }
QT_END_NAMESPACE

class Log : public QWidget
{
    Q_OBJECT

public:
    Log(QWidget *parent = nullptr);
    ~Log();
    static Log& instance();
private slots:

    void on_Register_Button_clicked();
    void on_Login_Button_clicked();
    void on_Logout_Button_clicked();

private:
    Ui::Log *ui;
public:
    void recvMsg(PDU* pdu);
};
#endif
