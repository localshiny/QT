//
//
// FileName: main.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "localshinyframe.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    QFont font = a.font();
    font.setFamily("Arial");
    a.setFont(font);
    LocalShinyFrame w;
    w.show();
    a.lastWindowClosed();
    return a.exec();
}
