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
    this->numWidth = 3;
    if ("INTERRUPT" == event)
    {
        ui.lblEvent->setStyleSheet("QLabel { color:red ;}");
    }
    
    QString text;
    
    text = "0x";
    for (int i = QString::number(to, 16).length(); i < this->numWidth; i++)
    {
        text.append("0");
    }
    ui.lblToAddr->setText(text + QString::number(to, 16).toUpper());

    text = "0x";
    for (int i = QString::number(from, 16).length(); i < this->numWidth; i++)
    {
        text.append("0");
    }
    ui.lblFromAddr->setText(text + QString::number(from, 16).toUpper());
}