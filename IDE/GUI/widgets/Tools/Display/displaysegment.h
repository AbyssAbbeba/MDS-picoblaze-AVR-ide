#ifndef DISPLAYSEGMENT_H
#define DISPLAYSEGMENT_H

#include <QWidget>
#include "displaywidget.h"

class DisplaySegment : public QWidget
{
    Q_OBJECT
    public:
        DisplaySegment(QWidget *parent);
        DisplayWidget* getWidget(int index);
    
    private:
        DisplayWidget *displayWidgets[7];
};


#endif
