#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    bool zeroCr;
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    //void rpmsInterrupt();
public slots:
    void PulsesPerMilSec();
   // void esperaCruce();
    //void scrTrigger();

private slots:
    void on_Knob_valueChanged(double value);

    void on_pushButton_clicked();



private:
    Ui::Widget *ui;
    QTimer *rpms;
    QTimer *findCross;
    QTimer *scr;
};

#endif // WIDGET_H
