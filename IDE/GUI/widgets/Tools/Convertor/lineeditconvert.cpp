#include <QtGui>
#include "lineeditconvert.h"

LineEditConvert::LineEditConvert(QWidget *parent, int base)
    : QLineEdit(parent)
{
    connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(catchEditSignal(const QString &)));
    this->base = base;
}


void LineEditConvert::catchEditSignal(const QString &text)
{
    emit textEditedSig(text, this->base);
}

void LineEditConvert::setTextSlot(const QString &text, int base)
{
    bool done;
    int number = text.toInt(&done, base);
    if (done)
    {
        QString a = QString::number(number, this->base);
        this->setText(a);
    }
    else if (text == "")
            this->setText("");
        else
            this->setText("INVALID NUMBER");
}
