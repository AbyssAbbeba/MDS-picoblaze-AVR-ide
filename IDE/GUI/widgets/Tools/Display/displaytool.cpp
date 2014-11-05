/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup DisplayTool
 * @file displaytool.cpp
 */


#include <QtGui>
#include "displaytool.h"
#include "combobox.h"
#include "displaysegment.h"

DisplayTool::DisplayTool(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("8 Segment Editor");
    this->setWindowIcon(QIcon(QPixmap(":resources/icons/8segedit.png")));
    QWidget *leftWidget = new QWidget(this);
    QWidget *rightWidget = new QWidget(this);
    leftWidget->show();
    leftWidget->setMinimumWidth(130);
    leftWidget->setMinimumHeight(160);
    rightWidget->show();
    rightWidget->setMinimumWidth(140);
    rightWidget->setMinimumHeight(220);
    leftWidget->move(0,0);
    rightWidget->move(270, 0);

    //center
    display = new DisplaySegment(this);
    connect(display, SIGNAL(pressed(int)), this, SLOT(segmentPressed(int)));
    connect(display, SIGNAL(numberChanged(int, bool)), SLOT(pinEdited(int, bool)));
    display->move(130, 0);

    //left
    cathodeLabels[0] = new QLabel("Common cathode", leftWidget);
    cathodeLabels[1] = new QLabel("Hex", leftWidget);
    cathodeLabels[2] = new QLabel("Dec", leftWidget);
    cathodeLabels[3] = new QLabel("Bin", leftWidget);
    anodeLabels[0] = new QLabel("Common anode", leftWidget);
    anodeLabels[1] = new QLabel("Hex", leftWidget);
    anodeLabels[2] = new QLabel("Dec", leftWidget);
    anodeLabels[3] = new QLabel("Bin", leftWidget);

    cathodeLE[0] = new QLineEdit("00", leftWidget);
    cathodeLE[0]->setMaximumWidth(30);
    cathodeLE[0]->setMaximumHeight(20);
    cathodeLE[0]->setInputMask("HH");
    cathodeLE[1] = new QLineEdit("000", leftWidget);
    cathodeLE[1]->setMaximumWidth(40);
    cathodeLE[1]->setMaximumHeight(20);
    cathodeLE[1]->setInputMask("999");
    QIntValidator *cathodeDec = new QIntValidator(0, 255, cathodeLE[1]);
    cathodeLE[1]->setValidator(cathodeDec);
    cathodeLE[2] = new QLineEdit("00000000", leftWidget);
    cathodeLE[2]->setMaximumWidth(80);
    cathodeLE[2]->setMaximumHeight(20);
    cathodeLE[2]->setInputMask("BBBBBBBB");
    anodeLE[0] = new QLineEdit("00", leftWidget);
    anodeLE[0]->setMaximumWidth(30);
    anodeLE[0]->setMaximumHeight(20);
    anodeLE[0]->setInputMask("HH");
    anodeLE[1] = new QLineEdit("000", leftWidget);
    anodeLE[1]->setMaximumWidth(40);
    anodeLE[1]->setMaximumHeight(20);
    anodeLE[1]->setInputMask("999");
    QIntValidator *anodeDec = new QIntValidator(0, 255, anodeLE[1]);
    anodeLE[1]->setValidator(anodeDec);
    anodeLE[2] = new QLineEdit("00000000", leftWidget);
    anodeLE[2]->setMaximumWidth(80);
    anodeLE[2]->setMaximumHeight(20);
    anodeLE[2]->setInputMask("BBBBBBBB");

    cathodeLabels[0]->move(0,0);
    cathodeLabels[1]->move(20,20);
    cathodeLabels[2]->move(20,40);
    cathodeLabels[3]->move(20,60);
    cathodeLE[0]->move(50, 20);
    cathodeLE[1]->move(50, 40);
    cathodeLE[2]->move(50, 60);
    anodeLabels[0]->move(0,80);
    anodeLabels[1]->move(20,100);
    anodeLabels[2]->move(20,120);
    anodeLabels[3]->move(20,140);
    anodeLE[0]->move(50, 100);
    anodeLE[1]->move(50, 120);
    anodeLE[2]->move(50, 140);

    connect(cathodeLE[0], SIGNAL(textEdited(const QString &)), this, SLOT(updateLEHexCat(const QString &)));
    connect(cathodeLE[1], SIGNAL(textEdited(const QString &)), this, SLOT(updateLEDecCat(const QString &)));
    connect(cathodeLE[2], SIGNAL(textEdited(const QString &)), this, SLOT(updateLEBinCat(const QString &)));
    connect(anodeLE[0], SIGNAL(textEdited(const QString &)), this, SLOT(updateLEHexAn(const QString &)));
    connect(anodeLE[1], SIGNAL(textEdited(const QString &)), this, SLOT(updateLEDecAn(const QString &)));
    connect(anodeLE[2], SIGNAL(textEdited(const QString &)), this, SLOT(updateLEBinAn(const QString &)));


    //right
    labelsUp[0] = new QLabel("LED", rightWidget);
    labelsUp[1] = new QLabel("VAL", rightWidget);
    labelsUp[2] = new QLabel("PIN", rightWidget);

    labelsUp[0]->move(0,0);
    labelsUp[1]->move(40,0);
    labelsUp[2]->move(80,0);

    labelsLED[0] = new QLabel("A", rightWidget);
    labelsLED[1] = new QLabel("B", rightWidget);
    labelsLED[2] = new QLabel("C", rightWidget);
    labelsLED[3] = new QLabel("D", rightWidget);
    labelsLED[4] = new QLabel("E", rightWidget);
    labelsLED[5] = new QLabel("F", rightWidget);
    labelsLED[6] = new QLabel("G", rightWidget);
    labelsLED[7] = new QLabel("P", rightWidget);

    labelsLED[0]->move(0, 20);
    labelsLED[1]->move(0, 45);
    labelsLED[2]->move(0, 70);
    labelsLED[3]->move(0, 95);
    labelsLED[4]->move(0, 120);
    labelsLED[5]->move(0, 145);
    labelsLED[6]->move(0, 170);
    labelsLED[7]->move(0, 195);

    labels[0] = new QLabel("0", rightWidget);
    labels[1] = new QLabel("0", rightWidget);
    labels[2] = new QLabel("0", rightWidget);
    labels[3] = new QLabel("0", rightWidget);
    labels[4] = new QLabel("0", rightWidget);
    labels[5] = new QLabel("0", rightWidget);
    labels[6] = new QLabel("0", rightWidget);
    labels[7] = new QLabel("0", rightWidget);

    labels[0]->move(40, 20);
    labels[1]->move(40, 45);
    labels[2]->move(40, 70);
    labels[3]->move(40, 95);
    labels[4]->move(40, 120);
    labels[5]->move(40, 145);
    labels[6]->move(40, 170);
    labels[7]->move(40, 195);

    for (int i = 0; i < 8; i++)
    {
        comboBoxes[i] = new ComboBox(rightWidget, i);
        comboBoxes[i]->move(80, i*25+20);
        comboBoxes[i]->setCurrentIndex(i);
        connect(comboBoxes[i], SIGNAL(changed(ComboBox*, int)), this, SLOT(updateComboBoxes(ComboBox*, int)));
    }

    for (int i = 0; i < 8; i++)
    {
        config[i] = false;
    }
    this->show();
    this->setFixedSize(this->size());
}

