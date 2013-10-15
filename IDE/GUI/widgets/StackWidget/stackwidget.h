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
        
    private slots:
        void push();
        void pop();

    private:
        QPushButton *btnPush;
        QPushButton *btnPop;
        QLineEdit *leInput;
        QListWidget *lwStack;
        MCUSimMemory * m_memory;
        MCUSimSubsys::SubsysId subsys;
        int m_startingAddress;
        int m_size;
        QIcon *iconSP;
        int sp;
};


#endif