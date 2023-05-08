#ifndef TASKINSTALLPKG_H
#define TASKINSTALLPKG_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "taskbase.h"
#include "rpathinput.h"

class AppXmler;
class TaskInstallPkg: public TaskBase
{
    Q_OBJECT
public:
    explicit TaskInstallPkg(QObject *parent = 0);

    void doTask(AppXmler* xml,QString appType,QString sourceTagName,QString appdataPath);

signals:

public slots:

private:
    RPathInput* m_RPathInput;
};
#endif // TASKINSTALLPKG_H
