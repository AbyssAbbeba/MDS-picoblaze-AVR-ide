#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <QWidget>
#include "core.h"


#include "../../../../simulators/SimControl/MCUSimControl.h"
#include "../../../../simulators/SimControl/MCUSimObserver.h"
#include "../../../../simulators/MCUSim/MCUSim.h"
#include "../../../../simulators/MCUSim/MCUSimPureLogicIO.h" 


class MCUSimControl;



namespace Ui {
class StopWatch;
}
class Core;

class StopWatch : public QWidget, public MCUSimObserver
{
    Q_OBJECT
    Core core;
    
public:
    explicit StopWatch(QWidget *parent, MCUSimControl *controlUnit);
    ~StopWatch();

    void connectSignals();
    void setStop();
    QSignalMapper *signalMapper;
    QRegExpValidator *valueValidator;
    QTimer *timer;
    QFile outFile;
    QString outPath;
    QString m_path;


    
private:
    void saveAsText();
    void updateCore();
    void clearStop();
    void clearCurrent();
    void handleEvent(int subsysId, int eventId, int locationOrReason, int detail);
    void deviceChanged();
    void deviceReset();
    void setReadOnly(bool readOnly);
    void update(int);


    Ui::StopWatch *ui;
    MCUSimControl *m_simControl;


private slots:
    void breakpointReachedSlot();
    void readButton(int button);
    void on_lineNano2_textChanged(const QString &arg1);
    void on_lineCycles2_textChanged(const QString &arg1);
    void on_lineInstr2_textChanged(const QString &arg1);
    void on_lineProgram2_textChanged(const QString &arg1);
    void on_lineInterrupt2_textChanged(const QString &arg1);
    void on_lineCalls2_textChanged(const QString &arg1);
    void on_lineReturns2_textChanged(const QString &arg1);
    void on_lineIntRet2_textChanged(const QString &arg1);
    void on_lineBreak2_textChanged(const QString &arg1);
    void handleUpdateRequest(int mask);

signals:
    void stopSim();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    //void paintEvent(QPaintEvent*);
  //  void mousePressEvent(QMouseEvent* pressEvent);
    void keyPressEvent(QKeyEvent *);
    //void mouseMoveEvent ( QMouseEvent * moveEvent );
};

#endif // STOPWATCH_H
