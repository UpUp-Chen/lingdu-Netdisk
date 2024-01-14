// The sub widget

#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "log.h"
namespace Ui {
class Friend;
}

class Friend : public QWidget
{
    Q_OBJECT
public slots:
    void showOnline();

private:
    QTextEdit *m_pShowMsgTE;
    QListWidget *m_pFriendListWidget;
    QLineEdit *m_pInputMsgLE;
    QPushButton *m_pDelFriendPB;
    QPushButton *m_pFlushFriendPB;
    QPushButton *m_pShowOnlineUsrPB;
    QPushButton *m_pSearchUsrPB;
    QPushButton *m_pMsgSendPB;
    QPushButton *m_pPrivateChatPB;
    Log *m_pOnline;
public:
    QString m_strSearchName;

public:
    explicit Friend(QWidget *parent = nullptr);
    ~Friend();
    static Friend& instance();

private slots:
    void on_online_Button_clicked();
    void searchUsr();

private:
    Ui::Friend *ui;
};

#endif // FRIEND_H