DisplayTool::~DisplayTool()
{
    delete this;
}


void DisplayTool::segmentPressed(int index)
{
    if (config[index] == true)
    {
        config[index] = false;
        labels[index]->setText("0");
        //qDebug() << "Display Tool: pin" << comboBoxes[index]->currentIndex();
        int currPin = comboBoxes[index]->currentIndex();
        for (int i = 0; i < 8; i++)
        {
            if (i != index && currPin == comboBoxes[i]->currentIndex())
            {
                display->activateSegment(i, false);
                labels[i]->setText("0");
                config[i] = false;
            }
        }
        updateLEGlobal(comboBoxes[index]->currentIndex(), false);
    }
    else
    {
        labels[index]->setText("1");
        config[index] = true;
        //qDebug() << "Display Tool: pin" << comboBoxes[index]->currentIndex();
        int currPin = comboBoxes[index]->currentIndex();
        for (int i = 0; i < 8; i++)
        {
            if (i != index && currPin == comboBoxes[i]->currentIndex())
            {
                display->activateSegment(i, true);
                labels[i]->setText("1");
                config[i] = true;
            }
        }
        updateLEGlobal(comboBoxes[index]->currentIndex(), true);
    }
}


void DisplayTool::pinEdited(int pin, bool active)
{
    for (int i = 0; i < 8; i++)
    {
        if (comboBoxes[i]->currentIndex() == pin)
        {
            display->activateSegment(i, active);
            config[i] = active;
            if (active == true)
            {
                labels[i]->setText("1");
            }
            else
            {
                labels[i]->setText("0");
            }
        }
    }
}


