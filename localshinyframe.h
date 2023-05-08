#ifndef LOCALSHINYFRAME_H
#define LOCALSHINYFRAME_H

#include <QMainWindow>
#include <QWidget>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QProcess>
#include <QList>
#include <QObject>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDockWidget>
#include <QUrl>
#include <QTextCodec>
#include <QRegExp>
#include <QRegExpValidator>
#include <QWebEngineView>
#include <QtWebEngineWidgets>

#include "about.h"
#include "optionform.h"
#include "record.h"
#include "rmanager.h"
#include "rwebengineview.h"
#include "rinspector.h"
#include "rautoinstall.h"
#include "rautodelete.h"
#include "rautorun.h"
#include "rautoopenmyspace.h"
#include "urlinstallform.h"
#include "localinstallform.h"
#include "rTabWidget.h"
namespace Ui {
class LocalShinyFrame;
}

class LocalShinyFrame : public QMainWindow
{
    Q_OBJECT

public:
    explicit LocalShinyFrame(QWidget *parent = 0);
    void progressChanged(int progress);
    ~LocalShinyFrame();

signals:
    void init_url(QUrl url);

private slots:
    void onOpenApp();
    void onOpenApp(QString appID);
    void onDeleteApp();
    void onDeleteApp(QString appID);
    void onInstallApps();
    void onAbout();
    void onOptions();
    void onStopControlUI();
    void changeUrlLineEnable(QString text);
    void openUrl(QString appName);
    void addNewpkg(QString newpkgID,QString newpkgName, QString oldpkgName);
    void onConsoleOutput(QString output, QString type);
    void onAutoInstall(QString url);
    void onCloseCurrentApp(QString appName);
    void initFinished(int exitCode);
    void changeInterface();
    void onInstallAppsFromLocal();
    void onOpenLocalHtml();
    void onChanTab(int index);

private:
    Ui::LocalShinyFrame *ui;
    QMenuBar* m_mainMenu;
    QToolBar* m_navigationBar;
    QMenu* m_fileMenu;
    QMenu* m_appMenu;
    QMenu* m_viewMenu;
    QMenu* m_windowsMenu;
    QMenu* m_installedAppsMenu;
    QMenu* m_installAppsMenu;
    QMenu* m_deleteAppsMenu;

    QAction* m_installAppsFromUrlAction;
    QAction* m_installAppsFromLocalAction;//show installform
    QAction* m_checkForVersionAction;    //show checkform
    QAction* m_optionsAction;              //show option
    QAction* m_aboutAction;
    QAction* m_exitAction;      //stop program

    QAction* m_back;
    QAction* m_forward;
    QAction* m_stopReload;
    QAction* m_large;
    QAction* m_small;
    QAction* m_mySpace;
    QAction* m_consoleAction;
    QAction* m_inspectorAction;
    QAction* m_viewToolbarAction;
    QAction* m_viewSidebarAction;
    QAction* m_viewStatusbarAction;
    QAction* m_homePageAction;
    QAction* m_closeCurrentAction;

    QProgressBar* m_progressBar;
    QVBoxLayout* m_webLayout;
    QDockWidget* m_dockWidget;

    QMap<QString, QString> m_optionMap;
    QString m_appdataPath;
    QString m_configPath;
    QString m_locationPath;
    QSettings* m_setting;

    RWebEngineView* m_webView;
    RWebEngineView* m_currentWebView;
    QString m_homePage;
    QLineEdit* m_urlLine;
    QTextEdit* m_consoleOutputText;
    rTabWidget* m_webTab;

    urlInstallForm* m_installform;
    localInstallForm*m_localinstallform;
    About* m_about;
    Option* m_optionform;
    Record* m_record;
    RManager* m_rManager;
    RInspector* m_inspector;
    RAutoInstall* m_autoInstall;
    RAutoDelete* m_autoDelete;
    RAutoRun* m_autoRun;
    RAutoOpenMySpace* m_autoOpenMySpace;

    void initAppMenu();
    void init();                                         //Initialize global variables
    void createMenu();                                  //create menu
    void createIcon();                                  //create icon
    void createWebEngine();                            //create web engine
    void createDockWidget();                           //create dock widget
    void loadApp();                                     //load R package by app.txt when program start
    void addPkgMenu(QString appName);                 //add a new R package menu
    void setLocalHtml();
    void deleteApp(QString oldpkgName);
    QString replaceTag(QString str,QString tag,QString value);

};

#endif // LOCALSHINYFRAME_H
