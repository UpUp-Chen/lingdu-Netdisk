/********************************************************************************
** Form generated from reading UI file 'online.ui'
**
** Created by: Qt User Interface Compiler version 6.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ONLINE_H
#define UI_ONLINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Online
{
public:
    QListWidget *listWidget;
    QPushButton *pushButton;

    void setupUi(QWidget *Online)
    {
        if (Online->objectName().isEmpty())
            Online->setObjectName("Online");
        Online->resize(481, 388);
        listWidget = new QListWidget(Online);
        listWidget->setObjectName("listWidget");
        listWidget->setGeometry(QRect(30, 20, 281, 341));
        pushButton = new QPushButton(Online);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(350, 170, 91, 41));

        retranslateUi(Online);

        QMetaObject::connectSlotsByName(Online);
    } // setupUi

    void retranslateUi(QWidget *Online)
    {
        Online->setWindowTitle(QCoreApplication::translate("Online", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("Online", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Online: public Ui_Online {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ONLINE_H
