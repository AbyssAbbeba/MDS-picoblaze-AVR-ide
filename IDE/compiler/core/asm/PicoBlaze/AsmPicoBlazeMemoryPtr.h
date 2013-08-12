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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeMemoryPtr.h
 */
// =============================================================================

#ifndef ASMPICOBLAZEMEMORYPTR_H
#define ASMPICOBLAZEMEMORYPTR_H

// Forward declarations.
class CompilerSemanticInterface;

// Common compiler header files.
#include "../../CompilerBase.h"
#include "../../CompilerSourceLocation.h"

// Standard header files.
#include <vector>

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeMemoryPtr
 */
class AsmPicoBlazeMemoryPtr
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum MemorySpace
        {
            MS_REG = 0,  ///<
            MS_DATA,     ///<
            MS_CODE,     ///<

            MS__MAX__    ///<
        };

    ////    Private Datatypes    ////
    private:
        /**
         * @brief
         */
        enum HardLimits
        {
            HARD_LIM_REG  = 31,
            HARD_LIM_DATA = 63,
            HARD_LIM_CODE = 1023
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         */
        AsmPicoBlazeMemoryPtr ( CompilerSemanticInterface * compilerCore );

    ////    Public Operations    ////
    public:
        /**
         * @brief Set all pointer values to zero.
         */
        void clear();

        /**
         * @brief
         * @param[in] location
         * @param[in] where
         * @param[in] address
         * @return
         */
        bool tryReserve ( const CompilerSourceLocation & location,
                          MemorySpace where,
                          unsigned int address );

        /**
         * @brief
         * @param[in] where
         * @param[in] address
         */
        void reserve ( MemorySpace where,
                       unsigned int address );

        /**
         * @brief
         * @param[in] where
         * @param[in] address
         * @return
         */
        bool isReserved ( MemorySpace where,
                          unsigned int address ) const;

    ////    Public Attributes    ////
    public:
        /// @brief Processor registers (REG).
        int m_reg;

        /// @brief Scratch Pad RAM (DATA).
        int m_data;

        /// @brief Program memory (CODE).
        int m_code;

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<unsigned int> m_reserved[MS__MAX__];

        /// @brief
        CompilerSemanticInterface * const m_compilerCore;
};

#endif // ASMPICOBLAZEMEMORYPTR_H
