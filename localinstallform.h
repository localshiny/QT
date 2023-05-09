#ifndef LOCALINSTALLFORM_H
#define LOCALINSTALLFORM_H
#include <QWidget>
#include <QProcess>
#include <QProgressBar>
#include <QCloseEvent>

#include <QApplication>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QUrl>
#include <QtNetwork>
#include <QTextCodec>
#include <QRegExp>
#include <QRegExpValidator>
#include <QMessageBox>

#include "taskrdownload.h"
#include "appxmler.h"
#include "record.h"
#include "taskrinstall.h"
#include "taskrtoolsdownload.h"
#include "taskrtoolsinstall.h"
#include "taskinstallpkg.h"
#include "taskinstallapp.h"
#include "rpathinput.h"
#include <QWidget>

namespace Ui {
class localInstallForm;
}

class localInstallForm : public QWidget
{
    Q_OBJECT

public:
    explicit localInstallForm(QWidget *parent = 0);
    ~localInstallForm();
    AppXmler* m_appxmler;             //download and parse xml
    TaskRDownload* m_RDownloader;
    TaskRInstall* m_RInstaller;
    TaskRtoolsDownload* m_RtoolsDownloader;
    TaskRtoolsInstall* m_RtoolsInstaller;
    TaskInstallPkg* m_PkgInstaller;
    TaskInstallApp* m_appInstaller;
    RPathInput* m_RPathInput;
    QString m_appdataPath;
    QString m_rtoolsResource;
    QString m_appID;
    QString m_preferred;
    QString m_sourceTagName;
    QString m_rtoolsTagName;
    QString m_rGroupName;
    QString m_rVersion;
    QString m_rtoolsGroupName;
    QString m_password;
    QString m_stats;
    QString m_RHome;
    QString m_rtoolsHome;
    QString m_expandPath;
    QString m_fileName;
    QString m_localfile;
    QString m_oldappName;

private slots:
    void on_openButton_clicked();
    void on_cancelButton_clicked();
    void onFinishDownload();
    void onUpdatePackageInfo(QString packageName, QString packageTitle, QString packageVersion, QString packageTime, QString packageDetails);
    void onFinishedProcess(int taskno,int exitCode);
    void onProcessError(int taskno,QString error);
    void onProcessOutput(int taskno,QString output, QString type);
    void onProcessOutput(QString output,QString type);
    void onStartRDownload(int taskno);
    void onStartRInstall(int taskno);
    void onStartRtoolsDownload(int taskno);
    void onStartRtoolsInstall(int taskno);
    void onStartPkgInstall(int taskno);
    void onStartAppInstall(int taskno);
    void on_installButton_clicked();
    void onPermissionGet();
    void onInstallAppFinished(int taskno);
    void onChooseXmlFile(int exitCode);
    void unzipFile(QString fileName);

signals:
    void newpkgSignal(QString newpkgID,QString newpkgName, QString oldpkgName);
    void outputSignal(QString output, QString type);
    void inputFinishedSignal(int taskno);
    void reloadSignal();

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::localInstallForm *ui;
    Record* m_record;             //write and read app.txt
    QString m_newpkgName;      //the name of new R package
    QString m_newpkgRHome;    //rHome of new R package
    QString m_outputMessage;   //Information about download status
    void hideInInstall();
};

#endif // LOCALINSTALLFORM_H
