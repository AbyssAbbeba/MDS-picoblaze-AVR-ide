/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup ShowHideWidget
 * @file showhidewidget.cpp
 */


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