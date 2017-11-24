#ifndef VIDEOSTUDIOLOG_H
#define VIDEOSTUDIOLOG_H

#include <QMutex>

class VideoStudioLog
{
public:
    ~VideoStudioLog();

public:
    void Init();
    void Log(QString info,QString Type);
    static VideoStudioLog* instance();
private:
    VideoStudioLog();
    VideoStudioLog(const VideoStudioLog&){}
    VideoStudioLog& operator==(const VideoStudioLog&){}
private:
    static QMutex m_MutexCloud;
    static VideoStudioLog* m_pInstance;

};
#define LOG(info,type) VideoStudioLog::instance()->Log(info,type)
#endif // VIDEOSTUDIOLOG_H
