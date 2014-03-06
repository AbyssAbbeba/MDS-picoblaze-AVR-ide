/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup ComboBox
 * @file combobox.cpp
 */


#include <QtGui>
#include "combobox.h"

ComboBox::ComboBox(QWidget *parent, int index)
    : QComboBox(parent)
{
    this->cbIndex = index;
    this->conflict = false;
    this->previousPin = index;
    for (int i = 0; i < 8; i++)
    {
        this->addItem(QString::number(i));
    }
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(changedEmit(int)));
    //this->setAutoFillBackground(true);
    this->setMaximumHeight(20);
}


void ComboBox::changedEmit(int index)
{
    //qDebug() << "changed";
    emit changed(this, index);
}