#include "widget.h"
#include "ui_widget.h"
#include "pigpio.h"
#include <unistd.h>
#include <stdio.h>
#include <QTimer>
#include <QDebug>
#include <QElapsedTimer>

#define pulses 26
#define cruce 19
#define scr1 13
#define scr2 6

void rpmsInterrupt(int gpio, int level, uint32_t tick);
void cruceInterrupt(int gpio, int level, uint32_t tick);

float pls=0.0;
bool zeroC=0;
float period=0;
int scrTime=0;
bool start=0;
int SetPoint=0;
//Variables de control
float error=0.0,errorAnt=0.0, prop=0.0,integ=0.0,t=0.0,kp=0.0,ki=0.0;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    if(gpioInitialise() < 0)
        {
            perror("Could not open the gpio library. ");
          //  return EXIT_FAILURE;
        }

     gpioSetMode(scr1, PI_OUTPUT);
     gpioWrite(scr1,0);
     gpioSetMode(scr2, PI_OUTPUT);
     gpioWrite(scr1,0);

     gpioSetMode(cruce, PI_INPUT);
     gpioSetPullUpDown(cruce,PI_PUD_UP);

     gpioSetISRFunc(cruce,FALLING_EDGE,NULL,cruceInterrupt);

     gpioSetMode(pulses, PI_INPUT);
     gpioSetPullUpDown(pulses,PI_PUD_UP);
     gpioSetISRFunc(pulses,FALLING_EDGE,NULL,rpmsInterrupt);

     //ui->qwtPlot->dr
    // scrTrigger();
     rpms= new QTimer(this);
      connect (rpms,SIGNAL(timeout()),this,SLOT(PulsesPerMilSec()));
      rpms->start(100);
}

Widget::~Widget()
{
    gpioTerminate();
    delete ui;
}
float freq=0.0;
float rpm=0.0;
float rpmD=0.0;

void Widget::PulsesPerMilSec()
{
//    freq=1/(period/1000000000.0);
//    if(freq<0.1){freq=0.1;}
//    rpm=freq/(0.1);
    if(pls==0.0){
        rpm=0;
    }else{
        rpm = (60.0/((0.1/pls)*20.0));//rpm=(1.0/((0.5/pls)*20.0))*59.0;
        pls=0.0;
    }

    ui->speed->setText(QString::number(rpm));//freq
    pls=0;
    //control
    // ui->qwtPlot->dra
//     if(start){
//     errorAnt=error;
//     error=SetPoint-rpm;
//     prop=kp*error;
//     integ=integ+ki*0.005*error;
//     //if(integ > 6000)

//     rpmD=prop+integ; //convertir a ancho de pulso
//     qDebug()<<rpmD;
//     if(rpmD>8330){rpmD=8330;}
//     if(rpmD<1){rpmD=0;}
//     t=t+0.005;
   //  }
}


QElapsedTimer timer;
void rpmsInterrupt(int gpio, int level, uint32_t tick)
{
//    if(pls==0){
//        timer.start();
//        pls++;
//    }else{
//        period=(float)timer.nsecsElapsed();
//        pls=0;
//    }
    pls++;
}

void cruceInterrupt(int gpio, int level, uint32_t tick)
{
    if(start){
    zeroC=zeroC^1;
      if(zeroC){
          gpioWrite(scr2,0);
          if(scrTime<8000){
          usleep(scrTime);
          gpioWrite(scr1,1);
          }
          usleep(2000);
          gpioWrite(scr1,0);
        }else{
           gpioWrite(scr1,0);
           if(scrTime<8000){
           usleep(scrTime);
           gpioWrite(scr2,1);
           }
           usleep(2000);
           gpioWrite(scr2,0);
        }
    }
}

void Widget::on_Knob_valueChanged(double value)
{
    scrTime=(int)value;
    ui->knob_value->setText(QString::number(scrTime));
}

void Widget::on_pushButton_clicked()
{
    QString sp,p,i;
    start=true;
    sp=ui->setpoint->toPlainText();
    SetPoint=sp.toInt();
    p=ui->kpNumber->toPlainText();
    kp=p.toFloat();
    i=ui->kiNumber->toPlainText();
    ki=i.toFloat();
    qDebug()<<p<<i<<sp;
}


