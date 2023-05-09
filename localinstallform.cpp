#include "localinstallform.h"
#include "QtWidgets/QInputDialog.h"
#include "ui_localinstallform.h"
#include "qfiledialog.h"

localInstallForm::localInstallForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::localInstallForm)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowTitle("Install packages");
    setFont(QFont("Arial"));
    this->setFixedSize(QSize(930, 610));
    hide();

    m_newpkgName = "";
    m_appxmler = new AppXmler();
    m_record = new Record();

    ui->downloadBar->hide();
    ui->expandLabel->hide();
    ui->installButton->setEnabled(false);

    connect(ui->pathLineEdit, &QLineEdit::textChanged, this, [=]{
        ui->openButton->setEnabled(true);
    });
    connect(m_appxmler,SIGNAL(updatePackageInfoSignal(QString,QString,QString,QString,QString)),this,SLOT(onUpdatePackageInfo(QString,QString,QString,QString,QString)));
    connect(m_appxmler,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onProcessOutput(QString,QString)));
}

localInstallForm::~localInstallForm()
{
    delete ui;
}

void localInstallForm::on_openButton_clicked()
{
    // open file when open button clicked
    QString fileName = QFileDialog::getOpenFileName(this,tr("open a file."),
#ifdef Q_OS_WIN
        "C:/",
#else
        "/",
#endif
        tr("compressed files(*.zip);;All files(*.*)"));
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the file!");
    }
    else {
        m_stats="running";
        ui->pathLineEdit->setText(fileName);
        if (fileName.mid(fileName.lastIndexOf(".")+1,fileName.length()-fileName.lastIndexOf("."))=="zip")
        {
            unzipFile(fileName);
            m_localfile=fileName;
        }
    }

}

