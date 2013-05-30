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
 * @file picoblazegrid.h
 */


#ifndef PICOBLAZEGRID_H
#define PICOBLAZEGRID_H


#include "../sim/McuMemoryView.h"
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMenu>
#include <QLabel>
#include <QWidget>
#include <QString>
#include <QListWidget>
#include <QLineEdit>
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/SimControl/MCUSimControl.h"


class MCUSimControl;

/**
 * @brief
 * @ingroup GUI
 * @class PicoBlazeGrid
 */
class PicoBlazeGrid : public QWidget
{
    Q_OBJECT
    public:
        PicoBlazeGrid(QWidget *parent, MCUSimControl *controlUnit);
        //~PicoBlazeGrid();
        QWidget* getParent();
        //void fixHeight();
        void setProjectPath(QString prjPath);
        
    private:
        MCUSimControl *controlUnit;
        QScrollArea *m_regListScroll;
        QHBoxLayout *layout;
        QWidget *parent;
        //QMenu *popupMenu;
        QString projectPath;
        McuMemoryView *memRegs;
        McuMemoryView *memScratch;
        McuMemoryView *memPorts;
        QListWidget *memStack;

        QLabel *lblRegs;
        QLabel *lblScratch;
        QLabel *lblPorts;
        QLabel *lblStack;
        QLabel *lblRD;
        QLabel *lblRW;
        QLabel *lblPC;
        QLabel *lblTime;
        QLabel *lblClock;

        QLineEdit *leSP;
        QLineEdit *lePC;
        QLineEdit *leTime;
        QLineEdit *leClock;
        

    //signals:
        //void widgetAdded();
        //void closeSignal();
        //void focusOutSignal();
        
    //protected:
        //void contextMenuEvent(QContextMenuEvent *event);
        //virtual void closeEvent(QCloseEvent *event);
        //virtual void enterEvent(QEvent *event);
};




#endif
