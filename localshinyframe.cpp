//
//
// FileName: LocalShinyFrame.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
// Description: this class is used to create interface and menu bar, and to change page.
//
#include "localshinyframe.h"
#include "ui_localshinyframe.h"
#include "urlinstallform.h"
#include<string>

using namespace std;

LocalShinyFrame::LocalShinyFrame(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LocalShinyFrame)
{
    ui->setupUi(this);
    //disable
    qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "7001");

    setAttribute(Qt::WA_QuitOnClose, true);
    createIcon();
    showMaximized();

    init();

    createMenu();// Create menu
    createDockWidget();// Create interface
    createWebEngine();// Accessing web pages

    loadApp();
}

LocalShinyFrame::~LocalShinyFrame()
{
    onStopControlUI();
    delete ui;
}

void LocalShinyFrame::init()
{
    // create url installer to Perform the operation of obtaining an app through a webpage or through xml document
    m_installform = new urlInstallForm();
    connect(m_installform,SIGNAL(newpkgSignal(QString,QString,QString)),this,SLOT(addNewpkg(QString,QString,QString)));
    connect(m_installform,SIGNAL(outputSignal(QString,QString)),this,SLOT(onConsoleOutput(QString,QString)));

    // create about and option action in menu
    m_about = new About();
    m_optionform = new Option();
    connect(m_optionform,SIGNAL(option_changed()),this,SLOT(changeInterface()));

    // m_record is the app.ini document which records downloaded app information.
    m_record = new Record();

    // m_rManager used to manage robject so that users can choose app to run.
    m_rManager = new RManager();

    // m_webTab let the web be a tab,so that users can change and close page freely.
    m_webTab=new rTabWidget();
    connect(m_webTab,SIGNAL(currentChanged(int)),this,SLOT(onChanTab(int)));
    connect(m_webTab,SIGNAL(tabCloseSignal(QString)),this,SLOT(onCloseCurrentApp(QString)));

    // m_webView is the main page in tabs, this page can't be closed.
    m_webView = new RWebEngineView;

    // create navigationBar
    m_webLayout = new QVBoxLayout;
    m_navigationBar = addToolBar(tr("Navigation"));
    m_urlLine = new QLineEdit;
    connect(m_urlLine,SIGNAL(textChanged(QString)),this,SLOT(changeUrlLineEnable(QString)));
    m_progressBar = new QProgressBar(this);
    m_dockWidget = new QDockWidget(tr("Console Output"));
    m_consoleOutputText = new QTextEdit;

    // init some variable
    m_locationPath = QCoreApplication::applicationDirPath();
    m_configPath = m_locationPath + "/config/options.ini";
    m_setting = new QSettings(m_configPath, QSettings::IniFormat);
    m_optionMap = m_optionform->getOptions();
    m_appdataPath = m_optionMap["file/defaultPath"];
    m_installform->m_appdataPath = m_appdataPath;
    m_homePage = m_optionMap["server/address"];
    if (m_homePage.indexOf("https") == -1)
        m_homePage = "https://" + m_homePage;
}

void LocalShinyFrame::initFinished(int exitCode)
{
    Q_UNUSED(exitCode);
    QMessageBox::about(NULL, "Information", "The software is initialized!");
}

