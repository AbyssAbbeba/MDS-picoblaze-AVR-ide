#include <QtGui>
#include "displaytool.h"

DisplayTool::DisplayTool(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("8 Segment Editor");
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
        comboBoxes[i] = new ComboBox(rightWidget);
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
    delete gridLayoutRight;
    delete gridLayout;
    delete this;
}

void DisplayTool::segmentPressed(int index)
{
    if (config[index] == true)
    {
        config[index] = false;
        labels[index]->setText("0");
    }
    else
    {
        labels[index]->setText("1");
        config[index] = true;
    }
}


void DisplayTool::updateComboBoxes(ComboBox *box, int index)
{
    qDebug() << "Combo Box: testing";
    bool changed = false;
    for (int i = 0; i < 8; i++)
    {
        if (comboBoxes[i] != box)
        {
            if (comboBoxes[i]->currentIndex() == index)
            {
                qDebug() << "Combo Box: color changing to red";
                changed = true;
                comboBoxes[i]->setItemData(index, Qt::red, Qt::BackgroundRole);
                box->setItemData(index, Qt::red, Qt::BackgroundRole);
                QPalette p = box->palette();

                // Outline around the menu
                //p.setColor(QPalette::Window, Qt::red);
                p.setColor(QPalette::WindowText, Qt::red);

                // combo button
                //p.setColor(QPalette::Button, Qt::red);
                //p.setColor(QPalette::ButtonText, Qt::red);

                // combo menu
                //p.setColor(QPalette::Base, Qt::red);
                p.setColor(QPalette::Text, Qt::red);

                // highlight button & menu
                //p.setColor(QPalette::Highlight, Qt::red);
                //p.setColor(QPalette::HighlightedText, Qt::red);

                // to customize the disabled color
                //p.setColor(QPalette::Disabled, QPalette::Button, Qt::red);
                //p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::red);

                box->setPalette(p);
                //box->setPalette(QPalette(Qt::red));
                //box->setStyleSheet("QComboBox {selection-color: red;"
                //         "selection-background-color: red;}");
                //box->update();
                //comboBoxes[i]->setPalette(QPalette(Qt::red));
                comboBoxes[i]->update();//*/
            }
            /*else
            {
                comboBoxes[i]->setItemData(index, comboBoxes[i]->palette().base().color(), Qt::BackgroundRole);
            }*/
        }
    }
    if (changed == false)
    {
        qDebug() << "Combo Box: color changing to normal";
        for (int i = 0; i < 8; i++)
        {
            box->setItemData(i, box->palette().base().color(), Qt::BackgroundRole);
        }
    }
}