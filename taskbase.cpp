#include "taskbase.h"

TaskBase::TaskBase(int taskno, QObject *parent) : QObject(parent)
{
    this->taskno=taskno;
}
void TaskBase::onTaskFinished(int exitCode)
{
    if(exitCode==0)
    {
        emit taskFinishSignal(taskno);
    }
    else
    {
        emit taskFinishSignal(taskno,exitCode);
    }
}

void TaskBase::onTaskError(QString error)
{
    emit taskErrorSignal(taskno,error);
}

void TaskBase::onTaskOutput(QString output, QString type)
{
    emit taskOutputSignal(taskno,output,type);
}

void TaskBase::onTaskOutput(qint64 bytesRead, qint64 totalBytes, qreal progress)
{
    QString output="-----"+QString::number(progress * 100, 'f', 2) + "%    "+ QString::number(bytesRead / (1024 * 1024), 'f', 2) + "MB" + "/" + QString::number(totalBytes / (1024 * 1024), 'f', 2) + "MB"+"-----";
    QString type="process";
    emit taskOutputSignal(taskno,output,type);
}
