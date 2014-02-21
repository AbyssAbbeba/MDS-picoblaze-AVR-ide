#include <QtGui>
#include "showhidewidget.h"

ShowHideWidget::ShowHideWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setFocusPolicy(Qt::TabFocus);
    this->isShown = true;
}

void ShowHideWidget::paintEvent ( QPaintEvent * event )
{
    //qDebug() << "ShowHideWidget: is shown " << isShown;
    emit show(isShown);
    isShown = !isShown;
}