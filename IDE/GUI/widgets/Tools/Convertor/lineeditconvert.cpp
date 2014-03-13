/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup LineEditConvert
 * @file lineeditconvert.cpp
 */


#include <QtGui>
#include "lineeditconvert.h"

LineEditConvert::LineEditConvert(QWidget *parent, int base)
    : QLineEdit(parent)
{
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(catchEditSignal(const QString &)));
    this->base = base;
    QFontMetrics metrics(this->font());
    if (base == 2)
    {
        //this->setInputMask("BBBBBBBB");
        QRegExpValidator *binValidator = new QRegExpValidator(QRegExp("[01]{8}"), this);
        this->setValidator(binValidator);
        this->setText("");
        this->setMaximumWidth(metrics.width("00000000")+10);
    }
    else if (base == 10)
    {
        //this->setInputMask("999");
        QIntValidator *dec8bitValidator = new QIntValidator(0, 255, this);
        this->setValidator(dec8bitValidator);
        this->setText("");
        this->setMaximumWidth(metrics.width("255")+10);
    }
    else
    {
        //this->setInputMask("HH");
        QRegExpValidator *hexValidator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{2}"), this);
        this->setValidator(hexValidator);
        this->setText("");
        this->setMaximumWidth(metrics.width("DD")+10);
    }
    
}


void LineEditConvert::catchEditSignal(const QString &text)
{
    emit textEditedSig(text, this->base);
}

void LineEditConvert::setTextSlot(const QString &text, int base)
{
    bool done;
    int number = text.toInt(&done, base);
    //qDebug () << "LineEditConvert: Number" << text << "of base" << base << "is" << number;
    if (done)
    {
        QString a = QString::number(number, this->base);
        //qDebug() << "LineEditConvert: Number converted to base" << this->base << "is" << a;
        /*int length = 0;
        if (this->base == 2)
        {
            length = 8 - a.length();
        }
        else if (this->base == 10)
        {
            length = 3 - a.length();
        }
        else
        {
            length = 2 - a.length();
        }
        while (length > 0)
        {
            a.prepend('0');
            length--;
        }*/
        //qDebug() << "LineEditConvert: Text set:" << a;
        this->setText(a);
    }
    else if (text == "")
    {
        this->setText("");
    }
    /*else
    {
        this->setText("INVALID NUMBER");
    }*/
}
