// =====================================================================================================================
/**
 * @brief
 * C++ Interface: PicoBlaze simulator subsystem for stack.
 *
 * Implements processor stack, requires no configuration, default stack size is 31, the stack in initialized to contain
 * only zeros after MCU reset. Fast operation of the subsystem was a priority. The stack subsystem simulator shares the
 * common memory interface, like register file, scratch pad RAM, program memory, etc.
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>, (C) 2013
 * @ingroup PicoBlaze
 * @file PicoBlazeStack.h
 */
// =====================================================================================================================

#ifndef PICOBLAZESTACK_H
#define PICOBLAZESTACK_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

/**
 * @brief
 * @ingroup PicoBlaze
 * @class PicoBlazeStack
 */
class PicoBlazeStack : public MCUSimMemory
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief Events generated by the subsystem.
         */
        enum Event
        {
            EVENT_STACK_OVERFLOW = EVENT_MEM__MAX__,  ///< Stack capacity was already exhausted by the previous push.
            EVENT_STACK_UNDERFLOW,                    ///< A value was been popped from the stack while the stack was already empty.
            EVENT_STACK_SP_CHANGED,                   ///<
        };

        /**
         * @brief Subsystem configuration.
         */
        struct Config
        {
            /// Stack capacity.
            unsigned int m_size;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief Initializes memory used by the stack.
         */
        PicoBlazeStack();

        /**
         * @brief Cleans up memory dynamically allocated for the stack.
         */
        ~PicoBlazeStack();

    ////    Public Operations    ////
    public:
        /**
         * @brief Link the subsystem with other simulator subsystems which this subsystem necessarily needs to function.
         * @param[in,out] eventLogger Simulator event observer.
         * @return This object.
         */
        PicoBlazeStack * link ( MCUSimEventLogger * eventLogger );

        /**
         * @brief Reset the subsystem in the specified mode.
         *
         * It is supposed to be called by mainly when the entire processor simulator is reseted.
         *
         * @param[in] mode There are various modes of reset, e.g. accept new configuration, processor master reset, etc.
         */
        void reset ( MCUSimBase::ResetMode mode );

        /**
         * @brief Read data directly from the stack bypassing its normal mode of operation.
         * @param[in] addr Source address, starts with 0; push increments, pop decrements.
         * @param[out] data Address to the program memory read from the specified address.
         * @return This method doesn't generate simulator events that's why it uses return code to report final status.
         */
        MCUSim::RetCode directRead ( unsigned int addr,
                                     unsigned int & data ) const;

        /**
         * @brief Write data directly to the stack bypassing its normal mode of operation.
         * @param[in] addr Target address, starts with 0; push increments, pop decrements.
         * @param[in] data Address to the program memory to store at the specified address.
         * @return This method doesn't generate simulator events that's why it uses return code to report final status.
         */
        MCUSim::RetCode directWrite ( unsigned int addr,
                                      unsigned int data );

        /**
         * @brief Change stack capacity.
         * @warning This operation does NOT preserve the memory contents, i.e. the stack is (re-)initialize to zeros.
         * @param[in] newSize New size of the stack in number of values which can be stored in the stack at once.
         */
        void resize ( unsigned int newSize );

        /**
         * @brief Load contents of the stack from a file, like Intel 8 HEX file or something of that sort.
         * @param[in] file File data container.
         */
        void loadDataFile ( const DataFile * file );

        /**
         * @brief Store contents of the stack to a file, like Intel 8 HEX file or something of that sort.
         * @param[in,out] file File data container.
         */
        void storeInDataFile ( DataFile * file ) const;

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief Get stack capacity.
         * @return Stack in number of values which can be stored in the stack at the same time, not bytes.
         */
        unsigned int size() const
        {
            return m_config.m_size;
        }

        /**
         * @brief Push value onto stack.
         *
         * This method is NOT supposed to be used outside the simulator's own subsystems, like from GUI.
         *
         * @param value Address to the program memory to be stored in stack.
         */
        inline void pushOnStack ( unsigned int value );

        /**
         * @brief Pop value from the stack.
         *
         * This method is NOT supposed to be used outside the simulator's own subsystems, like from GUI.
         *
         * @return Address to the program memory retrieved from the stack.
         */
        inline unsigned int popFromStack();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief Reset the subsystem to a state in which real hardware would be after power-up.
         */
        inline void resetToInitialValues();

        /**
         * @brief Instruct the subsystem to accept new configuration.
         */
        inline void loadConfig();

        /**
         * @brief Reset the subsystem to a state in which real hardware would be after master reset.
         */
        inline void mcuReset();

    ////    Public Attributes    ////
    public:
        /// Subsystem configuration.
        Config m_config;

    ////    Private Attributes    ////
    private:
        /// Array holding the values pushed onto the stack.
        unsigned int * m_data;

        /// Stack pointer, starts with 0; push increments, pop decrements.
        unsigned int m_position;
};

// -----------------------------------------------------------------------------
// Inline Function Definitions
// -----------------------------------------------------------------------------

inline void PicoBlazeStack::pushOnStack ( unsigned int value )
{
    if ( m_config.m_size == m_position )
    {
        logEvent(EVENT_STACK_OVERFLOW, m_position, value);
        m_position = 0;
    }

    logEvent(EVENT_MEM_INF_WR_VAL_WRITTEN, m_position, value);
    if ( m_data[m_position] != value )
    {
        logEvent(EVENT_MEM_INF_WR_VAL_CHANGED, m_position, value);
    }
    m_data[m_position++] = value;
    logEvent(EVENT_STACK_SP_CHANGED, m_position, -1);
}

inline unsigned int PicoBlazeStack::popFromStack()
{
    unsigned int result;

    if ( 0 == m_position )
    {
        logEvent(EVENT_STACK_UNDERFLOW, m_position, -1);
        m_position = m_config.m_size;
    }

    result = ( 0x3ff & m_data[m_position] );

    m_position--;
    logEvent(EVENT_STACK_SP_CHANGED, m_position, -1);
    logEvent(EVENT_MEM_INF_RD_VAL_READ, m_position, result);

    return result;
}

#endif // PICOBLAZESTACK_H
