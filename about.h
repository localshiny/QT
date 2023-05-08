#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QCloseEvent>

namespace Ui {
class About;
}

class About : public QWidget
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

protected:
    void closeEvent(QCloseEvent *event);  //overloaded closeEvent function

private:
    Ui::About *ui;
};

#endif // ABOUT_H
