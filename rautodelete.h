#ifndef RAUTODELETE_H
#define RAUTODELETE_H

#include "QObject"

class RAutoDelete: public QObject
{
    Q_OBJECT
public:
    explicit RAutoDelete(QObject *parent = nullptr);

Q_SIGNALS:
    void deleteStart(QString id);
public Q_SLOTS:
    void on_delete_handle(QString id);

public slots:
};
#endif // RAUTODELETE_H
