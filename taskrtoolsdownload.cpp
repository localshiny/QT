#include "taskrtoolsdownload.h"
#include "taskbase.h"
#include "appxmler.h"
#include "downloadtool.h"

TaskRtoolsDownload::TaskRtoolsDownload(QObject *parent) : TaskBase(3,parent)
{
    m_RPathInput=new RPathInput;
}


void TaskRtoolsDownload::doTask(AppXmler* xml,QString rtoolsTagName,QString appdataPath)
{

    QString rtoolsResource=xml->getXmlMapElement(rtoolsTagName);
    QString fileName = rtoolsResource.mid(rtoolsResource.lastIndexOf("/") + 1, rtoolsResource.length() - rtoolsResource.lastIndexOf("/") - 1);
    QString rtoolsDir=appdataPath + "/Rtools_installer";
    QString rtoolsName = rtoolsResource.mid(rtoolsResource.lastIndexOf("/") + 1, rtoolsResource.length() - rtoolsResource.lastIndexOf("/") - 1);
    QString rtoolsFile=rtoolsDir+"/"+rtoolsName;

    QFileInfo fileInfo(rtoolsFile);
    if(fileInfo.isFile())
    {
        emit taskOutputSignal(taskno,rtoolsName+" have been downloaded","Info");
        emit taskFinishSignal(taskno);
        return;
    }

    DownloadTool* dT;
    dT = new DownloadTool(rtoolsResource,rtoolsDir);

    connect(dT,SIGNAL(processFinishSignal(int)),this,SLOT(onTaskFinished(int)));
    connect(dT,SIGNAL(sigProgress(qint64,qint64,qreal)),this,SLOT(onTaskOutput(qint64,qint64,qreal)));
    dT->startDownload();
}



