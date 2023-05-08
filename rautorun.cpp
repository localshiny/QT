#include "rautorun.h"

RAutoRun::RAutoRun(QObject *parent) : QObject(parent) {
}

void RAutoRun::on_run_handle(QString id)
{
    Q_EMIT runStart(id);
}

