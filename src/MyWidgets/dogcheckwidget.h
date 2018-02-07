#ifndef DOGCHECKWIDGET_H
#define DOGCHECKWIDGET_H

#include <QWidget>
#include <QProcess>
namespace Ui {
class DogCheckWidget;
}

class DogCheckWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DogCheckWidget(QWidget *parent = 0);
    ~DogCheckWidget();
    bool Dogcheck();

signals:

public slots:
    void readFromClient();
private:
    QProcess *m_pro;
private:
    Ui::DogCheckWidget *ui;
};

#endif // DOGCHECKWIDGET_H
