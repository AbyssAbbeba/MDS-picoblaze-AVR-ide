/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup MCUSim
 * @file MCUSim.h
 */

#ifndef MCUSIM_H
#define MCUSIM_H

class DataFile;

#include <cstddef>

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSim
 */
class MCUSim {
public:
	/// @brief ...
	enum Arch {
		ARCH_INVALID = -1,

		ARCH_AVR8,
		ARCH_AVR32
	};

	/// @brief ...
	enum Family {
		FAMILY_INVALID = -1,///< Architecture hasn't been specified.

		/// @name AVR8
		//@{
		FAMILY_MEGAAVR,	///<
		FAMILY_TINYAVR,	///<
		FAMILY_XMEGA,	///<
		//@}

		/// @name AVR32
		//@{
		FAMILY_AVR32	///<
		//@}
	};

	/// @brief ...
	enum RetCode {
		RC_OK = 0,		///< Success.

		/// @name Warnings
		//@{
		RC_NONDETERMINISTIC,	///< Random factor involved in the result (warning only).
		//@}

		/// @name Errors
		//@{
		RC_NOT_COMPATIBLE,	///< Different architecture, etc.
		RC_ADDR_OUT_OF_RANGE,	///< Address out of range (memory operations only).
		RC_VAL_OUT_OF_RANGE,	///< Address out of range (memory operations only).
		RC_NOT_IMPLEMENTED,	///< Not implemented on the simulated micro-controller/processor.
		RC_NOT_SUPPORTED,	///< Implemented on the simulated micro-controller/processor but not in the simulator.
		RC_UNKNOWN_ERROR	///< This should be never used, it's provided only for convenience.
		//@}
	};

	enum ResetMode {
		RSTMD_NEW_CONFIG,
		RSTMD_INITIAL_VALUES,
		RSTMD_MCU_RESET,

		RSTMD__MAX__
	};

	/// @brief ...
	enum Mode {
		MD_POWERED_OFF,
		MD_STOPPED,
		MD_SLEEP,
		MD_NORMAL
	};

	/// @brief ...
	class Config {
	public:
		Config(Arch arch) : m_arch(arch) {}
		virtual ~Config() {}

		Arch getArch() const {
			return m_arch;
		}
	private:
		const Arch m_arch;
	};

	/**
	 * This class is the observer of events occurring inside the simulator. Basically it acts as a queue.
	 * TODO: Sim. stop conditions.
	 */
	class EventLogger {
	private:
		int m_size;
		int m_inPos;
		int m_outPos;

		int * /*__restict__*/ m_subsysId;
		int * /*__restict__*/ m_eventId;
		int * /*__restict__*/ m_location; // or reason
		int * /*__restict__*/ m_detail;

		void enlargeQueue() {
			int newSize = m_size * 2;
			int * m_subsysIdNew = new int[newSize];
			int * m_eventIdNew = new int[newSize];
			int * m_locationNew = new int[newSize];
			int * m_detailNew = new int[newSize];

			int i = 1;
			for ( int j = m_outPos + 1; j != m_inPos; j = ((j + 1) % m_size) ) {
				m_subsysIdNew[i] = m_subsysId[j];
				m_eventIdNew[i] = m_eventId[j];
				m_locationNew[i] = m_location[j];
				m_detailNew[i] = m_detail[j];

				i++;
				i %= m_size;
			}

			m_outPos = 0;
			m_inPos = i;
			m_size = newSize;

			delete[] m_subsysId;
			delete[] m_eventId;
			delete[] m_location;
			delete[] m_detail;
			m_subsysId = m_subsysIdNew;
			m_eventId = m_eventIdNew;
			m_location = m_locationNew;
			m_detail = m_detailNew;
		}
	public:
		EventLogger()
			 :
			m_size(10),
			m_inPos(1),
			m_outPos(0),
			m_subsysId(new int[m_size]),
			m_eventId(new int[m_size]),
			m_location(new int[m_size]),
			m_detail(new int[m_size])
		{
		}

		~EventLogger() {
			delete[] m_subsysId;
			delete[] m_eventId;
			delete[] m_location;
			delete[] m_detail;
		}

		void clear() {
			m_inPos = 1;
			m_outPos = 0;
		}

