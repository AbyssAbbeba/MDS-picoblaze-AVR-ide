#ifndef TESTPORTTOOL_H
#define TESTPORTTOOL_H

#include <QWidget>
#include <QtGui>
#include "../../../../simulators/SimControl/MCUSimControl.h" //8
#include "../../../../simulators/SimControl/MCUSimObserver.h"//8
#include "../../../../simulators/MCUSim/MCUSim.h"            //8
#include "../../../../simulators/MCUSim/MCUSimPureLogicIO.h" //8
#include "ui_testporttool.h"

namespace Ui {
class TestPortTool;
}

class TestPortTool : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    
public:
    explicit TestPortTool(QWidget *parent, MCUSimControl *controlUnit);
    ~TestPortTool();
    bool status;
    QStringList fileBuffer;
    QTextCursor * cursor;
    QTextStream * outFileText;


private slots:
    void activatePushed();
    void cancelApp();
    void readFileClicked();
    void outFileClicked();
    void inputEvent();
    void handleUpdateRequest(int mask);

private:
    void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
    void deviceChanged();
    void deviceReset();
    void setReadOnly(bool readOnly);
    void fillBuffer();
    int randInt(int,int);

    QStringList address, data;
    QFile inFile,outFile;
    QString loadPath;
    Ui::TestPortTool *ui;
    MCUSimControl *m_simControlUnit; //8
    MCUSimPureLogicIO * m_plio; //8
};

#endif // TESTPORTTOOL_H
