#include "taskinstallapp.h"
#include "rprocess.h"
#include "appxmler.h"

TaskInstallApp::TaskInstallApp(QObject *parent) : TaskBase(6,parent)
{
}

void TaskInstallApp::doTask(AppXmler* xml,QString sourceTagName,QString appdataPath)
{
    QString newpkgRHome=getNewpkgRHome(xml,sourceTagName, appdataPath);
    QString appPath = appdataPath + "/ShinyApps";
    QString order = xml->getXmlMapElement("install_localShiny_Rcommand");

    order = order.mid(8, order.length() - 8);
    order.insert(order.lastIndexOf(")"), ",'" + appPath + "'");
    order.insert(order.indexOf("\""), "\"");
    order.insert(order.lastIndexOf("\""), "\"");
    QString program="Rscript "+order;
    runProgram(program,newpkgRHome);
}

void TaskInstallApp::doTask(AppXmler* xml,QString sourceTagName,QString appdataPath,QString localFile)
{
    QString newpkgRHome=getNewpkgRHome(xml,sourceTagName, appdataPath);
    QString appPath = appdataPath + "/ShinyApps";
    QString order = xml->getXmlMapElement("install_localShiny_Rcommand");
    order = order.mid(8, order.length() - 8);
    order.insert(order.lastIndexOf(")"), ",appPath='" + appPath + "'");
    order.insert(order.lastIndexOf(")"), ",localPath='" + localFile + "'");
    order.insert(order.indexOf("\""), "\"");
    order.insert(order.lastIndexOf("\""), "\"");
    QString program="Rscript "+order;
    runProgram(program,newpkgRHome);
}

QString TaskInstallApp::getNewpkgRHome(AppXmler* xml,QString sourceTagName,QString appdataPath)
{
    QString rPackageResource=xml->getXmlMapElement(sourceTagName);
    QString fileName = rPackageResource.mid(rPackageResource.lastIndexOf("/") + 1, rPackageResource.length() - rPackageResource.lastIndexOf("/") - 1);
    QString newpkgRHome = appdataPath + "/R/" + fileName.mid(0,fileName.lastIndexOf("."));
    return newpkgRHome;
}

void TaskInstallApp::runProgram(QString program,QString newpkgRHome)
{
    m_RPathInput=new RPathInput();
    m_RPathInput->putRPath(newpkgRHome);
    RProcess *process = new RProcess();
    connect(process,SIGNAL(processFinishSignal(int)),this,SLOT(onTaskFinished(int)));
    connect(process,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onTaskOutput(QString,QString)));
    connect(process,SIGNAL(processErrorSignal(QString)),this,SLOT(onTaskError(QString)));
    QProcessEnvironment env =QProcessEnvironment::systemEnvironment();
    env.insert("R_HOME",newpkgRHome+"/bin");
    env.insert("R_LIBS",newpkgRHome+"/library");

    process->setProcessEnvironment(env);;
    process->run(program);

}
