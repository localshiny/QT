#include "rautoopenmyspace.h"

RAutoOpenMySpace::RAutoOpenMySpace(QObject *parent) : QObject(parent)
{

}
void RAutoOpenMySpace::on_open_my_space_handle()
{
    Q_EMIT openMySpaceStart();
}
