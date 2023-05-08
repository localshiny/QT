//
//
// FileName: installform.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "urlinstallform.h"
#include "ui_urlinstallform.h"
#include  "QInputDialog.h"

urlInstallForm::urlInstallForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::urlInstallForm)
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

//    m_timer = new QTimer(this);
    ui->downloadBar->hide();
    ui->expandLabel->hide();
    ui->installButton->setEnabled(false);

    connect(ui->urlLineEdit, &QLineEdit::textChanged, this, [=]{
        ui->getButton->setEnabled(true);
    });
    connect(m_appxmler,SIGNAL(updatePackageInfoSignal(QString,QString,QString,QString,QString)),this,SLOT(onUpdatePackageInfo(QString,QString,QString,QString,QString)));
    connect(m_appxmler,SIGNAL(processOutputSignal(QString,QString)),this,SLOT(onProcessOutput(QString,QString)));
}

urlInstallForm::~urlInstallForm()
{
    delete ui;
}

void urlInstallForm::setUrl(QString url)
{
    ui->urlLineEdit->setText(url);
    on_getButton_clicked();
}

void urlInstallForm::on_getButton_clicked()
{
    ui->detailTextEdit->clear();
    QString xmlUrl = ui->urlLineEdit->text();
    if(xmlUrl.isEmpty())
    {
        return;
    }
    m_appID = xmlUrl.mid(xmlUrl.lastIndexOf("/")+1,(xmlUrl.length()-xmlUrl.lastIndexOf("/") - 1));
    m_appxmler->m_xmlName = m_appID;
    m_appxmler->m_xmlUrl = xmlUrl;
    ui->getButton->setEnabled(false);
    m_appxmler->downloadXml();
}

void urlInstallForm::on_installButton_clicked()
{
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
    ui->getButton->setEnabled(false);
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
    if (m_record->getValue(m_rGroupName+"/downloadStats") !="true")
    {
        m_outputMessage = "Begin to download "+m_rGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartRDownload(1);
    }
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
    else if((m_record->getValue(m_rtoolsGroupName+"/downloadStats")!="true")&&osName=="Windows")
    {
        m_outputMessage = "Begin to download "+m_rtoolsGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartRtoolsDownload(3);
    }
    else if ((m_record->getValue(m_rtoolsGroupName+"/installStats") !="true")&&osName=="Windows")
    {
        m_outputMessage = "Begin to install "+m_rtoolsGroupName;
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartRtoolsInstall(4);
    }
    else if(m_record->getValue("pkg/installStats") !="true")
    {
        m_outputMessage = "Begin to install pkg";
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartPkgInstall(5);
    }
    else
    {
        m_outputMessage = "Begin to install app";
        QMessageBox::information(NULL, "Infomation", m_outputMessage, QMessageBox::Ok, QMessageBox::Ok);
        onStartAppInstall(6);
    }
    }
    hideInInstall();
}


