//
//
// FileName: about.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "about.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_QuitOnClose, false);
    setWindowTitle("Information Center");
    this->resize(QSize(400, 180));
    hide();
}

About::~About()
{
    delete ui;
}

void About::closeEvent(QCloseEvent *event)
{
    if(isVisible())
    {
        hide();
    }
    event->ignore();
}