		void logEvent(int subsysId, int eventId, int location /* or reason */, int detail) {
			if ( m_inPos == m_outPos) {
				// The queue is full -> enlarge it.
				enlargeQueue();

				// OR we can simply:
				// return;
			}

			m_subsysId[m_inPos] = subsysId;
			m_eventId[m_inPos] = eventId;
			m_location[m_inPos] = location;
			m_detail[m_inPos] = detail;

			m_inPos++;
			m_inPos %= m_size;
		}

		/**
		 * @brief This operation removes the read item.
		 * @return How many entries was there before this method was called, 0 means that there was nothing.
		 */
		int getEvent(int & /*__restict__*/ subsysId, int & /*__restict__*/ eventId, int & /*__restict__*/ location, int & /*__restict__*/ detail) {
			m_outPos++;
			m_outPos %= m_size;

			if ( m_outPos == m_inPos ) {
				// The queue is empty
				m_outPos--;
				if ( -1 == m_outPos ) {
					m_outPos += m_size;
				}
				return 0;
			}

			subsysId = m_subsysId[m_outPos];
			eventId = m_eventId[m_outPos];
			location = m_location[m_outPos];
			detail = m_detail[m_outPos];

			int result = m_inPos - m_outPos - 1;
			if ( result < 0 ) {
				result += m_size;
			}
			return result;
		}
	};

	/// @brief ...
	class Subsys {
	public:
		/// @brief ...
		enum SubsysId {
			ID_INVALID = 0,

			ID_MEM_CODE,
			ID_MEM_DATA,
			ID_MEM_EEPROM,

			ID_CPU,
			ID_FUSES,
			ID_INTERRUPTS,
			ID_EXT_INT,
			ID_WATCHDOG,
			ID_IO,
			ID_BOOT_LOADER,
			ID_SYS_CONTROL,
			ID_CLK_CONTROL,
			ID_COUNTER_0,
			ID_COUNTER_1,
			ID_COUNTER_2,
			ID_SPI,
			ID_USART,
			ID_TWI,
			ID_ADC,
			ID_ACOMP,
			ID_ISP,
			ID_PPROG,

			ID__MAX__
		};

		EventLogger * m_eventLogger;

		SubsysId getId() const {
			return m_id;
		}
		virtual void reset(ResetMode mode) = 0;
		virtual ~Subsys() {};

	protected:
		Subsys() : m_id(ID_INVALID) {};
		Subsys(EventLogger * eventLogger, SubsysId id) {
			link(eventLogger, id);
		};

		void link(EventLogger * eventLogger, SubsysId id) {
			m_eventLogger = eventLogger;
			m_id = id;
		}

		void logEvent(int eventId, int eventLocation = 0, int eventDetail = 0)
		{
			m_eventLogger->logEvent(m_id, eventId, eventLocation, eventDetail);
		}

	private:
		SubsysId m_id;
	};

	class CPU : public Subsys {
	public:
		enum Event {
			EVENT_CPU_SYS_FATAL_ERROR	= 0,

			EVENT_CPU_ERR_INVALID_OPCODE,
			EVENT_CPU_ERR_INVALID_JUMP,
			EVENT_CPU_ERR_INVALID_CALL,
			EVENT_CPU_ERR_INVALID_RET, // There is no active subprogram to return from
			EVENT_CPU_ERR_INVALID_RETI, // There is no active interrupt to return from
			EVENT_CPU_ERR_INVALID_OPSET, // undefined result of the operation due to invalid set of operands
			EVENT_CPU_UNSUPPORTED_INST,
			EVENT_CPU_INST_IGNORED, // (because of settings)
			EVENT_CPU_MODE_CHANGED,
			EVENT_CPU_PC_OVERFLOW,
			EVENT_CPU_PC_UNDERFLOW,
			EVENT_CPU_PC_CHANGED,

			EVENT_CPU_CALL,

			EVENT_CPU__MAX__
		};
		virtual unsigned int getProgramCounter() const = 0;
		virtual void setProgramCounter(unsigned int newPc) = 0;

	protected:
		CPU() {};
		CPU(EventLogger * eventLogger) : Subsys(eventLogger, ID_CPU) {};

		void link(EventLogger * eventLogger) {
			Subsys::link(eventLogger, ID_CPU);
		}
	};

	/// @brief ...
	class Memory : public Subsys {
	public:
		/// @brief Memory space, eg. program memory, data memory, etc.
		enum MemorySpace {
			SP_INVALID = 0,
			SP_CODE = ID_MEM_CODE,		///< Internal program memory.
			SP_DATA = ID_MEM_DATA,		///< Internal data memory.
			SP_EEPROM = ID_MEM_EEPROM,	///< Internal data EEPROM.

