/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file ioregswidget.cpp
 */


#include <QtGui>
#include "ioregswidget.h"

IORegsWidget::IORegsWidget(QWidget *parent, int index)
    : QWidget(parent)
{
    this->index = index;
    nextBtn = new QPushButton("Next", this);
    previousBtn = new QPushButton("Previous", this);
    indexLbl = new QLabel(QString::number(index, 10), this);

    ioRegsLayout = new QGridLayout(this);


    //IORegs
    ioRegsLblAddr = new QLabel("Addr", this);
    ioRegsLblValue = new QLabel("Value", this);
    ioRegsLblReadMask = new QLabel("Read Mask", this);
    ioRegsLblWriteMask = new QLabel("Write Mask", this);
    ioRegsLblRandomMask = new QLabel("Random Mask", this);
    ioRegsLblReserved = new QLabel("Reserved", this);
    ioRegsLblVirtual = new QLabel("Virtual", this);
    ioRegsLblName = new QLabel("Name", this);
    ioRegsLblDesc = new QLabel("Desc", this);

    ioRegsChckReserved = new QCheckBox(this);
    ioRegsChckVirtual = new QCheckBox(this);

    ioRegsLEAddr = new QLineEdit(this);
    ioRegsLEValue = new QLineEdit(this);
    ioRegsLEReadMask = new QLineEdit(this);
    ioRegsLEWriteMask = new QLineEdit(this);
    ioRegsLERandomMask = new QLineEdit(this);
    ioRegsLEName = new QLineEdit(this);
    ioRegsLEDesc = new QLineEdit(this);

    //BITS
    bitLblEnable = new QLabel("Bits Enabled", this);
    bitLbl7 = new QLabel("Bit 7", this);
    bitLbl6 = new QLabel("Bit 6", this);
    bitLbl5 = new QLabel("Bit 5", this);
    bitLbl4 = new QLabel("Bit 4", this);
    bitLbl3 = new QLabel("Bit 3", this);
    bitLbl2 = new QLabel("Bit 2", this);
    bitLbl1 = new QLabel("Bit 1", this);
    bitLbl0 = new QLabel("Bit 0", this);
    bitLblSTip7 = new QLabel("Bit 7 STip", this);
    bitLblSTip6 = new QLabel("Bit 6 STip", this);
    bitLblSTip5 = new QLabel("Bit 5 STip", this);
    bitLblSTip4 = new QLabel("Bit 4 STip", this);
    bitLblSTip3 = new QLabel("Bit 3 STip", this);
    bitLblSTip2 = new QLabel("Bit 2 STip", this);
    bitLblSTip1 = new QLabel("Bit 1 STip", this);
    bitLblSTip0 = new QLabel("Bit 0 STip", this);
    bitLblTTip7 = new QLabel("Bit 7 TTip", this);
    bitLblTTip6 = new QLabel("Bit 6 TTip", this);
    bitLblTTip5 = new QLabel("Bit 5 TTip", this);
    bitLblTTip4 = new QLabel("Bit 4 TTip", this);
    bitLblTTip3 = new QLabel("Bit 3 TTip", this);
    bitLblTTip2 = new QLabel("Bit 2 TTip", this);
    bitLblTTip1 = new QLabel("Bit 1 TTip", this);
    bitLblTTip0 = new QLabel("Bit 0 TTip", this);

    bitChckEnable = new QCheckBox(this);

    for (int i = 0; i < 8; i++)
    {
        QLineEdit *preBitLEName = new QLineEdit(this);
        bitLEName.append(preBitLEName);
        QLineEdit *preBitLESTip = new QLineEdit(this);
        bitLESTip.append(preBitLESTip);
        QLineEdit *preBitLETTip = new QLineEdit(this);
        bitLETTip.append(preBitLETTip);
    }
    
    ioRegsLayout->addWidget(ioRegsLblAddr, 0,0);
    ioRegsLayout->addWidget(ioRegsLblValue, 1,0);
    ioRegsLayout->addWidget(ioRegsLblReadMask, 2,0);
    ioRegsLayout->addWidget(ioRegsLblWriteMask, 3,0);
    ioRegsLayout->addWidget(ioRegsLblRandomMask, 4,0);
    ioRegsLayout->addWidget(ioRegsLblReserved, 5,0);
    ioRegsLayout->addWidget(ioRegsLblVirtual, 6,0);
    ioRegsLayout->addWidget(ioRegsLblName, 7,0);
    ioRegsLayout->addWidget(ioRegsLblDesc, 8,0);

    ioRegsLayout->addWidget(ioRegsLEAddr, 0,1);
    ioRegsLayout->addWidget(ioRegsLEValue, 1,1);
    ioRegsLayout->addWidget(ioRegsLEReadMask, 2,1);
    ioRegsLayout->addWidget(ioRegsLEWriteMask, 3,1);
    ioRegsLayout->addWidget(ioRegsLERandomMask, 4,1);
    ioRegsLayout->addWidget(ioRegsLEName, 7,1);
    ioRegsLayout->addWidget(ioRegsLEDesc, 8,1);
    
    ioRegsLayout->addWidget(ioRegsChckReserved, 5,1);
    ioRegsLayout->addWidget(ioRegsChckVirtual, 6,1);

    ioRegsLayout->addWidget(bitLblEnable, 0,2);
    ioRegsLayout->addWidget(bitLbl7, 1,2);
    ioRegsLayout->addWidget(bitLblSTip7, 2,2);
    ioRegsLayout->addWidget(bitLblTTip7, 3,2);
    ioRegsLayout->addWidget(bitLbl6, 4,2);
    ioRegsLayout->addWidget(bitLblSTip6, 5,2);
    ioRegsLayout->addWidget(bitLblTTip6, 6,2);
    ioRegsLayout->addWidget(bitLbl5, 7,2);
    ioRegsLayout->addWidget(bitLblSTip5, 8,2);
    ioRegsLayout->addWidget(bitLblTTip5, 9,2);
    ioRegsLayout->addWidget(bitLbl4, 10,2);
    ioRegsLayout->addWidget(bitLblSTip4, 11,2);
    ioRegsLayout->addWidget(bitLblTTip4, 12,2);
    ioRegsLayout->addWidget(bitLbl3, 13,2);
    ioRegsLayout->addWidget(bitLblSTip3, 14,2);
    ioRegsLayout->addWidget(bitLblTTip3, 15,2);
    ioRegsLayout->addWidget(bitLbl2, 1,4);
    ioRegsLayout->addWidget(bitLblSTip2, 2,4);
    ioRegsLayout->addWidget(bitLblTTip2, 3,4);
    ioRegsLayout->addWidget(bitLbl1, 4,4);
    ioRegsLayout->addWidget(bitLblSTip1, 5,4);
    ioRegsLayout->addWidget(bitLblTTip1, 6,4);
    ioRegsLayout->addWidget(bitLbl0, 7,4);
    ioRegsLayout->addWidget(bitLblSTip0, 8,4);
    ioRegsLayout->addWidget(bitLblTTip0, 9,4);

    ioRegsLayout->addWidget(bitChckEnable, 0,3);

    ioRegsLayout->addWidget(bitLEName.at(0), 1,3);
    ioRegsLayout->addWidget(bitLESTip.at(0), 2,3);
    ioRegsLayout->addWidget(bitLETTip.at(0), 3,3);
    ioRegsLayout->addWidget(bitLEName.at(1), 4,3);
    ioRegsLayout->addWidget(bitLESTip.at(1), 5,3);
    ioRegsLayout->addWidget(bitLETTip.at(1), 6,3);
    ioRegsLayout->addWidget(bitLEName.at(2), 7,3);
    ioRegsLayout->addWidget(bitLESTip.at(2), 8,3);
    ioRegsLayout->addWidget(bitLETTip.at(2), 9,3);
    ioRegsLayout->addWidget(bitLEName.at(3), 10,3);
    ioRegsLayout->addWidget(bitLESTip.at(3), 11,3);
    ioRegsLayout->addWidget(bitLETTip.at(3), 12,3);
    ioRegsLayout->addWidget(bitLEName.at(4), 13,3);
    ioRegsLayout->addWidget(bitLESTip.at(4), 14,3);
    ioRegsLayout->addWidget(bitLETTip.at(4), 15,3);
    ioRegsLayout->addWidget(bitLEName.at(5), 1,5);
    ioRegsLayout->addWidget(bitLESTip.at(5), 2,5);
    ioRegsLayout->addWidget(bitLETTip.at(5), 3,5);
    ioRegsLayout->addWidget(bitLEName.at(6), 4,5);
    ioRegsLayout->addWidget(bitLESTip.at(6), 5,5);
    ioRegsLayout->addWidget(bitLETTip.at(6), 6,5);
    ioRegsLayout->addWidget(bitLEName.at(7), 7,5);
    ioRegsLayout->addWidget(bitLESTip.at(7), 8,5);
    ioRegsLayout->addWidget(bitLETTip.at(7), 9,5);

    ioRegsLayout->addWidget(indexLbl, 13,5);
    ioRegsLayout->addWidget(nextBtn, 14,5);
    ioRegsLayout->addWidget(previousBtn, 15,5);
    
    
    this->setLayout(ioRegsLayout);
    
}