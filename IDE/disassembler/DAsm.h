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
 * @ingroup Disassembler
 * @file DAsm.h
 */
// =============================================================================

#ifndef DASM_H
#define DASM_H

// MCU memory data container library.
#include "../utilities/MCUDataFiles/DataFile.h"

// Standard header files.
#include <string>
#include <vector>
#include <ostream>

/**
 * @brief
 * @ingroup Disassembler
 * @class DAsm
 */
class DAsm
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct Config
        {
            ///
            Config();

            ///
            enum Indentation
            {
                IND_SPACES,
                IND_TABS
            } m_indentation;

            ///
            unsigned int m_tabSize;

            enum EndOfLine
            {
                EOF_LF,
                EOF_CR,
                EOF_CRLF
            } m_eof;

            ///
            enum SymbolsToGenerate
            {
                STG_NONE  = 0x00,
                STG_CODE  = 0x01,
                STG_DATA  = 0x02,
                STG_REG   = 0x04,
                STG_PORT  = 0x08,
                STG_CONST = 0x10,
            } m_symbolsToGenerate;

            ///
            enum LetterCase
            {
                LC_LOWERCASE,
                LC_UPPERCASE
            } m_letterCase;

            ///
            enum Radix
            {
                R_BIN,
                R_OCT,
                R_DEC,
                R_HEX
            } m_radix;
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] source
         * @return
         */
        virtual bool disassemble ( const DataFile & source ) = 0;

        /**
         * @brief
         * @return
         */
        virtual const std::vector<std::string> & getSourceCode() const = 0;

        /**
         * @brief
         * @return
         */
        virtual const std::vector<std::string> & getMessages() const = 0;

        /**
         * @brief
         */
        virtual void clear() = 0;

    ////    Public Attributes    ////
    public:
        ///
        Config m_config;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] dAsm
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const DAsm & dAsm );
//@}

#endif // DASM_H
