/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup OutputWidget
 * @file outputwidget.cpp
 */


#include <QtGui>
#include "outputwidget.h"

OutputWidget::OutputWidget(QWidget *parent)
    : QTextEdit(parent)
{
    this->setFont(QFont("Andale Mono", 10));
    //this->installEventFilter(this);
    //this->setWordWrap();
}