void LocalShinyFrame::createMenu()
{
    // define menu
    m_mainMenu = menuBar();
#ifdef Q_OS_MAC
    m_mainMenu->setNativeMenuBar(false);
#endif
    //Create toplevel menu
    m_fileMenu = new QMenu(tr("&File"));
    m_appMenu = new QMenu(tr("&App"));
    m_viewMenu = new QMenu(tr("&View "));
    m_installAppsMenu = new QMenu(tr("Install apps"));
    m_installedAppsMenu = new QMenu(tr("Installed apps"));
    m_deleteAppsMenu = new QMenu(tr("Delete apps"));

    //file_menu
    m_optionsAction = new QAction(tr("Options"));
    connect(m_optionsAction,SIGNAL(triggered(bool)),this,SLOT(onOptions()));
    m_fileMenu->addAction(m_optionsAction);
    m_exitAction = new QAction(tr("Exit"));
    connect(m_exitAction,SIGNAL(triggered(bool)),this,SLOT(onStopControlUI()));
    m_fileMenu->addAction(m_exitAction);
    m_mainMenu->addMenu(m_fileMenu);

    //app_menu
    m_installAppsFromUrlAction=new QAction(tr("install apps from url"));
    connect(m_installAppsFromUrlAction,SIGNAL(triggered(bool)),this,SLOT(onInstallApps()));
    m_installAppsFromLocalAction=new QAction(tr("install apps from local"));
    connect(m_installAppsFromLocalAction,SIGNAL(triggered(bool)),this,SLOT(onInstallAppsFromLocal()));
    m_appMenu->addMenu(m_installAppsMenu);
    m_installAppsMenu->addAction(m_installAppsFromUrlAction);
    m_installAppsMenu->addAction(m_installAppsFromLocalAction);
    m_appMenu->addMenu(m_installedAppsMenu);
    m_appMenu->addMenu(m_deleteAppsMenu);
    m_mainMenu->addMenu(m_appMenu);

    //view_menu
    m_back = new QAction(tr("Back"));
    m_viewMenu->addAction(m_back);
    m_forward = new QAction(tr("Forward"));
    m_viewMenu->addAction(m_forward);
    m_consoleAction = new QAction(tr("Console Output"));
    m_viewMenu->addAction(m_consoleAction);
    m_inspectorAction = new QAction(tr("Inspector"));
    m_viewMenu->addAction(m_inspectorAction);
    m_homePageAction = new QAction(tr("LocalShiny Page"));
    m_viewMenu->addAction(m_homePageAction);
    m_mySpace = new QAction(tr("My Space"));
    m_viewMenu->addAction(m_mySpace);
    m_large = new QAction(tr("Zoom &In"));
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_large);
    m_small = new QAction(tr("Zoom &Out"));
    m_viewMenu->addAction(m_small);
    m_viewMenu->addSeparator();
    m_viewToolbarAction = new QAction(tr("Hide Toolbar"));
    m_viewToolbarAction->setShortcut(tr("Ctrl+\\"));
    connect(m_viewToolbarAction, &QAction::triggered, [this]() {
        if (m_navigationBar->isVisible()) {
            m_viewToolbarAction->setText(tr("Show Toolbar"));
            m_navigationBar->close();
        } else {
            m_viewToolbarAction->setText(tr("Hide Toolbar"));
            m_navigationBar->show();
        }
    });
    m_viewStatusbarAction = new QAction(tr("Hide Status Bar"));
    m_viewStatusbarAction->setShortcut(tr("Ctrl+|"));
    connect(m_viewStatusbarAction, &QAction::triggered, [this]() {
        if (ui->statusbar->isVisible()) {
            m_viewStatusbarAction->setText(tr("Show Status Bar"));
            ui->statusbar->close();
        } else {
            m_viewStatusbarAction->setText(tr("Hide Status Bar"));
            ui->statusbar->show();
        }
    });
    m_viewMenu->addAction(m_viewStatusbarAction);
    m_viewMenu->addAction(m_viewToolbarAction);
    m_mainMenu->addMenu(m_viewMenu);

    //about_menu
    m_aboutAction = new QAction(tr("About"));
    connect(m_aboutAction,SIGNAL(triggered(bool)),this,SLOT(onAbout()));
    m_mainMenu->addAction(m_aboutAction);
}

