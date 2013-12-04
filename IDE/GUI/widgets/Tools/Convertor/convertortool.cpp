#include <QtGui>
#include "convertortool.h"

ConvertorTool::ConvertorTool(QWidget *parent)
    : QWidget(parent)
{
    LineEditConvert *le_dec = new LineEditConvert(this, 10);
    //le_dec->setFrame(false);
    LineEditConvert *le_hex = new LineEditConvert(this, 16);
    LineEditConvert *le_bin = new LineEditConvert(this, 2);
    connect(le_dec, SIGNAL(textEditedSig(const QString &, int)), le_hex, SLOT(setTextSlot(const QString &, int))); 
    connect(le_dec, SIGNAL(textEditedSig(const QString &, int)), le_bin, SLOT(setTextSlot(const QString &, int))); 
    connect(le_hex, SIGNAL(textEditedSig(const QString &, int)), le_dec, SLOT(setTextSlot(const QString &, int))); 
    connect(le_hex, SIGNAL(textEditedSig(const QString &, int)), le_bin, SLOT(setTextSlot(const QString &, int))); 
    connect(le_bin, SIGNAL(textEditedSig(const QString &, int)), le_dec, SLOT(setTextSlot(const QString &, int))); 
    connect(le_bin, SIGNAL(textEditedSig(const QString &, int)), le_hex, SLOT(setTextSlot(const QString &, int))); 
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *dec = new QLabel("Decimal", this);
    QLabel *hex = new QLabel("Hexadecimal", this);
    QLabel *bin = new QLabel("Binary", this);
    layout->addWidget(dec);
    layout->addWidget(le_dec);
    layout->addWidget(hex);
    layout->addWidget(le_hex);
    layout->addWidget(bin);
    layout->addWidget(le_bin);
    this->setLayout(layout);
    this->show();
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
}
