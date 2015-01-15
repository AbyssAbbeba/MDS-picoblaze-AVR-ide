/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CallItemWidget
 * @file callitemwidget.cpp
 */


#include <QtGui>
#include "callitemwidget.h"

CallItemWidget::CallItemWidget(QWidget *parent, QString event, int from, int to)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.lblEvent->setText(event);
    if ("INTERRUPT" == event)
    {
        ui.lblEvent->setStyleSheet("QLabel { color:red ;}");
    }
    ui.lblToAddr->setText("0x" + QString::number(to, 16));
    ui.lblFromAddr->setText("0x" + QString::number(from, 16));
}