//index is pin
void DisplayTool::updateComboBoxes(ComboBox *box, int index)
{
    //qDebug() << "Combo Box: testing";
    bool conflict = false;
    for (int i = 0; i < 8; i++)
    {
        if (comboBoxes[i] != box)
        {
            if (comboBoxes[i]->currentIndex() == index)
            {
                //qDebug() << "Combo Box: color changing to red";
                conflict = true;
                if (box->conflict == true)
                {
                    //qDebug() << "DisplayTool: combobox color changing to normal for pin" << index;
                    box->setItemData(box->previousPin, box->palette().base().color(), Qt::BackgroundRole);
                    char globalPinConflict = 0;
                    int lastPinConflict = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        if (comboBoxes[i] != box)
                        {
                            if (box->previousPin == comboBoxes[i]->previousPin)
                            {
                                globalPinConflict++;
                                lastPinConflict = i;
                            }
                        }
                    }
                    if (globalPinConflict == 1)
                    {
                        comboBoxes[lastPinConflict]->conflict = false;
                        comboBoxes[lastPinConflict]->previousPin = comboBoxes[lastPinConflict]->cbIndex;
                        comboBoxes[lastPinConflict]->setItemData(comboBoxes[lastPinConflict]->previousPin, comboBoxes[lastPinConflict]->palette().base().color(), Qt::BackgroundRole);
                        //change color of combo box
                    }
                }
                else
                {
                    updateLEGlobal(box->previousPin, false);
                }
                comboBoxes[i]->setItemData(index, Qt::red, Qt::BackgroundRole);
                box->setItemData(index, Qt::red, Qt::BackgroundRole);
                labels[box->cbIndex]->setText(labels[i]->text());
                config[box->cbIndex] = config[i];
                box->conflict = true;
                box->previousPin = index;
                comboBoxes[i]->conflict = true;
                comboBoxes[i]->previousPin = index;
                display->activateSegment(box->cbIndex, config[box->cbIndex]);
                //QPalette p = box->palette();

                // Outline around the menu
                //p.setColor(QPalette::Window, Qt::red);
                //p.setColor(QPalette::WindowText, Qt::red);

                // combo button
                //p.setColor(QPalette::Button, Qt::red);
                //p.setColor(QPalette::ButtonText, Qt::red);

                // combo menu
                //p.setColor(QPalette::Base, Qt::red);
                //p.setColor(QPalette::Text, Qt::red);

                // highlight button & menu
                //p.setColor(QPalette::Highlight, Qt::red);
                //p.setColor(QPalette::HighlightedText, Qt::red);

                // to customize the disabled color
                //p.setColor(QPalette::Disabled, QPalette::Button, Qt::red);
                //p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::red);

                //box->setPalette(p);
                //box->setPalette(QPalette(Qt::red));
                //box->setStyleSheet("QComboBox {selection-color: red;"
                //         "selection-background-color: red;}");
                //box->update();
                //comboBoxes[i]->setPalette(QPalette(Qt::red));
                //comboBoxes[i]->update();//*/
            }
        }
    }
    if (conflict == false)
    {
        if (box->conflict == true)
        {
            //qDebug() << "DisplayTool: combobox color changing to normal for pin" << index;
            box->setItemData(box->previousPin, box->palette().base().color(), Qt::BackgroundRole);
            char globalPinConflict = 0;
            int lastPinConflict = 0;
            for (int i = 0; i < 8; i++)
            {
                if (comboBoxes[i] != box)
                {
                    if (box->previousPin == comboBoxes[i]->previousPin)
                    {
                        globalPinConflict++;
                        lastPinConflict = i;
                    }
                }
            }
            if (globalPinConflict == 1)
            {
                comboBoxes[lastPinConflict]->conflict = false;
                comboBoxes[lastPinConflict]->previousPin = comboBoxes[lastPinConflict]->cbIndex;
                comboBoxes[lastPinConflict]->setItemData(comboBoxes[lastPinConflict]->previousPin, comboBoxes[lastPinConflict]->palette().base().color(), Qt::BackgroundRole);
                //change color of combo box
            }
            //else nothing, conflict still remains
        }
        box->conflict = false;
        box->previousPin = box->cbIndex;
        config[box->cbIndex] = false;
        display->activateSegment(box->cbIndex, false);
        labels[box->cbIndex]->setText("0");
        updateLEGlobal(index, false);
    }
}


