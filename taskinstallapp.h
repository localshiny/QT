#ifndef TASKINSTALLAPP_H
#define TASKINSTALLAPP_H
#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "taskbase.h"
#include "rpathinput.h"
#include "rprocess.h"
class AppXmler;

class TaskInstallApp: public TaskBase
{
    Q_OBJECT
public:
    explicit TaskInstallApp(QObject *parent = 0);

    void doTask(AppXmler* xml,QString sourceTagNname,QString appdataPath);
    void doTask(AppXmler* xml,QString sourceTagName,QString appdataPath,QString localFile);
    QString getNewpkgRHome(AppXmler* xml,QString sourceTagName,QString appdataPath);
    void runProgram(QString order,QString newpkgRHome);
signals:

public slots:

private:
    RPathInput* m_RPathInput;
    RProcess* m_process;
};

#endif // TASKINSTALLAPP_H
