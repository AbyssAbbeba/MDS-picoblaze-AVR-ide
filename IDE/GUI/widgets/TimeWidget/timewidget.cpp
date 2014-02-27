#include <QtGui>
#include "timewidget.h"


TimeWidget::TimeWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}


void TimeWidget::setTime(double time)
{
    double m = 0; // Minutes.
    double s = 0; // Seconds.
    double ms = 0; // Mili-seconds.
    double us = 0; // Micro-seconds.
    double ns = 0; // Nano-seconds
    double ps = 0; // Pico-seconds.

    if ( time >= 60.0 )
    {
        m = (time / 60);
        time -=  60 *  m;
    }
    if ( time >= 1.0 )
    {
        s = qFloor(time);
        time -= s;
    }
    if ( time >= MILI )
    {
        ms =  qFloor(time / MILI);
        time -= ( MILI *  ms );
    }
    if ( time >= MICRO )
    {
        us = qFloor(time / MICRO);
        time -=  ( MICRO * us );
    }
    if ( time >= NANO )
    {
        qDebug() << "TimeWidget: time/nano = " << QString::number(time/NANO,'f', 4);
        ns = qFloor(time / NANO);
        qDebug() << "TimeWidget: ns = " << QString::number(ns,'f', 4);
        time -=  ( NANO *  ns );
    }
    if ( time >= PICO )
    {
        ps = qFloor(time / PICO);
        //qDebug() << "TimeWidget: ns = " << QString::number(ps,'f', 4);
    }

    ui.lblMin->setText(QString::number(m, 'f', 0));
    ui.lblSec->setText(QString::number(s, 'f', 0));
    ui.lblMs->setText(QString::number(ms, 'f', 0));
    ui.lblUs->setText(QString::number(us, 'f', 0));
    ui.lblNs->setText(QString::number(ns, 'f', 0));
    ui.lblPs->setText(QString::number(ps, 'f', 0));
}