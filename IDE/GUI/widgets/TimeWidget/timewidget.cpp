#include <QtGui>
#include "timewidget.h"


TimeWidget::TimeWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}


void TimeWidget::setTime(double time)
{
    /*double s = 0; // Seconds.
    double ms = 0; // Mili-seconds.
    double us = 0; // Micro-seconds.
    double ns = 0; // Nano-seconds
    double ps = 0; // Pico-seconds.*/
    

    if ( time >= 60.0 )
    {
        ui.lblMin->setText(QString::number(time / 60, 'f', 0));
        time -=  60 * (unsigned int) (time / 60);
    }
    else
    {
        ui.lblMin->setText(QString::number(0, 10));
    }
    
    if ( time >= 1.0 )
    {
        ui.lblSec->setText(QString::number((unsigned int)(time), 10));
        time -= (unsigned int)(time);
    }
    else
    {
        ui.lblSec->setText(QString::number(0, 10));
    }
    time = time * 1000.0;
    
    if ( time >= 1.0 )
    {
        qDebug() << "TimeWidget: time/mili = " << QString::number((unsigned int)time,10);
        ui.lblMs->setText(QString::number((unsigned int)(time), 10));
        time -= ( (unsigned int) (time) );
        qDebug() << "TimeWidget: time = " << QString::number(time, 'f', 10);
    }
    else
    {
        ui.lblMs->setText(QString::number(0, 10));
    }
    time = time * 1000.0;
    
    if ( time >= 1.0 )
    {
        qDebug() << "TimeWidget: time/micro = " << QString::number((unsigned int)time,10);
        ui.lblUs->setText(QString::number((unsigned int)(time), 10));
        time -=  ( (unsigned int)(time) );
        qDebug() << "TimeWidget: time = " << QString::number(time, 'f', 10);
    }
    else
    {
        ui.lblUs->setText(QString::number(0, 10));
    }
    time = time * 1000.0;
    
    if ( time >= 1.0 )
    {
        qDebug() << "TimeWidget: time/nano = " << QString::number((unsigned int)time,10);
        if (QString::number(time-(unsigned int)(time), 'f', 1) == "1.0")
        {
            qDebug() << "TimeWidget: fail, recalculating";
            ui.lblNs->setText(QString::number((unsigned int)(time) + 1,10));
            time -=  ( (unsigned int)(time) + 1 );
        }
        else
        {
            ui.lblNs->setText(QString::number((unsigned int)(time),10));
            //qDebug() << "TimeWidget: ns = " << QString::number(ns,10);
            time -=  ( (unsigned int)(time) );
        }
        qDebug() << "TimeWidget: time = " << QString::number(time, 'f', 1);
    }
    else
    {
        ui.lblNs->setText(QString::number(0, 10));
    }
    time = time * 1000.0;
    
    if ( time >= 1.0 )
    {
        qDebug() << "TimeWidget: time/pico = " << QString::number((unsigned int)time, 10);
        ui.lblPs->setText(QString::number((unsigned int)(time), 10));
        //qDebug() << "TimeWidget: ns = " << QString::number(ps,10);
    }
    else
    {
        ui.lblPs->setText(QString::number(0, 10));
    }

    //ui.lblSec->setText(QString::number(s, 'f', 0));
    //ui.lblMs->setText(QString::number(ms, 'f', 0));
    //ui.lblUs->setText(QString::number(us, 'f', 0));
    //ui.lblNs->setText(QString::number(ns, 'f', 0));
    //ui.lblPs->setText(QString::number(ps, 'f', 0));
}