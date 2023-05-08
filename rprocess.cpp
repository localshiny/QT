//
//
// FileName: rprocess.cpp
// Creator: Zhang Runze
// Date: 2021-9-30
//
//
#include "rprocess.h"

RProcess::RProcess(QObject *parent) : QProcess(parent)
{
    // create QProcess to receive the output of it, then transfer output to localshinyframe.
    m_process = new QProcess();
    connect(m_process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(m_process,&QProcess::readyReadStandardOutput,this,&RProcess::onOutput);
    connect(m_process,&QProcess::readyReadStandardError,this,&RProcess::onError);
}

void RProcess::run(QString program)
{
    // start process and emit start signal
    m_process->start(program);
    emit processOutputSignal("Starting: " + program, "Start");
}

void RProcess::exrun(QString program)
{
    // start process and wait for finished
    m_process->execute(program);
    emit processOutputSignal("Starting: " + program, "Start");
}

void RProcess::run(QString program, QStringList args)
{
    // start process with args
    m_process->start(program, args);
    emit processOutputSignal("Starting: " + program + " " + args.join(" "), "Start");
}

void RProcess::exrun(QString program, QStringList args)
{
    // execute process with args
    m_process->execute(program, args);
    emit processOutputSignal("Starting: " + program + " " + args.join(" "), "Start");
}

void RProcess::setWorkDirectory(QString path)
{
    // change work directory to specified path
    m_process->setWorkingDirectory(path);
}

void RProcess::onFinished(int exitCode, QProcess::ExitStatus exit_status)
{
    // when process finished emit output signal and finish signal to localshinyframe, then localshiny frame will process these siganl
    Q_UNUSED(exit_status);
    QProcess* process = (QProcess *)sender();
    QString out = process->program() + " " + process->arguments().join(" ") + " exited with code " + QString::number(exitCode) + "\n";
    emit processOutputSignal(out, "Finish");
    emit processFinishSignal(exitCode);
}

void RProcess::onOutput()
{
    // get the process's output and emit to localshinyframe
    QProcess* process = (QProcess *)sender();
    QString out = QString::fromLocal8Bit(process ->readAllStandardOutput().data());
    emit processOutputSignal(out, "Out");
}

//QString RProcess::getOutput()
//{
//    QProcess* process = (QProcess *)sender();
//    QString out = QString::fromLocal8Bit(process->readAllStandardOutput().data());
//    return out;
//}

void RProcess::onError()
{
    QProcess* process = (QProcess *)sender();
    QString error = QString::fromLocal8Bit(process->readAllStandardError().data());
    emit processOutputSignal(error, "Error");
    emit processErrorSignal(error);
    error = "showError: " + error;
    qDebug()<<error;
}

void RProcess::onState(QProcess::ProcessState state)
{
    QString out = "show_state: ";
    if(state == QProcess::NotRunning)
    {
        out += "Not Running";
    }
    else if(state == QProcess::Starting)
    {
        out += "Starting";
    }
    else
    {
        out += "Running";
    }
    qDebug()<<out;
    emit processOutputSignal(out, "Out");
}