void LocalShinyFrame::createWebEngine()
{

    QWidget* centralWidget = new QWidget(this);

    m_webLayout->setSpacing(0);
    m_webLayout->setMargin(0);
    m_webTab->setTabsClosable(true);

    m_webTab->addNewTab("LocalShiny",m_webView);
    m_webTab->tabBar()->setTabButton(0,QTabBar::RightSide,NULL);
    m_webTab->setTabBarAutoHide(1);


//*************************** create tool bar *********************************//
    m_navigationBar->toggleViewAction()->setEnabled(false);
    m_navigationBar->setMovable(false);

    // back
    m_back->setIcon(QIcon(QStringLiteral(":/image/images/back.png")));
    connect(m_back,&QAction::triggered,m_webView,&RWebEngineView::back);
    m_navigationBar->addAction(m_back);

    // forward
    m_forward->setIcon(QIcon(QStringLiteral(":/image/images/forward.png")));
    connect(m_forward,&QAction::triggered,m_webView,&RWebEngineView::forward);
    m_navigationBar->addAction(m_forward);

    // reload
    m_stopReload = new QAction(tr("Reload Page"));
    m_stopReload->setIcon(QIcon(QStringLiteral(":/image/images/reload.png")));


    connect(m_stopReload, &QAction::triggered, [this]() {
        if (m_stopReload->text() == "reload")
        {
            m_webView->setHttpUserAgent();
            m_webView->reload();
            m_currentWebView->reload();
        }
        else
            m_webView->stop();
    });
    connect(m_installform,&urlInstallForm::reloadSignal,[this]() {
            m_webView->setHttpUserAgent();
            setLocalHtml();
            m_webView->reload();
    });
    m_navigationBar->addAction(m_stopReload);

    // urlLine
    m_urlLine->setReadOnly(true);
    m_urlLine->setFont(QFont("Arial", 12));
    m_urlLine->setEnabled(false);
    connect(m_urlLine,&QLineEdit::textChanged,[=]{
        m_back->setEnabled(m_webView->page()->action(QWebEnginePage::Back)->isEnabled());
        m_forward->setEnabled(m_webView->page()->action(QWebEnginePage::Forward)->isEnabled());
    });

    m_navigationBar->addWidget(m_urlLine);
    int size = m_urlLine->sizeHint().height();
    m_navigationBar->setIconSize(QSize(size, size));

    // large
    m_large->setIcon(QIcon(QStringLiteral(":/image/images/large.png")));
    connect(m_large,&QAction::triggered,m_currentWebView,[=]{
        if (m_currentWebView->zoomFactor() < 4.0)
       {
            m_currentWebView->setZoomFactor(m_currentWebView->zoomFactor() + 0.1);
            if (!m_small->isEnabled())
                m_small->setEnabled(true);
        }
        else
            m_large->setEnabled(false);
    });
    m_navigationBar->addAction(m_large);

    //small
    m_small->setIcon(QIcon(QStringLiteral(":/image/images/small.png")));
    connect(m_small,&QAction::triggered,m_currentWebView,[=]{
        if (m_currentWebView->zoomFactor() > 0.5)
        {
            m_currentWebView->setZoomFactor(m_currentWebView->zoomFactor() - 0.1);
            if (!m_large->isEnabled())
                m_large->setEnabled(true);
        }
        else
            m_small->setEnabled(false);
    });
    m_navigationBar->addAction(m_small);

    //mySpace
    m_mySpace->setIcon(QIcon(QStringLiteral(":/image/images/myspace.png")));
    m_mySpace->setIconVisibleInMenu(true);
    connect(m_mySpace,SIGNAL(triggered(bool)),this,SLOT(onOpenLocalHtml()));
    m_navigationBar->addAction(m_mySpace);

    //homePage
    m_homePageAction->setIcon(QIcon(QStringLiteral(":/image/images/logo.png")));
    m_homePageAction->setIconVisibleInMenu(false);
    connect(m_homePageAction,&QAction::triggered,[this](){
       m_webView->loadPage(QUrl(m_homePage));
       m_webTab->setCurrentWidget(m_webView);
    });
    m_navigationBar->addAction(m_homePageAction);
    m_navigationBar->show();

    connect(m_webView,&RWebEngineView::loadProgress,this,&LocalShinyFrame::progressChanged);

    // url changed the urlline will change
    connect(m_webView,&RWebEngineView::urlChanged,[=](){
            if(m_webView->url().isValid())
                m_urlLine->setText(m_webView->url().toString());
        });
//******************************* end create *************************************//

//*************************** create progress bar *********************************//
    m_progressBar->setMaximumHeight(1);
    m_progressBar->setTextVisible(false);
    m_progressBar->setStyleSheet(QStringLiteral("QProgressBar {border: 0px } QProgressBar::chunk { background-color: red; }"));
//******************************* end create *************************************//

//*************************** create side bar *********************************//
    ui->dockWidgetBar->toggleViewAction()->setEnabled(false);
    ui->dockWidgetBar->setMovable(false);

    m_consoleAction->setIconVisibleInMenu(false);
    m_consoleAction->setShortcut(QKeySequence(Qt::Key_F11));
    connect(m_consoleAction ,&QAction::triggered,this,[=](){
        m_dockWidget->show();
    });
    ui->dockWidgetBar->hide();
//******************************* end create *************************************//

//******************************* create Inspector *************************************//
    m_inspector = new RInspector();
    m_inspector->setWindowTitle(tr("Inspector"));
    this->addDockWidget(Qt::RightDockWidgetArea, m_inspector);
    if (m_optionMap["console_output/position"] == "right")
        splitDockWidget(m_inspector,m_dockWidget,Qt::Horizontal);
    m_inspector->hide();
    m_inspectorAction->setShortcut(QKeySequence(Qt::Key_F12));
    connect(m_inspectorAction ,&QAction::triggered,this,[=](){
        m_inspector->show();
    });
//********************************** end create ***************************************//

//******************************* make webchannel *************************************//

    // make a channel to recieve signal from page
    QWebChannel* channel = new QWebChannel;

    // install app
    m_autoInstall = new RAutoInstall;
    connect(m_autoInstall,SIGNAL(installStart(QString)),this,SLOT(onAutoInstall(QString)));
    channel->registerObject("qt_autoInstall", m_autoInstall);

    // delete app
    m_autoDelete = new RAutoDelete;
    connect(m_autoDelete,SIGNAL(deleteStart(QString)),this,SLOT(onDeleteApp(QString)));
    channel->registerObject("qt_autoDelete", m_autoDelete);

    // run app
    m_autoRun = new RAutoRun;
    connect(m_autoRun,SIGNAL(runStart(QString)),this,SLOT(onOpenApp(QString)));
    channel->registerObject("qt_autoRun", m_autoRun);
    // open MySpace
    m_autoOpenMySpace=new RAutoOpenMySpace;
    connect(m_autoOpenMySpace,SIGNAL(openMySpaceStart()),this,SLOT(onOpenLocalHtml()));
    channel->registerObject("qt_autoOpenMySpace", m_autoOpenMySpace);

    // load js
    m_webView->page()->setWebChannel(channel);

    // channel js
    QFile file(":/qtwebchannel/qwebchannel.js");
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "Couldn't load Qt's QWebChannel API!";
    QString apicode = QString::fromLatin1(file.readAll());
    file.close();

    // install, delete, run eventlisentner
    file.setFileName(":/qtwebchannel/autoinstall.js");
    if (!file.open(QIODevice::ReadOnly))
        qDebug() << "Couldn't load Auto Install javascript!";
    QString auincode = QString::fromLatin1(file.readAll());
    file.close();

    // once page changed the js need to load again
    connect(m_webView, &RWebEngineView::loadFinished,[=]{
        QString url = m_webView->url().toString();
        QString last_part = url.mid(url.lastIndexOf("/") + 1, url.length() - url.lastIndexOf("/") - 1);
        QStringList source_code;
        source_code << apicode;
        source_code << auincode;
        m_webView->page()->runJavaScript(source_code.join("\n"));});

//***************************** end make webchannel ***********************************//
    // set Layout
    m_webLayout->addWidget(m_progressBar);
    m_webLayout->addWidget(m_webTab);
    centralWidget->setLayout(m_webLayout);
    setCentralWidget(centralWidget);

    // get fileName
    connect(m_webView->page()->profile(), &QWebEngineProfile::downloadRequested, [this](QWebEngineDownloadItem *download) {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), download->path());
        if (fileName.isEmpty())
            return;

        download->setPath(fileName);
        qDebug() << download->path() << download->savePageFormat();
        download->accept();
    });

    connect(m_webView->page(), &QWebEnginePage::linkHovered, [=](QString url){
        ui->statusbar->showMessage(url) ;
    });
}

