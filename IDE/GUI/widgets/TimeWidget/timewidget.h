#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H


#include <QWidget>
#include "ui_timewidget.h"


constexpr double MILI = 0.001;
constexpr double MICRO = MILI * MILI;
constexpr double NANO = MILI * MICRO;
constexpr double PICO = MILI * NANO;

class TimeWidget : public QWidget
{
    public:
        TimeWidget(QWidget *parent);
        void setTime(double time);

    private:
        Ui_TimeWidget ui;
};




#endif