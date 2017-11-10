#include "callunifyloginsrv.h"
#include <QLibrary>
#include <QMutexLocker>
#include <QDebug>
QMutex CallUnifyLoginSrv::m_Mutex;
QSharedPointer<CallUnifyLoginSrv> CallUnifyLoginSrv::m_pInstance;

CallUnifyLoginSrv::CallUnifyLoginSrv()
         : m_lib(NULL)
{

}
CallUnifyLoginSrv::~CallUnifyLoginSrv()
{
    if(m_lib)
    {
        m_lib->unload();
        delete m_lib;
        m_lib = NULL;
    }
}

bool CallUnifyLoginSrv::LoadDll()
{
    m_lib = new QLibrary();
    m_lib->setFileName("UnifyLoginSrv.dll");
    if (!m_lib->load())
    {
        qDebug() << "load failed!";
        return false;
    }
    return true;
}

int CallUnifyLoginSrv::Init(QString strSysID,QString strJoincode,QString strUrl)
{
    typedef int (*Function)(const ushort*,const ushort*,const ushort*);
    Function fpInit = (Function)m_lib->resolve("DoInit");

    int ret = fpInit(strSysID.utf16(),strJoincode.utf16(),strUrl.utf16());
    return ret;
}

int CallUnifyLoginSrv::SetLogType(const int nType)
{
    typedef int (*Function)(const int);
    Function fpInit = (Function)m_lib->resolve("DoSetLogType");

    int ret = fpInit(nType);
    return ret;
}

int CallUnifyLoginSrv::GetLastError(wchar_t *pError, int *nSize)
{
    typedef int (*Function)(wchar_t *,int *);
    Function fpInit = (Function)m_lib->resolve("DoGetLastError");

    int ret = fpInit(pError,nSize);
    return ret;
}

int CallUnifyLoginSrv::LoginV2(QString strMoudle, wchar_t *pszUserToken, int *nSize, wchar_t *pUserID, int *nIDSize)
{
    typedef int (*Function)(const ushort*,wchar_t *, int *, wchar_t *, int *);
    Function fpInit = (Function)m_lib->resolve("DoLoginV2");

    int ret = fpInit(strMoudle.utf16(),pszUserToken,nSize,pUserID,nIDSize);
    return ret;
}

int CallUnifyLoginSrv::LoginExV2(QString strMoudle, QString strUser, QString strPwd, wchar_t *pszUserToken, int *nSize, wchar_t *pUserID, int *nIDSize)
{
    typedef int (*Function)(const ushort*,const ushort*,const ushort*,wchar_t*,int*,wchar_t*,int*);
    Function fpInit = (Function)m_lib->resolve("DoLoginExV2");

    int ret = fpInit(strMoudle.utf16(),strUser.utf16(),strPwd.utf16(),pszUserToken,nSize,pUserID,nIDSize);
    return ret;
}

int CallUnifyLoginSrv::AvoidLoginV2(QString strMoudle, wchar_t *pszUserToken, int *nSize, wchar_t *pUserID, int *nIDSize)
{
    typedef int (*Function)(const ushort*,wchar_t *, int *, wchar_t *, int *);
    Function fpInit = (Function)m_lib->resolve("DoAvoidLoginV2");

    int ret = fpInit(strMoudle.utf16(),pszUserToken,nSize,pUserID,nIDSize);
    return ret;
}

int CallUnifyLoginSrv::GetUserInfoEx(QString strNodeName, wchar_t *pValue, int *nSize)
{
    typedef int (*Function)(const ushort*,wchar_t *, int *);
    Function fpInit = (Function)m_lib->resolve("DoGetUserInfoEx");

    int ret = fpInit(strNodeName.utf16(),pValue,nSize);
    return ret;
}

int CallUnifyLoginSrv::Logout()
{
    typedef int (*Function)();
    Function fpInit = (Function)m_lib->resolve("DoLogout");

    int ret = fpInit();
    return ret;
}

QSharedPointer<CallUnifyLoginSrv> &CallUnifyLoginSrv::instance()
{
    if (m_pInstance.isNull())
    {
        QMutexLocker mutexLocker(&m_Mutex);
        if (m_pInstance.isNull())
        {
            m_pInstance = QSharedPointer<CallUnifyLoginSrv>(new CallUnifyLoginSrv());
            m_pInstance->LoadDll();
        }
    }
    return m_pInstance;
}
