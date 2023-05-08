#ifndef RWEBENGINEVIEW_H
#define RWEBENGINEVIEW_H

#include <QWebEngineView>
#include <QWidget>
#include <QUrl>
#include "record.h"
#include <QDebug>
class RWebEngineView : public QWebEngineView
{
    Q_OBJECT

public:
    explicit RWebEngineView(QWidget* parent = Q_NULLPTR);
    void init(QUrl url);
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override ;
    void linkHovered(QString url);
    void setHttpUserAgent();
    void loadPage(QUrl url);
protected:


private :
    QUrl m_newUrl;
    Record* m_record;
};

#endif // RWEBENGINEVIEW_H
