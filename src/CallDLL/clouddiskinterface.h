#ifndef CLOUDDISKINTERFACE_H
#define CLOUDDISKINTERFACE_H
#include <QMutex>
#include <QSharedPointer>
#include <QLibrary>
#include "publicinfo.h"
#include <windows.h>
class CloudDiskInterface
{
public:
    ~CloudDiskInterface();

public:
    bool LoadDll();
    int Init(QString strSysID,QString strUrl,QString strUserID,QString strJoincode);
    int SetLogType(const int nType);

    int SetToken(QString strTicket,QString strUserToken);

    int ResourceNoDialog(QString lpszMoudel,
                         QString lpszDirName,
                         QString lpszFileName,
                         QString lpszResourceSubTypeFilter);

    int ResourceDialog(int bIsOpen, QString lpszTitle, QString lpszFileName,
                       enResourceMajorType nResourceMajorTypeFilter,
                       QString lpszResourceSubTypeFilter,
                       HWND hParentWnd,
                       enWebFlag dwFlag);
    int ResourceDialogEx(int bIsOpen, QString lpszTitle, QString lpszFileName,
                       enResourceMajorType nResourceMajorTypeFilter,
                       QString lpszResourceSubTypeFilter,
                       HWND hParentWnd,
                       enWebFlag dwFlag,
                       QString lpszNotSub);

    int PreUploadResource(QString lpszResourceName, QString lpszResourceID,
                          QString lpszFile,QString lpszRelativeFile,
                          enResourceType nResourceMajorType,
                          QString lpszResourceSubType,
                          HWND hParentWnd,
                          enWebFlag dwFlag);

    int UploadResource(int nUploadJobID,QString lpszRelativeFile, int nCheck);

    int GetResourceList(wchar_t* szBuffer, int nSize);

    int DownloadResource(QString lpszResourceID,
                         QString lpszLocalPath,
                         wchar_t* szFileListBuffer,
                         int nSize,
                         HWND hParentWnd,
                         DWORD dwFlag);

    int AddUploadFile(int nUploadJobID, QString lpszFile, QString lpszRelativeFile,
                      enResourceType nResourceMajorType,
                      QString lpszResourceSubType);

    int LocalSend(QString lpResourceID);

    int Send2BroadCast(QString lpResourceID);

    int Send (QString lpResourceID, QString lpszDestSystemID);

    int GetMainFilePath(wchar_t* szMainPath, int nsize);

    int SearchOpenEx(int nResourceMajorTypeFilter,
                   QString lpszResourceSubTypeFilter,
                   HWND hParentWnd,
                   DWORD dwFlag,QString lpszNotSub);

    int GetTrialState(QString lpResourceID);

    static CloudDiskInterface* instance();
private:
    CloudDiskInterface();
    CloudDiskInterface(const CloudDiskInterface&){}
    CloudDiskInterface& operator==(const CloudDiskInterface&){}
private:
    static QMutex m_MutexCloud;
    static CloudDiskInterface* m_pInstance;
    QLibrary* m_lib;

};
#endif // CLOUDDISKINTERFACE_H