void localInstallForm::on_installButton_clicked()
{
    // when install clicked then get app's imformation to begin install
    m_preferred = m_appxmler->getXmlMapElement("app_install_preferred");

    QString osName;
    QString rtoolsVersion = "";
    QString loadway;
    QString appVerTagName;
    QString m_rtools_resource;

    if (m_preferred == "localShiny")
    {
        loadway = "rportable";
        appVerTagName = "install_" + m_preferred + "_pakVersion";
    }
    else
    {
        loadway = m_preferred;
        appVerTagName = m_preferred + "_" + osName + "_pakVersion";
    }

#if defined(Q_OS_WIN64)
    osName = "Windows";
    rtoolsVersion = "Rtools64";
#elif defined(Q_OS_WIN32)
    osName = "Windows";
    rtoolsVersion = "Rtools32";
#elif defined(Q_OS_LINUX)
    osName = "Ubuntu";
    Q_UNUSED(rtoolsVersion);
#else
    osName = "macOS";
    Q_UNUSED(rtoolsVersion);
#endif
    m_sourceTagName = loadway + "_" + osName + "_url";
    m_rtoolsGroupName=rtoolsVersion;
    m_rtoolsTagName=loadway+"_"+osName+"_"+rtoolsVersion;

    QString rPackageResource = m_appxmler->getXmlMapElement(m_sourceTagName);//R resource
    QString newVersion = m_appxmler->getXmlMapElement(appVerTagName);
    QString appName = m_appxmler->getXmlMapElement("app_head_pakname")+"-"+newVersion;
    m_newpkgName=m_appxmler->getXmlMapElement("app_head_pakname");
    QString recordAppID = m_record->getAppID(appName,true);
    QString appID=m_appxmler->getXmlMapElement("app_head_pakid");
    bool appState=m_record->appExist(appID);
    QString fileName = rPackageResource.mid(rPackageResource.lastIndexOf("/") + 1, rPackageResource.length() - rPackageResource.lastIndexOf("/") - 1);
    QString newpkgRHome = m_appdataPath + "/R/" + fileName.mid(0,fileName.lastIndexOf("."));
    m_RPathInput=new RPathInput;
    QString rtoolsResource=m_appxmler->getXmlMapElement(m_rtoolsTagName);
    QString rtoolsName = rtoolsResource.mid(rtoolsResource.lastIndexOf("/") + 1, rtoolsResource.length() - rtoolsResource.lastIndexOf("/") - 1);
#ifdef Q_OS_MAC
    m_RHome=m_RPathInput->getRPath(newpkgRHome);
#else
    m_rtoolsHome = m_appdataPath + "/Rtools/" + rtoolsName.mid(0,rtoolsName.lastIndexOf("."));
    m_RHome=newpkgRHome;
#endif

    // if the app does not support this os, display a reminder message
    if(rPackageResource == "")
    {
        m_appxmler->clearXmlReader();
        m_outputMessage = "App " + appName + " does not support " + osName + ".";
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    m_appxmler->deleteXml();
    if (m_preferred == "localShiny")
    {
        m_rGroupName = rPackageResource.mid(rPackageResource.lastIndexOf("/") + 1,rPackageResource.lastIndexOf(".") - rPackageResource.lastIndexOf("/") - 1);
#ifdef Q_OS_WIN
#else
        m_rGroupName = m_rGroupName.mid(0, m_rGroupName.lastIndexOf(".")+2);

#endif
        m_rVersion=m_rGroupName.mid(2, m_rGroupName.lastIndexOf("."));
        QString packagePath = m_appdataPath + "/R/" + m_rGroupName;
        m_newpkgRHome=m_appdataPath+"/ShinyApps/"+m_appID;
    }

    ui->installButton->setEnabled(false);
    ui->openButton->setEnabled(false);
    ui->downloadBar->show();

    m_stats="running";

    // if the app has been installed, display a reminder message
    if(appState)
    {
        if (m_record->getValue(m_appID+"/version") == newVersion)
        {
            qDebug()<<"app "<<appName  <<" already install";
            m_appxmler->clearXmlReader();
            m_outputMessage = "App " + appName + "-" + newVersion + " already install.";
            QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
            m_stats="";
            return;
        }
        // if the app could update, then update it
        else
        {
            m_oldappName=m_record->getValue(m_appID+"/appName");
            QString oldPackagePath=m_record->getValue(m_appID+"/newpkgHome");
            m_record->deleteLine(m_oldappName);
            QDir *temp = new QDir;
            if (temp->exists(oldPackagePath))
            {
                QDir qDir(oldPackagePath);
                qDir.removeRecursively();
            }
        }
    }

    if (!appState||(appState&&m_record->getValue(m_appID+"/version") != newVersion))
    {
    // if R have't downloaded, then download it
    if (m_record->getValue(m_rGroupName+"/downloadStats") !="true")
    {
        m_outputMessage = "Begin to download "+m_rGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartRDownload(1);
    }

    // if R have't installed, then install it
    else if (m_record->getValue(m_rGroupName+"/installStats") !="true")
    {
        m_outputMessage = "Begin to install "+m_rGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
#ifdef Q_OS_MAC
        onPermissionGet();
#else
        onStartRInstall(2);
#endif
    }

    // if Rtools have't downloaded, then download it
    else if((m_record->getValue(m_rtoolsGroupName+"/downloadStats")!="true")&&osName=="Windows")
    {
        m_outputMessage = "Begin to download "+m_rtoolsGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartRtoolsDownload(3);
    }

    // if Rtools have't installed, then install it
    else if ((m_record->getValue(m_rtoolsGroupName+"/installStats") !="true")&&osName=="Windows")
    {
        m_outputMessage = "Begin to install "+m_rtoolsGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartRtoolsInstall(4);
    }
    // if localshiny and devtools have not installed,then install then
    else if(m_record->getValue("pkg/installStats") !="true")
    {
        m_outputMessage = "Begin to install pkg";
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartPkgInstall(5);
    }
    // if R, Rtools,packages all downloaded,then install app
    else
    {
        m_outputMessage = "Begin to install app";
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartAppInstall(6);
    }
    }
    hideInInstall();
}


void localInstallForm::onStartRDownload(int taskno)
{
    // download R
    m_RDownloader = new TaskRDownload();
    emit outputSignal("Begin to download R.","Info");
    connect(m_RDownloader,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
#ifdef Q_OS_MAC
    connect(m_RDownloader,SIGNAL(taskFinishSignal(int)),this,SLOT(onPermissionGet()));
#else
    connect(m_RDownloader,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartRInstall(int)));
#endif
    connect(m_RDownloader,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_RDownloader,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_RDownloader->doTask(m_appxmler,m_sourceTagName,m_appdataPath);
}
void localInstallForm::onPermissionGet()
{
    // get Permission, if os is macos
    m_record->writeLine(m_rGroupName,m_RHome,true,false,m_rVersion);
    connect(this,SIGNAL(inputFinishedSignal(int)),this,SLOT(onStartRInstall(int)));
    QInputDialog *inputDialog = new QInputDialog(this);
    bool getInfo;
    m_password= inputDialog->getText(this,"password","please input password to get admin's permission",QLineEdit::Password,"",&getInfo,Qt::WindowFlags(0),Qt::ImhHiddenText);
    if (getInfo)
    {
        emit inputFinishedSignal(2);
    }
    else
    {
        QMessageBox::critical(NULL, "Error", "you have't input password,the installation is stopped", QMessageBox::Ok, QMessageBox::Ok);
        m_stats="";
    }
}
void localInstallForm::onStartRInstall(int taskno)
{
    // install R
    m_RInstaller = new TaskRInstall();
    emit outputSignal("Begin to install R.","Info");
    connect(m_RInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
#ifdef Q_OS_WIN
    m_record->writeLine(m_rGroupName,m_RHome,true,false,m_rVersion);
    connect(m_RInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartRtoolsDownload(int)));
#else

    connect(m_RInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartPkgInstall(int)));
#endif
    connect(m_RInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_RInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_RInstaller->doTask(m_appxmler,m_preferred,m_sourceTagName,m_appdataPath,m_password);
}

void localInstallForm::onStartRtoolsDownload(int taskno)
{
    //download Rtools
    m_record->changeKey(m_rGroupName,"installStats",true);
    emit outputSignal("Begin to download Rtools.","Info");
    m_RtoolsDownloader=new TaskRtoolsDownload();
    connect(m_RtoolsDownloader,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_RtoolsDownloader,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartRtoolsInstall(int)));
    connect(m_RtoolsDownloader,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_RtoolsDownloader,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_RtoolsDownloader->doTask(m_appxmler,m_rtoolsTagName,m_appdataPath);
}

void localInstallForm::onStartRtoolsInstall(int taskno)
{
    //install Rtools   
    m_record->writeLine(m_rtoolsGroupName,m_RHome,true,false,m_rVersion);
    m_RtoolsInstaller = new TaskRtoolsInstall();
    emit outputSignal("Begin to install Rtools.","Info");
    connect(m_RtoolsInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_RtoolsInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartPkgInstall(int)));
    connect(m_RtoolsInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_RtoolsInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_RtoolsInstaller->doTask(m_appxmler,m_rtoolsTagName,m_appdataPath);
}
void localInstallForm::onStartPkgInstall(int taskno)
{
    // install packages
#ifdef Q_OS_WIN
    m_record->changeKey(m_rtoolsGroupName,"installStats",true);
#else
    m_record->changeKey(m_rGroupName,"installStats",true);
#endif
    m_PkgInstaller=new TaskInstallPkg();
    emit outputSignal("Begin to install install Pkg.","Info");
    connect(m_PkgInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_PkgInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartAppInstall(int)));
    connect(m_PkgInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_PkgInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_PkgInstaller->doTask(m_appxmler,m_preferred,m_sourceTagName,m_appdataPath);
}
void localInstallForm::onStartAppInstall(int taskno)
{
    // install App
    m_record->writeLine("pkg",m_RHome,true,true,m_appxmler->getXmlMapElement("install_localShiny_localShinyVersion"));
    m_appInstaller=new TaskInstallApp(this);
    emit outputSignal("Begin to install APP.","Info");
    connect(m_appInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_appInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onInstallAppFinished(int)));//onProcessFinished
    connect(m_appInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_appInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_appInstaller->doTask(m_appxmler,m_sourceTagName,m_appdataPath,m_expandPath+".zip");
}

void localInstallForm::hideInInstall()
{
    // when install begins, the ui should be hidden
    hide();
    ui->pathLineEdit->clear();
    ui->detailTextEdit->clear();
    ui->installButton->setEnabled(false);
    ui->openButton->setEnabled(true);
    ui->downloadBar->hide();
    ui->expandLabel->hide();
}

void localInstallForm::on_cancelButton_clicked()
{
    // cancel install
    hideInInstall();
    m_newpkgName = "";
    m_appxmler->resetArgs();
    m_appxmler->clearXmlReader();
}

void localInstallForm::onFinishDownload()
{
    ui->downloadBar->hide();
    ui->downloadBar->reset();
}


void localInstallForm::onInstallAppFinished(int taskno)
{
    // after installed app, add record and let localshinyframe to know it
    QMessageBox::StandardButton result;

    if (m_outputMessage.contains("***"))
    {
        QMessageBox::critical(NULL, "Error", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);

        return;
    }
    else if (m_outputMessage.isEmpty())
    {
        m_outputMessage = "Install app failed! Unknown error!";
        QMessageBox::critical(NULL, "Error", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
    {
        result=QMessageBox::question(this, "ReloadSelection",m_outputMessage+" Did you want to reload this page to refresh your install imformation");
    }
    QString version = m_appxmler->getXmlMapElement("install_localShiny_pakVersion");
    m_newpkgName += "-" + version;
    QString appName = m_newpkgName;
    QStringList info = m_record->readLine(m_appID,false);
    m_record->writeLine(appName,version,m_RHome,m_appID,m_rtoolsHome,m_newpkgRHome,m_appxmler->getXmlMapElement("app_run_Rcommand"));
    if (result==QMessageBox::Yes)
    {
        emit reloadSignal();
    }
    emit newpkgSignal(m_appID,m_newpkgName, m_oldappName);
    on_cancelButton_clicked();
    m_stats="";
}

void localInstallForm::onUpdatePackageInfo(QString packageName, QString packageTitle, QString packageVersion, QString packageTime, QString packageDetails)
{
    // process app's information
    m_newpkgName = packageName;

    ui->detailTextEdit->append("Package name: ");
    ui->detailTextEdit->append(packageName);
    ui->detailTextEdit->append("");
    ui->detailTextEdit->append("Package title: ");
    ui->detailTextEdit->append(packageTitle);
    ui->detailTextEdit->append("");
    ui->detailTextEdit->append("Package version: ");
    ui->detailTextEdit->append(packageVersion);
    ui->detailTextEdit->append("");
    ui->detailTextEdit->append("Package upload time: ");
    ui->detailTextEdit->append(packageTime);
    ui->detailTextEdit->append("");
    ui->detailTextEdit->append("Package details: ");
    ui->detailTextEdit->append(packageDetails);

    ui->installButton->setEnabled(true);
}

void localInstallForm::onFinishedProcess(int taskno,int exitCode)
{
    // when task finished, display message
    if (exitCode!=0)
    {
        QMessageBox::information(NULL, "Infomation", "Installation is failed", QMessageBox::Ok, QMessageBox::Ok);
    }
    m_stats="";
}

void localInstallForm::onProcessError(int taskno,QString error)
{
    // when tsak have error,display error
    int firstIndex, secIndex;
    if ((firstIndex = error.indexOf("***")) != -1)
    {
        secIndex = error.indexOf("\r\n", firstIndex);
        m_outputMessage = error.mid(firstIndex, secIndex - firstIndex);
    }

    if (error.indexOf("Done") != -1)
    {
        m_outputMessage = "You have installed the application successfully!";
    }
}

void localInstallForm::onProcessOutput(QString output,QString type)
{
    // emit signal and the localshinyframe will recieve it
    emit outputSignal(output, type);
}

void localInstallForm::onProcessOutput(int taskno,QString output,QString type)
{
    emit outputSignal(output, type);
}

void localInstallForm::closeEvent(QCloseEvent *event)
{
    // when ui closed, cancel installation
    if(isVisible())
    {
        on_cancelButton_clicked();
    }
    event->ignore();
}

void localInstallForm::unzipFile(QString fileName)
{
    // unzip local file to install app
    QString path = QCoreApplication::applicationDirPath();
    QString program;
#ifdef Q_OS_WIN
    program = path + "/dependence/unzip/unzip.exe";
#else
    program = "unzip";
#endif
    RProcess* process = new RProcess();
    connect(process,SIGNAL(processFinishSignal(int)),this,SLOT(onChooseXmlFile(int)));
    connect(process,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onProcessOutput(QString,QString)));
    QString downloadPath =fileName;
    m_expandPath = fileName.mid(0,fileName.lastIndexOf("."));
    QDir *temp = new QDir;
    QStringList args;
    if (temp->exists(m_expandPath))
    {
        QDir qDir(m_expandPath);
        qDir.removeRecursively();
    }
    temp->mkdir(m_expandPath);
    args.append("-d");
    args.append(m_expandPath);
    args.append(downloadPath);
    process->run(program,args);
}

void localInstallForm::onChooseXmlFile(int exitCode)
{
    // choose xml to get app's imformation
    if (exitCode!=0)
    {
        QMessageBox::information(NULL, "Infomation", "The file can't be unzipped ", QMessageBox::Ok, QMessageBox::Ok);
        m_stats="";
    }
    else
    {
        QDir dir(m_expandPath);
        QFileInfoList xmlist = dir.entryInfoList(QStringList() << "*.xml");
        if (xmlist.empty())
        {
            QMessageBox::information(NULL, "Infomation", "The zip file does't contains xml file. ", QMessageBox::Ok, QMessageBox::Ok);
        }
        m_appxmler->parseXml(xmlist[0].filePath());
        m_appID=m_appxmler->getXmlMapElement("app_head_pakid");
    }
}

