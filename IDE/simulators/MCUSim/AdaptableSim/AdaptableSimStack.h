// =====================================================================================================================
/**
 * @brief
 * C++ Interface: AdaptableSim simulator subsystem for stack.
 *
 * Implements processor stack, requires no configuration, default stack size is 31, the stack in initialized to contain
 * only zeros after MCU reset. Fast operation of the subsystem was a priority. The stack subsystem simulator shares the
 * common memory interface, like register file, scratch pad RAM, program memory, etc.
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdaptableSim
 * @file AdaptableSimStack.h
 */
// =====================================================================================================================

#ifndef ADAPTABLESIMSTACK_H
#define ADAPTABLESIMSTACK_H

// Forward declarations
class DataFile;

#include "../MCUSim.h"

#include "AdaptableSimRegisters.h"
#include "AdaptableSimDataMemory.h"

/**
 * @brief
 * @ingroup AdaptableSim
 * @class AdaptableSimStack
 */
class AdaptableSimStack : public MCUSimMemory
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
            /// Stack capacity, set to 0 when simple stack is used or no stack is used at all.
            unsigned int m_size;

            /// Use stack as designated single purpose memory accessible only by specific instruction subset.
            bool m_useDesignatedStack;

            /**
             * @brief Settings related to simple stack, i.e. stack not implemented as designated single purpose memory.
             */
            struct SimpleStack
            {
                /// Target memory space.
                enum Space
                {
                    SP_REG, ///< Register file.
                    SP_DATA ///< Data memory.
                };

                /// Mode of stack operation.
                enum Operation
                {
                    OP_PREINC,  ///< Increment SP before PUSH, decrement after  POP.
                    OP_POSTINC, ///< Increment SP after  PUSH, decrement before POP.
                    OP_PREDEC,  ///< Decrement SP before PUSH, increment after  POP.
                    OP_POSTDEC  ///< Decrement SP after  PUSH, increment before POP.
                } m_operation;

                /// Location where the stacked values are stored.
                struct Content
                {
                    Space m_space; ///< Memory space.
                    int m_offset;  ///< This value is added to the stack pointer value before accessing stack memory.
                } m_content;

                /// Location designated for the stack pointer.
                struct Pointer
                {
                    Space m_space;   ///< Memory space.
                    int m_address;   ///< Address in the designated memory space.
                    int m_maxSize; ///< Maximum allowed stack pointer value, -1 means no limit.
                    bool m_indirect; ///< If true, "m_address" is threated as indirect address.
                } m_pointer;
            } m_simpleStack;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief Initializes memory used by the stack.
         */
        AdaptableSimStack();

        /**
         * @brief Cleans up memory dynamically allocated for the stack.
         */
        ~AdaptableSimStack();

    ////    Public Operations    ////
    public:
        /**
         * @brief Link the subsystem with other simulator subsystems which this subsystem necessarily needs to function.
         * @param[in,out] eventLogger Simulator event observer.
         * @param[in,out] registers Register file simulation subsystem.
         * @param[in,out] dataMemory Generic data memory simulation subsystem.
         * @return This object.
         */
        AdaptableSimStack * link ( MCUSimEventLogger      * eventLogger,
                                   AdaptableSimRegisters  * registers,
                                   AdaptableSimDataMemory * dataMemory );

        /**
         * @brief Reset the subsystem in the specified mode.
         *
         * It is supposed to be called by mainly when the entire processor simulator is reseted.
         *
         * @param[in] mode There are various modes of reset, e.g. accept new configuration, processor master reset, etc.
         */
        virtual void reset ( MCUSimBase::ResetMode mode ) override;

        /**
         * @brief Read data directly from the stack bypassing its normal mode of operation.
         * @param[in] addr Source address, starts with 0; push increments, pop decrements.
         * @param[out] data Address to the program memory read from the specified address.
         * @return This method doesn't generate simulator events that's why it uses return code to report final status.
         */
        virtual MCUSim::RetCode directRead ( unsigned int addr,
                                             unsigned int & data ) const override;

        /**
         * @brief Write data directly to the stack bypassing its normal mode of operation.
         * @param[in] addr Target address, starts with 0; push increments, pop decrements.
         * @param[in] data Address to the program memory to store at the specified address.
         * @return This method doesn't generate simulator events that's why it uses return code to report final status.
         */
        virtual MCUSim::RetCode directWrite ( unsigned int addr,
                                              unsigned int data ) override;

        /**
         * @brief Change stack capacity.
         * @warning This operation does NOT preserve the memory contents, i.e. the stack is (re-)initialize to zeros.
         * @param[in] newSize New size of the stack in number of values which can be stored in the stack at once.
         */
        virtual void resize ( unsigned int newSize ) override;

        /**
         * @brief Load contents of the stack from a file, like Intel 8 HEX file or something of that sort.
         * @param[in] file File data container.
         */
        virtual void loadDataFile ( const DataFile * file ) override;

        /**
         * @brief Store contents of the stack to a file, like Intel 8 HEX file or something of that sort.
         * @param[in,out] file File data container.
         */
        virtual void storeInDataFile ( DataFile * file ) const override;

        /**
         * @brief Push value onto stack.
         *
         * This method is NOT supposed to be used outside the simulator's own subsystems, like from GUI.
         *
         * @param value Address to the program memory to be stored in stack.
         */
        void pushOnStack ( unsigned int value );

        /**
         * @brief Pop value from the stack.
         *
         * This method is NOT supposed to be used outside the simulator's own subsystems, like from GUI.
         *
         * @return Address to the program memory retrieved from the stack.
         */
        unsigned int popFromStack();

    ////    Private Operations    ////
    private:
        /**
         * @return
         */
        unsigned int getSPVal();

        /**
         * @param[in] by
         */
        void incrSP ( int by );

        /**
         * @param[in] value
         */
        void pushBySP ( unsigned int value );

        /**
         * @return
         */
        unsigned int popBySP();

    ////    Inline Public Operations    ////
    public:
        /**
         * @brief Get stack capacity.
         * @return Stack in number of values which can be stored in the stack at the same time, not bytes.
         */
        virtual unsigned int size() const override
        {
            return m_config.m_size;
        }

        /**
         * @brief Set stack pointer to the given value.
         *
         * @param[in] sp New SP value; WARNING: setting this value incorrectly may result in undefined behavior.
         */
        void setSP ( unsigned int sp )
        {
            m_position = (int) sp;
        }

        /**
         * @brief Get current value of stack pointer.
         * @return SP.
         */
        unsigned int getSP() const
        {
            return (unsigned int) m_position;
        }

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
        int m_position;

        /// @name AdaptableSim simulator subsystems
        //@{
            ///
            AdaptableSimRegisters * m_registers;

            ///
            AdaptableSimDataMemory * m_dataMemory;
        //@}
};

#endif // ADAPTABLESIMSTACK_H