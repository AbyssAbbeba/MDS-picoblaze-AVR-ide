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
 * @file AsmTranslatorConfig.h
 */
// =============================================================================

#ifndef ASMTRANSLATORCONFIG_H
#define ASMTRANSLATORCONFIG_H

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslatorConfig
 */
class AsmTranslatorConfig
{
    ////    Public Datatypes    ////
    public:
        ///
        enum LetterCase
        {
            LC_LOWERCASE, ///<
            LC_UPPERCASE  ///<
        };

        ///
        enum Field
        {
            F_SYMBOL,     ///<
            F_DIRECTIVE,  ///<
            F_INSTRUCTION ///<
        };

        ///
        enum Indentation
        {
            IND_KEEP,  ///<
            IND_TABS,  ///<
            IND_SPACES ///<
        };

        ///
        enum EndOfLine
        {
            EOF_LF,  ///<
            EOF_CR,  ///<
            EOF_CRLF ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        AsmTranslatorConfig();

        /**
         * @brief
         */
        virtual ~AsmTranslatorConfig() {}

    ////    Public Attributes    ////
    public:
        ///
        LetterCase m_letterCase [ F_INSTRUCTION + 1 ];

        ///
        EndOfLine m_eol;

        ///
        Indentation m_indentation;

        ///
        unsigned int m_tabSize;

        ///
        bool m_shortInstructions;
};

#endif // ASMTRANSLATORCONFIG_H
