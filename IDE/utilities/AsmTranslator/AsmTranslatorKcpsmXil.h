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
 * @ingroup AsmTranslator
 * @file AsmTranslatorKcpsmXil.h
 */
// =============================================================================

#ifndef ASMTRANSLATORKCPSMXIL_H
#define ASMTRANSLATORKCPSMXIL_H

// AsmTranslator header files.
#include "AsmTranslatorBase.h"

// Standard header files.
#include <set>
#include <string>
#include <vector>

// Boost regular expressions library.
#include <boost/regex.hpp>

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslatorKcpsmXil
 */
class AsmTranslatorKcpsmXil : public AsmTranslatorBase
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmTranslatorKcpsmXil();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] line
         * @param[in] secondPass
         * @return
         */
        virtual bool process ( std::vector<std::string> & messages,
                               std::string & line,
                               bool secondPass = false );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] lineFields
         * @param[in] i
         */
        void fixRadix ( LineFields & lineFields,
                        int i );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] lineFields
         * @return
         */
        inline bool processInstructions ( std::vector<std::string> & messages,
                                          LineFields & lineFields );

        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] lineFields
         * @return
         */
        inline bool processDirectives ( std::vector<std::string> & messages,
                                        LineFields & lineFields );

        /**
         * @brief
         * @param[in] st
         * @return
         */
        inline bool ishex ( const std::string & str ) const;

    ////    Private Attributes    ////
    private:
        /// @brief
        boost::regex m_reWhiteSpace;

        /// @brief
        boost::regex m_reLabel;

        /// @brief
        boost::regex m_reInstruction;

        /// @brief
        boost::regex m_reWord;

        /// @brief
        boost::regex m_reOperand;

        /// @brief
        boost::regex m_reOperandSep;

        /// @brief
        boost::regex m_reComment;

        /// @brief
        boost::regex m_reAtMark;

        /// @brief
        boost::regex m_reAndReturn;

        /// @brief
        boost::regex m_reLdAndRet;

        /// @brief
        std::set<std::string> m_registers;

        /// @brief
        std::set<std::string> m_usedIDs;

        /// @brief
        bool m_instFlag;
};

#endif // ASMTRANSLATORKCPSMXIL_H