void LocalShinyFrame::createDockWidget()
{
    // create consoleOutput
    m_consoleOutputText->setReadOnly(true);
    m_dockWidget->setWidget(m_consoleOutputText);
    m_dockWidget->setAllowedAreas(Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);
    m_dockWidget->setFeatures(QDockWidget::DockWidgetClosable);
    if (m_optionMap["console_output/position"] == "right")
        this->addDockWidget(Qt::RightDockWidgetArea, m_dockWidget);
    else
        this->addDockWidget(Qt::BottomDockWidgetArea, m_dockWidget);
}

void LocalShinyFrame::createIcon()
{
    // set logo
    QIcon icon(":/image/images/logo.png");
    setWindowIcon(icon);
    setWindowTitle(QStringLiteral("LocalShiny"));
}


void LocalShinyFrame::progressChanged(int progress)
{
    // creat progress bar
    if(progress==100)
    {
        m_webView->window()->setWindowTitle(QString("LocalShiny"));
    }
    else
    {
        m_webView->window()->setWindowTitle(QString("%1%").arg(progress) + QString(" - LocalShiny"));
    }
    static QIcon stopIcon(QStringLiteral(":/image/images/stop.png"));
    static QIcon reloadIcon(QStringLiteral(":/image/images/reload.png"));

    if (progress < 100 && progress > 0)
    {
        m_stopReload->setText(QString("stop"));
        m_stopReload->setIcon(stopIcon);
        m_stopReload->setToolTip(tr("Stop loading the current page"));
    }
    else
    {
        m_stopReload->setText(QString("reload"));
        m_stopReload->setIcon(reloadIcon);
        m_stopReload->setToolTip(tr("Reload the current page"));
    }
    m_progressBar->setValue(progress < 100 ? progress : 0);
}

