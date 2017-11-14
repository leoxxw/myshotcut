#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "CallDLL/callunifyloginsrv.h"
#include <QDebug>
#include "basemainwidget.h"
#include <QDomNodeList>
#include "settings.h"
#include <QMessageBox>
LoginWidget::LoginWidget(QWidget *parent) :
    BaseMainWidget(parent),
    ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
   // this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    ui->pushButton_login->setToolTip("登陆");
    ui->pushButton_exit->setToolTip("注销");
    ui->pushButton_open->setToolTip("打开工程");
    ui->pushButton_send->setToolTip("发送");
    ui->pushButton_save->setToolTip("保存");
    ui->pushButton_saveoth->setToolTip("另存为");
    ui->pushButton_openvideo->setToolTip("打开视频");
    ui->pushButton_savevideo->setToolTip("保存视频");
    ui->pushButton_sendvideo->setToolTip("发送视频");
    m_strSysID = ("00000000-0000-0000-0000-000000000003");
    m_strJoincode = ("75c6c9bb17432454edf676f233800bb2");
    m_strUrl = ("http://192.1.2.202:3000/ConfigCenter/ConfigurationCenterService.asmx");
    Init();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::Init()
{
    //初始化
    CallUnifyLoginSrv::instance()->Init(m_strSysID,m_strJoincode,m_strUrl);

    //设置日志类型
    CallUnifyLoginSrv::instance()->SetLogType(ELT_ALL);
    //自动登录
    QString strMoudle("VideoStudio");
    wchar_t pszUserToken[256];
    memset(pszUserToken,0,256 * sizeof(wchar_t));
    int nSize =256;
    wchar_t pUserID[256];
    memset(pUserID,0,256 * sizeof(wchar_t));
    int nIDSize =256;
    int ret = CallUnifyLoginSrv::instance()->AvoidLoginV2(strMoudle,pszUserToken,&nSize,pUserID,&nIDSize);
    if(ret == ERT_TRUE)
    {
        m_loginType = ERT_TRUE;
        qDebug() << "自动登录成功";
        wchar_t pszTicket[256];
        memset(pszTicket,0,256 * sizeof(wchar_t));
        int nsize =256;
        ret = CallUnifyLoginSrv::instance()->GetTicket(pszTicket,&nsize);
        if(ret == ERT_TRUE)
        {
            qDebug() << QStringLiteral("GetTicket succeed");
            m_Ticket = QString::fromWCharArray(pszTicket);
        }
        SetLoginSucceed(pUserID,pszUserToken);
    }
    if(ret == ERT_NewTGT)
    {
        m_loginType = ERT_NewTGT;//需要注销
        qDebug() << "自动登录成功";
        wchar_t pszTicket[256];
        memset(pszTicket,0,256 * sizeof(wchar_t));
        int nsize =256;
        ret = CallUnifyLoginSrv::instance()->GetTicket(pszTicket,&nsize);
        if(ret == ERT_TRUE)
        {
            qDebug() << QStringLiteral("GetTicket succeed");
            m_Ticket = QString::fromWCharArray(pszTicket);
        }
        SetLoginSucceed(pUserID,pszUserToken);
    }

}

void LoginWidget::SetLoginSucceed(wchar_t *pUserID, wchar_t *pszUserToken)
{
    m_userID= QString::fromWCharArray(pUserID);
    m_UserToken= QString::fromWCharArray(pszUserToken);
    ui->pushButton_login->setEnabled(false);
    ui->pushButton_exit->setEnabled(true);
    ui->pushButton_open->setEnabled(true);
    ui->pushButton_save->setEnabled(true);
    ui->pushButton_send->setEnabled(true);
    ui->pushButton_saveoth->setEnabled(true);
    //初始化CloudDiskInterface
    CloudDiskInterface::instance()->Init(m_strSysID,m_strUrl,m_userID,m_strJoincode);
    CloudDiskInterface::instance()->SetLogType(ELT_ALL);
    CloudDiskInterface::instance()->SetToken(m_Ticket,m_UserToken);
}