			SP__MAX__
		};

		/// @brief
		enum Event {
			/// @name System condition, not directly related to the simulated memory.
			//@{
			EVENT_MEM_SYS_FATAL_ERROR = 0,		///< Severe failure occurred during the subsystem operations.
			//@}

			/// @name Errors, the request couldn't have been executed correctly.
			//@{
			EVENT_MEM_ERR_RD_NONEXISTENT,		///< Attempt to read from a nonexistent memory space.
			EVENT_MEM_ERR_WR_NONEXISTENT,		///< Attempt to write to a nonexistent memory space.
			EVENT_MEM_ERR_RD_NOT_IMPLEMENTED,	///< Attempt to read from an unimplemented memory cell.
			EVENT_MEM_ERR_WR_NOT_IMPLEMENTED,	///< Attempt to write to an unimplemented memory cell.
			EVENT_MEM_ERR_RD_ACCESS_DENIED,
			EVENT_MEM_ERR_WR_ACCESS_DENIED,
			//@}

			/// @name Warnings, the request might have involved a random chance.
			//@{
			EVENT_MEM_WRN_RD_UNDEFINED,		///< Undefined (uninitialized) value read.
			EVENT_MEM_WRN_RD_DEFAULT,		///< Value defined by default condition was read.
			EVENT_MEM_WRN_RD_WRITE_ONLY,		///< Value read from write only (pseudo-)memory cell.
			EVENT_MEM_WRN_WR_READ_ONLY,		///< Value written to read only memory cell.
			EVENT_MEM_WRN_RD_PAR_WRITE_ONLY,	///< Value read from partially write only memory cell.
			EVENT_MEM_WRN_WR_PAR_READ_ONLY,		///< Value written to partially read only memory cell.
			EVENT_MEM_WRN_RD_RESERVED_READ,		///<
			EVENT_MEM_WRN_WR_RESERVED_WRITTEN,	///<
			//@}

			/// @name Informative, something happened but nothing went wrong.
			//@{
			EVENT_MEM_INF_WR_VAL_CHANGED,		///< Memory cell content has been changed.
			EVENT_MEM_INF_WR_VAL_WRITTEN,		///< Memory cell content was written.
			EVENT_MEM_INF_WR_VAL_READ,		///< Memory cell content was read.
			//@}

			EVENT_MEM__MAX__			///< Number of elements in this enumeration.
		};

		/// @brief
		enum Flags {
			//MFLAG_		= 0x01 << 24,	///<
			//MFLAG_		= 0x02 << 24,	///<
 			//MFLAG_		= 0x04 << 24,	///<
			//MFLAG_		= 0x08 << 24,	///<
			MFLAG_VIRTUAL		= 0x10 << 24,	///< This location is mapped to more than oe memory cell, or is completely special.
			MFLAG_DEFAULT		= 0x20 << 24,	///< Default value contained, must be cleared by write.
			MFLAG_RESERVED		= 0x40 << 24,	///< Memory location is reserved, i.e. it should not be used
			MFLAG_UNDEFINED		= 0x80 << 24,	///< Uninitialized, so the value might be virtually anything.

			MFLAG_MEM__MAX__	= 8		///< Number of elements in this enumeration.
		};

		MemorySpace getSpace() const {
			return m_space;
		}

		virtual RetCode directRead(unsigned int addr, unsigned int & data) const = 0;
		virtual RetCode directWrite(unsigned int addr, unsigned int data) = 0;
		virtual void resize(unsigned int newSize) = 0;
		virtual unsigned int size() const = 0;
		virtual void loadDataFile(const DataFile * file) = 0;
		virtual void storeInDataFile(DataFile * file) const = 0;

	protected:
		Memory() : m_space(SP_INVALID) {};
		Memory(EventLogger * eventLogger, MemorySpace space) : Subsys(eventLogger, (Subsys::SubsysId)(space)), m_space(space) {};

		void link(EventLogger * eventLogger, MemorySpace space) {
			Subsys::link(eventLogger, (Subsys::SubsysId)(space));
			m_space = space;
		}

	private:
		MemorySpace m_space;
	};

	class IO : public Subsys
	{
	public:
		#ifdef SIM_IO_QUADRUPLE_PRECISION
			typedef long double SimFloatType;
		#elseifdef SIM_IO_DOUBLE_PRECISION
			typedef double SimFloatType;
		#else
			typedef float SimFloatType;
		#endif

