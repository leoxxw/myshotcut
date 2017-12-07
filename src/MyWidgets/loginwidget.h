#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H


#include "basemainwidget.h"
#include <QWidget>
#include "CallDLL/clouddiskinterface.h"
namespace Ui {
class LoginWidget;
}

class LoginWidget : public BaseMainWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();
    //初始化
    void Init();
    //登录成功设置
    void SetLoginSucceed(wchar_t* pUserID,wchar_t* pszUserToken);
    //获取资源信息列表信息
    bool GetListInfo(QString strBuffer,ResourceInfo &InfoList);
    //保存工程
    bool SaveProject(QString strFilePath,int ntype);
    //工程另存为
    bool SaveProjectOther(QString strFilePath);
    //保存视频资源
    void SaveVideo(QString strFilePath);
    void SendVideoNoDlg(QString strFilePath);
    void SendProjectNoDlg(QString strFilePath);
    bool DelDir(const QString &path);
    void AddFile(QString filePath,int JobID,QString fatherPath,int type);
    enResourceType GetFileType(QString fileName);

    void UploadVideo(QString strFilePath);
    void UploadSendVideo(QString strFilePath);

    void GetInitFileInfo();
    void SetIsWorking(bool bIsWorking);
    bool IsWoking();
    void SetProjrctType(int nType);
    void open_clicked();
public slots:
    void slot_raise();
protected:
    // Event handlers
    void mouseMoveEvent(QMouseEvent *event);
private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_open_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_saveoth_clicked();

    void on_pushButton_openvideo_clicked();

    void on_pushButton_savevideo_clicked();

    void on_pushButton_sendvideo_clicked();

    void on_pushButton_sendOthvideo_clicked();

    void on_pushButton_send_clicked();

    void on_pushButton_search_clicked();


signals:
    void signal_SaveProject(int ntype,QString ProjectName);
    void signal_SaveVideo(int ntype);
    void signal_OpenProject(QString ProjectPath);
    void signal_OpenVideo(QString VideoPath);
    void signal_SendProject();
    void signal_SendVideo();
    void Signal_UploadVideo(bool);
    void Signal_CloseProject();
    void Signal_SysName(QString strName);

private:
    Ui::LoginWidget *ui;
    QString         m_userID;//用户ID
    QString         m_UserToken;//用户验证表
    QString         m_Ticket;//票据
    QString         m_strSysID;//系统ID
    QString         m_strJoincode;//验证码
    QString         m_strUrl;//配置中心地址
    enReturnType    m_loginType;//登录类型（是否需要注销）
    ResourceInfo    m_ProResourceInfo;//解析的资源信息
    QString         m_strBuffInfo;//获得的资源信息
    bool            m_bIsworking;
    int             m_nSearch;//搜索按钮配置
    int             m_ProjectType;//工程类型
    QString         m_sysName;
};

#endif // LOGINWIDGET_H
