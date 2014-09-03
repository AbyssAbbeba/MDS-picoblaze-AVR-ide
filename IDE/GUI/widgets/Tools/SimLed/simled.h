#ifndef LEDS_SIM_H
#define LEDS_SIM_H

#include <QWidget>
#include <QDialog>
#include <QtGui>
#include <QtCore>
#include "ui_simled.h"
#include "../../../../simulators/SimControl/MCUSimControl.h" //8
#include "../../../../simulators/SimControl/MCUSimObserver.h"//8
#include "../../../../simulators/MCUSim/MCUSim.h"            //8
#include "../../../../simulators/MCUSim/MCUSimPureLogicIO.h" //8


class MCUSimControl; //8


namespace Ui {
class Leds_sim;
}

class Leds_sim : public QWidget, public MCUSimObserver //8
{
    Q_OBJECT
    
    public:
        Leds_sim(QWidget *parent, MCUSimControl *controlUnit); //8
        //~Leds_sim();

    private:
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void CreateItems(void);
        unsigned char value,address;
        void ValueDecode(void);
        void DisplayNumber(void);

        
        Ui_Leds_sim ui;
        QPolygonF *widgetPolygon;
        MCUSimControl *controlUnit; //8
        MCUSimPureLogicIO * m_plio; //8


        bool flag_0, flag_1, flag_2, flag_3, flag_4, flag_5, flag_6,
        flag_7;

    private slots:
        void handleUpdateRequest(int mask);
        void addrChanged();
        void ValueChanged();
        void handleChange(int o);

    protected:
        void paintEvent(QPaintEvent *e);
};

#endif // LEDS_SIM_H
