#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H


#include "basemainwidget.h"
#include <QWidget>

namespace Ui {
class LoginWidget;
}

class LoginWidget : public BaseMainWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = 0);
    ~LoginWidget();

private slots:
    void on_pushButton_login_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::LoginWidget *ui;
};

#endif // LOGINWIDGET_H
