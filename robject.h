#ifndef ROBJECT_H
#define ROBJECT_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QDesktopServices>
#include "record.h"
#include "rpathinput.h"

class RObject : public QObject
{
    Q_OBJECT

public:
    explicit RObject(QString appID,QString appName, QString appdataPath, QObject *parent = 0);

    qint64 m_processId;    //R process id
    QString m_url;          //R url
    QString m_appName;     //R name
    QString m_appdataPath;
    QString m_appID;

    void start();           //start R
    void startProcess();   //start R process
    void killRObject();   //kill R process

private slots:
    void showFinished(int, QProcess::ExitStatus);  //response the finish of R process
    void showOut();                                //response the out of R process
    void showError();                              //response the error of R process

signals:
    void openUrlSignal(QString appName);
    void appClosed();
    void outputSignal(QString output, QString type);

private:
    Record* m_record;                //write and read app.txt
    QProcess* m_process;            //R process
    RPathInput* m_RPathInput;
    bool isValidUrl(QString url);  //judge validity of url
    QString getRHome();            //get rHome of RObject by appName
};

#endif // ROBJECT_H
