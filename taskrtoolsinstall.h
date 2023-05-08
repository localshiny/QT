#ifndef TASKRTOOLSINSTALL_H
#define TASKRTOOLSINSTALL_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "taskbase.h"

class AppXmler;
class TaskRtoolsInstall: public TaskBase
{
    Q_OBJECT
public:

    explicit TaskRtoolsInstall(QObject *parent = 0);
    void doTask(AppXmler* xml,QString rtoolsTagName,QString appdataPath);

signals:

public slots:

};

#endif // TASKRTOOLSINSTALL_H
