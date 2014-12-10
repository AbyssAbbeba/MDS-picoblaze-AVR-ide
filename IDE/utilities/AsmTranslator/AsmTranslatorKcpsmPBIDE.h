
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
 * @file AsmTranslatorKcpsmPBIDE.h
 */
// =============================================================================

#ifndef ASMTRANSLATORKCPSMPBIDE_U
#define ASMTRANSLATORKCPSMPBIDE_U

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
 * @class AsmTranslatorKcpsmPBIDE
 */
class AsmTranslatorKcpsmPBIDE : public AsmTranslatorBase
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmTranslatorKcpsmPBIDE();

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
        bool m_instFlag;
};

#endif // ASMTRANSLATORKCPSMPBIDE_U
