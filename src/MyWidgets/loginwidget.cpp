#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "CallDLL/callunifyloginsrv.h"
#include <QDebug>
#include "basemainwidget.h"
#include <QDomNodeList>
#include "settings.h"
#include <QMessageBox>
#include <QDir>
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
    //初始化
    Init();
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

void LoginWidget::Init()
{
    //删除临时文件
    QString strLoadPath = QCoreApplication::applicationDirPath()+"/Temp";
    DelDir(strLoadPath);
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

bool LoginWidget::GetListInfo(QString strBuffer,ResourceInfo &InfoList)
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
                                InfoList.m_strResourceID = e.text();
                            }
                            if (e.tagName() =="ParentID")
                            {
                                InfoList.m_strParentID = e.text();
                            }
                            if (e.tagName() =="Name")
                            {
                                InfoList.m_strResourceName = e.text();
                            }
                            if (e.tagName() =="Owner")
                            {
                                InfoList.m_strOwnerID = e.text();
                            }
                            if (e.tagName() =="OwnerType")
                            {
                                InfoList.m_strOwnerType = e.text();
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
    if(InfoList.m_strResourceID == "")
    {
        return false;
    }
    return true;
}

void LoginWidget::SaveProject(QString strFilePath)
{
    //获取资源选中窗口的信息
    if(m_ProResourceInfo.m_strResourceID == "")
    {
        SaveProjectOther(strFilePath);
        return;
    }
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    int n= strFilePath.lastIndexOf('.');
    QString filePath = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    QString fileType =strFilePath.mid(n+1,strFilePath.length()-n);
    QString fileName = strFilePath.mid(npoint+1,n-npoint-1);

    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceNoDialog("AudioStudio","shotcut临时目录",
                                                                 fileName,fileType);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
        return;
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
        qDebug() <<QString::fromWCharArray(pBuff);
        ResourceInfo infoList;
        GetListInfo(QString::fromWCharArray(pBuff),infoList);
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(infoList.m_strResourceName,
                                                                       infoList.m_strResourceID,
                                                                       strFilePath,
                                                                       filePath,
                                                                       ERMT_ALL,
                                                                       fileType,
                                                                       NULL,
                                                                       EWF_WEB_SAVE_COVER_PROMPT);
        if(nJobID <= 0)
        {
            qDebug() <<"save failed";
            return;
        }
        AddFile(strFilePath,nJobID,"",1);
        int ret = CloudDiskInterface::instance()->UploadResource(nJobID,QString::fromWCharArray(pBuff));
        if(ret == ERT_TRUE)
        {
            qDebug() <<"save succeed";
            QMessageBox::about(NULL, QStringLiteral("成功"), QStringLiteral("工程保存成功"));
        }else{
            qDebug() <<"save failed";
        }
    }
}

void LoginWidget::SaveProjectOther(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    int tpoint = strFilePath.lastIndexOf('.');
    QString filePath = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    QString fileName = strFilePath.mid(npoint+1,tpoint-npoint-1);
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(0,"视频编辑",
                                                               fileName,
                                                               ERMT_ALL,
                                                               "mlt",
                                                               NULL,EWF_WEB_SAVE_COVER_PROMPT);
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
        if(m_ProResourceInfo.m_strResourceID == "")
        {

            CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
            qDebug() <<QString::fromWCharArray(pBuff);
            GetListInfo(QString::fromWCharArray(pBuff),m_ProResourceInfo);
        }
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(m_ProResourceInfo.m_strResourceName,
                                                                      m_ProResourceInfo.m_strResourceID,
                                                                      strFilePath,
                                                                      filePath,
                                                                      ERMT_ALL,
                                                                      "mlt",
                                                                      NULL,
                                                                      EWF_WEB_SAVE_COVER_PROMPT);
        if(nJobID <= 0)
        {
            qDebug() <<"save failed";
            return;
        }
        AddFile(strFilePath,nJobID,"",1);
        int ret = CloudDiskInterface::instance()->UploadResource(nJobID,QString::fromWCharArray(pBuff));
        if(ret == ERT_TRUE)
        {
            qDebug() <<"save succeed";
            QMessageBox::about(NULL, QStringLiteral("成功"), QStringLiteral("工程保存成功"));
        }
    }
}

void LoginWidget::SaveVideo(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    int tpoint = strFilePath.lastIndexOf('.');
    QString filePath = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    QString fileName = strFilePath.mid(npoint+1,tpoint-npoint-1);
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
        CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
            qDebug() <<QString::fromWCharArray(pBuff);
            ResourceInfo infoList;
            GetListInfo(QString::fromWCharArray(pBuff),infoList);

        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(infoList.m_strResourceName,
                                                                      infoList.m_strResourceID,
                                                                      strFilePath,
                                                                      filePath,
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
            QMessageBox::about(NULL, QStringLiteral("成功"), QStringLiteral("视频保存成功"));
        }
    }
}

