#include <QtGui>
#include "lineeditconvert.h"

LineEditConvert::LineEditConvert(QWidget *parent, int base)
    : QLineEdit(parent)
{
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(catchEditSignal(const QString &)));
    this->base = base;
    if (base == 2)
    {
        this->setInputMask("BBBBBBBBBBBBBBBB");
        this->setText("0000000000000000");
    }
    else if (base == 10)
    {
        this->setInputMask("99999");
        QIntValidator *dec32bitValidator = new QIntValidator(0, 65535, this);
        this->setValidator(dec32bitValidator);
        this->setText("00000");
    }
    else
    {
        this->setInputMask("HHHH");
        this->setText("0000");
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
    qDebug () << "LineEditConvert: Number" << text << "of base" << base << "is" << number;
    if (done)
    {
        QString a = QString::number(number, this->base);
        qDebug() << "LineEditConvert: Number converted to base" << this->base << "is" << a;
        int length = 0;
        if (this->base == 2)
        {
            length = 16 - a.length();
        }
        else if (this->base == 10)
        {
            length = 5 - a.length();
        }
        else
        {
            length = 4 - a.length();
        }
        while (length > 0)
        {
            a.prepend('0');
            length--;
        }
        qDebug() << "LineEditConvert: Text set:" << a;
        this->setText(a);
    }
    else if (text == "")
    {
        this->setText("0");
    }
    /*else
    {
        this->setText("INVALID NUMBER");
    }*/
}