void LocalShinyFrame::loadApp()
{
    // load App through r object
    QString appIniFile = QCoreApplication::applicationDirPath() + "/config/app.ini";
    QStringList groupList=m_record->getSection();
    QSettings settings(appIniFile, QSettings::IniFormat);
    foreach (QString group, groupList)
    {
        qDebug()<<group;       
        if (m_record->getValue(group+"/appName")!="")
        {
            QString appName=m_record->getValue(group+"/appName");
            RObject* r_object = new RObject(group,appName,m_appdataPath);
            connect(r_object,SIGNAL(openUrlSignal(QString)),this,SLOT(openUrl(QString)));
//            connect(r_object,&RObject::appClosed,this,&LocalShinyFrame::changePage);
            connect(r_object,SIGNAL(outputSignal(QString,QString)),this,SLOT(onConsoleOutput(QString,QString)));
            m_rManager->addR(appName,r_object);
            addPkgMenu(appName);
        }
    }
    QString initApp = m_setting->value("initApp").toString();
    if (initApp != "")
    {
        QList<QAction*> actList = m_installedAppsMenu->actions();
        for (int i = 0; i < actList.length(); i++)
            if (actList.at(i)->text() == initApp)
            {
                actList.at(i)->trigger();
                break;
            }
    }
    else

    {
        m_webView->loadPage(QUrl(m_homePage));
        m_webTab->setCurrentWidget(m_webView);
    }

}
void LocalShinyFrame::addNewpkg(QString newpkgID,QString newpkgName, QString oldpkgName)
{
    // when add new package if there is a old version, then delete it
    qDebug()<<"addNewpkg";
    if (oldpkgName != "")
    {
        deleteApp(oldpkgName);
    }
    QString appName = newpkgName;
    QString info = m_record->getValue(newpkgID+"/appName");
    addPkgMenu(info);
    RObject* r_object = new RObject(newpkgID,appName,m_appdataPath);
    connect(r_object,&RObject::openUrlSignal,this,&LocalShinyFrame::openUrl);
    connect(r_object,SIGNAL(outputSignal(QString,QString)),this,SLOT(onConsoleOutput(QString,QString)));
    m_rManager->addR(appName,r_object);
    m_installform->m_appxmler->clearXmlReader();
}

void LocalShinyFrame::addPkgMenu(QString appName)
{
    // add new actions for new package
    QAction* appAction = new QAction(appName, m_installedAppsMenu);
    QAction* appDeleteAction = new QAction(appName, m_deleteAppsMenu);
    connect(appAction,SIGNAL(triggered(bool)),this,SLOT(onOpenApp()));
    connect(appDeleteAction,SIGNAL(triggered(bool)),this,SLOT(onDeleteApp()));
    m_installedAppsMenu->addAction(appAction);
    m_deleteAppsMenu->addAction(appDeleteAction);
}

