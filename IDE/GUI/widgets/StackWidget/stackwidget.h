#ifndef STACKWIDGET_H
#define STACKWIDGET_H


#include <QListWidget>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../../../simulators/SimControl/MCUSimControl.h"

class StackWidget : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    public:
        StackWidget(QWidget *parent, MCUSimControl * controlUnit, MCUSim::Subsys::SubsysId subsys);
        virtual ~StackWidget();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void fixHeight();
        
    private slots:
        void push();
        void pop();

    private:
        QPushButton *btnPush;
        QPushButton *btnPop;
        QLineEdit *leInput;
        QListWidget *lwStack;
        MCUSim::Memory * m_memory;
        MCUSim::Subsys::SubsysId subsys;
        int m_startingAddress;
        int m_size;
};


#endif