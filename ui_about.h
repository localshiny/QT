/********************************************************************************
** Form generated from reading UI file 'about.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUT_H
#define UI_ABOUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_About
{
public:
    QLabel *running_label;
    QLabel *running_label_2;
    QLabel *running_label_3;
    QLabel *running_label_4;
    QLabel *label;
    QLabel *running_label_5;

    void setupUi(QWidget *About)
    {
        if (About->objectName().isEmpty())
            About->setObjectName(QString::fromUtf8("About"));
        About->resize(376, 218);
        running_label = new QLabel(About);
        running_label->setObjectName(QString::fromUtf8("running_label"));
        running_label->setEnabled(true);
        running_label->setGeometry(QRect(10, 10, 371, 31));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(10);
        font.setBold(false);
        font.setWeight(50);
        running_label->setFont(font);
        running_label_2 = new QLabel(About);
        running_label_2->setObjectName(QString::fromUtf8("running_label_2"));
        running_label_2->setEnabled(true);
        running_label_2->setGeometry(QRect(10, 40, 371, 31));
        running_label_2->setFont(font);
        running_label_3 = new QLabel(About);
        running_label_3->setObjectName(QString::fromUtf8("running_label_3"));
        running_label_3->setEnabled(true);
        running_label_3->setGeometry(QRect(10, 70, 51, 31));
        running_label_3->setFont(font);
        running_label_4 = new QLabel(About);
        running_label_4->setObjectName(QString::fromUtf8("running_label_4"));
        running_label_4->setEnabled(true);
        running_label_4->setGeometry(QRect(10, 100, 71, 31));
        running_label_4->setFont(font);
        label = new QLabel(About);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 70, 211, 31));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Arial"));
        font1.setPointSize(10);
        label->setFont(font1);
        label->setOpenExternalLinks(true);
        running_label_5 = new QLabel(About);
        running_label_5->setObjectName(QString::fromUtf8("running_label_5"));
        running_label_5->setEnabled(true);
        running_label_5->setGeometry(QRect(70, 100, 221, 31));
        running_label_5->setFont(font);
        running_label_5->setOpenExternalLinks(true);

        retranslateUi(About);

        QMetaObject::connectSlotsByName(About);
    } // setupUi

    void retranslateUi(QWidget *About)
    {
        About->setWindowTitle(QCoreApplication::translate("About", "Form", nullptr));
        running_label->setText(QCoreApplication::translate("About", "LocalShiny version: 1.0 GPL to Liscense", nullptr));
        running_label_2->setText(QCoreApplication::translate("About", "Dependences:", nullptr));
        running_label_3->setText(QCoreApplication::translate("About", "R:", nullptr));
        running_label_4->setText(QCoreApplication::translate("About", "RShiny:", nullptr));
        label->setText(QCoreApplication::translate("About", "<a href=\"https://www.r-project.org/\">https://www.r-project.org/", nullptr));
        running_label_5->setText(QCoreApplication::translate("About", "<a href=\"https://shiny.rstudio.com/\">https://shiny.rstudio.com/", nullptr));
    } // retranslateUi

};

namespace Ui {
    class About: public Ui_About {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUT_H
