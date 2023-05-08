//
//
// FileName: record.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "record.h"

Record::Record(QObject *parent) : QObject(parent)
{
    m_appIniFile = QCoreApplication::applicationDirPath() + "/config/app.ini";
}

QString Record::getAppID(QString tag, bool flag)
{

    QStringList groupList=getSection();
    foreach (QString group, groupList)
    {
        if(getValue(group+"/appName")==tag)
        {
            return group;
        }
    }
    return "";
}

QStringList Record::readLine(QString tag, bool flag)
{

    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    QStringList info;
    settings->beginGroup(tag);
    QStringList keyList=settings->childKeys();
    settings->endGroup();
    foreach (QString key, keyList)
    {
        info.append(settings->value(tag+"/"+key).toString());
    }
    delete settings;
    return info;
}

void Record::deleteLine(QString appName)
{
    // delete one group through appName
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    QStringList groupList=getSection();
    foreach (QString group, groupList)
    {
        qDebug()<<group;
        settings->beginGroup(group);
        QString hh=getValue("appName");
        if(settings->value("appName")==appName)
        {
            settings->endGroup();
            settings->remove(group);
            break;
        }
        settings->endGroup();
    }
    delete settings;
}

QStringList Record::getSection()
{
    // get all group's name
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    qDebug()<<m_appIniFile;
    QFile* iniFile = new QFile(m_appIniFile);
    QStringList groupList;
    if(!iniFile->open(QFile::ReadWrite))
    {
        qDebug()<<"open ini failed!";
        delete settings;
        return groupList;
    }
    delete iniFile;
    groupList = settings->childGroups();
    delete settings;
    return groupList;
}

void Record::writeLine(QString appName, QString version, QString rHome, QString appID,QString rtoolsHome,QString newpkgHome,QString runcmd)
{
    // record app's imformation
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    settings->beginGroup(appID);
    settings->setValue("appName", appName);
    settings->setValue("version", version);
    settings->setValue("rHome", rHome);
    settings->setValue("rtoolsHome", rtoolsHome);
    settings->setValue("newpkgHome", newpkgHome);
    settings->setValue("runcmd", runcmd);
    settings->endGroup();
    settings->sync();
    delete settings;
}

void Record::writeLine(QString dependenceName,QString rHome,bool downloadStats,bool installStats,QString version="4.0.5")
{
    // record app's imformation
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    settings->beginGroup(dependenceName);
    settings->setValue("version", version);
    settings->setValue("rHome", rHome);
    settings->setValue("downloadStats", downloadStats);
    settings->setValue("installStats", installStats);
    settings->endGroup();

    settings->sync();
    delete settings;
}

void Record::changeLine(QString dependenceInfo,QString rHome,bool downloadStats,bool installStats,QString version="4.0.5")
{
    // overwrite  one group
    deleteLine(dependenceInfo);
    writeLine(dependenceInfo,rHome,downloadStats,installStats,version="4.0.5");
}

void Record::changeLine(QString appName, QString version, QString rHome, QString appID,QString rtoolsHome,QString newpkgHome,QString runcmd)
{
    // overwrite  one group
    deleteLine(appID);
    writeLine(appName,version,rHome,appID,rtoolsHome,newpkgHome,runcmd);
}

void Record::changeKey(QString groupName,QString key, QString value)
{
    // change value in group
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    settings->beginGroup(groupName);
    settings->setValue(key, value);
    settings->endGroup();
    delete settings;
}

void Record::changeKey(QString groupName,QString key, bool value)
{
    // change value in group
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    settings->beginGroup(groupName);
    settings->setValue(key, value);
    settings->endGroup();
    delete settings;
}

QString Record::getValue(QString keyName)
{
    // Search for record information based on appID
    QSettings* settings=new QSettings(m_appIniFile,QSettings::IniFormat);
    QString value=settings->value(keyName).toString();
    delete settings;
    return value;
}

bool Record::appExist(QString appID)
{
    // check the record to confirm if the app exists
    QStringList groupList=getSection();
    foreach (QString group, groupList)
    {
        if(appID==group)
        {
            return true;
        }
    }
    return false;
}
