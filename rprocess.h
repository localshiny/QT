#ifndef RPROCESS_H
#define RPROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>

class RProcess : public QProcess
{
    Q_OBJECT
public:
    explicit RProcess(QObject *parent = nullptr);
    void run(QString program);
    void exrun(QString program);
    void run(QString program, QStringList args);
    void exrun(QString program, QStringList args);
    void setWorkDirectory(QString path);

private:
    QProcess* m_process;

signals:
    void processOutputSignal(QString output, QString type);
    void processFinishSignal(int exitCode);
    void processErrorSignal(QString error);
    //void out_msg(QString out);

public slots:
    void onFinished(int, QProcess::ExitStatus);
    void onOutput();
    void onError();
    void onState(QProcess::ProcessState);
//    QString getOutput();
};

#endif // RPROCESS_H
