// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUSim
 * @file MCUSimMemory.h
 */
// =============================================================================

#ifndef MCUSIMMEMORY_H
#define MCUSIMMEMORY_H

#include "MCUSimSubsys.h"

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimMemory
 */
class MCUSimMemory : public MCUSimSubsys
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief Memory space, eg. program memory, data memory, etc.
         */
        enum MemorySpace
        {
            SP_INVALID = 0,                 ///<
            SP_DATA    = ID_MEM_DATA,       ///< Internal data memory.
            SP_CODE    = ID_MEM_CODE,       ///< Internal program memory.
            SP_STACK   = ID_STACK,          ///< Internal separate stack.
            SP_EEPROM  = ID_MEM_EEPROM,     ///< Internal data EEPROM.
            SP_REGS    = ID_MEM_REGISTERS,  ///< Internal processor registers.

            SP__MAX__                       ///<
        };

        /**
         * @brief
         */
        enum Event
        {
            /// @name System condition, not directly related to the simulated memory.
            //@{
                EVENT_MEM_SYS_FATAL_ERROR = 0,    ///< Severe failure occurred during the subsystem operations.
            //@}

            /// @name Errors, the request couldn't have been executed correctly.
            //@{
                EVENT_MEM_ERR_RD_NONEXISTENT,      ///< Attempt to read from a nonexistent memory space.
                EVENT_MEM_ERR_WR_NONEXISTENT,      ///< Attempt to write to a nonexistent memory space.
                EVENT_MEM_ERR_RD_NOT_IMPLEMENTED,  ///< Attempt to read from an unimplemented memory cell.
                EVENT_MEM_ERR_WR_NOT_IMPLEMENTED,  ///< Attempt to write to an unimplemented memory cell.
                EVENT_MEM_ERR_RD_ACCESS_DENIED,
                EVENT_MEM_ERR_WR_ACCESS_DENIED,
            //@}

            /// @name Warnings, the request might have involved a random chance.
            //@{
                EVENT_MEM_WRN_RD_UNDEFINED,        ///< Undefined (uninitialized) value read.
                EVENT_MEM_WRN_RD_DEFAULT,          ///< Value defined by default condition was read.
                EVENT_MEM_WRN_RD_WRITE_ONLY,       ///< Value read from write only (pseudo-)memory cell.
                EVENT_MEM_WRN_WR_READ_ONLY,        ///< Value written to read only memory cell.
                EVENT_MEM_WRN_RD_PAR_WRITE_ONLY,   ///< Value read from partially write only memory cell.
                EVENT_MEM_WRN_WR_PAR_READ_ONLY,    ///< Value written to partially read only memory cell.
                EVENT_MEM_WRN_RD_RESERVED_READ,    ///<
                EVENT_MEM_WRN_WR_RESERVED_WRITTEN, ///<
            //@}

            /// @name Informative, something happened but nothing went wrong.
            //@{
                EVENT_MEM_INF_WR_VAL_CHANGED,      ///< Memory cell content has been changed.
                EVENT_MEM_INF_WR_VAL_WRITTEN,      ///< Memory cell content was written.
                EVENT_MEM_INF_RD_VAL_READ,         ///< Memory cell content was read.
                EVENT_MEM_INF_RD_ADDR_OVERFLOW,    ///< "address = ( address % size )" happened during read
                EVENT_MEM_INF_WR_ADDR_OVERFLOW,    ///< "address = ( address % size )" happened during write
            //@}

            EVENT_MEM__MAX__                        ///< Number of elements in this enumeration.
        };

        /**
         * @brief
         */
        enum Flags
        {
            //MFLAG_                = 0x01 << 24,   ///<
            //MFLAG_                = 0x02 << 24,   ///<
            //MFLAG_                = 0x04 << 24,   ///<
            //MFLAG_                = 0x08 << 24,   ///<
            MFLAG_VIRTUAL           = 0x10 << 24,   ///< This location is mapped to more than oe memory cell, or is completely special.
            MFLAG_DEFAULT           = 0x20 << 24,   ///< Default value contained, must be cleared by write.
            MFLAG_RESERVED          = 0x40 << 24,   ///< Memory location is reserved, i.e. it should not be used
            MFLAG_UNDEFINED         = 0x80 << 24,   ///< Uninitialized, so the value might be virtually anything.

            MFLAG_MEM__MAX__        = 8             ///< Number of elements in this enumeration.
        };

    ////    Constructors and Destructors    ////
    protected:
        /**
         * @brief
         */
        MCUSimMemory() : m_space(SP_INVALID) {};

        /**
         * @brief
         * @param[in,out] eventLogger
         * @param[in] space
         */
        MCUSimMemory ( MCUSimEventLogger * eventLogger,
                       MemorySpace space )
                     : MCUSimSubsys ( eventLogger, (MCUSimSubsys::SubsysId)(space) ),
                       m_space(space)
        {
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] addr
         * @param[out] data
         * @return
         */
        virtual MCUSimBase::RetCode directRead ( unsigned int addr,
                                                 unsigned int & data ) const = 0;

        /**
         * @brief
         * @param[in] addr
         * @param[in] data
         * @return
         */
        virtual MCUSimBase::RetCode directWrite ( unsigned int addr,
                                                  unsigned int data ) = 0;

        /**
         * @brief
         * @param[in] newSize
         */
        virtual void resize ( unsigned int newSize ) = 0;

        /**
         * @brief
         * @return
         */
        virtual unsigned int size() const = 0;

        /**
         * @brief
         * @param[in] file
         */
        virtual void loadDataFile ( const DataFile * file ) = 0;

        /**
         * @brief
         * @param[in,out] file
         */
        virtual void storeInDataFile ( DataFile * file ) const = 0;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        MemorySpace getSpace() const
        {
            return m_space;
        }

    ////    Inline Protected Operations    ////
    protected:
        /**
         * @brief
         */
        void link ( MCUSimEventLogger * eventLogger,
                    MemorySpace space )
        {
            MCUSimSubsys::link(eventLogger, (MCUSimSubsys::SubsysId)(space));
            m_space = space;
        }

    ////    Private Attributes    ////
    private:
        /**
         * @brief
         */
        MemorySpace m_space;
};

#endif // MCUSIMMEMORY_H
