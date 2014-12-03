/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup Find
 * @file find.cpp
 */


#include <QtGui>
#include "find.h"



/**
 * @brief Constructor.
 * @param parent Parent QWidget.
 */
Find::Find(QWidget *parent, QString query)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.leQuery->setText(query);
    connect(ui.btnNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(ui.btnPrev, SIGNAL(clicked()), this, SLOT(findPrevious()));
}


void Find::findNext()
{
    if (false == ui.leQuery->text().isEmpty())
    {
        emit find(ui.leQuery->text(), true, ui.chckCase->isChecked());
    }
}


void Find::findPrevious()
{
    if (false == ui.leQuery->text().isEmpty())
    {
        emit find(ui.leQuery->text(), false, ui.chckCase->isChecked());
    }
}