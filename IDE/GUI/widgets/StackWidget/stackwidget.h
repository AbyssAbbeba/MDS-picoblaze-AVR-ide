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
 * @file stackwidget.h
 */


#ifndef STACKWIDGET_H
#define STACKWIDGET_H


#include <QListWidget>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QIcon>
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeStack.h"
#include "../../errordialog/errordlg.h"

class StackWidget : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        StackWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys);
        virtual ~StackWidget();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void fixHeight();
        void unhighlight();
        void updateWidget();

    signals:
        void stopSimSig();
        
    private slots:
        void push();
        void pop();

    private:
        QPushButton *btnPush;
        QPushButton *btnPop;
        QLineEdit *leInput;
        QListWidget *lwStack;
        PicoBlazeStack * m_memory;
        MCUSimSubsys::SubsysId subsys;
        int m_startingAddress;
        int m_size;
        int numWidth;
        QIcon *iconSP;
        int sp;
};


#endif