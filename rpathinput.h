#ifndef RPATHINPUT_H
#define RPATHINPUT_H


#include <QApplication>
#include <QProcess>
#include <QMessageBox>

class RPathInput
{

public:
    explicit RPathInput();
    QString getWgetPath();
    QString getRPath(QString newpkgRHome);
    void putRPath(QString newpkgRHome);
};

#endif // RPATHINPUT_H
