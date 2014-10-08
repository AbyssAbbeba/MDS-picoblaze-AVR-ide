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

/*
namespace Ui {
class SimSwitch;
}*/

class SimSwitch : public QWidget, public MCUSimObserver //8
{
    Q_OBJECT
    
    public:
        SimSwitch(QWidget *parent, MCUSimControl *controlUnit); //8
        //~SimSwitch();
        static const QRect switch_rect[8];
        bool switchState[8];
        bool rectHighlight[8];
        bool flag_change;
        unsigned char switchStatus;
        int boxNum[8];

    private:
        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void CreateItems(void);
        unsigned char address;
        void rectClicked(int pos);
        void changeBox(int i,int boxNumber);
        
        Ui_SimSwitch ui;
        MCUSimControl *controlUnit; //8
        MCUSimPureLogicIO * m_plio; //8

        bool flag_0, flag_1, flag_2, flag_3, flag_4, flag_5, flag_6,
        flag_7;

    private slots:
        void handleUpdateRequest(int mask);
        void addrChanged();
        void ValueChanged();
       // void handleChange(int o);
    /*
    void handleChange_0(int change);
    void handleChange_1(int change);
    void handleChange_2(int change);
    void handleChange_3(int change);
    void handleChange_4(int change);
    void handleChange_5(int change);
    void handleChange_6(int change);
    void handleChange_7(int change);
*/

    protected:
        void paintEvent(QPaintEvent *e);
        void mousePressEvent(QMouseEvent* event);
        void mouseMoveEvent ( QMouseEvent * ev );
};

#endif // SIMSWITCH_H
