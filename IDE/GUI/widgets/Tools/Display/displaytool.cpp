#include <QtGui>
#include "displaytool.h"

DisplayTool::DisplayTool(QWidget *parent)
    : QWidget(parent)
{
    gridLayoutLeft = new QGridLayout();
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

    for (int i = 0; i < 8; i++)
    {
        gridLayoutLeft->addWidget(labels[i], i, 0);
        gridLayoutLeft->addWidget(comboBoxes[i], i, 1);
    }
    
    gridLayout = new QGridLayout(this);
    display = new DisplaySegment(this);
    connect(display, SIGNAL(pressed(int)), this, SLOT(segmentPressed(int)));
    
    gridLayout->addWidget(display, 0, 0);
    gridLayout->addLayout(gridLayoutLeft, 0, 1);

    for (int i = 0; i < 8; i++)
        config[i] = false;
    this->show();
}

DisplayTool::~DisplayTool()
{
    delete gridLayoutLeft;
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
    qDebug() << "testing";
    bool changed = false;
    for (int i = 0; i < 8; i++)
    {
        if (comboBoxes[i] != box)
        {
            if (comboBoxes[i]->currentIndex() == index)
            {
                qDebug() << "color changing to red";
                changed = true;
                box->setPalette(QPalette(Qt::red));
                box->update();
                comboBoxes[i]->setPalette(QPalette(Qt::red));
                comboBoxes[i]->update();
            }
            else
            {
                comboBoxes[i]->setPalette(QPalette(comboBoxes[i]->palette().base().color()));
            }
        }
    }
    if (changed == false)
    {
        qDebug() << "color changing to red";
        this->setPalette(QPalette(this->palette().base().color()));
    }
}