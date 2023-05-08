#-------------------------------------------------
#
# Project created by QtCreator 2020-02-17T18:25:25
#
#-------------------------------------------------

QT       += core gui network xml webenginewidgets webchannel

QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LocalShiny
TEMPLATE = app

VERSION = 1.0.0

QMAKE_TARGET_PRODUCT = LocalShiny

QMAKE_TARGET_DESCRIPTION = LocalShiny based on Qt 5.15.2

QMAKE_TARGET_COPYRIGHT = Copyright(c)2022 DLUT

RC_LANG = 0x0409

RC_ICONS = localshiny.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
    appxmler.cpp \
    downloadtool.cpp \
    localinstallform.cpp \
    localshinyframe.cpp \
    rautodelete.cpp \
    rautoopenmyspace.cpp \
    rautorun.cpp \
    robject.cpp \
    rmanager.cpp \
    rpathinput.cpp \
    rtabwidget.cpp \
    taskbase.cpp \
    taskinstallapp.cpp \
    taskinstallpkg.cpp \
    taskrdownload.cpp \
    rwebengineview.cpp \
    rinspector.cpp \
    rautoinstall.cpp \
    about.cpp \
    optionform.cpp \
    record.cpp \
    rprocess.cpp \
    taskrinstall.cpp \
    taskrtoolsdownload.cpp \
    taskrtoolsinstall.cpp \
    urlinstallform.cpp

HEADERS  += \
    appxmler.h \
    downloadtool.h \
    localinstallform.h \
    localshinyframe.h \
    rautodelete.h \
    rautoopenmyspace.h \
    rautorun.h \
    robject.h \
    rmanager.h \
    rpathinput.h \
    rtabwidget.h \
    taskbase.h \
    taskinstallapp.h \
    taskinstallpkg.h \
    taskrdownload.h \
    rwebengineview.h \
    rinspector.h \
    rautoinstall.h \
    about.h \
    optionform.h \
    record.h \
    rprocess.h \
    taskrinstall.h \
    taskrtoolsdownload.h \
    taskrtoolsinstall.h \
    urlinstallform.h

RESOURCES += \
    icon.qrc


FORMS += \
    about.ui \
    localinstallform.ui \
    localshinyframe.ui \
    optionform.ui \
    urlinstallform.ui