void urlInstallForm::onStartRDownload(int taskno)
{
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
void urlInstallForm::onPermissionGet()
{
    m_record->writeLine(m_rGroupName,m_RHome,true,false,m_rVersion);
    connect(this,SIGNAL(inputFinishedSignal(int)),this,SLOT(onStartRInstall(int)));
    QInputDialog *inputDialog = new QInputDialog(this);
    bool getInfo;
    m_password= inputDialog->getText(this,"Password","Please input password to get admin's permission",QLineEdit::Password,"",&getInfo,Qt::WindowFlags(0),Qt::ImhHiddenText);
    if (getInfo)
    {
        emit inputFinishedSignal(2);
    }
    else
    {
        QMessageBox::critical(NULL, "Error", "You have't input password,the installation is stopped", QMessageBox::Ok, QMessageBox::Ok);
        m_stats="";
    }
}
void urlInstallForm::onStartRInstall(int taskno)
{
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

void urlInstallForm::onStartRtoolsDownload(int taskno)
{
    m_record->changeKey(m_rGroupName,"installStats",true);
    m_RtoolsDownloader=new TaskRtoolsDownload();
    emit outputSignal("Begin to download Rtools.","Info");
    connect(m_RtoolsDownloader,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_RtoolsDownloader,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartRtoolsInstall(int)));
    connect(m_RtoolsDownloader,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_RtoolsDownloader,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_RtoolsDownloader->doTask(m_appxmler,m_rtoolsTagName,m_appdataPath);
}

void urlInstallForm::onStartRtoolsInstall(int taskno)
{
    m_record->writeLine(m_rtoolsGroupName,m_RHome,true,false,m_rVersion);
    m_RtoolsInstaller = new TaskRtoolsInstall();
    emit outputSignal("Begin to install Rtools.","Info");
    connect(m_RtoolsInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_RtoolsInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartPkgInstall(int)));
    connect(m_RtoolsInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_RtoolsInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_RtoolsInstaller->doTask(m_appxmler,m_rtoolsTagName,m_appdataPath);
}
void urlInstallForm::onStartPkgInstall(int taskno)
{

#ifdef Q_OS_WIN
    m_record->changeKey(m_rtoolsGroupName,"installStats",true);
#else
    m_record->changeKey(m_rGroupName,"installStats",true);
#endif
    m_PkgInstaller=new TaskInstallPkg();
    emit outputSignal("Begin to install Pkg.","Info");
    connect(m_PkgInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_PkgInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onStartAppInstall(int)));
    connect(m_PkgInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_PkgInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_PkgInstaller->doTask(m_appxmler,m_preferred,m_sourceTagName,m_appdataPath);
}
void urlInstallForm::onStartAppInstall(int taskno)
{
    m_record->writeLine("pkg",m_RHome,true,true,m_appxmler->getXmlMapElement("install_localShiny_localShinyVersion"));
    m_AppInstaller=new TaskInstallApp(this);
    emit outputSignal("Begin to install APP.","Info");
    connect(m_AppInstaller,SIGNAL(taskFinishSignal(int,int)),this,SLOT(onFinishedProcess(int,int)));
    connect(m_AppInstaller,SIGNAL(taskFinishSignal(int)),this,SLOT(onInstallAppFinished(int)));//onProcessFinished
    connect(m_AppInstaller,SIGNAL(taskErrorSignal(int,QString)),this,SLOT(onProcessError(int,QString)));
    connect(m_AppInstaller,SIGNAL(taskOutputSignal(int,QString,QString)),this,SLOT(onProcessOutput(int,QString,QString)));
    m_AppInstaller->doTask(m_appxmler,m_sourceTagName,m_appdataPath);
}

void urlInstallForm::hideInInstall()
{
    hide();
    ui->urlLineEdit->clear();
    ui->detailTextEdit->clear();
    ui->installButton->setEnabled(false);
    ui->getButton->setEnabled(true);
    ui->downloadBar->hide();
    ui->expandLabel->hide();
}

void urlInstallForm::on_cancelButton_clicked()
{
    hideInInstall();
    m_newpkgName = "";
    m_appxmler->resetArgs();
    m_appxmler->clearXmlReader();
}

void urlInstallForm::onUpdateBar(int taskno,QString out)
{
    QRegExp regexp("\\d+");
    int pos = regexp.indexIn(out);
    if(pos != -1)
    {
        m_i++;
        if(m_i > 8)
        {
            m_i--;
            int value = regexp.cap(0).toInt();
            if((value >= 0) && (value <= 100))
            {
                if(ui->downloadBar->value() < value)
                {
                    QString info = "update bar's value to " + QString::number(value);
                    qDebug()<<info;
                    ui->downloadBar->setValue(value);
                    ui->downloadBar->setFormat(tr("downloading : %1%").arg(QString::number(ui->downloadBar->value())));
                }
            }
        }
    }
}

void urlInstallForm::onUpdateExpand()
{
    ui->expandLabel->show();
}

void urlInstallForm::onUpdateExpandLabel()
{
    QString text = "expanding";
    if(m_j > 3)
    {
        m_j = 0;
    }
    for(int i = 0; i < m_j ; i++)
    {
        text+=".";
    }
    m_j++;
    ui->expandLabel->setText(text);
}

void urlInstallForm::onFinishDownload()
{
    ui->downloadBar->hide();
    ui->downloadBar->reset();
}


void urlInstallForm::onInstallAppFinished(int taskno)
{
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
    //QString appID = m_installer->m_appID;
    QString oldpkgName;
    QStringList info = m_record->readLine(m_appID,false);
    m_record->writeLine(appName,version,m_RHome,m_appID,m_rtoolsHome,m_newpkgRHome,m_appxmler->getXmlMapElement("app_run_Rcommand"));
    if (result==QMessageBox::Yes)
    {
        emit reloadSignal();
    }
    if (info.isEmpty())
    {
        oldpkgName = "";
    }
    else
    {
        oldpkgName = info[0];
    }
    emit newpkgSignal(m_appID,m_newpkgName, m_oldappName);
    on_cancelButton_clicked();
    m_stats="";
}

void urlInstallForm::onUpdatePackageInfo(QString packageName, QString packageTitle, QString packageVersion, QString packageTime, QString packageDetails)
{
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

void urlInstallForm::onFinishedProcess(int taskno,int exitCode)
{
    if (exitCode!=0)
    {
        QMessageBox::information(NULL, "Infomation", "Installation is failed", QMessageBox::Ok, QMessageBox::Ok);
    }
    m_stats="";
}

void urlInstallForm::onProcessError(int taskno,QString error)
{
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

void urlInstallForm::onProcessOutput(QString output,QString type)
{
    emit outputSignal(output, type);
}

void urlInstallForm::onProcessOutput(int taskno,QString output,QString type)
{
    emit outputSignal(output, type);
}

void urlInstallForm::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        on_cancelButton_clicked();
    }
    event->ignore();
}

