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
#include <QPushButton>
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../StackWidget/stackwidget.h"
#include "../RegistersWidget/registerswidget.h"
#include "../PortHexEdit/porthexedit.h"


class MCUSimControl;

/**
 * @brief
 * @ingroup GUI
 * @class PicoBlazeGrid
 */
class PicoBlazeGrid : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        PicoBlazeGrid(QWidget *parent, MCUSimControl *controlUnit);
        //~PicoBlazeGrid();
        QWidget* getParent();
        void setProjectPath(QString prjPath);
        void fixHeight();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        
    private:
        MCUSimControl *controlUnit;
        QScrollArea *m_regListScroll;
        QHBoxLayout *layout;
        QWidget *parent;
        //QMenu *popupMenu;
        QString projectPath;
        RegistersWidget *memRegs;
        McuMemoryView *memScratch;
        PortHexEdit *memPorts;
        StackWidget *memStack;

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

        QPushButton *btnIntr;
        QPushButton *btnPorts;
        QPushButton *btnCarry;
        QPushButton *btnZero;

        MCUSimCPU* m_cpu;
        

    //signals:
        //void widgetAdded();
        //void closeSignal();
        //void focusOutSignal();

    private slots:
        void switchPorts();
        
    //protected:
        //void contextMenuEvent(QContextMenuEvent *event);
        //virtual void closeEvent(QCloseEvent *event);
        //virtual void enterEvent(QEvent *event);
};




#endif
