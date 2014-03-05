#ifndef TIMEWIDGET_H
#define TIMEWIDGET_H


#include <QWidget>
#include "ui_timewidget.h"

class TimeWidget : public QWidget
{
    public:
        TimeWidget(QWidget *parent);
        void setTime(long double time);

    private:
        Ui_TimeWidget ui;
};




#endif