#ifndef WSIMULATIONINFO_H
#define WSIMULATIONINFO_H


#include "../MovGridLayout/movgridlayout.h"
#include "../HexEdit/hexedit.h"
#include <QHBoxLayout>
#include <QScrollArea>


class McuMemoryView;
class MCUSimControl;
class RegDisplayList;
class MovGridLayout;

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
