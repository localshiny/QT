#include "rautodelete.h"

RAutoDelete::RAutoDelete(QObject *parent) : QObject(parent) { }

void RAutoDelete::on_delete_handle(QString id)
{
    Q_EMIT deleteStart(id);
}
