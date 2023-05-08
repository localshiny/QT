#ifndef RTABWIDGET_H
#define RTABWIDGET_H
#include <QTabWidget>
#include "rwebengineview.h"

class rTabWidget: public QTabWidget
{
    Q_OBJECT

public:
    explicit rTabWidget(QWidget* parent = Q_NULLPTR);
signals:
    void tabCloseSignal(QString appName);
private:
    QTabWidget* m_tabWidget;
    QMap<QString,RWebEngineView*> m_webEngineViewMap;
public slots:
    void onRemoveTab(int tabIndex);
public:
    int addNewTab(QString Title,RWebEngineView* webView);
    RWebEngineView* getWebEngineView(QString Title);
};


#endif // RTABWIDGET_H
