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

/*
 * NOTES:
 * -
 */

#ifndef MCUSIM_H
#define MCUSIM_H

class MCUSim {
public:
	/// @brief ...
	enum Arch {
		ARCH_UNSPEC = 0,///< Architecture haven't been specified.

		/// @name AVR
		//@{
		ARCH_MEGAAVR,	///<
		ARCH_TINYAVR,	///<
		ARCH_XMEGA,	///<
		ARCH_AVR32	///<
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
		RC_ADDR_OUT_OF_RANGE,	///< Address out of range (memory operations only).
		RC_VAL_OUT_OF_RANGE,	///< Address out of range (memory operations only).
		RC_NOT_IMPLEMENTED,	///< Not implemented on the simulated micro-controller/processor.
		RC_NOT_SUPPORTED,	///< Implemented on the simulated micro-controller/processor but not in the simulator.
		RC_UNKNOWN_ERROR	///< This should be never used, it's provided only for convenience.
		//@}
	};

	/// @brief ...
	enum Mode {
		MD_POWERED_OFF,
		MD_STOPPED,
		MD_DEFAULT
	};

	/// @brief ...
	struct Config {
	};

	/**
	 * This class is the observer of events occurring inside the simulator. Basically it acts as a queue.
	 * TODO: Sim. stop conditions.
	 */
	class EventLogger {
	private:
		int * /*__restict__*/ m_subsysId;
		int * /*__restict__*/ m_eventId;
		int * /*__restict__*/ m_location;
		int * /*__restict__*/ m_detail;

		int m_size;
		int m_inPos;
		int m_outPos;

		void enlargeQueue() {
			int newSize = m_size * 2;
			int * m_subsysIdNew = new int[newSize];
			int * m_eventIdNew = new int[newSize];
			int * m_locationNew = new int[newSize];
			int * m_detailNew = new int[newSize];

			int i = 1;
			for ( int j = m_outPos + 1; j != m_inPos; j++) {
				m_subsysIdNew[i] = m_subsysId[j];
				m_eventIdNew[i] = m_eventId[j];
				m_locationNew[i] = m_location[j];
				m_detailNew[i] = m_detail[j];

				i++;
			}

			m_outPos = 0;
			m_inPos = i;
			m_size = newSize;

			delete m_subsysId;
			delete m_eventId;
			delete m_location;
			delete m_detail;
			m_subsysId = m_subsysIdNew;
			m_eventId = m_eventIdNew;
			m_location = m_locationNew;
			m_detail = m_detailNew;
		}
	public:
		EventLogger() : m_subsysId(new int[m_size]), m_eventId(new int[m_size]), m_location(new int[m_size]), m_detail(new int[m_size]), m_size(10), m_inPos(1), m_outPos(0) {}

		void logEvent(int subsysId, int eventId, int location, int detail) {
			if ( m_inPos == m_outPos) {
				// The queue is full
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
			ID_CPU,
			ID_FUSES,
			ID_INTERRUPTS,
			ID_TMR_CNT0,
			ID_TMR_CNT1,
			ID_TMR_CNT2,
// 			ID_SIMCONTROL,
// 			ID_IO
		};

		enum SubsysResetMode {
			RSTMD_NEW_CONFIG,
			RSTMD_INITIAL_VALUES,
			RSTMD_MCU_RESET,

			RSTMD__MAX__
		};

		EventLogger * const m_eventLogger;
		const SubsysId m_id;

		virtual void reset(SubsysResetMode mode) = 0;
		virtual ~Subsys();

	protected:
		Subsys(EventLogger * eventLogger, SubsysId id) : m_eventLogger(eventLogger), m_id(id) {};

		void logEvent(int eventId, int eventLocation = 0, int eventDetail = 0)
		{
			m_eventLogger->logEvent(m_id, eventId, eventLocation, eventDetail);
		}
	private:
		Subsys();
	};

	class CPU : public Subsys {
	public:
		enum Event {
			EVENT_CPU_SYS_FATAL_ERROR = 0,

