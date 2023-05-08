#ifndef TASKRTOOLSDOWNLOAD_H
#define TASKRTOOLSDOWNLOAD_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "taskbase.h"
#include "rpathinput.h"

class AppXmler;
class TaskRtoolsDownload :  public TaskBase
{
    Q_OBJECT
public:

    explicit TaskRtoolsDownload(QObject *parent = 0);

    void doTask(AppXmler* xml,QString rtoolsTagName,QString appdataPath);
private:  
    RPathInput* m_RPathInput;
signals:

public slots:

};
#endif // TASKRTOOLSDOWNLOAD_H
