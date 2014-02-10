#include <QtGui>
#include "convertortool.h"

ConvertorTool::ConvertorTool(QWidget *parent)
    : QWidget(parent)
{
    //le_dec->setFrame(false);
    LineEditConvert *le_hex1 = new LineEditConvert(this, 16);
    LineEditConvert *le_hex2 = new LineEditConvert(this, 16);
    LineEditConvert *le_hex3 = new LineEditConvert(this, 16);
    LineEditConvert *le_bin1 = new LineEditConvert(this, 2);
    LineEditConvert *le_bin2 = new LineEditConvert(this, 2);
    LineEditConvert *le_bin3 = new LineEditConvert(this, 2);
    LineEditConvert *le_dec1 = new LineEditConvert(this, 10);
    LineEditConvert *le_dec2 = new LineEditConvert(this, 10);
    LineEditConvert *le_dec3 = new LineEditConvert(this, 10);
    QGridLayout *layout = new QGridLayout(this);
    layout->setSpacing(1);
    QLabel *hex = new QLabel("HEX", this);
    QLabel *bin = new QLabel("BIN", this);
    QLabel *dec = new QLabel("DEC", this);
    layout->addWidget(hex, 0, 0);
    layout->addWidget(le_hex1, 1, 0);
    layout->addWidget(le_hex2, 2, 0);
    layout->addWidget(le_hex3, 3, 0);
    layout->addWidget(bin, 0, 1);
    layout->addWidget(le_bin1, 1, 1);
    layout->addWidget(le_bin2, 2, 1);
    layout->addWidget(le_bin3, 3, 1);
    layout->addWidget(dec, 0, 2);
    layout->addWidget(le_dec1, 1, 2);
    layout->addWidget(le_dec2, 2, 2);
    layout->addWidget(le_dec3, 3, 2);
    this->setLayout(layout);
    this->show();
    //this->setWindowFlags(Qt::WindowStaysOnTopHint);
    
    connect(le_dec1, SIGNAL(textEditedSig(const QString &, int)), le_hex1, SLOT(setTextSlot(const QString &, int)));
    connect(le_dec1, SIGNAL(textEditedSig(const QString &, int)), le_bin1, SLOT(setTextSlot(const QString &, int)));
    connect(le_hex1, SIGNAL(textEditedSig(const QString &, int)), le_dec1, SLOT(setTextSlot(const QString &, int)));
    connect(le_hex1, SIGNAL(textEditedSig(const QString &, int)), le_bin1, SLOT(setTextSlot(const QString &, int)));
    connect(le_bin1, SIGNAL(textEditedSig(const QString &, int)), le_dec1, SLOT(setTextSlot(const QString &, int)));
    connect(le_bin1, SIGNAL(textEditedSig(const QString &, int)), le_hex1, SLOT(setTextSlot(const QString &, int)));

    connect(le_dec2, SIGNAL(textEditedSig(const QString &, int)), le_hex2, SLOT(setTextSlot(const QString &, int)));
    connect(le_dec2, SIGNAL(textEditedSig(const QString &, int)), le_bin2, SLOT(setTextSlot(const QString &, int)));
    connect(le_hex2, SIGNAL(textEditedSig(const QString &, int)), le_dec2, SLOT(setTextSlot(const QString &, int)));
    connect(le_hex2, SIGNAL(textEditedSig(const QString &, int)), le_bin2, SLOT(setTextSlot(const QString &, int)));
    connect(le_bin2, SIGNAL(textEditedSig(const QString &, int)), le_dec2, SLOT(setTextSlot(const QString &, int)));
    connect(le_bin2, SIGNAL(textEditedSig(const QString &, int)), le_hex2, SLOT(setTextSlot(const QString &, int)));

    connect(le_dec3, SIGNAL(textEditedSig(const QString &, int)), le_hex3, SLOT(setTextSlot(const QString &, int)));
    connect(le_dec3, SIGNAL(textEditedSig(const QString &, int)), le_bin3, SLOT(setTextSlot(const QString &, int)));
    connect(le_hex3, SIGNAL(textEditedSig(const QString &, int)), le_dec3, SLOT(setTextSlot(const QString &, int)));
    connect(le_hex3, SIGNAL(textEditedSig(const QString &, int)), le_bin3, SLOT(setTextSlot(const QString &, int)));
    connect(le_bin3, SIGNAL(textEditedSig(const QString &, int)), le_dec3, SLOT(setTextSlot(const QString &, int)));
    connect(le_bin3, SIGNAL(textEditedSig(const QString &, int)), le_hex3, SLOT(setTextSlot(const QString &, int)));
}
