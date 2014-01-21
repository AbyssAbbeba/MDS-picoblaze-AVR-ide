
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
         * @return
         */
        inline unsigned int indSz() const;

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
        bool m_instFlag;
};

#endif // ASMTRANSLATORKCPSMMED_H
