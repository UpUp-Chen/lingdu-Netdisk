// The main widget

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include "file.h"
#include "friend.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();
    static MainWidget& instance();

private slots:
    // switch between subWidgets
    void on_Friend_Button_clicked();
    void on_File_Button_clicked();

private:
    Ui::MainWidget *ui;

};

#endif // MAINWIDGET_H
