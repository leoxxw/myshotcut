#include "aboutwidget.h"
#include "ui_aboutwidget.h"
#include<QDesktopServices>

AboutWidget::AboutWidget(QWidget *parent) :
    BaseMainWidget(parent),
    ui(new Ui::AboutWidget)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    ui->label_link->setText("<a href =http://www.hzlh.com>http://www.hzlh.com</a>");
    ui->label_link->show();
    connect(ui->label_link,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
}

AboutWidget::~AboutWidget()
{
    delete ui;
}

void AboutWidget::on_pushButton_close_clicked()
{
    this->hide();
}

void AboutWidget::on_pushButton_clicked()
{
    this->hide();
}

void AboutWidget::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}
