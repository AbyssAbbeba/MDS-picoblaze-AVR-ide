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
 * @file AsmTranslator.h
 */
// =============================================================================

#ifndef ASMTRANSLATOR_H
#define ASMTRANSLATOR_H

// Forward declarations.
class AsmTranslatorBase;

// AsmTranslator header files.
#include "AsmTranslatorConfig.h"

// Standard header files.
#include <string>
#include <vector>
#include <utility>

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslator
 */
class AsmTranslator
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        enum Variant
        {
            V_KCPSM_XILINX,      ///<
            V_KCPSM_MEDIATRONIX, ///<
            V_KCPSM_OPENPICIDE,  ///<

            V_INVALID            ///<
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~AsmTranslator();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] variant
         * @param[in,out] output
         * @param[in,out] input
         */
        bool translate ( Variant variant,
                         std::ostream & output,
                         std::istream & input );

        /**
         * @brief
         * @param[in] variant
         * @param[in,out] output
         * @param[in,out] input
         * @param[in] makeBackup
         * @return
         */
        bool translate ( Variant variant,
                         const std::string & output,
                         const std::string & input,
                         bool makeBackup = false );

        /**
         * @brief
         * @return
         */
        const std::vector<std::pair<unsigned int, std::string> > & getMessages() const;

        /**
         * @brief
         * @return
         */
        unsigned int getLineNoCorrection() const;

        /**
         * @brief
         */
        void clear();

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] translator
         * @param[in,out] output
         * @param[in,out] input
         * @return
         */
        inline bool translate ( AsmTranslatorBase * translator,
                                std::ostream & output,
                                std::istream & input );

    ////    Public Attributes    ////
    public:
        /// @brief
        AsmTranslatorConfig m_config;

    ////    Private Attributes    ////
    private:
        /// @brief
        std::vector<std::pair<unsigned int, std::string> > m_messages;

        /// @brief
        unsigned int m_lineNoCorrection;
};

#endif // ASMTRANSLATOR_H
