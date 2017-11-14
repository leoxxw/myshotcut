﻿#include "clouddiskinterface.h"
#include <QLibrary>
#include <QMutexLocker>
#include <QDebug>
QMutex CloudDiskInterface::m_MutexCloud;
QSharedPointer<CloudDiskInterface> CloudDiskInterface::m_pInstance;


CloudDiskInterface::CloudDiskInterface()
        : m_lib(NULL)
{

}

CloudDiskInterface::~CloudDiskInterface()
{
    if(m_lib)
    {
        m_lib->unload();
        delete m_lib;
        m_lib = NULL;
    }
}
//加载dll
bool CloudDiskInterface::LoadDll()
{
    m_lib = new QLibrary();
    m_lib->setFileName("LHCloudDiskInterface.dll");
    if (!m_lib->load())
    {
        qDebug() << "load failed!";
        return false;
    }
    return true;
}
//初始化
int CloudDiskInterface::Init(QString strSysID,QString strUrl,QString strUserID,QString strJoincode)
{
    typedef int (*Function)(const ushort*,const ushort*,const ushort*,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoInit");

    int ret = fpInit(strSysID.utf16(),strUrl.utf16(),strUserID.utf16(),strJoincode.utf16());
    return ret;
}

int CloudDiskInterface::SetLogType(const int nType)
{
    typedef int (*Function)(const int);
    Function fpInit = (Function)m_lib->resolve("DoSetLogType");

    int ret = fpInit(nType);
    return ret;
}
//设置票据
int CloudDiskInterface::SetToken(QString strTicket, QString strUserToken)
{
    typedef int (*Function)(const ushort*,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoSetToken");

    int ret = fpInit(strTicket.utf16(),strUserToken.utf16());
    return ret;
}

int CloudDiskInterface::ResourceNoDialog(QString lpszMoudel,
                                         QString lpszDirName,
                                         QString lpszFileName,
                                         QString lpszResourceSubTypeFilter)
{
    typedef int (*Function)(const ushort*,const ushort*,const ushort*,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoResourceNoDialog");

    int ret = fpInit(lpszMoudel.utf16(),
                     lpszDirName.utf16(),
                     lpszFileName.utf16(),
                     lpszResourceSubTypeFilter.utf16());
    return ret;
}
//打开（保存，选择）窗口
int CloudDiskInterface::ResourceDialog(int bIsOpen,
                                       QString lpszTitle,
                                       QString lpszFileName,
                                       enResourceMajorType nResourceMajorTypeFilter,
                                       QString lpszResourceSubTypeFilter,
                                       HWND hParentWnd,
                                       enWebFlag dwFlag)
{
    typedef int (*Function)(int ,const ushort*,const ushort*,int,const ushort*,HWND,DWORD);
    Function fpInit = (Function)m_lib->resolve("DoResourceDialog");

    int ret = fpInit(bIsOpen,lpszTitle.utf16(),lpszFileName.utf16(),nResourceMajorTypeFilter,
                     lpszResourceSubTypeFilter.utf16(),hParentWnd,dwFlag);
    return ret;
}
//预上传资源
int CloudDiskInterface::PreUploadResource(QString lpszResourceName,
                                          QString lpszResourceID,
                                          QString lpszFile,
                                          QString lpszRelativeFile,
                                          enResourceMajorType nResourceMajorType,
                                          QString lpszResourceSubType,
                                          HWND hParentWnd,
                                          enWebFlag dwFlag)
{
    typedef int (*Function)(const ushort*,
                            const ushort*,
                            const ushort*,
                            const ushort*,
                            int,
                            const ushort*,
                            HWND,
                            DWORD);
    Function fpInit = (Function)m_lib->resolve("DoPreUploadResource");

    int ret = fpInit(lpszResourceName.utf16(),
                     lpszResourceID.utf16(),
                     lpszFile.utf16(),
                     lpszRelativeFile.utf16(),
                     nResourceMajorType,
                     lpszResourceSubType.utf16(),
                     hParentWnd,
                     dwFlag);
    return ret;
}
//上传资源
int CloudDiskInterface::UploadResource(int nUploadJobID, QString lpszRelativeFile)
{
    typedef int (*Function)(int,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoUploadResource");

    int ret = fpInit(nUploadJobID,lpszRelativeFile.utf16());
    return ret;
}
//获取资源列表
int CloudDiskInterface::GetResourceList(wchar_t *szBuffer, int nSize)
{
    typedef int (*Function)(wchar_t*,int);
    Function fpInit = (Function)m_lib->resolve("DoGetResourceList");

    int ret = fpInit(szBuffer,nSize);
    return ret;
}
//下载资源
int CloudDiskInterface::DownloadResource(QString lpszResourceID, QString lpszLocalPath, wchar_t *szFileListBuffer, int nSize, HWND hParentWnd, DWORD dwFlag)
{
    typedef int (*Function)(const ushort*,
                            const ushort*,
                            wchar_t *,
                            int,
                            HWND,
                            DWORD);
    Function fpInit = (Function)m_lib->resolve("DoDownloadResource");

    int ret = fpInit(lpszResourceID.utf16(),
                     lpszLocalPath.utf16(),
                     szFileListBuffer,
                     nSize,
                     hParentWnd,
                     dwFlag);
    return ret;
}
//上传资源添加多个文件
int CloudDiskInterface::AddUploadFile(int nUploadJobID, QString lpszFile, QString lpszRelativeFile)
{
    typedef int (*Function)(int,const ushort*,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoAddUploadFile");

    int ret = fpInit(nUploadJobID,lpszFile.utf16(),lpszRelativeFile.utf16());
    return ret;
}
//云盘内资源发送
int CloudDiskInterface::LocalSend(QString lpResourceID)
{
    typedef int (*Function)(const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoLocalSend");

    int ret = fpInit(lpResourceID.utf16());
    return ret;
}
//发送资源到播出系统
int CloudDiskInterface::Send2BroadCast(QString lpResourceID)
{
    typedef int (*Function)(const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoSend2BroadCast");

    int ret = fpInit(lpResourceID.utf16());
    return ret;
}
//发生资源到目标系统
int CloudDiskInterface::Send(QString lpResourceID, QString lpszDestSystemID)
{
    typedef int (*Function)(const ushort*,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoSend");

    int ret = fpInit(lpResourceID.utf16(),lpszDestSystemID.utf16());
    return ret;
}
//单利对象
QSharedPointer<CloudDiskInterface> &CloudDiskInterface::instance()
{
    if (m_pInstance.isNull())
    {
        QMutexLocker mutexLocker(&m_MutexCloud);
        if (m_pInstance.isNull())
        {
            m_pInstance = QSharedPointer<CloudDiskInterface>(new CloudDiskInterface());
            m_pInstance->LoadDll();
        }
    }
    return m_pInstance;
}