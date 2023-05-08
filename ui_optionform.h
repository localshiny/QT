/********************************************************************************
** Form generated from reading UI file 'optionform.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONFORM_H
#define UI_OPTIONFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Option
{
public:
    QGridLayout *gridLayout;
    QGroupBox *server;
    QLineEdit *servAddr;
    QLabel *servLabel;
    QGroupBox *conOutput;
    QRadioButton *posRight;
    QRadioButton *posBottom;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QSpacerItem *horizontalSpacer;
    QGroupBox *fileMa;
    QLabel *fileLabel;
    QLineEdit *filePath;
    QLabel *errorLabel;

    void setupUi(QWidget *Option)
    {
        if (Option->objectName().isEmpty())
            Option->setObjectName(QString::fromUtf8("Option"));
        Option->resize(443, 336);
        Option->setAutoFillBackground(false);
        Option->setStyleSheet(QString::fromUtf8(""));
        gridLayout = new QGridLayout(Option);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        server = new QGroupBox(Option);
        server->setObjectName(QString::fromUtf8("server"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        server->setFont(font);
        servAddr = new QLineEdit(server);
        servAddr->setObjectName(QString::fromUtf8("servAddr"));
        servAddr->setGeometry(QRect(110, 30, 281, 20));
        servLabel = new QLabel(server);
        servLabel->setObjectName(QString::fromUtf8("servLabel"));
        servLabel->setGeometry(QRect(10, 30, 101, 16));

        gridLayout->addWidget(server, 1, 0, 1, 3);

        conOutput = new QGroupBox(Option);
        conOutput->setObjectName(QString::fromUtf8("conOutput"));
        conOutput->setFont(font);
        posRight = new QRadioButton(conOutput);
        posRight->setObjectName(QString::fromUtf8("posRight"));
        posRight->setGeometry(QRect(20, 30, 89, 16));
        posBottom = new QRadioButton(conOutput);
        posBottom->setObjectName(QString::fromUtf8("posBottom"));
        posBottom->setGeometry(QRect(90, 30, 89, 16));
        posBottom->setCheckable(true);
        posBottom->setChecked(false);

        gridLayout->addWidget(conOutput, 0, 0, 1, 3);

        cancelButton = new QPushButton(Option);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setFont(font);

        gridLayout->addWidget(cancelButton, 3, 2, 1, 1);

        okButton = new QPushButton(Option);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setFont(font);

        gridLayout->addWidget(okButton, 3, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 3, 0, 1, 1);

        fileMa = new QGroupBox(Option);
        fileMa->setObjectName(QString::fromUtf8("fileMa"));
        fileMa->setFont(font);
        fileLabel = new QLabel(fileMa);
        fileLabel->setObjectName(QString::fromUtf8("fileLabel"));
        fileLabel->setGeometry(QRect(10, 30, 81, 16));
        filePath = new QLineEdit(fileMa);
        filePath->setObjectName(QString::fromUtf8("filePath"));
        filePath->setGeometry(QRect(110, 30, 281, 21));
        errorLabel = new QLabel(fileMa);
        errorLabel->setObjectName(QString::fromUtf8("errorLabel"));
        errorLabel->setGeometry(QRect(110, 50, 281, 31));
        QFont font1;
        font1.setPointSize(12);
        errorLabel->setFont(font1);
        errorLabel->setLayoutDirection(Qt::LeftToRight);

        gridLayout->addWidget(fileMa, 2, 0, 1, 3);


        retranslateUi(Option);

        QMetaObject::connectSlotsByName(Option);
    } // setupUi

    void retranslateUi(QWidget *Option)
    {
        Option->setWindowTitle(QCoreApplication::translate("Option", "Form", nullptr));
        server->setTitle(QCoreApplication::translate("Option", "Server", nullptr));
        servLabel->setText(QCoreApplication::translate("Option", "Server Address:", nullptr));
        conOutput->setTitle(QCoreApplication::translate("Option", "Console Output", nullptr));
        posRight->setText(QCoreApplication::translate("Option", "Right", nullptr));
        posBottom->setText(QCoreApplication::translate("Option", "Bottom", nullptr));
        cancelButton->setText(QCoreApplication::translate("Option", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("Option", "OK", nullptr));
        fileMa->setTitle(QCoreApplication::translate("Option", "File Management", nullptr));
        fileLabel->setText(QCoreApplication::translate("Option", "Default Path: ", nullptr));
        errorLabel->setText(QCoreApplication::translate("Option", "TextLabel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Option: public Ui_Option {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONFORM_H
