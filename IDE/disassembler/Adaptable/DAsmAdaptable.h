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
 * @file DAsmAdaptable.h
 */
// =============================================================================

#ifndef DASMADAPTABLE_H
#define DASMADAPTABLE_H

// Disassembler header files.
#include "../DAsmCore.h"

#include "../../utilities/AdjSimProcDef/AdjSimProcDef.h"

// Standard header files.
#include <set>
#include <map>
#include <string>
#include <vector>

/**
 * @brief
 * @ingroup Disassembler
 * @class DAsmAdaptable
 */
class DAsmAdaptable : public DAsmCore
{
    ////    Protected Datatypes    ////
    protected:
        /**
         * @brief
         */
        enum AddressSpace
        {
            CODE,   ///<
            DATA,   ///<
            REG,    ///<
            PORT,   ///<
            CONST,  ///<

            AS_MAX_ ///<
        };

        /**
         * @brief
         */
        enum InstructionSize
        {
            IS_16b, ///<
            IS_18b  ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        DAsmAdaptable ( const AdjSimProcDef & procDef ) : m_procDef ( procDef ) {}

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] source
         * @return
         */
        virtual bool disassemble ( const DataFile & source ) override;

        /**
         * @brief
         * @return
         */
        virtual const std::vector<std::string> & getSourceCode() const override;

        /**
         * @brief
         * @return
         */
        virtual const std::vector<std::string> & getMessages() const override;

        /**
         * @brief
         */
        virtual void clear() override;

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] code
         * @param[in] addr
         */
        bool phase1 ( unsigned int code,
                      unsigned int addr );

        /**
         * @brief
         * @param[in] code
         * @param[in] addr
         */
        void phase2 ( unsigned int code,
                      unsigned int addr );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] addr
         */
        void reg ( std::string & line,
                   unsigned int addr );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] addr
         */
        void port ( std::string & line,
                    unsigned int addr );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] addr
         */
        void data ( std::string & line,
                    unsigned int addr );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] addr
         */
        void imm ( std::string & line,
                   unsigned int addr );

        /**
         * @brief
         * @param[in,out] line
         * @param[in] addr
         * @param[in] definition
         */
        void label ( std::string & line,
                     unsigned int addr,
                     bool definition = false );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] code
         * @param[in] instruction
         * @param[out] operands
         */
        void getOperands ( unsigned int code,
                           const AdjSimProcDef::Instruction & instruction,
                           std::vector<int> & operands );

        /**
         * @brief
         * @param[in] code
         * @param[in] instruction
         * @return
         */
        bool recognizeInstruction ( unsigned int code,
                                    AdjSimProcDef::Instruction & instruction );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void phase1Leave();

        /**
         * @brief
         * @param[in,out] line
         * @param[in] addr
         * @param[in] addrSp
         * @param[in] stg
         */
        inline void appendAddr ( std::string & line,
                                 unsigned int addr,
                                 AddressSpace addrSp,
                                 Config::SymbolsToGenerate stg );

    ////    Protected Attributes    ////
    protected:
        ///
        std::vector<std::string> m_code;

        ///
        std::vector<std::string> m_messages;

        ///
        std::set<unsigned int> m_usedCode;

        ///
        std::set<unsigned int> m_addresses [ AS_MAX_ ];

        ///
        std::map<unsigned int,std::string> m_symbols [ AS_MAX_ ];

        ///
        unsigned int m_lastAddr;

        ///
        const AdjSimProcDef m_procDef;
};

#endif // DASMADAPTABLE_H
