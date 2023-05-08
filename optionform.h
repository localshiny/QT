#ifndef OPTIONFORM_H
#define OPTIONFORM_H

#include <QWidget>
#include <QCloseEvent>
#include <QSettings>
#include <QDebug>
#include <QMap>
#include <QDesktopServices>
#include <QMessageBox>

namespace Ui {
class Option;
}

class Option : public QWidget
{
    Q_OBJECT

public:
    explicit Option(QWidget *parent = 0);
    void setOptions();
    QMap<QString, QString> getOptions();
    ~Option();

signals:
    void option_changed();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_okButton_clicked();

    void on_cancelButton_clicked();

    void judgePath(QString);

private:
    Ui::Option *ui;
    QString m_locationPath;
    QString m_configPath;
    QSettings* m_setting;
    void initOptions();
    bool isPathValid(QString);
};

#endif // OPTIONFORM_H
