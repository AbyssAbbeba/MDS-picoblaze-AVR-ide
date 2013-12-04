
#ifndef SIMCONTROLBUTTONS_H
#define SIMCONTROLBUTTONS_H

class QPushButton;
class QHBoxLayout;

class McuMemoryView;
class RegDisplayList;
class MCUSimControl;

#include <QWidget>

class SimControlButtons : public QWidget {
	Q_OBJECT
public:
	SimControlButtons();

	McuMemoryView * m_mcuMemoryView;
	RegDisplayList * m_regList;
	MCUSimControl * m_simControlUnit;

public slots:
	void startStopButtonPressed();
	void resetButtonPressed();
	void stepButtonPressed();
private:
	QHBoxLayout * m_primaryLayout;
	QPushButton * m_startStopButton;
	QPushButton * m_resetButton;
	QPushButton * m_stepButton;

	inline void createWidgets();
	inline void setupConnections();

	bool m_simStarted;
};

#endif // SIMCONTROLBUTTONS_H
