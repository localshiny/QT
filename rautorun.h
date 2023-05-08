#ifndef RAUTORUN_H
#define RAUTORUN_H

#include "QObject"

class RAutoRun: public QObject
{
    Q_OBJECT
public:
    explicit RAutoRun(QObject *parent = nullptr);

Q_SIGNALS:
    void runStart(QString id);
public Q_SLOTS:
    void on_run_handle(QString id);

public slots:
};

#endif // RAUTORUN_H
