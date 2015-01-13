/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file mcumemoryview.h
 */


#ifndef MCUMEMORYVIEW_H
#define MCUMEMORYVIEW_H

// Forward declarations
class HexEdit;
class MCUSimControl;
class QGridLayout;
class QLabel;


#include <cstdint>
#include <QWidget>

#include "../../../simulators/MCUSim/MCUSim.h"
#include "../../../simulators/MCUSim/MCUSimSubsys.h"
#include "../../../simulators/SimControl/MCUSimObserver.h"
#include "../../errordialog/errordlg.h"
#include "../../guicfg/guicfg.h"


class McuMemoryView : public QWidget, public MCUSimObserver  {
	Q_OBJECT
    public:
        McuMemoryView(QWidget * parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys);
        virtual ~McuMemoryView();

        void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
        void deviceChanged();
        void deviceReset();
        void setReadOnly(bool readOnly);
        void fixHeight();
        void unhighlight();
        void updateWidget();
        int getSize();
        void setWarningOpt(GuiCfg::WarningsOpt options);

    signals:
        void stopSimSig();

    public slots:
        void changeValue(int address);

    private:
        int m_startingAddress;
        int m_size;
        HexEdit *m_hexEdit;
        MCUSimMemory *m_memory;
        MCUSimSubsys::SubsysId subsys;
        QLabel *m_lblScratch;

        QGridLayout *m_layout;

        GuiCfg::WarningsOpt warningOptions;

        inline void deleteHexEdit();

    protected:
        void resizeEvent(QResizeEvent *event);
};

#endif // MCUMEMORYVIEW_H
