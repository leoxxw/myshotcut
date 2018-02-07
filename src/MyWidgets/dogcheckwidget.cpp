#include "dogcheckwidget.h"
#include "ui_dogcheckwidget.h"
#include <QMessageBox>
#include <QDebug>
DogCheckWidget::DogCheckWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DogCheckWidget),
    m_pro(NULL)
{
    ui->setupUi(this);
    this->Dogcheck();
}

DogCheckWidget::~DogCheckWidget()
{
    delete ui;
}

bool DogCheckWidget::Dogcheck()
{
    if(m_pro == NULL)
    {
        m_pro = new QProcess(this);
    }
    connect(m_pro, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
    connect(m_pro, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError(QProcess::ProcessError)));
    connect(m_pro, SIGNAL(readyRead()), this, SLOT(readFromClient()));
    m_pro->start("DogCheck.exe");
}

void DogCheckWidget::readFromClient()
{
    qDebug()<<"readFromClient";
    if(!m_pro)
    {
        return;
    }
    QByteArray output = m_pro->readAllStandardOutput();
     m_pro->close();
    if(output == "true")
    {

    }
    else
    {

        QMessageBox dialog(QMessageBox::Warning,
                           "提示",
                           QStringLiteral("\r\n请不要非法使用软件!\r\n"),
                           QMessageBox::Ok,
                           NULL);
        dialog.setButtonText (QMessageBox::Ok,QString("确定"));
        dialog.exec();
    }
}
