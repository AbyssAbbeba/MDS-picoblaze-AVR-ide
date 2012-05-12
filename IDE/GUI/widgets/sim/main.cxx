
#include "main.h"

#include "RegDisplay.h"

#include "McuSimCfgMgr.h"
#include "AVR8Sim.h"
#include "AVR8ProgramMemory.h"
#include "AVR8DataMemory.h"
#include "AVR8InstructionSet.h"
#include "HexFile.h"


#include <iostream>
#include <cstdlib>

#include <QApplication>
#include <QVBoxLayout>
#include <QScrollArea>

AVR8Sim * g_AVR8Sim;
HexFile * g_HexFile;
MCUSim::EventLogger * m_log;
RegDisplay ** regDisplays;
const McuDeviceSpecAVR8 * devSpec;

// ---------------------------------------------------------------------------------------

#include <QPushButton>
#include <QHBoxLayout>

SimControlButtons::SimControlButtons() {
	m_simStarted = false;

	createWidgets();
	setupConnections();
}
SimControlButtons::~SimControlButtons() {
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
		std::cout << "SIMULATOR START!" << std::endl;
		m_startStopButton->setText(tr("Stop"));
		m_resetButton->setEnabled(true);
		m_stepButton->setEnabled(true);

		g_AVR8Sim->reset(MCUSim::RSTMD_INITIAL_VALUES);

		const char * filename = "avr8_test_code.hex";
		try {
		g_HexFile->clearAndLoad(filename);
		} catch ( DataFile::DataFileException e ) {
			std::cerr << "Failed to load program memory from the given IHEX file." << std::endl;
			abort();
		}
		dynamic_cast<AVR8ProgramMemory*>(g_AVR8Sim->getSubsys(MCUSim::Subsys::ID_MEM_CODE))->loadDataFile(g_HexFile);

		for ( int i = 0; i < devSpec->m_dataMemory.m_ioRegSize; i++ ) {
			regDisplays[i]->setReadOnly(false);
		}

	} else {
		std::cout << "SIMULATOR STOP!" << std::endl;
		m_startStopButton->setText(tr("Start"));
		m_resetButton->setEnabled(false);
		m_stepButton->setEnabled(false);

		for ( int i = 0; i < devSpec->m_dataMemory.m_ioRegSize; i++ ) {
			regDisplays[i]->setReadOnly(true);
		}
	}

	m_simStarted = !m_simStarted;
}
void SimControlButtons::resetButtonPressed() {
	std::cout << "SIMULATOR RESET!" << std::endl;
	g_AVR8Sim->reset(MCUSim::RSTMD_MCU_RESET);
}
void SimControlButtons::stepButtonPressed() {
	std::cout << "SIMULATOR STEP!" << std::endl;
	g_AVR8Sim->executeInstruction();

	std::cout << "\tPC = " << dynamic_cast<AVR8InstructionSet*>(g_AVR8Sim->getSubsys(MCUSim::Subsys::ID_CPU))->getProgramCounter() << std::endl;
	
	int subsysId, eventId, location, detail;

	while ( 0 != m_log->getEvent(subsysId, eventId, location, detail)) {
		std::cout << "\tEVENT: subsysId = " << subsysId << ", eventId = " << eventId << ", location = " << location<< ", detail = " << detail << std::endl;

		switch ( subsysId ) {
			case MCUSim::Subsys::ID_MEM_DATA:
				switch ( eventId ) {
					case MCUSim::Memory::EVENT_MEM_INF_WR_VAL_CHANGED:
						unsigned int data;
						dynamic_cast<AVR8DataMemory*>(g_AVR8Sim->getSubsys(MCUSim::Subsys::ID_MEM_DATA))->directRead((unsigned int)location, data);

						std::cout << "\t\t--> Data memory at address " << location << " changed to " << data << "." << std::endl;

						if ( (unsigned int)location < devSpec->m_dataMemory.m_regFileSize ) {
							// Processor internal register file
						} else {
							// IO registers
							if ( ((unsigned int)location - devSpec->m_dataMemory.m_regFileSize) >= devSpec->m_dataMemory.m_ioRegSize ) {
								std::cout << "\t\t--> address out of range."<< std::endl;
							} else {
								regDisplays[(unsigned int)location - devSpec->m_dataMemory.m_regFileSize]->setValue((uint)data);
								regDisplays[(unsigned int)location - devSpec->m_dataMemory.m_regFileSize]->setHighlighted(true);
							}
						}
	
						break;
					default:
// 						std::cout << "\t(eventId not recognizd)" << std::endl;
						break;
				}
				break;
			default:
// 				std::cout << "\t(subsysId not recognizd)" << std::endl;
				break;
		}
	}
}

