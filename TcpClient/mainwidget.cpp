#include "mainwidget.h"
#include "ui_mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    ui->stackedWidget->addWidget(&(File::instance()));
    ui->stackedWidget->addWidget(&(Friend::instance()));

}


MainWidget::~MainWidget()
{
    delete ui;
}
MainWidget& MainWidget::instance()
{
    static MainWidget mw;
    return mw;
}

void MainWidget::on_Friend_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(&(Friend::instance()));
}

void MainWidget::on_File_Button_clicked()
{
    ui->stackedWidget->setCurrentWidget(&(File::instance()));
    File::instance().refresh();//Show root directory
}

