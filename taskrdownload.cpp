#include "taskrdownload.h"
#include "appxmler.h"
#include "downloadtool.h"

TaskRDownload::TaskRDownload(QObject *parent) : TaskBase(1,parent)
{

}
void TaskRDownload::doTask(AppXmler *xml, QString sourceTagName ,QString appdataPath)//download_R
{
    QString rPackageResource=xml->getXmlMapElement(sourceTagName);
    if(rPackageResource.isEmpty())
    {
        emit taskErrorSignal(taskno,"the R doesnt support");
        return;
    }
    QString rDir=appdataPath + "/R_installer";
    QString rName = rPackageResource.mid(rPackageResource.lastIndexOf("/") + 1, rPackageResource.length() - rPackageResource.lastIndexOf("/") - 1);
    QString rFile=rDir+"/"+rName;
    QFileInfo fileInfo(rFile);
    if(fileInfo.isFile())
    {
        emit taskOutputSignal(taskno,rName+" have been downloaded","Info");
        emit taskFinishSignal(taskno);
        return;
    }

    DownloadTool* dT;
    dT = new DownloadTool(rPackageResource,rDir);
    connect(dT,SIGNAL(processFinishSignal(int)),this,SLOT(onTaskFinished(int)));
    connect(dT,SIGNAL(sigProgress(qint64,qint64,qreal)),this,SLOT(onTaskOutput(qint64,qint64,qreal)));
    dT->startDownload();
}
