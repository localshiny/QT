//
//
// FileName: rinspector.cpp
// Creator: Zhang Runze
// Date: 2021-8-28
//
//
#include "rinspector.h"

RInspector::RInspector(QWidget *parent) :
    QDockWidget(parent)
{
    setFeatures(QDockWidget::DockWidgetClosable);
    setAllowedAreas(Qt::RightDockWidgetArea);
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* webLayout = new QVBoxLayout;
    m_webview = new QWebEngineView;
    webLayout->setSpacing(0);
    webLayout->setMargin(0);
    webLayout->addWidget(m_webview);
    centralWidget->setLayout(webLayout);
    m_webview->load(QUrl("http://127.0.0.1:7001"));
    setWidget(centralWidget);
}

RInspector::~RInspector() { }

void RInspector::show()
{
    m_webview->reload();
    QDockWidget::show();
}
