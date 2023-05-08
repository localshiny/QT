#ifndef RAUTOOPENMYSPACE_H
#define RAUTOOPENMYSPACE_H

#include "QObject"

class RAutoOpenMySpace:public QObject
{
    Q_OBJECT
public:
    explicit RAutoOpenMySpace(QObject *parent = nullptr);

Q_SIGNALS:
    void openMySpaceStart();
public Q_SLOTS:
    void on_open_my_space_handle();

public slots:
};

#endif // RAUTOOPENMYSPACE_H
