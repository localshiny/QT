#include "taskinstallpkg.h"
#include "taskbase.h"
#include "appxmler.h"
#include "rprocess.h"

TaskInstallPkg::TaskInstallPkg(QObject *parent) : TaskBase(5,parent)
{
}
void TaskInstallPkg::doTask(AppXmler* xml,QString appType,QString sourceTagName ,QString appdataPath)
{
    QString rPackageResource=xml->getXmlMapElement(sourceTagName);
    QString fileName = rPackageResource.mid(rPackageResource.lastIndexOf("/") + 1, rPackageResource.length() - rPackageResource.lastIndexOf("/") - 1);
    QString newpkgRHome = appdataPath + "/R/" + fileName.mid(0,fileName.lastIndexOf("."));

    if (appType == "standalone")
    {
        newpkgRHome += "/bin";
        emit taskFinishSignal(taskno);
    }
    else
    {
        QString install_pkg = QCoreApplication::applicationDirPath();
        install_pkg += "/dependence/install_pkg.R";
        RProcess *process = new RProcess();
        connect(process,SIGNAL(processFinishSignal(int)),this,SLOT(onTaskFinished(int)));
        connect(process,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onTaskOutput(QString,QString)));
        connect(process,SIGNAL(processErrorSignal(QString)),this,SLOT(onTaskError(QString)));
        m_RPathInput=new RPathInput;
        m_RPathInput->putRPath(newpkgRHome);
        QStringList argsEnvironment;
        argsEnvironment.append("rHome=" + newpkgRHome);
        process->setEnvironment(argsEnvironment);
        QStringList args;
        args.append(install_pkg);
        QString program="Rscript";
        process->run(program, args);
    }
}

