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
        void setChar(int number, bool comma);
    
    private:
        DisplayWidget *displayWidgets[8];

    public slots:
        void emitPressed(int index);

    signals:
        void pressed(int index);
};


#endif
