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
//#include "../HexEdit/hexedit.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMenu>
//#include "MCUSimControl.h"
#include "../MovGridLayout/movgridlayoutitem.h"


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
        WSimulationInfo(MCUSimControl *controlUnit, WSimulationInfo *copySimInfo, QWidget *parent);
        WSimulationInfo(QWidget *parent, MCUSimControl *controlUnit);
        ~WSimulationInfo();
        MovGridLayout* getGridLayout();
        McuMemoryView* getHexEdit();
        QWidget* getParent();
        void fixHeight();
        void setProjectPath(QString prjPath);
        
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
        QString projectPath;

    private slots:
        void rearangeLayoutSlot();
        void catchDragSlot(MovGridLayoutItem *item);
        void saveLayoutSlot();
        void setFocusSlot();

    signals:
        void widgetAdded();
        //void closeSignal();
        //void focusOutSignal();
        
    protected:
        void contextMenuEvent(QContextMenuEvent *event);
        //virtual void closeEvent(QCloseEvent *event);
        //virtual void enterEvent(QEvent *event);
};




#endif
