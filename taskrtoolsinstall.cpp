#include "taskrtoolsinstall.h"
#include "taskbase.h"
#include "appxmler.h"
#include "rprocess.h"

TaskRtoolsInstall::TaskRtoolsInstall(QObject *parent) : TaskBase(4,parent)
{

}


void TaskRtoolsInstall::doTask(AppXmler* xml,QString rtoolsTagName,QString appdataPath)
{
    RProcess *process = new RProcess();
    connect(process,SIGNAL(processFinishSignal(int)),this,SLOT(onTaskFinished(int)));
    connect(process,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onTaskOutput(QString,QString)));
    connect(process,SIGNAL(processErrorSignal(QString)),this,SLOT(onTaskError(QString)));

    QString rtoolsResource=xml->getXmlMapElement(rtoolsTagName);
    QString rtools_name = rtoolsResource.mid(rtoolsResource.lastIndexOf("/") + 1, rtoolsResource.length() - rtoolsResource.lastIndexOf("/") - 1);
    QString rtools_dir = appdataPath + "/Rtools/" + rtools_name.mid(0,rtools_name.lastIndexOf("."));
    QString program = appdataPath + "/Rtools_installer/" + rtools_name;
    QStringList args;
    QDir *temp = new QDir;
    if (temp->exists(rtools_dir))
    {
        QDir qDir(rtools_dir);
        qDir.removeRecursively();
    }
    args.append("/VERYSILENT");
    args.append("/DIR=" + rtools_dir);
    process->run(program,args);
}
