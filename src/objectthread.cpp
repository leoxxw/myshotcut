#include "objectthread.h"
#include <QThread>
#include <QDebug>
#include <QMutexLocker>
#include <QElapsedTimer>
#include <limits>
#include <QFile>
#include <QDir>

ObjectThread::ObjectThread(QObject *parent)
    :QObject(parent)
{

}

ObjectThread::~ObjectThread()
{
    qDebug() << "ThreadObject destroy";
}

void ObjectThread::runWork(QMap<QString,QString> pathList)
{

    QMutexLocker locker(&m_stopMutex);

    qDebug() <<"runWork";
    QMap<QString,QString>::iterator it; //遍历map
    for ( it = pathList.begin(); it != pathList.end(); ++it )
    {
        QString sourceFile =it.key();
        qDebug() <<"sourceFile" <<sourceFile;
        QString targetFile = it.value();
        qDebug() <<"targetFile" <<targetFile;
        if (sourceFile == targetFile){
            qDebug() <<"1111";
            continue ;
        }
        if (!QFile::exists(sourceFile)){
            qDebug() <<"22222";
            continue;
        }
        QDir *createfile  = new QDir;
        bool exist = createfile->exists(targetFile);
        if (exist)
        {
            createfile->remove(targetFile);
            qDebug() <<"remove" << targetFile;
        }

        if(!QFile::copy(sourceFile,targetFile))
        {
            qDebug() <<"failed" << sourceFile;
            continue;
        }
    }
    emit signal_WorkFinished(true);

}
