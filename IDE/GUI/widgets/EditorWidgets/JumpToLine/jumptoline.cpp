/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup JumpToLine
 * @file jumptoline.cpp
 */


#include <QtGui>
#include "jumptoline.h"



/**
 * @brief Constructor.
 * @param parent Parent QWidget.
 */
JumpToLine::JumpToLine(QWidget *parent, int maxLines, int currLine)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.spinLine->setMaximum(maxLines);
    ui.spinLine->setValue(currLine);
    connect(ui.btnJump, SIGNAL(clicked()), this, SLOT(emitJump()));
}


void JumpToLine::emitJump()
{
    emit jmptoline(ui.spinLine->value());
}