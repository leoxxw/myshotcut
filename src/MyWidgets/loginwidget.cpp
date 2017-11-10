#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "CallDLL/callunifyloginsrv.h"
#include <QDebug>
#include "basemainwidget.h"
LoginWidget::LoginWidget(QWidget *parent) :
    BaseMainWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::on_pushButton_login_clicked()
{
    QString strSysID("00000000-0000-0000-0000-000000000003");
    QString strJoincode("75c6c9bb17432454edf676f233800bb2");
    QString strUrl("http://192.1.2.202:3000/ConfigCenter/ConfigurationCenterService.asmx");
    //初始化
    CallUnifyLoginSrv::instance()->Init(strSysID,strJoincode,strUrl);
    //登录有界面
    QString strMoudle("VideoStudio");
    wchar_t pszUserToken[256];
    memset(pszUserToken,0,256 * sizeof(wchar_t));
    int nSize =256;
    wchar_t pUserID[256];
    memset(pUserID,0,256 * sizeof(wchar_t));
    int nIDSize =256;
    int ret = CallUnifyLoginSrv::instance()->LoginV2(strMoudle,pszUserToken,&nSize,pUserID,&nIDSize);
    if(ret != 1)
    {
        qDebug() << ret <<"登录失败";
        wchar_t pError[256];
        memset(pError,0,256 * sizeof(wchar_t));
        int nSize =256;
        ret = CallUnifyLoginSrv::instance()->GetLastError(pError,&nSize);
        qDebug() << pError << nSize;

    }
    ui->pushButton_login->setEnabled(false);
    ui->pushButton_exit->setEnabled(true);
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
    ui->pushButton_send->setEnabled(true);
    ui->pushButton_save0->setEnabled(true);
}

void LoginWidget::on_pushButton_close_clicked()
{
    this->close();
}

void LoginWidget::on_pushButton_exit_clicked()
{
    //调用注销接口
    int ret = CallUnifyLoginSrv::instance()->Logout();
    if(ret == 1)
    {
        ui->pushButton_login->setEnabled(true);
        ui->pushButton_exit->setEnabled(false);
        ui->pushButton_open->setEnabled(false);
        ui->pushButton_save->setEnabled(false);
        ui->pushButton_send->setEnabled(false);
        ui->pushButton_save0->setEnabled(false);
    }
}
