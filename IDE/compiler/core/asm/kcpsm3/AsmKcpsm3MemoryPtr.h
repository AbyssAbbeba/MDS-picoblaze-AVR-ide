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
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3MemoryPtr.h
 */
// =============================================================================

#ifndef ASMKCPSM3MEMORYPTR_H
#define ASMKCPSM3MEMORYPTR_H

// Forward declarations.
class CompilerSemanticInterface;

// Common compiler header files.
#include "../../CompilerBase.h"
#include "../../CompilerSourceLocation.h"

// Standard header files.
#include <vector>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3MemoryPtr
 */
class AsmKcpsm3MemoryPtr
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

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         */
        AsmKcpsm3MemoryPtr ( CompilerSemanticInterface * compilerCore );

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

#endif // ASMKCPSM3MEMORYPTR_H