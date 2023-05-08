#ifndef RINSPECTOR_H
#define RINSPECTOR_H

#include <QDockWidget>
#include <QWebEngineView>
#include <QVBoxLayout>

class RInspector : public QDockWidget
{
    Q_OBJECT

public:
    explicit RInspector(QWidget *parent = 0);
    void show();
    ~RInspector();

private:
    QWebEngineView* m_webview;
};

#endif // RINSPECTOR_H
