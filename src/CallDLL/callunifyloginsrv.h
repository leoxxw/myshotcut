#ifndef CALLUNIFYLOGINSRV_H
#define CALLUNIFYLOGINSRV_H
#include <QMutex>
#include <QSharedPointer>
#include <QLibrary>
class CallUnifyLoginSrv
{
public:

    ~CallUnifyLoginSrv();

public:
    bool LoadDll();
    int Init(QString strSysID,QString strJoincode,QString strUrl);
    int SetLogType(const int nType);
    int GetLastError(wchar_t *pError, int *nSize);
    int LoginV2(QString strMoudle,wchar_t *pszUserToken, int *nSize, wchar_t *pUserID, int *nIDSize);
    int LoginExV2(QString strMoudle,QString strUser,QString strPwd,wchar_t *pszUserToken, int *nSize, wchar_t *pUserID, int *nIDSize);
    int AvoidLoginV2(QString strMoudle,wchar_t *pszUserToken, int *nSize, wchar_t *pUserID, int *nIDSize);
    int GetUserInfoEx(QString strNodeName,wchar_t *pValue, int *nSize);
    int GetTicket(wchar_t *pTicket, int *nSize);    // 返回票据
    int Logout();
    static CallUnifyLoginSrv* instance();
private:
    CallUnifyLoginSrv();
    CallUnifyLoginSrv(const CallUnifyLoginSrv&){}
    CallUnifyLoginSrv* operator==(const CallUnifyLoginSrv&){}
private:
    static QMutex m_Mutex;
    static CallUnifyLoginSrv* m_pInstance;
    QLibrary* m_lib;

};

#endif // CALLUNIFYLOGINSRV_H
