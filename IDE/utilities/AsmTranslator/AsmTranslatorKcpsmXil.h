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
#include <map>
#include <string>
#include <vector>

// Boost regular expressions library.
#include "boost/regex.hpp"

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
         * @param[in] lineNumber
         * @param[in] secondPass
         * @return
         */
        virtual bool process ( std::vector<std::pair<unsigned int, std::string> > & messages,
                               std::string & line,
                               unsigned int lineNumber,
                               bool secondPass = false ) override;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] lineFields
         * @param[in] i
         */
        void fixRadix ( LineFields & lineFields,
                        int i );

        /**
         * @brief
         * @param[in] id
         * @return
         */
        std::string newIdentifier ( const std::string & id );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] lineFields
         * @param[in] lineNumber
         * @return
         */
        inline bool processInstructions ( std::vector<std::pair<unsigned int, std::string> > & messages,
                                          LineFields & lineFields,
                                          unsigned int lineNumber );

        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] lineFields
         * @param[in] lineNumber
         * @return
         */
        inline bool processDirectives ( std::vector<std::pair<unsigned int, std::string> > & messages,
                                        LineFields & lineFields,
                                        unsigned int lineNumber );

        /**
         * @brief
         * @param[in] st
         * @return
         */
        inline bool ishex ( const std::string & str ) const;

        /**
         * @brief
         * @param[in,out] lineFields
         */
        inline void translateIdentifiers ( AsmTranslatorBase::LineFields & lineFields );

        /**
         * @brief
         * @return
         */
        inline unsigned int indSz() const;

        /**
         * @brief
         * @param[in,out] id
         */
        inline void removeTrailingDollar ( std::string & id );

    ////    Public Attributes    ////
    public:
        /// @brief
        static const boost::regex m_reWhiteSpace;

        /// @brief
        static const boost::regex m_reLabel;

        /// @brief
        static const boost::regex m_reInstruction;

        /// @brief
        static const boost::regex m_reWord;

        /// @brief
        static const boost::regex m_reOperand;

        /// @brief
        static const boost::regex m_reOperandSep;

        /// @brief
        static const boost::regex m_reComment;

        /// @brief
        static const boost::regex m_reAtMark;

        /// @brief
        static const boost::regex m_reAndReturn;

        /// @brief
        static const boost::regex m_reLdAndRet;

    ////    Private Attributes    ////
    private:
        /// @brief
        std::set<std::string> m_registers;

        /// @brief
        std::set<std::string> m_usedIDs;

        /// @brief
        std::set<std::string> m_strings;

        /// @brief
        std::map<std::string,std::string> m_idTranslationMap;

        /// @brief
        std::map<std::string,std::string> m_defaultSymbols;

        /// @brief
        bool m_instFlag;
};

#endif // ASMTRANSLATORKCPSMXIL_H
