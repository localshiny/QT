//
//
// FileName: rmanager.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "rmanager.h"

RManager::RManager()
{
    m_record = new Record();
}

RObject* RManager::find(QString appName)
{
    return m_rMap[appName];
}

void RManager::addR(QString appName, RObject* r_object)
{
    m_rMap.insert(appName,r_object);
}

void RManager::killR(QMap<QString,RObject*>::iterator it)
{
    RObject* r_object = it.value();
    qint64 process_id = r_object->m_processId;
    if(process_id >0)
    {
        QString appName = r_object->m_appName;
        QString appID=r_object->m_appID;
        QString url = r_object->m_url;
        r_object->killRObject();
        QString out = "appName: " + appName + "  process_id: " + QString::number(process_id) + "  url: " + url + "  success close";
        qDebug()<<out;
        r_object->m_processId = 0;
        r_object->m_url = "";
        m_rMap.insert(appName,r_object);
    }
}

void RManager::deleteR(QString appName)
{
    QMap<QString,RObject*>::iterator it;
    for(it = m_rMap.begin();it!=m_rMap.end();)
    {
        if(it.key() == appName)
        {
            killR(it);
            it = m_rMap.erase(it);
            continue;
        }
        it++;
    }
}

void RManager::closeR(QString url)
{
    QMap<QString,RObject*>::iterator it;
    for(it = m_rMap.begin();it!=m_rMap.end();)
    {
        if(it.value()->m_url.indexOf(url.mid(0,url.length()-2)) != -1)
        {
            killR(it);
            break;
        }
        it++;
    }
}

void RManager::killAllRObject()
{
    QMap<QString,RObject*>::Iterator it = m_rMap.begin();
    while(it != m_rMap.end())
    {
        killR(it);
        it++;
    }
}