void LoginWidget::SendVideoNoDlg(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    int n= strFilePath.lastIndexOf('.');
    QString filePath = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    QString fileType =strFilePath.mid(n+1,strFilePath.length()-n);
    QString fileName = strFilePath.mid(npoint+1,n-npoint-1);
    qDebug() <<" filename =" << fileName;
    qDebug() <<"fileType ="<<fileType;
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceNoDialog("AudioStudio","shotcut临时目录",
                                                                 fileName,fileType);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
        return;
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
        qDebug() <<QString::fromWCharArray(pBuff);
        ResourceInfo infoList;
        GetListInfo(QString::fromWCharArray(pBuff),infoList);
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(infoList.m_strResourceName,
                                                                      infoList.m_strResourceID,
                                                                      strFilePath,
                                                                      filePath,
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
            //云盘内资源发送
            int t = CloudDiskInterface::instance()->LocalSend(infoList.m_strResourceID);
            if(t == ERT_TRUE)
            {
                 qDebug()<<"LocalSend successful";
                 QMessageBox::about(NULL, QStringLiteral("成功"), QStringLiteral("视频发送成功"));
            }
        }
    }
}

void LoginWidget::SendProjectNoDlg(QString strFilePath)
{
    wchar_t pBuff[1024];
    memset(pBuff,0,1024 * sizeof(wchar_t));
    int npoint= strFilePath.lastIndexOf(QRegExp("/"));
    int n= strFilePath.lastIndexOf('.');
    QString filePath = strFilePath.mid(npoint+1,strFilePath.length()-npoint);
    QString fileType =strFilePath.mid(n+1,strFilePath.length()-n);
    QString fileName = strFilePath.mid(npoint+1,n-npoint-1);
    qDebug() <<" filename =" << fileName;
    qDebug() <<"fileType ="<<fileType;
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceNoDialog("AudioStudio","shotcut临时目录",fileName,fileType);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("CloudDiskInterface failed");
        return;
    }
    if(nsize == ERT_CANCEL)
    {
        qDebug() <<QStringLiteral("cancle");
        return;
    }
    if(nsize > ERT_CANCEL)
    {
        //获取资源选中窗口的信息
        CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
        qDebug() <<QString::fromWCharArray(pBuff);
        //获取资源选中窗口的信息
        if(m_ProResourceInfo.m_strResourceID == "")
        {

            CloudDiskInterface::instance()->GetResourceList(pBuff, 1024);
            qDebug() <<QString::fromWCharArray(pBuff);
            GetListInfo(QString::fromWCharArray(pBuff),m_ProResourceInfo);
        }
        //预保存资源到云盘
        int nJobID = CloudDiskInterface::instance()->PreUploadResource(m_ProResourceInfo.m_strResourceName,
                                                                      m_ProResourceInfo.m_strResourceID,
                                                                      strFilePath,
                                                                      filePath,
                                                                      ERMT_ALL,
                                                                      "mlt",
                                                                      NULL,
                                                                      EWF_WEB_SAVE_COVER_PROMPT);
        if(nJobID <= 0)
        {
            qDebug() <<"save failed";
            return;
        }
        AddFile(strFilePath,nJobID,"",1);
        int ret = CloudDiskInterface::instance()->UploadResource(nJobID,QString::fromWCharArray(pBuff));
        if(ret == ERT_TRUE)
        {
            qDebug() <<"save succeed";
            //云盘内资源发送
            int t = CloudDiskInterface::instance()->LocalSend(m_ProResourceInfo.m_strResourceID);
            if(t == ERT_TRUE)
            {
                 qDebug()<<"LocalSend successful";
                 QMessageBox::about(NULL, QStringLiteral("成功"), QStringLiteral("工程发送成功"));
            }
        }
    }
}

