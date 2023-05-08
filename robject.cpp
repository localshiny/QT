//
//
// FileName: robject.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "robject.h"

RObject::RObject(QString appID,QString appName, QString appdataPath,QObject *parent) : QObject(parent)
{
    m_processId = 0;
    m_url = "";
    m_appName = appName;
    m_appID=appID;
    m_appdataPath = appdataPath;
    m_record = new Record();
    m_process = new QProcess();
    connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(showFinished(int,QProcess::ExitStatus)));
    connect(m_process,&QProcess::readyReadStandardOutput,this,&RObject::showOut);
    connect(m_process,&QProcess::readyReadStandardError,this,&RObject::showError);
}

void RObject::start()
{
    if(m_processId <= 0)
    {
        startProcess();
    }
    else
    {
        if(m_url != "")
        {
            emit openUrlSignal(m_appName);
        }
    }
}

void RObject::startProcess()
{
    QString rHome = getRHome();
    QString programPath;
    QString program="Rscript";
    QString appID;
    QString newpkgHome;
    newpkgHome= m_record->getValue(m_appID+"/newpkgHome");
    QString runName = newpkgHome.mid(newpkgHome.lastIndexOf("/")+1, (newpkgHome.length() - newpkgHome.lastIndexOf("/") - 1));
    QString preferred;
    QString rtools_home;
    m_RPathInput->putRPath(rHome);
    if (runName == "bin")
    {
        preferred = "standalone";
        programPath = rHome;
    }
    else
    {
        preferred = "localShiny";
#ifdef Q_OS_WIN
        rtools_home = m_record->getValue(m_appID+"/rtoolsHome");
#else
        Q_UNUSED(rtools_home);
#endif
        appID = runName;
    }
    QProcessEnvironment env =QProcessEnvironment::systemEnvironment();
    env.insert("R_HOME",m_RPathInput->getRPath(rHome));
    env.insert("RTOOLS_HOME",rtools_home+"/usr/bin");
    env.insert("R_LIBS",rHome+"/library");
    m_process->setProcessEnvironment(env);
    QStringList args;
    QString appName = m_appName.mid(0, m_appName.lastIndexOf("-"));
    if (preferred == "standalone")
    {

        args.append("-e");
        args.append("library(" + appName + ");imShiny();");
    }
    else
    {
        QString runNamePath = m_appdataPath + "/ShinyApps/" + appID;
        m_process->setWorkingDirectory(runNamePath);
        QString runcmd =m_record->getValue(m_appID+"/runcmd");
        runcmd=runcmd.mid(runcmd.lastIndexOf("/")+1, (runcmd.length() - runcmd.lastIndexOf("/") - 1));
        args.append(runNamePath + "/" + runcmd);
        QFile* temp = new QFile(runNamePath);
        if (!temp->exists())
        {
            QMessageBox::information(NULL, "Infomation","The ID of this app is wrong, can't find the file.", QMessageBox::Ok, QMessageBox::Ok);
        }
    }

    m_process->start(program,args);
    emit outputSignal("Starting: " + program + " " + args.join(" "), "Start");
    m_process->waitForStarted();
}

bool RObject::isValidUrl(QString url)
{
    if(url.length() <= 17)
    {
        return false;
    }
    if(url.mid(0,17) == "http://127.0.0.1:")
    {
        url = url.mid(17,url.length()-17);
        if(url.toInt() > 0)
        {
            return true;
        }
    }
    return false;
}

void RObject::killRObject()
{

    QString cmd = QString("taskkill /F /PID %1 /T").arg(m_processId);
    m_process->kill();
    m_process->start(cmd);
    m_process->waitForFinished();
    m_process->close();
}

QString RObject::getRHome()
{
    QString rHome = m_record->getValue(m_appID+"/rHome");
    return rHome;
}

void RObject::showFinished(int exitCode, QProcess::ExitStatus exit_status)
{
    Q_UNUSED(exit_status);
    emit appClosed();
    QString out = "R program has ended !";
    emit outputSignal(out, "Out");
    out = m_process->program() + " " + m_process->arguments().join(" ") + " exited with code " + QString::number(exitCode) + "\n";
    emit outputSignal(out, "Finish");
    qDebug()<<out;
    m_processId = 0;
}

void RObject::showOut()
{
    QString out = QString::fromLocal8Bit(m_process->readAllStandardOutput().data());
    emit outputSignal(out, "Out");
    qDebug() << "Out: " + out;
}

void RObject::showError()
{
    QString error = QString::fromLocal8Bit(m_process->readAllStandardError().data());
    emit outputSignal(error, "Error");
    QString out = error;
    if(error.indexOf("http://127.0.0.1") == -1)
    {
        return;
    }
    QString url = error.mid(error.indexOf("http://127.0.0.1"),error.length()-error.indexOf("http://127.0.0.1"));
    url = url.replace("\r","").replace("\n","");
    if(isValidUrl(url))
    {
        m_processId = m_process->processId();
        m_url = url;
        emit openUrlSignal(m_appName);
    }
}
