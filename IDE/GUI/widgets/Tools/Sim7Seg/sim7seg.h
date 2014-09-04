#ifndef SIM7SEG_H
#define SIM7SEG_H

#include <QWidget>
#include <QDialog>
#include <QtGui>
#include <QtCore>
#include "ui_sim7seg.h"
#include "../../../../simulators/SimControl/MCUSimControl.h" //8
#include "../../../../simulators/SimControl/MCUSimObserver.h"//8
#include "../../../../simulators/MCUSim/MCUSim.h"            //8
#include "../../../../simulators/MCUSim/MCUSimPureLogicIO.h" //8


class MCUSimControl; //8


namespace Ui {
class Sim7Seg;
}

class Sim7Seg : public QWidget, public MCUSimObserver //8
{
    Q_OBJECT
    
    public:
        Sim7Seg(QWidget *parent, MCUSimControl *controlUnit); //8
        //~Sim7Seg();

    private:
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void CreateItems(void);
        unsigned char value,address;
        void ValueDecode(void);
        void DisplayNumber(unsigned char Numero);
        

        
        Ui_Sim7Seg ui;
        QPolygonF *widgetPolygon;
        MCUSimControl *controlUnit; //8
        MCUSimPureLogicIO * m_plio; //8
        QList<QPolygonF*> list;

        unsigned char pin_status[7];
        bool flag_0, flag_1, flag_2, flag_3, flag_4, flag_5, flag_6,
        flag_7;

    private slots:
        void comboChanged(int c);
        void handleUpdateRequest(int mask);
        void addrChanged();
        void ValueChanged();
        void handleChange(int o);

    protected:
        void paintEvent(QPaintEvent *e);
};

#endif // SIM7SEG_H
