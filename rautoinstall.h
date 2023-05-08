#ifndef RAUTOINSTALL_H
#define RAUTOINSTALL_H

#include <QObject>
#include <QString>

class RAutoInstall : public QObject
{
    Q_OBJECT
public:
    explicit RAutoInstall(QObject *parent = nullptr);

Q_SIGNALS:
    void installStart(QString url);
public Q_SLOTS:
    void on_install_handle(QString url);

public slots:
};

#endif // RAUTOINSTALL_H
