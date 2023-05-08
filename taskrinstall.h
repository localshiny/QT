#ifndef TASKRINSTALL_H
#define TASKRINSTALL_H

#include <QObject>
#include <QProcess>
#include <QApplication>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "taskbase.h"
#include "rpathinput.h"

class AppXmler;
class TaskRInstall :public TaskBase
{
    Q_OBJECT
public:

    explicit TaskRInstall(QObject *parent = 0);
    void doTask(AppXmler* xml,QString appType,QString sourceTagName,QString appdataPath,QString password);

signals:

public slots:
private:
    RPathInput* m_RPathInput;
    QString m_input;
};

#endif // TASKRINSTALL_H
