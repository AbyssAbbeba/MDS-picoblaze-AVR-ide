#ifndef MCUSPECFILEDESIGNER_H
#define MCUSPECFILEDESIGNER_H


#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QPushButton>

#include "ioregswidget.h"

class McuSpecFileDesigner : public QWidget
{
    Q_OBJECT
    public:
        McuSpecFileDesigner(QWidget *parent);

    private slots:
        void reallocateIORegs();
        void setNextIOReg();
        void setPreviousIOReg();
        void getConfig();
        void finaliseXML();
        void saveXML();
        
    private:
        QTabWidget *mainTabs;
        QTabWidget *ioRegTabs;
        QTabWidget *dataMemIORegsTabs;

        QList<IORegsWidget *> ioRegs;

        //general tab labels
        QLabel *generalLblTag;
        QLabel *generalLblFamily;
        QLabel *generalLblName;
        QLabel *generalLblAdc;
        QLabel *generalLblAnalogComp;
        QLabel *generalLblClockControl;
        QLabel *generalLblExtInterr;
        QLabel *generalLblFusesNLocks;
        QLabel *generalLblIsp;
        QLabel *generalLblProgMem;
        QLabel *generalLblParalProg;
        QLabel *generalLblSpi;
        QLabel *generalLblTimer0;
        QLabel *generalLblTimer1;
        QLabel *generalLblTimer2;
        QLabel *generalLblWatchdog;
        QLabel *generalLblTwi;
        QLabel *generalLblUsart;
        QLabel *generalLblDataEEProm;
        QLabel *generalLblDataMemory;
        QLabel *generalLblBootloader;
        //instructions and interrupts tab labels
        QLabel *instructionLblPcWidth;
        QLabel *instructionLblAvailableInst;
        QLabel *instructionLblPossibleIntr;
        //IO tab labels
        QLabel *ioLblPullUpRes;
        QLabel *ioLblHasPorts;
        QLabel *ioLblSpecFuncMap;
        QLabel *ioLblPins;
        //system control tab labels
        QLabel *systemLblRstTresLow;
        QLabel *systemLblRstTresHigh;
        QLabel *systemLblMinRstPulse;
        QLabel *systemLblPORTresFall;
        QLabel *systemLblPORTresRaise;
        QLabel *systemLblTBOD;
        QLabel *systemLblBrownOutTres0;
        QLabel *systemLblBrownOutTres1;
        QLabel *systemLblHasPOR;
        QLabel *systemLblHasBrownOut;
        QLabel *systemLblHasExtRst;
        QLabel *systemLblHasWatchdog;
        //data memory general tab labels
        QLabel *dataMemLblRegFileSize;
        QLabel *dataMemLblSRamSize;
        QLabel *dataMemLblIORegSize;
        QLabel *dataMemLblMem2Size;
        QLabel *dataMemLblSPWidth;
        //general tab check boxes
        QCheckBox *generalChckAdc;
        QCheckBox *generalChckAnalogComp;
        QCheckBox *generalChckClockControl;
        QCheckBox *generalChckExtInterr;
        QCheckBox *generalChckIsp;
        QCheckBox *generalChckParalProg;
        QCheckBox *generalChckSpi;
        QCheckBox *generalChckTimer0;
        QCheckBox *generalChckTimer1;
        QCheckBox *generalChckTimer2;
        QCheckBox *generalChckWatchdog;
        QCheckBox *generalChckTwi;
        QCheckBox *generalChckUsart;
        QCheckBox *generalChckDataEEProm;
        QCheckBox *generalChckBootloader;
        //system control tab check boxes
        QCheckBox *systemChckHasPOR;
        QCheckBox *systemChckHasBrownOut;
        QCheckBox *systemChckHasExtRst;
        QCheckBox *systemChckHasWatchdog;
        //general tab line edits
        QLineEdit *generalLETag;
        QLineEdit *generalLEFamily;
        QLineEdit *generalLEName;
        QLineEdit *generalLEProgMem;
        QLineEdit *generalLEFuses;
        QLineEdit *generalLELocks;
        QLineEdit *generalLEEEPromSize;
        QLineEdit *generalLEEEPromWTime;
        QLineEdit *generalLEDataMemRegFileSize;
        QLineEdit *generalLEDataMemSRamSize;
        QLineEdit *generalLEDataMemIORegSize;
        QLineEdit *generalLEDataMem2Size;
        QLineEdit *generalLEDataMemSPWidth;
        QLineEdit *generalLEBootMinProgTime;
        QLineEdit *generalLEBootMaxProgTime;
        QLineEdit *generalLEBootPageSize;
        QLineEdit *generalLEBootResetAddress0;
        QLineEdit *generalLEBootResetAddress1;
        QLineEdit *generalLEBootResetAddress2;
        QLineEdit *generalLEBootResetAddress3;
        QLineEdit *generalLEBootRWWSectionSize;
        //instructions and interrupts tab line edits
        QLineEdit *instructionLEPcWidth;
        //IO tab line edits
        QLineEdit *ioLEPullUpRes;
        QLineEdit *ioLEHasPorts;
        //system control tab line edits
        QLineEdit *systemLERstTresLow;
        QLineEdit *systemLERstTresHigh;
        QLineEdit *systemLEMinRstPulse;
        QLineEdit *systemLEPORTresFall;
        QLineEdit *systemLEPORTresRaise;
        QLineEdit *systemLETBOD;
        QLineEdit *systemLEBrownOutTres0;
        QLineEdit *systemLEBrownOutTres1;
        //data memory general tab line edits
        QLineEdit *dataMemLERegFileSize;
        QLineEdit *dataMemLESRamSize;
        QLineEdit *dataMemLEIORegSize;
        QLineEdit *dataMemLEMem2Size;
        QLineEdit *dataMemLESPWidth;
        //instructions and interrupts tab plaintextedits
        QPlainTextEdit *instructionPTEInst;
        QPlainTextEdit *instructionPTEIntr;
        //IO tab plaintextedits
        QPlainTextEdit *ioPTESpecFuncMap;
        QPlainTextEdit *ioPTEPins;
        //finalise tab plaintextedits
        QPlainTextEdit *finalisePTE;
        //data memory general tab buttons
        QPushButton *dataMemBtnChange;
        //finalise tab buttons
        QPushButton *finaliseBtnMake;
        QPushButton *finaliseBtnSave;
        //grid layouts
        QGridLayout *generalLayout;
        QGridLayout *instructionLayout;
        QGridLayout *ioLayout;
        QGridLayout *systemLayout;
        QGridLayout *dataMemLayout;
        QGridLayout *finaliseLayout;
};



#endif
