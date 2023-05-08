#ifndef TASKRDOWNLOAD_H
#define TASKRDOWNLOAD_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "taskrinstall.h"
#include "taskbase.h"
#include "rpathinput.h"


class AppXmler;
class TaskRDownload:  public TaskBase
{
    Q_OBJECT
public:
    TaskRInstall* m_RInstaller;
    explicit TaskRDownload(QObject *parent = 0);

    void doTask(AppXmler *xml,QString sourceTagName,QString appdataPath);
private:
    RPathInput* m_RPathInput;
signals:


public slots:


private:
};

#endif // TASKRINSTALL_H
