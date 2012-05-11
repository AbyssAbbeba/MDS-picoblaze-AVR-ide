#ifndef WSIMULATIONINFO_H
#define WSIMULATIONINFO_H


#include "movgridlayout.h"
#include "hexedit.h"
#include <QHBoxLayout>

class WSimulationInfo : public QWidget
{
    Q_OBJECT
    public:
        WSimulationInfo(QWidget *parent);
    private:
        HexEdit *hexEdit;
        MovGridLayout *gridLayout;
        QHBoxLayout *layout;
};




#endif
