#include "rtabwidget.h"

rTabWidget::rTabWidget(QWidget *parent):QTabWidget(parent)
{
    m_tabWidget=new QTabWidget();
    this->m_tabWidget=this;
    connect(m_tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(onRemoveTab(int)));
}

void rTabWidget::onRemoveTab(int index)
{
    emit tabCloseSignal(m_tabWidget->tabText(index));
    RWebEngineView* removeWebView=getWebEngineView(m_tabWidget->tabText(index));
    QMap<QString,RWebEngineView *>::iterator iter=m_webEngineViewMap.find(m_tabWidget->tabText(index));
    m_webEngineViewMap.erase(iter);
    m_tabWidget->removeTab(index);
    delete(removeWebView);
}

int rTabWidget::addNewTab(QString Title,RWebEngineView* widgetView)
{
    m_webEngineViewMap.insert(Title,widgetView);
    return m_tabWidget->addTab(widgetView,Title);
}

RWebEngineView* rTabWidget::getWebEngineView(QString Title)
{
    return m_webEngineViewMap[Title];
}
