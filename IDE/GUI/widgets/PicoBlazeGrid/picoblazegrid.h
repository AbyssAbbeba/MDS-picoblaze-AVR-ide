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
 * @file picoblazegrid.h
 */


#ifndef PICOBLAZEGRID_H
#define PICOBLAZEGRID_H


#include <QWidget>
#include <QString>
#include "../../guicfg/guicfg.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"

class MCUSimControl;
class QLabel;
class QMenu;
class QListWidget;
class QPushButton;
class QComboBox;
class QLineEdit;
class QScrollArea;
class QHBoxLayout;
class RegistersWidget;
class McuMemoryView;
class PortHexEdit;
class StackWidget;
class TimeWidget;
class MCUSimCPU;
class PicoBlazeStatusFlags;
class PicoBlazeInterruptController;
class PicoBlazeStack;

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
        void updateWidget();
        void setClock(double clock, int clockMult);
        void setWarningOpt(GuiCfg::WarningsOpt options);
        
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
        TimeWidget *wTime;

        QLabel *lblRegs;
        QLabel *lblScratch;
        QLabel *lblPortsIn;
        QLabel *lblPortsOut;
        QLabel *lblStack;
        QLabel *lblRD;
        QLabel *lblWR;
        QLabel *lblPC;
        QLabel *lblTime;
        QLabel *lblCycles;
        QLabel *lblClock;

        QLineEdit *leSP;
        QLineEdit *lePC;
        QLineEdit *leCycles;
        QLineEdit *leClock;

        QPushButton *btnIntr;
        QPushButton *btnPorts;
        QPushButton *btnCarry;
        QPushButton *btnZero;
        QPushButton *btnInte;

        QComboBox *cmbClock;

        MCUSimCPU *m_cpu;
        PicoBlazeStatusFlags *m_flags;
        PicoBlazeInterruptController *m_interrupt;
        PicoBlazeStack *m_stack;

        double clock;
        double clockMult;

        GuiCfg::WarningsOpt warningOptions;
        

    //signals:
        //void widgetAdded();
        //void closeSignal();
        //void focusOutSignal();

    private slots:
        //void switchPorts();
        void setIntE();
        void interrupt();
        void handleUpdateRequest(int mask);

    public slots:
        void unhighlight();

    private slots:
        void stopSimSlot();
        void changeClock(const QString &text);
        void changeClockMult(int index);

    signals:
        void stopSimSig();
        void clockChanged(double clock, int clockMult);

    protected:
        bool eventFilter(QObject *target, QEvent *event);
        //void contextMenuEvent(QContextMenuEvent *event);
        //virtual void closeEvent(QCloseEvent *event);
        //virtual void enterEvent(QEvent *event);
};




#endif