// ---------------------------------------------------------------------------------------

void initSim()
{
	g_AVR8Sim = new AVR8Sim();
	g_HexFile = new HexFile();

	const char * MCUSPECFILE = "../../../simulators/MCUSim/McuSimCfgMgr/mcuspecfile.xml";

	if ( false == McuSimCfgMgr::getInstance()->openConfigFile(MCUSPECFILE) ) {
		std::cerr << "Unable to load MCU config file" << std::endl;
		abort();
	}

	McuSimCfgMgr::getInstance()->setupSimulator("ATmega8A", g_AVR8Sim->getConfig());
	g_AVR8Sim->reset(MCUSim::RSTMD_NEW_CONFIG);

	m_log = g_AVR8Sim->getLog();
}

int main(int argc, char ** argv) {
	QApplication app(argc, argv);

	initSim();

	const McuDeviceSpec * devSpecTmp = McuSimCfgMgr::getInstance()->getDeviceSpec("ATmega8A");
	if ( NULL == devSpecTmp ) {
		std::cerr << "Failed to retrieve the device configuration specification." << std::endl;
		abort();
	}

	devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(devSpecTmp);
	regDisplays = new RegDisplay * [devSpec->m_dataMemory.m_ioRegSize];


	QWidget * widget = new QWidget;
	QVBoxLayout * layout = new QVBoxLayout;
	QScrollArea * scrollArea = new QScrollArea;

	SimControlButtons * simControlButtons = new SimControlButtons;
	layout->addWidget(simControlButtons);

	unsigned int address = devSpec->m_dataMemory.m_regFileSize;
	for ( int i = (devSpec->m_dataMemory.m_ioRegSize - 1); i >= 0; i-- ) {
		if ( 0 == devSpec->m_dataMemory.m_ioRegDesc[i].m_name.size() ) {
			continue;
		}
		uint8_t mask = uint8_t(
			((devSpec->m_dataMemory.m_ioRegInitValues[i] & 0xff00) >> 8)
				|
			((devSpec->m_dataMemory.m_ioRegInitValues[i] & 0xff0000) >> 16));

		QStringList bitNames, toolsTips, statusTips;
		if ( 0 == devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[0].m_name.size() ) {
			regDisplays[i] = new RegDisplay(
				QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_name),
				QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_desc),
				mask);
		} else {
			for ( int j = 0; j < 8; j++ ) {
				bitNames << QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_name);
				toolsTips << QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_ttip);
				statusTips << QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_bit[j].m_stip);
			}

			regDisplays[i] = new RegDisplay(
				QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_name),
				QString::fromStdString(devSpec->m_dataMemory.m_ioRegDesc[i].m_desc),
				mask,
				&bitNames,
				&toolsTips,
				&statusTips);
		}
		regDisplays[i]->setReadOnly(true);

		layout->addWidget(regDisplays[i]);
		address++;
	}

	widget->setLayout(layout);
	scrollArea->setWidget(widget);
	scrollArea->show();


// 	QStringList * bitNames = new QStringList;
// 	*bitNames << "C" << "Z" << "N" << "V" << "S" << "H" << "T" << "I";
// 	QStringList * toolsTips = new QStringList;
// 	*toolsTips << "some text for I" << "some text for T" << "some text for H" << "some text for S" << "some text for V" << "some text for N" << "some text for Z" << "some text for C";
// 	RegDisplay * regDisplay = new RegDisplay("SREG", "The AVR Status Register", 0xFC, bitNames, toolsTips);
// 	regDisplay->show();
// 	regDisplay->setHighlighted(true);
// 	regDisplay->setReadOnly(true);

	return app.exec();
}
