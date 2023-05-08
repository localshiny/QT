//
//
// FileName: xmler.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "appxmler.h"
#include "downloadtool.h"

AppXmler::AppXmler(QObject *parent) : QObject(parent)
{
    m_RPathInput=new RPathInput;
    resetArgs();
}

void AppXmler::readXml(QString xmlName)
{
    QFile* xmlFile = new QFile(xmlName);
    if(!xmlFile->open(QFile::ReadOnly | QFile::Text))
    {
        qDebug()<<"Error: open failed!";
        return;
    }

    QDomDocument doc;
    QString errorString;
    int errorLine;
    int errorColumn;

    // Parse the xml file
    if(!doc.setContent(xmlFile,&errorString,&errorLine,&errorColumn))
    {
        xmlFile->close();
        qDebug()<<"Error: doc setContent error at line "<<errorLine<<", "
               <<"column "<<errorColumn<<": "<<errorString;
        return;
    }
    QDomElement rootElement = doc.documentElement();
    QDomNode childNode = rootElement.firstChild();
    while(!childNode.isNull())  // traverse all nodes
    {
        if(childNode.isElement())  // if the node is an element, extract its content
        {
            m_xmlMap.insert(childNode.toElement().tagName(),childNode.toElement().text());
            if(childNode.toElement().hasAttributes())
            {
                QDomNamedNodeMap nodeMap =  childNode.toElement().attributes();
                for(int i =0 ; i < nodeMap.count(); i++)
                {
                    QDomNode n = nodeMap.item(i);
                    QString nName = childNode.parentNode().toElement().tagName() + "_" + childNode.toElement().tagName() + "_" + n.nodeName();
                    m_xmlMap.insert(nName,n.nodeValue());
                }
            }
            readByRes(childNode);
        }
        childNode = childNode.nextSibling();  // go to the next node
    }
    xmlFile->close();
}

void AppXmler::readXmlMap()
{
    QMap<QString,QString>::Iterator it = m_xmlMap.begin();
    while(it != m_xmlMap.end())
    {
        it++;
    }
}

QString AppXmler::getXmlMapElement(QString elementName)
{
    return m_xmlMap[elementName];
}

void AppXmler::readByRes(QDomNode childNode)
{
    QDomNodeList nodeList = childNode.childNodes();
    if(nodeList.isEmpty())
    {
        return;
    }
    for (int i = 0; i< nodeList.count();i++)
    {
        QDomNode node = nodeList.at(i);
        if(node.isElement())
        {
            QString nodeName = node.parentNode().parentNode().toElement().tagName() + "_" + node.parentNode().toElement().tagName() + "_" + node.toElement().tagName();
            m_xmlMap.insert(nodeName,node.toElement().text());
            if(node.toElement().hasAttributes())
            {
                QDomNamedNodeMap nodeMap = node.toElement().attributes();
                for(int i =0 ; i < nodeMap.count(); i++)
                {
                    QDomNode n = nodeMap.item(i);
                    QString nName = node.parentNode().toElement().tagName() + "_" + node.toElement().tagName() + "_" + n.nodeName();
                    m_xmlMap.insert(nName,n.nodeValue());
                }
            }
            readByRes(node);
        }
    }
}

void AppXmler::clearXmlReader()
{
    m_xmlMap.clear();
}

void AppXmler::resetArgs()
{
    m_xmlUrl = "";
    m_xmlName = "";
}

void AppXmler::downloadXml()
{
    QString downloadPath = QDir::tempPath();
    DownloadTool* dT;    
    dT = new DownloadTool(m_xmlUrl,downloadPath);
    connect(dT,SIGNAL(processFinishSignal(int)),this,SLOT(onProcessFinished(int)));
    connect(dT,SIGNAL(sigProgress(qint64,qint64,qreal)),this,SLOT(onProcessOutput(qint64,qint64,qreal)));
    dT->startDownload();
}

void AppXmler::onProcessFinished(int)
{
    emit processOutputSignal("the xml downloaded", "finished");
    parseXml();
}

void AppXmler::parseXml()
{
    m_xmlName = QDir::tempPath() + "/" + m_xmlName;
    readXml(m_xmlName);

    QString preferred = getXmlMapElement("app_install_preferred");
    QString osName;
#if defined(Q_OS_WIN)
    osName = "Windows";
#elif defined(Q_OS_LINUX)
    osName = "Linux";
#elif defined(Q_OS_macos)
    osName="macOS";
#else
    QMessageBox::information(NULL, "Error", "Your operating system does not support this app.", QMessageBox::Ok, QMessageBox::Ok);
#endif

    QString appVerTagName;
    if (preferred == "localShiny")
    {
        appVerTagName = "install_" + preferred + "_pakVersion";
    }
    else
    {
        appVerTagName = preferred + "_" + osName + "_pakVersion";
    }

    QString packageName = getXmlMapElement("app_head_pakname");
    QString packageTitle = getXmlMapElement("app_head_title");
    QString packageVersion = getXmlMapElement(appVerTagName);
    QString packageTime = getXmlMapElement("app_head_pakdate");
    QString packageDetails = getXmlMapElement("app_head_description");

    emit updatePackageInfoSignal(packageName,packageTitle,packageVersion,packageTime,packageDetails);
}

void AppXmler::parseXml(QString xmlPath)
{

    readXml(xmlPath);

    QString preferred = getXmlMapElement("app_install_preferred");
    QString osName;
#if defined(Q_OS_WIN)
    osName = "Windows";
#elif defined(Q_OS_LINUX)
    osName = "Linux";
#elif defined(Q_OS_macos)
    osName="macOS";
#else
    //show massage
#endif

    QString appVerTagName;
    if (preferred == "localShiny")
    {
        appVerTagName = "install_" + preferred + "_pakVersion";
    }
    else
    {
        appVerTagName = preferred + "_" + osName + "_pakVersion";
    }

    QString packageName = getXmlMapElement("app_head_pakname");
    QString packageTitle = getXmlMapElement("app_head_title");
    QString packageVersion = getXmlMapElement(appVerTagName);
    QString packageTime = getXmlMapElement("app_head_pakdate");
    QString packageDetails = getXmlMapElement("app_head_description");

    emit updatePackageInfoSignal(packageName,packageTitle,packageVersion,packageTime,packageDetails);
}

void AppXmler::onProcessOutput(QString output,QString type)
{
    //transfer signal to installform
    emit processOutputSignal(output, type);
}

void AppXmler::onProcessOutput(qint64 bytesRead, qint64 totalBytes, qreal progress)
{
    QString output="-----"+QString::number(progress * 100, 'f', 2) + "%    "+ QString::number(bytesRead / (1024 * 1024), 'f', 2) + "MB" + "/" + QString::number(totalBytes / (1024 * 1024), 'f', 2) + "MB"+"-----";
    QString type="process";
    emit processOutputSignal(output,type);
}

void AppXmler::onProcessError(QString error)
{
    //transfer signal to installform
    emit processErrorSignal(error);
}

void AppXmler::deleteXml()
{
    QString downloadPath = QDir::tempPath();
    QFile::remove(m_xmlName);
}
