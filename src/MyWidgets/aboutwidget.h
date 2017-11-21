#ifndef ABOUTWIDGET_H
#define ABOUTWIDGET_H

#include <QWidget>
#include "basemainwidget.h"
namespace Ui {
class AboutWidget;
}

class AboutWidget : public BaseMainWidget
{
    Q_OBJECT

public:
    explicit AboutWidget(QWidget *parent = 0);
    ~AboutWidget();

private slots:
    void on_pushButton_close_clicked();

    void on_pushButton_clicked();
    void openUrl(QString url);

private:
    Ui::AboutWidget *ui;
};

#endif // ABOUTWIDGET_H
