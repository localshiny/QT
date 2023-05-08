#ifndef RECORD_H
#define RECORD_H

#include <QObject>
#include <QFile>
#include <QDebug>
#include <QApplication>
#include <QSettings>

class Record : public QObject
{
    Q_OBJECT
public:
    explicit Record(QObject *parent = 0);

    QString getValue(QString keyName);
    QStringList readLine(QString tag, bool flag);
    QString getAppID(QString tag, bool flag);
    QStringList getSection();
    bool appExist(QString appID);
    void writeLine(QString appName, QString version, QString rHome, QString appID,QString rtoolsHome,QString newpkgHome,QString runcmd);   //write line to app.txt
    void changeLine(QString appName, QString version, QString rHome, QString appID,QString rtoolsHome,QString newpkgHome,QString runcmd);  //change line to app.txt
    void deleteLine(QString appName);                                                  //delete line to app.txt
    void changeLine(QString dependenceInfo,  QString rHome, bool downloadStats,bool installStats,QString version);
    void writeLine(QString dependenceInfo,  QString rHome, bool downloadStats,bool installStats,QString version);
    void changeKey(QString groupName,QString key, QString value);
    void changeKey(QString groupName,QString key, bool value);
    QString m_locationPath;
    QString m_appIniFile;
    QSettings* m_settings;
};

#endif // RECORD_H
