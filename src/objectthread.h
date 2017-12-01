#ifndef OBJECTTHREAD_H
#define OBJECTTHREAD_H

#include <QObject>
#include <QMutex>
#include <QMap>
class ObjectThread : public QObject
{
    Q_OBJECT
public:
    explicit ObjectThread(QObject *parent = 0);
    ~ObjectThread();
signals:
    void signal_WorkFinished(bool);
    void signal_ProgressItem(int);
public slots:
    void runWork(QMap<QString,QString> pathList);
private:
    QMutex m_stopMutex;
};


#endif // OBJECTTHREAD_H
