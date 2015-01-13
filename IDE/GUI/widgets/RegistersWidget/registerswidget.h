/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file registerswidget.h
 */


#ifndef REGISTERSWIDGET_H
#define REGISTERSWIDGET_H


#include <QWidget>
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../../guicfg/guicfg.h"

class MCUSimControl;
class MCUSimMemory;
class QTableWidget;
class QVBoxLayout;
class QLabel;

class RegistersWidget : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        RegistersWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys);
        virtual ~RegistersWidget();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        //void fixHeight();
        void unhighlight();
        void updateWidget();
        void setWarningOpt(GuiCfg::WarningsOpt options);

    signals:
        void stopSimSig();
        
    private slots:
        void updateValue(int row, int column);
        void unhighlightCell(int row, int column);
        
    private:
        MCUSimMemory * m_memory;
        MCUSimSubsys::SubsysId subsys;
        int m_startingAddress;
        int m_size;
        bool update;
        GuiCfg::WarningsOpt m_warningOptions;
        QLabel *m_lblReg;
        QTableWidget *m_tblReg;
        QVBoxLayout *m_layout;
};


#endif