bool LoginWidget::GetListInfo(QString strBuffer)
{
    QDomDocument doc;
    doc.setContent(strBuffer);
    QDomNode node = doc.firstChild();
    while (!node.isNull())
    {
        QDomElement element = node.toElement(); // try to convert the node to an element.
        if(!element.isNull())
        {
            qDebug()<<element.tagName() << ":" << element.text();
            QDomNode nodeson = element.firstChild();
            while(!nodeson.isNull())
            {
                QDomElement elementson = nodeson.toElement();
                if(!elementson.isNull())
                {
                    qDebug()<< "---" <<elementson.tagName();
                    QDomNode n = elementson.firstChild();
                    while(!n.isNull())
                    {
                        QDomElement e = n.toElement();
                        if(!e.isNull())
                        {
                            qDebug()<< "---" <<e.tagName();
                            if (e.tagName() == "GUID")
                            {
                                m_strResourceID = e.text();
                            }
                            if (e.tagName() =="ParentID")
                            {
                                m_strParentID = e.text();
                            }
                            if (e.tagName() =="Name")
                            {
                                m_strResourceName = e.text();
                            }
                            if (e.tagName() =="Owner")
                            {
                                m_strOwnerID = e.text();
                            }
                            if (e.tagName() =="OwnerType")
                            {
                                m_strOwnerType = e.text();
                            }
                        }
                        n = n.nextSibling();
                    }
                }
                nodeson = nodeson.nextSibling();
            }
        }
        node = node.nextSibling();
    }
    if(m_strResourceID == "")
    {
        return false;
    }
    return true;
}

void LoginWidget::SaveProject(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    QString fileName = strFilePath.mid(npoint+1,strFilePath.length()-npoint);

    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceNoDialog("AudioStudio","shotcut临时目录",fileName,"mlt");
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("ResourceNoDialog failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        if(m_strResourceID == "")
        {

            CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
            qDebug() <<QString::fromWCharArray(pBuff);
            GetListInfo(QString::fromWCharArray(pBuff));
        }
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(m_strResourceName,
                                                                       m_strResourceID,
                                                                       strFilePath,
                                                                       fileName,
                                                                       ERMT_AUTO,
                                                                       "mlt",
                                                                       NULL,
                                                                       EWF_WEB_SAVE_COVER_PROMPT);
        if(nJobID <= 0)
        {
            qDebug() <<"save failed";
            return;
        }
        int ret = CloudDiskInterface::instance()->UploadResource(nJobID,QString::fromWCharArray(pBuff));
        if(ret == ERT_TRUE)
        {
            qDebug() <<"save succeed";
        }
    }
}

void LoginWidget::SaveProjectOther(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    QString fileName = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(0,"视频编辑",fileName,ERMT_ALL,"mlt",NULL,EWF_WEB_OPEN_MUTI_SELECT);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        //获取资源选中窗口的信息
        if(m_strResourceID == "")
        {

            CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
            qDebug() <<QString::fromWCharArray(pBuff);
            GetListInfo(QString::fromWCharArray(pBuff));
        }
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(m_strResourceName,
                                                                      m_strResourceID,
                                                                      strFilePath,
                                                                      fileName,
                                                                      ERMT_ALL,
                                                                      "mlt",
                                                                      NULL,
                                                                      EWF_WEB_SAVE_COVER_PROMPT);
        if(nJobID <= 0)
        {
            qDebug() <<"save failed";
            return;
        }
        int ret = CloudDiskInterface::instance()->UploadResource(nJobID,QString::fromWCharArray(pBuff));
        if(ret == ERT_TRUE)
        {
            qDebug() <<"save succeed";
        }
    }
}

void LoginWidget::SaveVideo(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    QString fileName = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(0,"视频编辑",fileName,ERMT_ALL,"",NULL,EWF_WEB_OPEN_MUTI_SELECT);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        //获取资源选中窗口的信息
        if(m_strResourceID == "")
        {

            CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
            qDebug() <<QString::fromWCharArray(pBuff);
            GetListInfo(QString::fromWCharArray(pBuff));
        }
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(m_strResourceName,
                                                                      m_strResourceID,
                                                                      strFilePath,
                                                                      fileName,
                                                                      ERMT_ALL,
                                                                      "",
                                                                      NULL,
                                                                      EWF_WEB_SAVE_COVER_PROMPT);
        if(nJobID <= 0)
        {
            qDebug() <<"save failed";
            return;
        }
        int ret = CloudDiskInterface::instance()->UploadResource(nJobID,QString::fromWCharArray(pBuff));
        if(ret == ERT_TRUE)
        {
            qDebug() <<"save succeed";
        }
    }
}

