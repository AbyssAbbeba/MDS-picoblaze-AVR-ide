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


#include <QWidget>
//#include <QIcon>
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../../guicfg/guicfg.h"

class PicoBlazeStack;
class MCUSimControl;
class QPushButton;
class QLineEdit;
class QListWidget;


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
        void setWarningOpt(GuiCfg::WarningsOpt options);

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
        //QIcon *iconSP;
        int sp;
        GuiCfg::WarningsOpt warningOptions;
};


#endif