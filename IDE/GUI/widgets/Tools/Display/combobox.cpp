#include <QtGui>
#include "combobox.h"

ComboBox::ComboBox(QWidget *parent)
    : QComboBox(parent)
{
    for (int i = 0; i < 8; i++)
        this->addItem(QString::number(i));
    connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(changedEmit(int)));
}


void ComboBox::changedEmit(int index)
{
    qDebug() << "changed";
    emit changed(this, index);
}