bool LoginWidget::DelDir(const QString &path)
{
    if (path.isEmpty()){
            return false;
        }
        QDir dir(path);
        if(!dir.exists()){
            return true;
        }
        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
        QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
        foreach (QFileInfo file, fileList){ //遍历文件信息
            if (file.isFile()){ // 是文件，删除
                file.dir().remove(file.fileName());
            }else{ // 递归删除
                DelDir(file.absoluteFilePath());
            }
        }
        return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

void LoginWidget::AddFile(QString filePath, int JobID,QString fatherPath,int type)
{
    QString path = filePath;
    QString ProFileName ="";
    if(type == 1)
    {
        int npoint= filePath.lastIndexOf(QRegExp("/"));
        path = filePath.mid(0,npoint+1);
        ProFileName = filePath.mid(npoint+1,filePath.length()-npoint);
    }
    if (path.isEmpty()){
            return ;
        }
        QDir dir(path);
        if(!dir.exists()){
            return ;
        }

        dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
        QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
        foreach (QFileInfo file, fileList)
        {
            //遍历文件信息
            if (file.isFile())
            {
                // 是文件，添加
                QString fileName =file.fileName();
                if(fileName == ProFileName)
                {
                    continue;
                }
                QString fpath = path+"/"+fileName;
                QString fName = fatherPath+fileName;
                int ret =  CloudDiskInterface::instance()->AddUploadFile(JobID,fpath,fName,ERMT_ALL,"");
                if(ret == 1)
                {
                    qDebug()<<"AddUploadFile successful";
                }else{
                    qDebug()<<"AddUploadFile failed";

                }

            }
            else
            {
                // 递归
                QString fpath = fatherPath +file.fileName()+"/";
                AddFile(file.absoluteFilePath(),JobID,fpath,0);
            }
        }
        return;
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
        m_loginType = ERT_NewTGT;
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
    if(m_loginType == ERT_NewTGT)
    {
        CallUnifyLoginSrv::instance()->Logout();
    }
    ui->pushButton_login->setEnabled(true);
    ui->pushButton_exit->setEnabled(false);
    ui->pushButton_open->setEnabled(false);
    ui->pushButton_save->setEnabled(false);
    ui->pushButton_send->setEnabled(false);
    ui->pushButton_saveoth->setEnabled(false);
    m_userID = "";
    m_UserToken = "";
    m_Ticket = "";
    m_loginType = ERT_FALSE;
}

void LoginWidget::on_pushButton_open_clicked()
{
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(1,"视频编辑","",ERMT_ALL,"mlt",NULL,EWF_WEB_OPEN_FLAG_DEFAULT);
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
        m_strBuffInfo = QString::fromWCharArray(pBuff);
        qDebug() << m_strBuffInfo;
        if(!GetListInfo(QString::fromWCharArray(pBuff),m_ProResourceInfo))
        {
            qDebug()<<"获取资源信息失败";
            return;
        }

        //下载资源
        QString strLoadPath = QCoreApplication::applicationDirPath()+"/DownLoad";
        wchar_t FileListBuffer[1024 *5];
        memset(FileListBuffer,0,1024 *5 * sizeof(wchar_t));
        int nbufferSize = 1024 *5;
        int t = CloudDiskInterface::instance()->DownloadResource(m_ProResourceInfo.m_strResourceID,
                                                                 strLoadPath,
                                                                 FileListBuffer,
                                                                 nbufferSize,
                                                                 NULL,
                                                                 EDFalg_PROMPT);
        if(t == ERT_TRUE)
        {
            qDebug()<<"DownloadResource successful";
        }
        //加载到工程
        //获取主工程路径
        wchar_t FilePathBuffer[512];
        memset(FilePathBuffer,0,512 * sizeof(wchar_t));
        int rt =  CloudDiskInterface::instance()->GetMainFilePath(FilePathBuffer,512);
        if(rt != ERT_TRUE)
        {
            qDebug()<<"GetMainFilePath failed";
        }
        QString strProjectPath = strLoadPath + "/"+m_ProResourceInfo.m_strResourceID+"/"+QString::fromWCharArray(FilePathBuffer);
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
    int nsize = CloudDiskInterface::instance()->ResourceDialog(1,"视频编辑","",ERMT_ALL,"",NULL,EWF_WEB_OPEN_FLAG_DEFAULT);
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
        ResourceInfo infoList;
        if(!GetListInfo(QString::fromWCharArray(pBuff),infoList))
        {
            qDebug()<<"获取资源信息失败";
            return;
        }

        //下载资源
        QString strLoadPath = QCoreApplication::applicationDirPath()+"/DownLoad";
        wchar_t FileListBuffer[512];
        memset(FileListBuffer,0,512 * sizeof(wchar_t));
        int nbufferSize = 512;
        int t = CloudDiskInterface::instance()->DownloadResource(infoList.m_strResourceID,
                                                                 strLoadPath,
                                                                 FileListBuffer,
                                                                 nbufferSize,
                                                                 NULL,
                                                                 EDFalg_PROMPT);
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
    emit signal_SaveVideo(2);//2代表自己选择路径保存
}

void LoginWidget::on_pushButton_sendvideo_clicked()
{
    //发送视频前先默认保存视频
    emit signal_SaveVideo(1);//1代表默认路径
}

void LoginWidget::on_pushButton_sendOthvideo_clicked()
{
    //打开选择窗口
    int nsize = CloudDiskInterface::instance()->ResourceDialog(3,"视频编辑","",ERMT_ALL,"",NULL,EWF_WEB_OPEN_MUTI_SELECT);
    if(nsize < ERT_TRUE)
    {
        qDebug() <<QStringLiteral("ResourceDialog failed");
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
        ResourceInfo infoList;
        if(!GetListInfo(QString::fromWCharArray(pBuff),infoList))
        {
            qDebug()<<"获取资源信息失败";
            return;
        }

        //云盘内资源发送
        int t = CloudDiskInterface::instance()->LocalSend(infoList.m_strResourceID);
        if(t == ERT_TRUE)
        {
             qDebug()<<"LocalSend successful";
             QMessageBox::about(NULL, QStringLiteral("成功"), QStringLiteral("视频发送成功"));
        }
    }
}

void LoginWidget::on_pushButton_send_clicked()
{
    emit signal_SaveProject(3);//3代表默认保存并发送
}
