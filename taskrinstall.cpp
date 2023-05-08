#include "taskrinstall.h"
#include "taskbase.h"
#include "appxmler.h"
#include "rprocess.h"

TaskRInstall::TaskRInstall(QObject *parent) : TaskBase(2,parent)
{
}
void TaskRInstall::doTask(AppXmler* xml,QString appType,QString sourceTagName,QString appdataPath,QString password)
{

    QString path = QCoreApplication::applicationDirPath();
    path += "/dependence";
    QString program;
    QStringList args;

    RProcess* process = new RProcess();
    connect(process,SIGNAL(processFinishSignal(int)),this,SLOT(onTaskFinished(int)));
    connect(process,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onTaskOutput(QString,QString)));
    connect(process,SIGNAL(processErrorSignal(QString)),this,SLOT(onTaskError(QString)));
    QString rPackageResource=xml->getXmlMapElement(sourceTagName);
    QString fileName = rPackageResource.mid(rPackageResource.lastIndexOf("/") + 1, rPackageResource.length() - rPackageResource.lastIndexOf("/") - 1);


    if (appType == "localShiny")
    {
    #ifdef Q_OS_WIN
        program = appdataPath + "/R_installer/" + fileName;
        QString newpkgRHome = appdataPath + "/R/" + fileName.mid(0,fileName.lastIndexOf("."));
        args.append("/VERYSILENT");
        args.append("/DIR=" +newpkgRHome);
    #elif defined Q_OS_LINUX
        RProcess* pro = new RProcess();
        connect(pro,processFinishSignal(int)),this,SLOT(onRinstallFinished(int)))
        connect(pro,SIGNAL(processErrorSignal(QString)),this,SLOT(onTaskError(QString)));
        connect(pro,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onTaskOutput(QString,QString)));

        QString R_version =fileName.mid(0, fileName.indexOf(".tar.gz"));
        QString newpkgRHome = appdataPath + "/R/" + R_version;

        program = path + "/prepare_file.sh";
        args.append(appdataPath);
        args.append(R_version);
        args.append(path);
        pro->exrun(program,args);

        program = path + "/install_dependence.sh";
        pro->exrun(program);

        program = path +  "/install_R.sh";
        QString rInstallerPath = appdataPath+ "/R_installer/" + R_version;
        args.clear();
        args.append(appdataPath);
        args.append(R_version);
        process->setWorkDirectory(rInstallerPath);
    #else
        QString R_version =fileName.mid(0, fileName.indexOf(".pkg"));
        program="sh "+path+"/install_R_os.sh "+appdataPath+" "+R_version+" "+password;
    #endif
    }
    else
    {
    #ifdef Q_OS_WIN
        program = path + "/unzip/unzip.exe";
    #else
        program = "unzip";
    #endif
        QString downloadPath = QDir::tempPath() + "/" + fileName;
        QString expandPath = appdataPath + "/ShinyApps/" + fileName.mid(0,fileName.lastIndexOf("."));
        QDir *temp = new QDir;
        if (temp->exists(expandPath))
        {
            QDir qDir(expandPath);
            qDir.removeRecursively();
        }
        temp->mkdir(expandPath);
        args.append("-d");
        args.append(expandPath);
        args.append(downloadPath);
    }
#ifdef Q_OS_MAC
    process->run(program);
#else
    process->run(program,args);
#endif
}

