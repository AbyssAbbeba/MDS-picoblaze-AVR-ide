#ifndef SIMSWITCH_H
#define SIMSWITCH_H

#include <QWidget>
#include <QDialog>
#include <QtGui>
#include <QtCore>
#include "ui_simswitch.h"
#include "../../../../simulators/SimControl/MCUSimControl.h" //8
#include "../../../../simulators/SimControl/MCUSimObserver.h"//8
#include "../../../../simulators/MCUSim/MCUSim.h"            //8
#include "../../../../simulators/MCUSim/MCUSimPureLogicIO.h" //8


class MCUSimControl; //8


namespace Ui {
class SimSwitch;
}

class SimSwitch : public QWidget, public MCUSimObserver //8
{
    Q_OBJECT
    
    public:
        SimSwitch(QWidget *parent, MCUSimControl *controlUnit); //8
        //~SimSwitch();

    private:
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void CreateItems(void);
        unsigned char value,address;
        void ValueDecode(void);
        unsigned char GetValue();
        

        
        Ui_SimSwitch ui;
        QPolygonF *widgetPolygon;
        MCUSimControl *controlUnit; //8
        MCUSimPureLogicIO * m_plio; //8
        QList<QPolygonF*> list;

        unsigned char pin_status[7];
        bool flag_0, flag_1, flag_2, flag_3, flag_4, flag_5, flag_6,
        flag_7,flag_change;

    private slots:
        void comboChanged(int c);
        void handleUpdateRequest(int mask);
        void addrChanged();
        void ValueChanged();
        void handleChange(int o);

    protected:
        void paintEvent(QPaintEvent *e);
        void mousePressEvent(QMouseEvent *);
};

#endif // SIMSWITCH_H
