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
        WSimulationInfo(MCUSimControl *controlUnit, WSimulationInfo *copySimInfo);
        WSimulationInfo(MCUSimControl *controlUnit);
        ~WSimulationInfo();
        MovGridLayout* getGridLayout();
        
    private:
        MCUSimControl *controlUnit;
        McuMemoryView *m_hexEdit;
        RegDisplayList *m_regList;
        QScrollArea *m_regListScroll;
        MovGridLayout *gridLayout;
        QHBoxLayout *layout;
        QWidget *parent;
        QMenu *popupMenu;
        MovGridLayoutItem *dragItem;
        bool focusTracking;

    private slots:
        void rearangeLayoutSlot();
        void catchDragSlot(MovGridLayoutItem *item);
        void saveLayoutSlot();
        void setFocusSlot();

    signals:
        void closeSignal();
        void focusOutSignal();
        
    protected:
        void contextMenuEvent(QContextMenuEvent *event);
        virtual void closeEvent(QCloseEvent *event);
        virtual void enterEvent(QEvent *event);
};




#endif
