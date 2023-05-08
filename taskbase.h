#ifndef TASKBASE_H
#define TASKBASE_H
#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>


class TaskBase:public QObject
{
    Q_OBJECT
public:
    explicit TaskBase(int taskno,QObject *parent = 0);

signals:
    void taskFinishSignal(int taskno);
    void taskFinishSignal(int taskno,int exitCode);
    void taskErrorSignal(int taskno,QString error);
    void taskOutputSignal(int taskno,QString output, QString type);

public slots:
    void onTaskFinished(int exitCode);
    void onTaskError(QString error);
    void onTaskOutput(QString output, QString type);
    void onTaskOutput(qint64 bytesRead, qint64 totalBytes, qreal progress);
protected:
    int taskno;
};

#endif // TASKBASE_H
