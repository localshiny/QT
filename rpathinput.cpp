#include "rpathinput.h"

RPathInput::RPathInput()
{

}

QString RPathInput::getRPath(QString newpkgRHome)
{

#ifdef Q_OS_MAC
    QString RscriptPath="/Library/Frameworks/R.framework/Resources";
#elif defined Q_OS_WIN64
    QString RscriptPath = newpkgRHome + "/bin/x64";
#elif defined Q_OS_WIN32
    QString RscriptPath = newpkgRHome + "/bin";

#endif
    return RscriptPath;
}
void RPathInput::putRPath(QString newpkgRHome)
{
    QString RscriptPath=getRPath(newpkgRHome);
#ifdef Q_OS_MAC
    QString path=RscriptPath+":"+qgetenv("PATH");
#else
    QString path=RscriptPath+";"+qgetenv("PATH");
#endif
    qputenv("PATH",path.toStdString().c_str());
    return;
}
