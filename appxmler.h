#ifndef APPXMLER_H
#define APPXMLER_H

#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QMap>
#include <QtCore>
#include <QFile>
#include <QProcess>
#include "rpathinput.h"
#include <QDebug>


class AppXmler : public QObject
{
    Q_OBJECT
public:
    explicit AppXmler(QObject *parent = 0);

    QString m_xmlUrl;
    QString m_xmlName;
    void downloadXml();                                 //download xml
    QString getXmlMapElement(QString elementName);   //get xml_map element by elementName
    void clearXmlReader();                             //clear xml map
    void resetArgs();                                   //reset args
    void deleteXml();
    void parseXml(QString xmlPath);

signals:
    void updatePackageInfoSignal(QString packageName, QString packageTitle, QString packageVersion, QString packageTime, QString packageDetails);
    void processOutputSignal(QString output, QString type);
    void processErrorSignal(QString error);

public slots:
    void onProcessFinished(int exitCode);
    void onProcessOutput(QString output, QString type);
    void onProcessError(QString error);
    void onProcessOutput(qint64 bytesRead, qint64 totalBytes, qreal progress);
private:
    QMap<QString,QString> m_xmlMap;
    void readXml(QString xmlName);        //read xml by xmlName
    void readXmlMap();                    //read xml_map
    void readByRes(QDomNode childNode);  //read xml by recursion
    void parseXml();                       //parse xml 

    RPathInput* m_RPathInput;
};

#endif // APPXMLER_H