void LocalShinyFrame::onOpenApp()
{
    // when action clicked then start r
    QAction* appAction = (QAction*)sender();
    QString appName = appAction->text();
    RObject* r_object = m_rManager->find(appName);
    r_object->start();
}

void LocalShinyFrame::onOpenApp(QString appID)
{
    // when open a app in mySpace then start r
    QString appName=m_record->getValue(appID+"/appName");
    RObject* r_object = m_rManager->find(appName);
    r_object->start();
}

void LocalShinyFrame::onDeleteApp()
{
    // when delete action clicked then delete app
    QAction* appDeleteAction = (QAction*)sender();
    QString appName = appDeleteAction->text();
    for (int i=0;i<=m_webTab->count();i++)
    {
        if(m_webTab->tabText(i)==appName)
        {
            QString outputMessage = "It cannot be recovered once deleted. Are you sure to delete " + appName + "?";
            int isClose=QMessageBox::question(NULL, "Confirm", "The app is running,do you want to stop it?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (isClose==QMessageBox::No)
                return;
            m_webTab->onRemoveTab(i);
        }
    }
    QString outputMessage = "It cannot be recovered once deleted. Are you sure to delete " + appName + "?";
    int ret = QMessageBox::question(NULL, "Confirm", outputMessage, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::No)
        return ;
    deleteApp(appName);
    m_deleteAppsMenu->removeAction(appDeleteAction);
}

void LocalShinyFrame::deleteApp(QString appName)
{

    m_rManager->deleteR(appName);
    QString appID = m_record->getAppID(appName, true);
    QString path = m_record->getValue(appID+"/rHome");
    QString last_position = path.mid(path.lastIndexOf("/")+1,(path.length() - path.lastIndexOf("/") - 1));

    path = path.mid(0, path.lastIndexOf("/"));
    if (last_position == "bin")
    {
        path = path.mid(0, path.lastIndexOf("/"));
    }
    else
    {
#ifdef Q_OS_WIN
        path = path.mid(0, path.lastIndexOf("/"));
#endif
        path = path.mid(path.lastIndexOf("/")+1,(path.length() - path.lastIndexOf("/") - 1));
        path = m_appdataPath + "/ShinyApps/" + appID;
    }
    QDir *temp = new QDir;
    if (temp->exists(path))
    {
        QDir qDir(path);
        qDir.removeRecursively();
    }
    else
    {
         QMessageBox::information(NULL, "Infomation","The app doesn/'t exist." , QMessageBox::Ok, QMessageBox::Ok);
    }
    QList<QAction*> actList = m_installedAppsMenu->actions();
    for (int i = 0; i < actList.length(); i++)
    {
        if (actList.at(i)->text() == appName)
        {
            m_installedAppsMenu->removeAction(actList.at(i));
            break;
        }
    }
    m_record->deleteLine(appName);
    m_webView->setHttpUserAgent();
    m_webView->reload();
}

void LocalShinyFrame::onDeleteApp(QString appID)
{
    // when delete a app in mySpace, there will be a appID, then delete it according to the appID
    QString appName = m_record->getValue(appID+"/appName");
    QString outputMessage = "It cannot be recovered once deleted. Are you sure to delete " + appName + "?";
    for (int i=0;i<=m_webTab->count();i++)
    {
        if(m_webTab->tabText(i)==appName)
        {
            QString outputMessage = "It cannot be recovered once deleted. Are you sure to delete " + appName + "?";
            int isClose=QMessageBox::question(NULL, "Confirm", "The app is running,do you want to stop it?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            if (isClose==QMessageBox::No)
                return;
            m_webTab->onRemoveTab(i);
        }
    }
    int ret = QMessageBox::question(NULL, "Confirm", outputMessage, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (ret == QMessageBox::No)
        return ;
    deleteApp(appName);
    QList<QAction*> actList=m_deleteAppsMenu->actions();
    for (int i = 0; i < actList.length(); i++)
    {
        if (actList.at(i)->text() == appName)
        {
            m_deleteAppsMenu->removeAction(actList.at(i));
            break;
        }
    }
    if(m_webView->url().toString().lastIndexOf("local.html")!=-1)
    {
        setLocalHtml();
    }
}

void LocalShinyFrame::onInstallApps()
{
    m_installform->setWindowModality(Qt::ApplicationModal);
    m_installform->show();
}

void LocalShinyFrame::onAbout()
{
    m_about->setWindowModality(Qt::ApplicationModal);
    m_about->show();
}

void LocalShinyFrame::onOptions()
{
    m_optionform->setWindowModality(Qt::ApplicationModal);
    m_optionform->show();
}

void LocalShinyFrame::onStopControlUI()
{
    m_setting->sync();
    m_rManager->killAllRObject();
    qApp->quit();
}

void LocalShinyFrame::onOpenLocalHtml()
{
    //open mySpace
    setLocalHtml();
    m_webView->loadPage(QUrl(QCoreApplication::applicationDirPath() +"/html/local.html"));
    m_webTab->setCurrentWidget(m_webView);
}

void LocalShinyFrame::setLocalHtml()
{
    // init local.html
    QStringList groupList=m_record->getSection();
    QFile htmlfile(QCoreApplication::applicationDirPath() +"/html/head.html");
    if (!htmlfile.open(QIODevice::ReadOnly))
        qDebug() << "Couldn't load headhtml!";
    QString html = QString::fromLatin1(htmlfile.readAll());
    htmlfile.close();
    QString startTag="<!-- %FORSTART% -->";
    QString endTag="<!-- %FOREND% -->";
    QString section=html.mid(html.indexOf(startTag)+startTag.length(),html.indexOf(endTag)-html.indexOf(startTag)-startTag.length());
    QString sections;
    html=replaceTag(html,"%HOMEPAGE%",m_optionMap["server/address"]);

    foreach(QString group,groupList)
    {
        if(m_record->getValue(group+"/appName")!="")
        {
            QString appName=m_record->getValue(group+"/appName");
            QString version=m_record->getValue(group+"/version");
            QString newpkgHome=m_record->getValue(group+"/newpkgHome");
            QString idTag="%GROUP%";
            QString nameTag="%APPNAME%";
            QString versionTag="%VERSION%";
            QString newpkgHomeTag="%NEWPKGHOME%";
            QString sectionModule=section;
            sectionModule=replaceTag(sectionModule,idTag,group);
            sectionModule=replaceTag(sectionModule,nameTag,appName);
            sectionModule=replaceTag(sectionModule,versionTag,version);
            sectionModule=replaceTag(sectionModule,newpkgHomeTag,newpkgHome);
            sections=sections+sectionModule+"\n";
        }
    }
    if(sections!="")
    {
        html=replaceTag(html,"%VISIBILITY%","style='visibility:hidden'");
    }
    else
    {
        html=replaceTag(html,"%VISIBILITY%","");
    }
    html=html.mid(0,html.indexOf(startTag))+sections+html.mid(html.indexOf(endTag),html.length()-html.indexOf(endTag));
    QFile localHtml(QCoreApplication::applicationDirPath() +"/html/local.html");
    if(localHtml.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream out(&localHtml);
        out<<html;
    }
    localHtml.close();
}

QString LocalShinyFrame::replaceTag(QString str,QString tag,QString value)
{
    // replace tag in local.html
    if(str.indexOf(tag)!=-1)
    {
        str=str.mid(0,str.indexOf(tag))+value+str.mid(str.indexOf(tag)+tag.length(),str.length()-str.indexOf(tag)-tag.length());
        return replaceTag(str,tag,value);
    }
    else
        return str;
}

void LocalShinyFrame::changeUrlLineEnable(QString text)
{
    if (!text.isEmpty())
        m_urlLine->setEnabled(true);
    else
        m_urlLine->setEnabled(false);
}

void LocalShinyFrame::openUrl(QString appName)
{
    // open page to load app
    RObject* r_object = (RObject*)sender();
    QString url = r_object->m_url;
    QString out = "R App URL: " + url;
    this->showMaximized();
    int index;
    for(int i=0;i<=m_webTab->count();i++)
    {
        if(m_webTab->tabText(i)==appName)
        {
            index=i;
            m_webTab->setCurrentIndex(index);
            m_webTab->show();
            return;
        }
    }
    RWebEngineView *webView=new RWebEngineView;
    webView->loadPage(QUrl(url));
    index=m_webTab->addNewTab(appName,webView);
    webView->setUrl(QUrl(url));
    qDebug()<<webView->url().toString();
    m_webTab->setCurrentIndex(index);
    m_webTab->show();
}

void LocalShinyFrame::onConsoleOutput(QString output, QString type)
{
    // Processing output information

    qDebug()<<output;
    m_consoleOutputText->setFocus();
    QTextCursor storeCursorPos = m_consoleOutputText->textCursor();
    m_consoleOutputText->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    m_consoleOutputText->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    m_consoleOutputText->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
    QString last_line = m_consoleOutputText->textCursor().selectedText();
    while (last_line.indexOf("-----")==0)
    {
        m_consoleOutputText->textCursor().removeSelectedText();
        m_consoleOutputText->textCursor().deletePreviousChar();
        m_consoleOutputText->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
        m_consoleOutputText->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
        m_consoleOutputText->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
        last_line = m_consoleOutputText->textCursor().selectedText();
    }
    m_consoleOutputText->setTextCursor(storeCursorPos);
    QString finalOutput;
    if (type == "Start" || type == "Finish")
    {
        if (m_dockWidget->isHidden() && type == "Start")
        {
            m_dockWidget->show();
        }
        finalOutput = QObject::tr("<font color = blue>%1</font>").arg(output);
    }
    else if(type == "Info")
    {
        finalOutput = QObject::tr("<font color = green>%1</font>").arg(output);
    }
    else if(type == "Error")
    {
        finalOutput = QObject::tr("<font color = red>%1</font>").arg(output);
    }
    else
    {
        finalOutput = QObject::tr("<font color = black>%1</font>").arg(output);
    }
    if (output.indexOf("=")==0)
    {
        m_consoleOutputText->insertPlainText(output);
    }
    else
    {
        m_consoleOutputText->append(finalOutput);
    }

}

void LocalShinyFrame::onAutoInstall(QString url)
{
    // Transfer the downloaded information to the installform
    if(m_installform->m_stats=="running")
    {
        QMessageBox::critical(NULL, "Error", "Another app is installing", QMessageBox::Ok, QMessageBox::Ok);
    }
    else
    {
        m_installform->setWindowModality(Qt::ApplicationModal);
        m_installform->show();
        m_installform->setUrl(url);
    }
}

void LocalShinyFrame::onCloseCurrentApp(QString appName)
{    
    // when tab closed then close app
    QString url = m_webTab->getWebEngineView(appName)->url().toString();
    m_rManager->closeR(url);
}

void LocalShinyFrame::onInstallAppsFromLocal()
{
    // when install app from local action clicked creat a new installform
    m_localinstallform=new localInstallForm();
    connect(m_localinstallform,SIGNAL(newpkgSignal(QString,QString,QString)),this,SLOT(addNewpkg(QString,QString,QString)));
    connect(m_localinstallform,SIGNAL(outputSignal(QString,QString)),this,SLOT(onConsoleOutput(QString,QString)));
    connect(m_localinstallform,&localInstallForm::reloadSignal,[this]() {
            m_webView->setHttpUserAgent();
            m_webView->reload();
    });
    m_localinstallform->m_appdataPath = m_appdataPath;
    m_localinstallform->setWindowModality(Qt::ApplicationModal);
    m_localinstallform->show();
}

void LocalShinyFrame::onChanTab(int index)
{
    // when tab changed, change currentWebView
    m_currentWebView=m_webTab->getWebEngineView(m_webTab->tabText(index));
    m_urlLine->setText(m_currentWebView->url().toString());
    QString appName=m_webTab->tabText(index);
}


void LocalShinyFrame::changeInterface()
{
    // change interface's layout
    m_optionMap = m_optionform->getOptions();
    m_appdataPath = m_optionMap["file/defaultPath"];
    m_installform->m_appdataPath = m_appdataPath;
    m_homePage = m_optionMap["server/address"];
    if (m_homePage.indexOf("http") == -1)
        m_homePage = "http://" + m_homePage;
    if (m_optionMap["console_output/position"] == "right")
    {
        this->addDockWidget(Qt::RightDockWidgetArea, m_dockWidget);
        splitDockWidget(m_inspector,m_dockWidget,Qt::Horizontal);
    }
    else
        this->addDockWidget(Qt::BottomDockWidgetArea, m_dockWidget);
}




