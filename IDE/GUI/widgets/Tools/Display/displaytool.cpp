#include <QtGui>
#include "displaytool.h"

DisplayTool::DisplayTool(QWidget *parent)
    : QWidget(parent)
{
    gridLayoutRight = new QGridLayout();

    labelsUp[0] = new QLabel("LED", this);
    labelsUp[1] = new QLabel("VAL", this);
    labelsUp[2] = new QLabel("PIN", this);
    
    labelsLED[0] = new QLabel("A", this);
    labelsLED[1] = new QLabel("B", this);
    labelsLED[2] = new QLabel("C", this);
    labelsLED[3] = new QLabel("D", this);
    labelsLED[4] = new QLabel("E", this);
    labelsLED[5] = new QLabel("F", this);
    labelsLED[6] = new QLabel("G", this);
    labelsLED[7] = new QLabel("P", this);
    
    labels[0] = new QLabel("0", this);
    labels[1] = new QLabel("0", this);
    labels[2] = new QLabel("0", this);
    labels[3] = new QLabel("0", this);
    labels[4] = new QLabel("0", this);
    labels[5] = new QLabel("0", this);
    labels[6] = new QLabel("0", this);
    labels[7] = new QLabel("0", this);
    
    for (int i = 0; i < 8; i++)
    {
        comboBoxes[i] = new ComboBox(this);
        comboBoxes[i]->setCurrentIndex(i);
        connect(comboBoxes[i], SIGNAL(changed(ComboBox*, int)), this, SLOT(updateComboBoxes(ComboBox*, int)));
    }

    gridLayoutRight->addWidget(labelsUp[0], 0, 0);
    gridLayoutRight->addWidget(labelsUp[1], 0, 1);
    gridLayoutRight->addWidget(labelsUp[2], 0, 2);
    for (int i = 0; i < 8; i++)
    {
        gridLayoutRight->addWidget(labelsLED[i], i+1, 0);
        gridLayoutRight->addWidget(labels[i], i+1, 1);
        gridLayoutRight->addWidget(comboBoxes[i], i+1, 2);
    }
    
    gridLayout = new QGridLayout(this);
    display = new DisplaySegment(this);
    connect(display, SIGNAL(pressed(int)), this, SLOT(segmentPressed(int)));
    
    gridLayout->addWidget(display, 0, 0);
    gridLayout->addLayout(gridLayoutRight, 0, 1);

    for (int i = 0; i < 8; i++)
        config[i] = false;
    this->show();
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

                QPalette p = box->palette();

                // Outline around the menu
                p.setColor(QPalette::Window, Qt::red);
                p.setColor(QPalette::WindowText, Qt::white);

                // combo button
                p.setColor(QPalette::Button, Qt::darkCyan);
                p.setColor(QPalette::ButtonText, Qt::white);

                // combo menu
                p.setColor(QPalette::Base, Qt::darkCyan);
                p.setColor(QPalette::Text, Qt::white);

                // highlight button & menu
                p.setColor(QPalette::Highlight, Qt::blue);
                p.setColor(QPalette::HighlightedText, Qt::red);

                // to customize the disabled color
                p.setColor(QPalette::Disabled, QPalette::Button, Qt::gray);
                p.setColor(QPalette::Disabled, QPalette::ButtonText, Qt::lightGray);

                box->setPalette(p);
                //box->setPalette(QPalette(Qt::red));
                //box->setStyleSheet("QComboBox {selection-color: red;"
                //         "selection-background-color: red;}");
                box->update();
                comboBoxes[i]->setPalette(QPalette(Qt::red));
                comboBoxes[i]->update();
            }
            //else
            //{
            //    comboBoxes[i]->setPalette(QPalette(comboBoxes[i]->palette().base().color()));
            //}
        }
    }
    if (changed == false)
    {
        qDebug() << "Combo Box: color changing to normal";
        this->setPalette(QPalette(this->palette().base().color()));
    }
}