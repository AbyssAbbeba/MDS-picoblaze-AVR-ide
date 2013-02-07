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
#include "combobox.h"
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
        QGridLayout *gridLayoutRight;
        QLabel *labelsUp[3];
        QLabel *labelsLED[8];
        QLabel *labels[8];
        ComboBox *comboBoxes[8];
        bool config[8];
        
    public slots:
        void segmentPressed(int index);
        void updateComboBoxes(ComboBox *box, int index);
};




#endif
