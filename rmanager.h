#ifndef RMANAGER_H
#define RMANAGER_H

#include "robject.h"
#include "record.h"

class RManager
{
public:
    RManager();

    RObject* find(QString appName);                   //find the RObject* of appName, return RObject*
    void addR(QString appName, RObject* r_object);   //add a new map between of appName and RObject to m_rMap
    void killR(QMap<QString,RObject*>::iterator it);  //kill a RObject
    void deleteR(QString appName);                   //delete a RObject
    void closeR(QString url);                         //close a RObject
    void killAllRObject();                          //kill all RObject

private:
    QMap<QString,RObject*> m_rMap;     //the map between appName and RObject
    Record* m_record;                     //write and read app.txt
};

#endif // RMANAGER_H
