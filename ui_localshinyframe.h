/********************************************************************************
** Form generated from reading UI file 'localshinyframe.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOCALSHINYFRAME_H
#define UI_LOCALSHINYFRAME_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LocalShinyFrame
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStatusBar *statusbar;
    QMenuBar *menubar;
    QToolBar *dockWidgetBar;

    void setupUi(QMainWindow *LocalShinyFrame)
    {
        if (LocalShinyFrame->objectName().isEmpty())
            LocalShinyFrame->setObjectName(QString::fromUtf8("LocalShinyFrame"));
        LocalShinyFrame->resize(800, 600);
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        LocalShinyFrame->setFont(font);
        centralwidget = new QWidget(LocalShinyFrame);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        LocalShinyFrame->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(LocalShinyFrame);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        LocalShinyFrame->setStatusBar(statusbar);
        menubar = new QMenuBar(LocalShinyFrame);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 23));
        LocalShinyFrame->setMenuBar(menubar);
        dockWidgetBar = new QToolBar(LocalShinyFrame);
        dockWidgetBar->setObjectName(QString::fromUtf8("dockWidgetBar"));
        dockWidgetBar->setLayoutDirection(Qt::RightToLeft);
        dockWidgetBar->setMovable(true);
        LocalShinyFrame->addToolBar(Qt::RightToolBarArea, dockWidgetBar);

        retranslateUi(LocalShinyFrame);

        QMetaObject::connectSlotsByName(LocalShinyFrame);
    } // setupUi

    void retranslateUi(QMainWindow *LocalShinyFrame)
    {
        LocalShinyFrame->setWindowTitle(QCoreApplication::translate("LocalShinyFrame", "MainWindow", nullptr));
        dockWidgetBar->setWindowTitle(QCoreApplication::translate("LocalShinyFrame", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LocalShinyFrame: public Ui_LocalShinyFrame {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOCALSHINYFRAME_H
