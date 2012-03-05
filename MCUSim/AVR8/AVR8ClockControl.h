/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */


#ifndef AVR8CLOCKCONTROL_H
#define AVR8CLOCKCONTROL_H

class AVR8DataMemory;
class AVR8FusesAndLocks;

#include "../MCUSim.h"

class AVR8ClockControl : public MCUSim::Clock {
public:
	AVR8ClockControl(
		MCUSim::EventLogger	* eventLogger,
		AVR8DataMemory		* dataMemory,
		AVR8FusesAndLocks	& fusesAndLocks);

	enum Event {
		EVENT_CLK_WRN_SOURCE_NOT_USED,
		EVENT_CLK_WRN_CAPACITY_TOO_LOW,
		EVENT_CLK_WRN_ATYPICAL_FREQ,
		EVENT_CLK_WRN_ATYPICAL_CAP,

		EVENT_CLK_ERR_SOURCE_USELESS,
		EVENT_CLK_ERR_INCOMPATIBLE_SOURCE,
		EVENT_CLK_ERR_OUT_OF_FREQ_RANGE,

		EVENT_CLK__MAX__
	};

	struct StartUpTime {
		StartUpTime() {
			m_startUpTime = -1;
			m_addtionalDelay = -1;
		}

		int m_startUpTime;
		float m_addtionalDelay;
	};

	struct Config {
	};

	Config m_config;
	ClockSource m_clockSource;

	void reset(SubsysResetMode mode);

	float getFrequency();
	StartUpTime startUpTime();

protected:
	AVR8DataMemory * m_dataMemory;
	AVR8FusesAndLocks & m_fusesAndLocks;

	inline unsigned int getClksel() const;
	inline unsigned int getSut() const;

	inline float freqHighFreqCrystal(const unsigned int clksel);
	inline float freqLowFreqCrystal();
	inline float freqExternalRC(const unsigned int clksel);
	inline float freqInternalRC(const unsigned int clksel);
	inline float freqExternalSource();

	inline StartUpTime startUpExternalSource(const unsigned int sut);
	inline StartUpTime startUpInternalRC(const unsigned int sut);
	inline StartUpTime startUpExternalRC(const unsigned int sut);
	inline StartUpTime startUpLowFreqCrystal(const unsigned int sut);
	inline StartUpTime startUpHighFreqCrystal(const unsigned int clksel, const unsigned int sut);

private:
	AVR8ClockControl();
};

#endif // AVR8CLOCKCONTROL_H
