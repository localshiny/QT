//
//
// FileName: rwebengineview.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "rwebengineview.h"
#include "localshinyframe.h"
#include <QWidget>
#include <QMessageBox>
#include <QMainWindow>

RWebEngineView::RWebEngineView(QWidget *parent) : QWebEngineView(parent)
{
    connect(this->page(),&QWebEnginePage::linkHovered,this,&RWebEngineView::linkHovered);
}

void RWebEngineView::init(QUrl homePage)
{
    this->loadPage(homePage);
}

void RWebEngineView::loadPage(QUrl url)
{
    setHttpUserAgent();
    this->load(url);
}

QWebEngineView *RWebEngineView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);
    this->loadPage(m_newUrl);
    return 0;
}

void RWebEngineView::setHttpUserAgent()
{
    QString user_agent = this->page()->profile()->httpUserAgent();
    user_agent = user_agent.mid(0, user_agent.indexOf(" LocalShiny"));
    user_agent += " LocalShiny/";
    QString appIniFile = QCoreApplication::applicationDirPath() + "/config/app.ini";
    m_record = new Record();
    QStringList groupList=m_record->getSection();
    foreach (QString group, groupList)
    {
        if (m_record->getValue(group+"/appName")!="")
        {
            QString appID=group;
            QString version=m_record->getValue(group+"/version");
            user_agent += appID + "-" + version + ",";
        }
    }
    this->page()->profile()->setHttpUserAgent(user_agent);
}

void RWebEngineView::linkHovered(QString url)
{
    m_newUrl = QUrl(url);
}

