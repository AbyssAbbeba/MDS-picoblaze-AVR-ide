
#include "main.h"

#include "McuSimCfgMgr.h"
#include "MCUSimControl.h"
#include "RegDisplayList.h"
#include "McuMemoryView.h"

#include <iostream>
#include <cstdlib>

#include <QApplication>
#include <QVBoxLayout>

// ---------------------------------------------------------------------------------------

#include <QPushButton>
#include <QHBoxLayout>

SimControlButtons::SimControlButtons() {
	m_simStarted = false;

	createWidgets();
	setupConnections();

	McuSimCfgMgr::getInstance()->openConfigFile("../../../simulators/MCUSim/McuSimCfgMgr/mcuspecfile.xml");
	m_simControlUnit = new MCUSimControl("ATmega8A");
}
inline void SimControlButtons::createWidgets() {
	m_primaryLayout = new QHBoxLayout(this);
	setLayout(m_primaryLayout);

	m_startStopButton = new QPushButton(tr("Start"), this);
	m_primaryLayout->addWidget(m_startStopButton);

	m_resetButton = new QPushButton(tr("Reset"), this);
	m_resetButton->setEnabled(false);
	m_primaryLayout->addWidget(m_resetButton);

	m_stepButton = new QPushButton(tr("Step"), this);
	m_stepButton->setEnabled(false);
	m_primaryLayout->addWidget(m_stepButton);
}
inline void SimControlButtons::setupConnections() {
	connect(m_startStopButton, SIGNAL(clicked()), this, SLOT(startStopButtonPressed()));
	connect(m_resetButton, SIGNAL(clicked()), this, SLOT(resetButtonPressed()));
	connect(m_stepButton, SIGNAL(clicked()), this, SLOT(stepButtonPressed()));
}
void SimControlButtons::startStopButtonPressed() {
	if ( false == m_simStarted ) {
		m_startStopButton->setText(tr("Stop"));
		m_resetButton->setEnabled(true);
		m_stepButton->setEnabled(true);

		m_simControlUnit->start("avr8_test_code.hex");

	} else {
		m_startStopButton->setText(tr("Start"));
		m_resetButton->setEnabled(false);
		m_stepButton->setEnabled(false);

		m_simControlUnit->stop();
	}

	m_simStarted = !m_simStarted;
}
void SimControlButtons::resetButtonPressed() {
	m_simControlUnit->reset();
}
void SimControlButtons::stepButtonPressed() {
	m_simControlUnit->step();
}

int main(int argc, char ** argv) {
	QApplication app(argc, argv);

	SimControlButtons buttons;
	buttons.show();

	buttons.m_regList = new RegDisplayList(buttons.m_simControlUnit);
	buttons.m_regList->show();

	buttons.m_mcuMemoryView = new McuMemoryView(buttons.m_simControlUnit);
	buttons.m_mcuMemoryView->show();

	return app.exec();
}
