
#ifndef SIMCONTROLBUTTONS_H
#define SIMCONTROLBUTTONS_H

class QPushButton;
class QHBoxLayout;

#include <QWidget>

class SimControlButtons : public QWidget {
	Q_OBJECT
public:
	SimControlButtons();
	virtual ~SimControlButtons();
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
