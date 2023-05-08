//
//
// FileName: rautoinstall.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "rautoinstall.h"

RAutoInstall::RAutoInstall(QObject *parent) : QObject(parent) { }

void RAutoInstall::on_install_handle(QString url)
{
    Q_EMIT installStart(url);
}
