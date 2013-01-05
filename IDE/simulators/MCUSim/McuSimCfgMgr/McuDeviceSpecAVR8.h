// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup McuSimCfgMgr
 * @file McuDeviceSpecAVR8.h
 */
// =============================================================================

#ifndef MCUDEVICESPECAVR8_H
#define MCUDEVICESPECAVR8_H

#include "McuDeviceSpec.h"
#include "MCUSim.h"
#include "AVR8PinNames.h"
#include "AVR8InsNames.h"
#include "AVR8InstructionSet.h"
#include "AVR8InterruptController.h"
#include "AVR8IO.h"
#include "AVR8Config.h"

#include <cstdint>
#include <string>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuDeviceSpecAVR8
 */
class McuDeviceSpecAVR8 : public McuDeviceSpec
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        McuDeviceSpecAVR8();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] mcuConfig
         * @return
         */
        bool setupSimulator ( AVR8Config & mcuConfig ) const;

    ////    Public Datatypes & Public Attributes    ////
    public:
        /**
         * @brief
         */
        struct Adc
        {
            bool m_enabled; ///<
        } m_adc;

        /**
         * @brief
         */
        struct AnalogComparator
        {
            bool m_enabled; ///<
        } m_analogComparator;

        /**
         * @brief
         */
        struct BootLoader
        {
            bool m_enabled;                ///<
            float m_minProgTime;           ///<
            float m_maxProgTime;           ///<
            unsigned int m_pageSize;       ///<
            int m_bootResetAddress [ 4 ];  ///<
            unsigned int m_rwwSectionSize; ///<
        } m_bootLoader;

        /**
         * @brief
         */
        struct ClockControl
        {
        } m_clockControl; ///<

        /**
         * @brief
         */
        struct DataEEPROM
        {
            bool m_enabled;              ///<
            unsigned int m_addrRegWidth; ///<
            unsigned int m_size;         ///<
            float m_writeTime;           ///<
        } m_dataEEPROM;

        /**
         * @brief
         */
        struct DataMemory
        {

            /**
             * @brief
             */
            DataMemory();

            /**
             * @brief
             */
            ~DataMemory();

            unsigned int m_regFileSize;     ///<
            unsigned int m_sramSize;        ///<
            unsigned int m_ioRegSize;       ///<

            uint32_t * m_ioRegInitValues;   ///<
            uint8_t * m_ioRegRandomInit;    ///<

            unsigned int m_mem2size;        ///<
            unsigned int * m_mem2sizes;     ///<

            uint32_t ** m_ioMem2InitValues; ///<

            unsigned int m_spWidth;         ///<

            /**
             * @brief
             */
            struct IORegDesc
            {
                std::string m_name;  ///<
                std::string m_desc;  ///<

                /**
                 * @brief
                 */
                struct Bit
                {
                    std::string m_name;  ///<
                    std::string m_ttip;  ///<
                    std::string m_stip;  ///<
                } m_bit[8];
            } * m_ioRegDesc;
        } m_dataMemory;

        /**
         * @brief
         */
        struct ExternalInterrupts
        {
            bool m_enabled;  ///<
        } m_externalInterrupts;

        /**
         * @brief
         */
        struct FusesAndLocks
        {
            unsigned char m_defaultFuses;    ///<
            unsigned char m_defaultLockBits; ///<
        } m_fusesAndLocks;

        /**
         * @brief
         */
        struct InstructionSet
        {
            bool m_availableInstructions [ AVR8InsNames::SPECI__MAX__ ]; ///<
            AVR8InstructionSet::PCWidth m_pcWidth;                       ///<
        } m_instructionSet;

        /**
         * @brief
         */
        struct InterruptController
        {
            bool m_possibleInterrupts [ AVR8InterruptController::INTVEC__MAX__ ]; ///<
        } m_interruptController;

        /**
         * @brief
         */
        struct IO
        {
            AVR8IO::RandValMode m_random;                                 ///<
            AVR8IO::SimFloatType m_pullUpresistance;                      ///<
            AVR8PinNames::PIN m_specFuncMap [ AVR8PinNames::SPF__MAX__ ]; ///<
            bool m_hasPort [ AVR8IO::NUMBER_OF_PORTS ];                   ///<
            bool m_availablePins [ AVR8IO::NUMBER_OF_PINS ];              ///<
        } m_io;

        /**
         * @brief
         */
        struct Isp
        {
            bool m_enabled; ///<
        } m_isp;

        /**
         * @brief
         */
        struct ParallelProg
        {
            bool m_enabled; ///<
        } m_parallelProg;

        /**
         * @brief
         */
        struct AVR8ProgramMemory
        {
            unsigned int m_size; ///<
        } m_programMemory;

        /**
         * @brief
         */
        struct Spi {
            bool m_enabled; ///<
        } m_spi;

        /**
         * @brief
         */
        struct SystemControl
        {
            float m_resetTresholdLow;            ///<
            float m_resetTresholdHigh;           ///<
            float m_minResetPulseWidth;          ///<

            float m_powerOnResetTresholdFalling; ///<
            float m_powerOnResetTresholdRaising; ///<

            float m_tBOD;                        ///<
            float m_brownOutTreshold[2];         ///<

            bool m_hasPowerOnReset;              ///<
            bool m_hasBrownOutReset;             ///<
            bool m_hasExternalReset;             ///<
            bool m_hasWatchDog;                  ///<
        } m_systemControl;

        /**
         * @brief
         */
        struct TimerCounter0
        {
            bool m_enabled; ///<
        } m_timerCounter0;

        /**
         * @brief
         */
        struct TimerCounter1
        {
            bool m_enabled; ///<
        } m_timerCounter1;

        /**
         * @brief
         */
        struct TimerCounter2
        {
            bool m_enabled; ///<
        } m_timerCounter2;

        /**
         * @brief
         */
        struct Twi
        {
            bool m_enabled; ///<
        } m_twi;

        /**
         * @brief
         */
        struct Usart
        {
            bool m_enabled; ///<
        } m_usart;

        /**
         * @brief
         */
        struct WatchdogTimer
        {
        } m_watchdogTimer;
};

#endif // MCUDEVICESPECAVR8_H
