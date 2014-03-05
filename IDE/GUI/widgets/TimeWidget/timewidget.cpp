#include <QtGui>
#include <cmath>
#include "timewidget.h"

TimeWidget::TimeWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
}

void TimeWidget::setTime ( long double time )
{
    long long unsigned int picosec = round ( time * 1000000000000.0 );

    ui.lblMin -> setText ( QString::number ( picosec / 60000000000000ULL          , 10 ) );
    ui.lblSec -> setText ( QString::number ( picosec /  1000000000000ULL %   60ULL, 10 ) );
    ui.lblMs  -> setText ( QString::number ( picosec /     1000000000ULL % 1000ULL, 10 ) );
    ui.lblUs  -> setText ( QString::number ( picosec /        1000000ULL % 1000ULL, 10 ) );
    ui.lblNs  -> setText ( QString::number ( picosec /           1000ULL % 1000ULL, 10 ) );
    ui.lblPs  -> setText ( QString::number ( picosec                     % 1000ULL, 10 ) );
}
