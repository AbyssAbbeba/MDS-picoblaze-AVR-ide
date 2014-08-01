/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup AboutWidget
 * @file aboutwidget.cpp
 */


#include <QtGui>
#include "aboutwidget.h"
#include "../../guicfg/guicfg.h"



AboutWidget::AboutWidget(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}