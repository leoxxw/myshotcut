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
    void Init();
    void SetLoginSucceed(wchar_t* pUserID,wchar_t* pszUserToken);
    bool GetListInfo(QString strBuffer,ResourceInfo &InfoList);
    void SaveProject(QString strFilePath,int ntype);
    void SaveProjectOther(QString strFilePath);
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
    void signal_SaveProject(int ntype);
    void signal_SaveVideo(int ntype);
    void signal_OpenProject(QString ProjectPath);
    void signal_OpenVideo(QString VideoPath);
    void signal_SendProject();
    void signal_SendVideo();
    void Signal_UploadVideo(bool);

private:
    Ui::LoginWidget *ui;
    QString     m_userID;
    QString     m_UserToken;
    QString     m_Ticket;
    QString m_strSysID;
    QString m_strJoincode;
    QString m_strUrl;
    enReturnType m_loginType;
    ResourceInfo m_ProResourceInfo;
    QString m_strBuffInfo;
    bool m_bIsworking;
    int m_nSearch;
};

#endif // LOGINWIDGET_H
