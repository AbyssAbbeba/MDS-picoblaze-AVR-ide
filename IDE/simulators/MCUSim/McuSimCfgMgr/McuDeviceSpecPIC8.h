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
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpecPIC8.h
 */
// =============================================================================

#ifndef MCUDEVICESPECPIC8_H
#define MCUDEVICESPECPIC8_H

#include "McuDeviceSpec.h"

#include "MCUSim.h"
#include "PIC8/PIC8Config.h"
#include "PIC8/PIC8ISP.h"
#include "PIC8/PIC8IO.h"
#include "PIC8/PIC8PinNames.h"
#include "PIC8/PIC8InterruptController.h"

#include <cstdint>
#include <string>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuDeviceSpecPIC8
 */
class McuDeviceSpecPIC8 : public McuDeviceSpec
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        McuDeviceSpecPIC8();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] mcuConfig
         * @return
         */
        bool setupSimulator ( PIC8Config & mcuConfig ) const;

    ////    Public Datatypes & Public Attributes    ////
    public:
        /**
         * @brief
         */
        struct InstructionSet
        {
        } m_instructionSet;

        /**
         * @brief
         */
        struct ProgramMemory
        {
            ///
            unsigned int m_size;

            ///
            unsigned int m_configWordAddress;

            ///
            unsigned int m_idLocationsRange [ 2 ];
        } m_programMemory;

        /**
         * @brief
         */
        struct DataMemory
        {
            DataMemory();
            ~DataMemory();

            unsigned int m_size;            ///<
            unsigned int * m_randomInit;    ///<
            int * m_addrTransTab;           ///<
            uint32_t * m_initValues;        ///<

            /**
             * @brief
             */
            struct RegDescription
            {
                RegDescription()
                {
                    m_numberOfAddresses = 0;
                    m_addresses = NULL;
                }

                std::string m_name;  ///<
                std::string m_desc;  ///<

                unsigned int m_numberOfAddresses;
                unsigned int * m_addresses;

                /**
                 * @brief
                 */
                struct Bit
                {
                    std::string m_name;  ///<
                    std::string m_ttip;  ///<
                    std::string m_stip;  ///<
                } m_bit[8];
            } * m_regDescription;
        } m_dataMemory;

        /**
         * @brief
         */
        struct ConfigWord
        {
            ///
            uint8_t m_defaultCfgWord;
        } m_configWord;

        /**
         * @brief
         */
        struct IO
        {
            ///
            PIC8PinNames::PIN m_specFuncMap [ PIC8PinNames::SPF__MAX__ ];

            ///
            bool m_hasPort [ PIC8IO::NUMBER_OF_PORTS ];

            ///
            bool m_availablePins [ PIC8IO::NUMBER_OF_PINS ];

            ///
            PIC8IO::SimFloatType m_pullUpresistance;

            ///
            bool m_enabled;
        } m_io;

        /**
         * @brief
         */
        struct ClockControl
        {
        } m_clockControl;

        /**
         * @brief
         */
        struct Stack
        {
            ///
            unsigned int m_size;
        } m_stack;

        /**
         * @brief
         */
        struct InterruptController
        {
            ///
            bool m_possibleInterrupts [ PIC8InterruptController::INTVEC__MAX__ ];
        } m_interruptController;

        /**
         * @brief
         */
        struct DataEEPROM
        {
            unsigned int m_size;         ///<
            float m_writeTime;           ///< (in seconds)
        } m_dataEEPROM;

        /**
         * @brief
         */
        struct WatchDogTimer
        {
            ///
            bool m_enabled;
        } m_watchDogTimer;

        /**
         * @brief
         */
        struct TimerCounter0
        {
            ///
            bool m_enabled;
        } m_timerCounter0;

        /**
         * @brief
         */
        struct Timer0WdtPrescaller
        {
            ///
            bool m_enabled;
        } m_timer0WdtPrescaller;

        /**
         * @brief
         */
        struct ExternalInterrupts
        {
            ///
            bool m_enabled;
        } m_externalInterrupts;

        /**
         * @brief
         */
        struct ISP
        {
            ///
            bool m_enabled;

            ///
            float m_delayBetweenCmdAndData;

            ///
            float m_progTime;

            ///
            PIC8ISP::CommandSet m_commandSet;
        } m_isp;
};

#endif // MCUDEVICESPECPIC8_H