void DisplayTool::updateLEHexCat(const QString &text)
{
    unsigned char hex = text.toInt(NULL, 16);
    unsigned char hexInv = ~hex&0xFF;
    QString number;
    int length;
    //dec
    number = QString::number(hex, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[1]->setText(number);
    //bin
    number = QString::number(hex, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[2]->setText(number);
    //hex
    number = QString::number(hexInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    anodeLE[0]->setText(number);
    //dec
    number = QString::number(hexInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[1]->setText(number);
    //bin
    number = QString::number(hexInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[2]->setText(number);

    display->setSegment(hex);
}


void DisplayTool::updateLEDecCat(const QString &text)
{
    unsigned char dec = text.toInt(NULL, 10);
    unsigned char decInv = ~dec&0xFF;
    QString number;
    int length;
    //hex
    number = QString::number(dec, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    cathodeLE[0]->setText(number);
    //bin
    number = QString::number(dec, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[2]->setText(number);
    //hex
    number = QString::number(decInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    anodeLE[0]->setText(number);
    //dec
    number = QString::number(decInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[1]->setText(number);
    //bin
    number = QString::number(decInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[2]->setText(number);

    display->setSegment(dec);
}


void DisplayTool::updateLEBinCat(const QString &text)
{
    unsigned char bin = text.toInt(NULL, 2);
    unsigned char binInv = ~bin&0xFF;
    QString number;
    int length;
    //hex
    number = QString::number(bin, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    cathodeLE[0]->setText(number);
    //dec
    number = QString::number(bin, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[1]->setText(number);
    //hex
    number = QString::number(binInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    anodeLE[0]->setText(number);
    //dec
    number = QString::number(binInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[1]->setText(number);
    //bin
    number = QString::number(binInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[2]->setText(number);

    display->setSegment(bin);
}


void DisplayTool::updateLEHexAn(const QString &text)
{
    unsigned char hex = text.toInt(NULL, 16);
    unsigned char hexInv = ~hex&0xFF;
    QString number;
    int length;
    //hex
    number = QString::number(hexInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    cathodeLE[0]->setText(number);
    //dec
    number = QString::number(hexInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[1]->setText(number);
    //bin
    number = QString::number(hexInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[2]->setText(number);
    //dec
    number = QString::number(hex, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[1]->setText(number);
    //bin
    number = QString::number(hex, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[2]->setText(number);

    display->setSegment(hexInv);
}


void DisplayTool::updateLEDecAn(const QString &text)
{
    unsigned char dec = text.toInt(NULL, 10);
    unsigned char decInv = ~dec&0xFF;
    QString number;
    int length;
    //hex
    number = QString::number(decInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    cathodeLE[0]->setText(number);
    //dec
    number = QString::number(decInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[1]->setText(number);
    //bin
    number = QString::number(decInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[2]->setText(number);
    //hex
    number = QString::number(dec, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    anodeLE[0]->setText(number);
    //bin
    number = QString::number(dec, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[2]->setText(number);

    display->setSegment(decInv);
}


void DisplayTool::updateLEBinAn(const QString &text)
{
    unsigned char bin = text.toInt(NULL, 2);
    unsigned char binInv = ~bin&0xFF;
    QString number;
    int length;
    //hex
    number = QString::number(binInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    cathodeLE[0]->setText(number);
    //dec
    number = QString::number(binInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[1]->setText(number);
    //bin
    number = QString::number(binInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[2]->setText(number);
    //hex
    number = QString::number(bin, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    anodeLE[0]->setText(number);
    //dec
    number = QString::number(bin, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[1]->setText(number);

    display->setSegment(binInv);
}


void DisplayTool::updateLEGlobal(int pin, bool active)
{
    unsigned char dec = cathodeLE[1]->text().toInt(NULL, 10);
    unsigned char decInv;
    QString number;
    int length;
    //qDebug() << "DisplayTool: old dec number:" << (unsigned int)dec;
    if (active == true)
    {
        //qDebug() << "DisplayTool: active == true";
        dec |= 1 << pin;
        //dec = dec&0xFF;
    }
    else
    {
        //qDebug() << "DisplayTool: active == false";
        dec &= ~(1 << pin);
    }
    //qDebug() << "DisplayTool: new dec number:" << (unsigned int)dec;
    decInv = ~dec&0xFF;
    //hex
    number = QString::number(dec, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    cathodeLE[0]->setText(number);
    //dec
    number = QString::number(dec, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[1]->setText(number);
    //bin
    number = QString::number(dec, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    cathodeLE[2]->setText(number);
    //hex
    number = QString::number(decInv, 16);
    if (number.length() < 2)
    {
        number.prepend('0');
    }
    anodeLE[0]->setText(number);
    //dec
    number = QString::number(decInv, 10);
    length = 3 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[1]->setText(number);
    //bin
    number = QString::number(decInv, 2);
    length = 8 - number.length();
    while (length > 0)
    {
        number.prepend('0');
        length--;
    }
    anodeLE[2]->setText(number);
}