
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
 * @file AsmTranslatorKcpsmMed.h
 */
// =============================================================================

#ifndef ASMTRANSLATORKCPSMMED_H
#define ASMTRANSLATORKCPSMMED_H

// AsmTranslator header files.
#include "AsmTranslatorBase.h"

// Standard header files.
#include <set>
#include <string>
#include <vector>

// Boost regular expressions library.
#include "boost/regex.hpp"

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslatorKcpsmMed
 */
class AsmTranslatorKcpsmMed : public AsmTranslatorBase
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmTranslatorKcpsmMed();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] line
         * @param[in] lineNumber
         * @param[in] secondPass
         * @return
         */
        virtual bool process ( std::string & line,
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
         * @param[in,out] lineFields
         * @param[in] lineNumber
         * @return
         */
        inline bool processInstructions ( LineFields & lineFields,
                                          unsigned int lineNumber );

        /**
         * @brief
         * @param[in,out] lineFields
         * @param[in] lineNumber
         * @return
         */
        inline bool processDirectives ( LineFields & lineFields,
                                        unsigned int lineNumber );

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
         * @return
         */
        bool suffixHex ( const std::string & str ) const;

    ////    Public Attributes    ////
    public:
        /// @brief
        static const boost::regex m_reWhiteSpace;

        /// @brief
        static const boost::regex m_reLabel;

        /// @brief
        static const boost::regex m_reInstruction;

        /// @brief
        static const boost::regex m_reSymDef;

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
        static const boost::regex m_dollar;

        /// @brief
        static const boost::regex m_reAndReturn;

        /// @brief
        static const boost::regex m_reLdAndRet;

        /// @brief
        static const boost::regex m_reUNumber;

    ////    Private Attributes    ////
    private:
        /// @brief
        std::set<std::string> m_registers;

        /// @brief
        std::set<std::string> m_usedIDs;

        /// @brief
        std::map<std::string,std::string> m_idTranslationMap;

        /// @brief
        std::map<std::string,std::string> m_defaultSymbols;

        /// @brief
        bool m_instFlag;
};

#endif // ASMTRANSLATORKCPSMMED_H
