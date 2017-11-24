#include "videostudiolog.h"
#include <QMutexLocker>
#include <QFile>
#include <QTextStream>
#include<QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
QMutex VideoStudioLog::m_MutexCloud;
VideoStudioLog *VideoStudioLog::m_pInstance;


VideoStudioLog::VideoStudioLog()
{

}

VideoStudioLog::~VideoStudioLog()
{

}

//初始化
void VideoStudioLog::Init()
{
    // 创建日志目录
    QString path = QCoreApplication::applicationDirPath();
    path.append("/log");
    QDir dir(path);
    if(!dir.exists())
    {
       dir.mkpath(path);//创建多级目录
    }
    path.append("/VideoStudio_log.txt");
    QFile file(path);
    QFileInfo info(path);

    //如果文件大于10M则清空日志文件
    if(info.size() > 1048576)
    {
        file.remove();
    }
    file.close();
}

void VideoStudioLog::Log(QString info, QString Type)
{
    // 加锁
    static QMutex mutex;
    mutex.lock();
    // 设置输出信息格式
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
    QString strMessage = QString("Type:%1 DateTime:%2 Message:%3").arg(Type).arg(strDateTime)
            .arg(info);

    // 输出信息至文件中（读写、追加形式）
    QString path = QCoreApplication::applicationDirPath();
    path.append("/log/VideoStudio_log.txt");
    QFile file(path);

    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strMessage << "\r\n";
    file.flush();
    file.close();
    // 解锁
    mutex.unlock();
}

//单利对象
VideoStudioLog *VideoStudioLog::instance()
{
    if (m_pInstance == NULL)
    {
        QMutexLocker mutexLocker(&m_MutexCloud);
        if (m_pInstance == NULL)
        {
            m_pInstance = new VideoStudioLog();
            m_pInstance->Init();
        }
    }
    return m_pInstance;
}

