#ifndef WSIMULATIONINFO_H
#define WSIMULATIONINFO_H


#include "movgridlayout.h"
#include "hexedit.h"
#include <QHBoxLayout>
#include <QScrollArea>


class McuMemoryView;
class MCUSimControl;
class RegDisplayList;

class WSimulationInfo : public QWidget
{
    Q_OBJECT
    public:
        WSimulationInfo(MCUSimControl *controlUnit, QWidget *parent);
        ~WSimulationInfo();
    private:
        McuMemoryView *m_hexEdit;
        RegDisplayList *m_regList;
        QScrollArea *m_regListScroll;
        MovGridLayout *gridLayout;
        QHBoxLayout *layout;
        QWidget *parent;
};




#endif