			EVENT_CPU_ERR_INVALID_OPCODE,
			EVENT_CPU_ERR_INVALID_JUMP,
			EVENT_CPU_ERR_INVALID_CALL,
			EVENT_CPU_ERR_INVALID_RET, // There is no active subprogram to return from
			EVENT_CPU_ERR_INVALID_RETI, // There is no active interrupt to return from
			EVENT_CPU_ERR_INVALID_OPSET, // undefined result of the operation due to invalid set of operands
			EVENT_CPU_UNSUPPORTED_INST,
			EVENT_CPU_INST_IGNORED, // (because of settings)
			EVENT_CPU_STOPPED,
			EVENT_CPU_PC_OVERFLOW,
			EVENT_CPU_PC_UNDERFLOW,
			EVENT_CPU_PC_CHANGED,

			EVENT_CPU_CALL,

			EVENT_CPU__MAX__
		};

		CPU(EventLogger * eventLogger) : Subsys(eventLogger, ID_CPU) {};
		virtual int execInstruction() = 0;
	};

	/// @brief ...
	class Memory : public Subsys {
	public:
		/// @brief Memory space, eg. program memory, data memory, etc.
		enum MemorySpace {
			SP_CODE = 1,	///< Internal program memory.
			SP_DATA,	///< Internal data memory.
			SP_GPR		///< General purpose register, on some architectures it might be the same as IDATA.
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

			/// @name Other ...
			//@{
			EVENT_MEM_STACK_OVERFLOW,
			EVENT_MEM_STACK_UNDERFLOW,
			//@}

			EVENT_MEM__MAX__			///< Number of elements in this enumeration.
		};

		/// @brief
		enum Flags {
			// MFLAG_NOT_IMPLEMENTED= 0x80000000,	///< Memory cell is NOT physically present in the memory.
 			// MFLAG_		= 0x40000000,	///<
			MFLAG_DEFAULT		= 0x20000000,	///< Default value contained, must be cleared by write.
			MFLAG_UNDEFINED		= 0x10000000,	///< Uninitialized, so the value might be virtually anything.
			// MFLAG_WR_ONLY	= 0x08000000,	///< For writing only, read is an invalid operation.
			// MFLAG_RD_ONLY	= 0x04000000,	///< For reading only, write is an invalid operation.
			MFLAG_RESERVED		= 0x02000000,	///< Memory location is reserved, i.e. it should not be used
			MFLAG_VIRTUAL		= 0x01000000,	///< This location is mapped to more than oe memory cell, or is completely special.

			MFLAG_MEM__MAX__	= 8		///< Number of elements in this enumeration.
		};

		const MemorySpace m_space;

		virtual RetCode directRead(unsigned int addr, unsigned int & data) const = 0;
		virtual RetCode directWrite(unsigned int addr, unsigned int data) = 0;
		virtual void resize(unsigned int newSize) = 0;
		virtual unsigned int size() = 0;

	protected:
		Memory(EventLogger * eventLogger, MemorySpace space) : Subsys(eventLogger, (Subsys::SubsysId)(0-space)), m_space(space) {};
	};

// 	class IO : public Subsys
// 	{
// 	public:
// 		IO(EventLogger * eventLogger) : Subsys(eventLogger, ID_IO) {};
// 	};

// 	/**
// 	 * @brief General interface for control of simulation
// 	 *
// 	 */
// 	class SimControl
// 	{
// 	public:
// 		virtual void execInstructionCycle() = 0;
// 		virtual void clockRaise() = 0;
// 		virtual void clockFall() = 0;
// 		virtual void powerOn() = 0;
// 		virtual void powerOff() = 0;
// 	};

// 	virtual Memory * getMemory(Memory::MemorySpace memType) = 0;
	virtual Subsys * getSubsys(Subsys::SubsysId id) = 0;

// 	virtual Config * getConfig() = 0;
	virtual RetCode setConfig(Config * newConfig) = 0;
	virtual EventLogger * getLog() = 0;
	virtual Arch arch() = 0;
	virtual Mode mode() = 0;
};

#endif // MCUSIM_H
