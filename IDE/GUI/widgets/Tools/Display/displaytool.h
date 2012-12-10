/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file displaytool.h
 */


#ifndef DISPLAYTOOL_H
#define DISPLAYTOOL_H


#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include "displaysegment.h"

/**
 * @brief
 * @ingroup GUI
 * @class DisplayTool
 */
class DisplayTool : public QWidget
{
    Q_OBJECT
    public:
        DisplayTool(QWidget *parent);
        ~DisplayTool();

    private:
        DisplaySegment *display;
        QGridLayout *gridLayout;
        QVBoxLayout *vBoxLayout;
        QLabel *labels[7];

    private slots:
        void segmentPressed(int index);
};




#endif