void LoginWidget::on_pushButton_login_clicked()
{
    //初始化
    CallUnifyLoginSrv::instance()->Init(m_strSysID,m_strJoincode,m_strUrl);
    //登录有界面
    QString strMoudle("VideoStudio");
    wchar_t pszUserToken[256];
    memset(pszUserToken,0,256 * sizeof(wchar_t));
    int nSize =256;
    wchar_t pUserID[256];
    memset(pUserID,0,256 * sizeof(wchar_t));
    int nIDSize =256;
    int ret = CallUnifyLoginSrv::instance()->LoginV2(strMoudle,pszUserToken,&nSize,pUserID,&nIDSize);
    if(ret != ERT_TRUE)
    {
        qDebug() << ret <<"登录失败";
        wchar_t pError[256];
        memset(pError,0,256 * sizeof(wchar_t));
        int nSize =256;
        ret = CallUnifyLoginSrv::instance()->GetLastError(pError,&nSize);
        qDebug() << pError << nSize;

    }
    wchar_t pszTicket[256];
    memset(pszTicket,0,256 * sizeof(wchar_t));
    int nsize =256;
    ret = CallUnifyLoginSrv::instance()->GetTicket(pszTicket,&nsize);
    if(ret == ERT_TRUE)
    {
        qDebug() << QStringLiteral("GetTicket succeed");
        m_Ticket = QString::fromWCharArray(pszTicket);
    }
    SetLoginSucceed(pUserID,pszUserToken);
}

void LoginWidget::on_pushButton_close_clicked()
{
    this->hide();
}

void LoginWidget::on_pushButton_exit_clicked()
{
    //调用注销接口
    if(m_loginType != ERT_TRUE)
    {
        int ret = CallUnifyLoginSrv::instance()->Logout();
        if(ret == ERT_TRUE)
        {
            ui->pushButton_login->setEnabled(true);
            ui->pushButton_exit->setEnabled(false);
            ui->pushButton_open->setEnabled(false);
            ui->pushButton_save->setEnabled(false);
            ui->pushButton_send->setEnabled(false);
            ui->pushButton_saveoth->setEnabled(false);
        }
    }
}

void LoginWidget::on_pushButton_open_clicked()
{
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(1,"视频编辑站","",ERMT_ALL,"mlt",NULL,EWF_WEB_OPEN_FLAG_DEFAULT);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        wchar_t pBuff[1024];
        memset(pBuff,0,1024 * sizeof(wchar_t));
        CloudDiskInterface::instance()->GetResourceList(pBuff,nsize);
        qDebug() <<QString::fromWCharArray(pBuff);
        if(!GetListInfo(QString::fromWCharArray(pBuff)))
        {
            qDebug()<<"获取资源信息失败";
            return;
        }

        //下载资源
        QString strLoadPath = QCoreApplication::applicationDirPath()+"/DownLoad";
        wchar_t FileListBuffer[512];
        memset(FileListBuffer,0,512 * sizeof(wchar_t));
        int nbufferSize = 512;
        int t = CloudDiskInterface::instance()->DownloadResource(m_strResourceID,strLoadPath,FileListBuffer,nbufferSize,NULL,EDFalg_PROMPT);
        if(t == ERT_TRUE)
        {
            qDebug()<<"DownloadResource successful";
        }
        //加载到工程
        QString strProjectPath = strLoadPath + QString::fromWCharArray(FileListBuffer);
        emit signal_OpenProject(strProjectPath);
    }
}

void LoginWidget::on_pushButton_save_clicked()
{
    emit signal_SaveProject(1);//1代表默认保存
}


void LoginWidget::on_pushButton_saveoth_clicked()
{
    emit signal_SaveProject(2);//2代表另存为
}

void LoginWidget::on_pushButton_openvideo_clicked()
{
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(1,"视频编辑站","",ERMT_ALL,"",NULL,EWF_WEB_OPEN_FLAG_DEFAULT);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        wchar_t pBuff[1024];
        memset(pBuff,0,1024 * sizeof(wchar_t));
        CloudDiskInterface::instance()->GetResourceList(pBuff,nsize);
        qDebug() <<QString::fromWCharArray(pBuff);
        if(!GetListInfo(QString::fromWCharArray(pBuff)))
        {
            qDebug()<<"获取资源信息失败";
            return;
        }

        //下载资源
        QString strLoadPath = QCoreApplication::applicationDirPath()+"/DownLoad";
        wchar_t FileListBuffer[512];
        memset(FileListBuffer,0,512 * sizeof(wchar_t));
        int nbufferSize = 512;
        int t = CloudDiskInterface::instance()->DownloadResource(m_strResourceID,strLoadPath,FileListBuffer,nbufferSize,NULL,EDFalg_PROMPT);
        if(t == ERT_TRUE)
        {
            qDebug()<<"DownloadResource successful";
        }
        QString strVideoPath = strLoadPath + QString::fromWCharArray(FileListBuffer);
        emit signal_OpenVideo(strVideoPath);
    }
}



void LoginWidget::on_pushButton_savevideo_clicked()
{
    emit signal_SaveVideo();
}

void LoginWidget::on_pushButton_sendvideo_clicked()
{

}

void LoginWidget::on_pushButton_sendOthvideo_clicked()
{

}