		enum Event {
			EVENT_IO_INDETERMINABLE_LOG,

			EVENT_IO__MAX__
		};
		enum InterfaceIndex {
			II_VOLTAGE_INT	= 0,
			II_REAL_VOLTAGE	= 1,
			II_CURRENT	= 2,
			II_RESISTANCE	= 3,
			II__MAX__
		};

		/*
		 * NOTE:
		 * Voltages and currents can be anything except for NaN, since NaN means that state of the pin is a "mystery",
		 * Values of resistance:
		 *   - equal to +INFINITY  : purely input pin
		 *   - lower than 0        : the pin has a very special function, and therefore must be excluded from nodal analysis
		 *   - equal to -INFINITY  : (reserved for future use)
		 *   - not a number (NaN)  : pin is not implemented!
		 *   - otherwise           : normal pin
		 */

		/*
		 * NOTE:
		 * math.h defines also these macros:
		 *   - INFINITY
		 *   - NAN
		 *
		 * And don't check whether NAN macro has been defined (it's a GNU extension)!
		 */

		virtual unsigned int getNumberOfPins() const = 0;
		virtual void setSourceVoltage(SimFloatType voltage) = 0;
		virtual SimFloatType getSourceVoltage() const = 0;
		virtual SimFloatType ** getLowLevelInterface() = 0;
	protected:
		IO() {};
		IO(EventLogger * eventLogger) : Subsys(eventLogger, ID_IO) {};

		void link(EventLogger * eventLogger) {
			Subsys::link(eventLogger, ID_IO);
		}
	};

	/**
	 * @brief
	 */
	class Clock : public Subsys
	{
	public:
		class ClockSource {
		public:
			enum Type {
				TYPE_NONE,
				TYPE_RC,
				TYPE_CERAMIC_RES,
				TYPE_LOW_FREQ_CRYSTAL,
				TYPE_CRYSTAL,
				TYPE_EXTERNAL
			};

			ClockSource() : m_type(TYPE_NONE) {};

			Type  getType()		const	{ return m_type;	}
			float getResistance()	const	{ return m_resistance;	}
			float getCapacity()	const	{ return m_capacity;	}
			float getFrequency()	const	{ return m_frequency;	}

		protected:
			Type m_type;
			float m_resistance;
			float m_capacity;
			float m_frequency;
		};

		class ClockSourceSpec : public ClockSource {
		private:
			ClockSourceSpec();
		public:
			void setInternalClockSource() {
				m_type = TYPE_NONE;
			}
			void setRcOscilator(float resistance, float capacity) {
				m_type = TYPE_RC;
				m_resistance = resistance;
				m_capacity = capacity;
			}
			void setCeramicResonator(float resFrequency, float condCapacity = 0) {
				m_type = TYPE_RC;
				m_frequency = resFrequency;
				m_capacity = condCapacity;
			}
			void setLowFreqCrystal(float crystFrequency, float condCapacity = 0) {
				m_type = TYPE_LOW_FREQ_CRYSTAL;
				m_frequency = crystFrequency;
				m_capacity = condCapacity;
			}
			void setCrystal(float crystFrequency, float condCapacity = 0) {
				m_type = TYPE_CRYSTAL;
				m_frequency = crystFrequency;
				m_capacity = condCapacity;
			}
			void setExternalClockSource(float frequency) {
				m_type = TYPE_EXTERNAL;
				m_frequency = frequency;
			}
		};
	protected:
		Clock() {};
		Clock(EventLogger * eventLogger) : Subsys(eventLogger, ID_CLK_CONTROL) {};

		void link(EventLogger * eventLogger) {
			Subsys::link(eventLogger, ID_CLK_CONTROL);
		}
	};

	virtual ~MCUSim() {};

	virtual void reset(ResetMode mode) = 0;

	virtual float cycles2time(int numOfCycles) = 0;
	virtual int executeInstruction() = 0;
	virtual int timeStep(float time) = 0;

	virtual Subsys * getSubsys(Subsys::SubsysId id) = 0;

	virtual Clock::ClockSource & getClockSource() = 0;
	virtual Config & getConfig() = 0;
	virtual EventLogger * getLog() = 0;
	virtual Arch arch() const = 0;
	virtual Family family() const = 0;
	virtual const char * name() const = 0;
	virtual Mode mode() const = 0;
};

#endif // MCUSIM_H
