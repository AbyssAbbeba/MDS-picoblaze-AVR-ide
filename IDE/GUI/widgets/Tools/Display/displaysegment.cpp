/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup DisplaySegment
 * @file displaysegment.cpp
 */


#include <QtGui>
#include "displaysegment.h"
#include "displaywidget.h"

/*set canvas_objects(0) [$canvas_widget create polygon  \
            36 15   46 5    97 5    107 15  97 25   46 25   \
        ]
        set canvas_objects(1) [$canvas_widget create polygon    \
            110 18  120 28  112 72  100 84  91 75   99 29   \
        ]
        set canvas_objects(2) [$canvas_widget create polygon    \
            100 90  110 100 102 144 90 156  81 147  89 101  \
        ]
        set canvas_objects(3) [$canvas_widget create polygon    \
            87 159  77 169  26 169  16 159  26 149  77 149  \
        ]
        set canvas_objects(4) [$canvas_widget create polygon    \
            13 156  25 144  33 100  23 90   12 101  4 147   \
        ]
        set canvas_objects(5) [$canvas_widget create polygon    \
            23 84   35 72   43 28   33 18   22 29   14 75   \
        ]
        set canvas_objects(6) [$canvas_widget create polygon    \
            26 87   36 97   87 97   97 87   87 77   36 77   \
        ]
        set canvas_objects(7) [$canvas_widget create oval 98 155 116 173]
*/


DisplaySegment::DisplaySegment(QWidget *parent)
    : QWidget(parent)
{
    this->show();
    this->setMinimumWidth(140);
    this->setMinimumHeight(200);
    displayWidgets[0] = new DisplayWidget(this, 0, true, false);
    connect(displayWidgets[0], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[0]->move(35, 25);
    displayWidgets[5] = new DisplayWidget(this, 5, false, false);
    connect(displayWidgets[5], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[5]->move(13, 40);
    displayWidgets[1] = new DisplayWidget(this, 1, false, false);
    connect(displayWidgets[1], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[1]->move(90, 40);
    displayWidgets[6] = new DisplayWidget(this, 6, true, false);
    connect(displayWidgets[6], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[6]->move(23, 102);
    displayWidgets[4] = new DisplayWidget(this, 4, false, false);
    connect(displayWidgets[4], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[4]->move(0, 115);
    displayWidgets[2] = new DisplayWidget(this, 2, false, false);
    connect(displayWidgets[2], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[2]->move(80, 115);
    displayWidgets[3] = new DisplayWidget(this, 3, true, false);
    connect(displayWidgets[3], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[3]->move(14, 173);
    displayWidgets[7] = new DisplayWidget(this, 7, true, true);
    connect(displayWidgets[7], SIGNAL(pressed(int)), this, SLOT(emitPressed(int)));
    displayWidgets[7]->move(95, 175);
}

DisplayWidget* DisplaySegment::getWidget(int index)
{
    return displayWidgets[index];
}

void DisplaySegment::emitPressed(int index)
{
    emit pressed(index);
}


void DisplaySegment::setSegment(unsigned char mask)
{
    //qDebug() << "DisplaySegment: setting mask:" << mask;
    //qDebug() << "DisplaySegment: binary mask:" << QString::number(mask, 2);
    unsigned char maskCpy = mask;
    for (int i = 0; i < 8; i++)
    {
        if (maskCpy & 0x01)
        {
            emit numberChanged(i, true);
        }
        else
        {
            emit numberChanged(i, false);
        }
        maskCpy = maskCpy >> 1;
    }
}

void DisplaySegment::activateSegment(int index, bool active)
{
    displayWidgets[index]->activate(active);
}
