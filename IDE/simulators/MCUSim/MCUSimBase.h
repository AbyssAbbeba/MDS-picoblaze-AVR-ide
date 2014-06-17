// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup MCUSim
 * @file MCUSimBase.h
 */
// =============================================================================

#ifndef MCUSIMBASE_H
#define MCUSIMBASE_H

/**
 * @brief
 * @ingroup MCUSim
 * @class MCUSimBase
 */
class MCUSimBase
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Arch
        {
            ARCH_INVALID = -1,  ///< Architecture is not specified, in such a case this thing will hardly work at all...

            /**
             * Advanced Virtual Risc (8-bit variant).
             * Modified Harvard architecture microcontroller, original design by Atmel Corporation.
             */
            ARCH_AVR8,

            /**
             * Advanced Virtual Risc (32-bit variant).
             * Modified Harvard architecture microcontroller, original design by Atmel Corporation.
             */
            ARCH_AVR32,

            /**
             * Peripheral Interface Controller (8-bit variant).
             * Modified Harvard architecture microcontroller, original design by Microchip Technology.
             */
            ARCH_PIC8,

            /**
             * PicoBlaze - (K)Constant Coded Programmable State Machine.
             * Harvard architecture soft processor core, original design by Xilinx, Inc.
             */
            ARCH_PICOBLAZE,

            /**
             * AdaptableSim - simulator for user-defined processors.
             * Harvard architecture processor core inspired by KCPSM.
             */
            ARCH_ADAPTABLE,

            ARCH__MAX__         ///<
        };

        /**
         * @brief
         */
        enum Family
        {
            FAMILY_INVALID = -1, ///< Architecture hasn't been specified.

            /// @name AVR8
            //@{
                FAMILY_MEGAAVR,      ///<
                FAMILY_TINYAVR,      ///<
                FAMILY_XMEGA,        ///<
            //@}

            /// @name AVR32
            //@{
                FAMILY_AVR32,        ///<
            //@}

            /// @name PIC8
            //@{
                FAMILY_PIC16F,       ///<
            //@}

            /// @name PicoBlaze
            //@{
                FAMILY_KCPSM1CPLD,   ///<
                FAMILY_KCPSM1,       ///<
                FAMILY_KCPSM2,       ///<
                FAMILY_KCPSM3,       ///<
                FAMILY_KCPSM6,       ///<
            //@}

            FAMILY__MAX__            ///<
        };

        /**
         * @brief
         */
        enum RetCode
        {
            RC_OK = 0,                  ///< Success.

            /// @name Warnings
            //@{
                RC_NONDETERMINISTIC,    ///< Random factor involved in the result (warning only).
            //@}

            /// @name Errors
            //@{
                RC_NOT_COMPATIBLE,      ///< Different architecture, etc.
                RC_ADDR_OUT_OF_RANGE,   ///< Address out of range (memory operations only).
                RC_VAL_OUT_OF_RANGE,    ///< Value out of range (memory operations only).
                RC_NOT_IMPLEMENTED,     ///< Not implemented on the simulated micro-controller/processor.
                RC_NOT_SUPPORTED,       ///< Implemented on the simulated micro-controller/processor but not in the simulator.
                RC_UNKNOWN_ERROR        ///< This should be never used, it's provided only for convenience.
            //@}
        };

        /**
         * @brief
         */
        enum ResetMode
        {
            RSTMD_NEW_CONFIG,     ///<
            RSTMD_INITIAL_VALUES, ///<
            RSTMD_MCU_RESET,      ///<

            RSTMD__MAX__          ///<
        };

        /**
         * @brief
         */
        enum Mode
        {
            MD_POWERED_OFF,     ///<
            MD_STOPPED,         ///<
            MD_SLEEP,           ///<
            MD_NORMAL           ///<
        };
};

#endif // MCUSIMBASE_H
