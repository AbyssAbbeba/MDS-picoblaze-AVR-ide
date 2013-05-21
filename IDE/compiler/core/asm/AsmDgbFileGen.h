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
 * @ingroup Assembler
 * @file AsmDgbFileGen.h
 */
// =============================================================================

#ifndef ASMDGBFILEGEN_H
#define ASMDGBFILEGEN_H

// Common compiler header files.
#include "../CompilerSemanticInterface.h"
#include "../CompilerOptions.h"
#include "../FileHeaders.h"

// Standard headers.
#include <string>
#include <vector>
#include <ostream>

/**
 * @brief
 * @ingroup Assembler
 * @class AsmDgbFileGen
 */
class AsmDgbFileGen
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct DbgRecord
        {
            /**
             * @brief
             */
            DbgRecord() : m_code ( -1 ), m_address ( -1 ) {};

            /**
             * @brief
             * @param[in] location
             * @param[in] code
             * @param[in] address
             */
            DbgRecord ( const CompilerBase::SourceLocation & location,
                        int code,
                        int address )
                      : m_location ( location ),
                        m_code ( code ),
                        m_address ( address ) {};

            /// 
            CompilerBase::SourceLocation m_location;

            /// 
            int m_code;

            /// 
            int m_address;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmDgbFileGen();

        /**
         * @brief
         */
        ~AsmDgbFileGen();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] location
         * @param[in] code
         * @param[in] address
         */
        void setCode ( const CompilerBase::SourceLocation & location,
                       int code,
                       int address );

        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         */
        void output ( CompilerSemanticInterface * compilerCore,
                      const CompilerOptions * opts );

        /**
         * @brief
         */
        void clear();

    ////    Private Attributes    ////
    private:
        ///
        std::vector<DbgRecord> m_data;
};

#endif // ASMDGBFILEGEN_H
