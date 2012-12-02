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
 * @file wsimulationinfo.h
 */


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

/**
 * @brief
 * @ingroup GUI
 * @class WSimulationInfo
 */
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
