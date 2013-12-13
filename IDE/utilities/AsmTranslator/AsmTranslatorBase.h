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
 * @file AsmTranslatorBase.h
 */
// =============================================================================

#ifndef ASMTRANSLATORBASE_H
#define ASMTRANSLATORBASE_H

// AsmTranslator header files.
#include "AsmTranslatorConfig.h"

// Standard header files.
#include <string>
#include <vector>
#include <ostream>
#include <utility>

// Used for i18n only.
#include <QObject>

/**
 * @brief
 * @ingroup AsmTranslator
 * @class AsmTranslatorBase
 */
class AsmTranslatorBase
{
    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct LineFields
        {
            friend std::ostream & operator << ( std::ostream & out,
                                                const LineFields & obj );

            /**
             * @brief
             * @param[in,out] line
             */
            LineFields ( std::string & line );

            /**
             * @brief
             * @return
             */
            std::string getLabel() const;

            /**
             * @brief
             * @return
             */
            std::string getInstruction() const;

            /**
             * @brief
             * @param[in] number
             * @return
             */
            std::string getOperand ( unsigned int number = 0 ) const;

            /**
             * @brief
             * @param[in] number
             * @return
             */
            bool hasOperand ( unsigned int number = 0 ) const;

            /**
             * @brief
             * @param[in] substitute
             */
            void replaceLabel ( const std::string & substitute );

            /**
             * @brief
             * @param[in] substitute
             */
            void replaceInst ( const std::string & substitute );

            /**
             * @brief
             * @param[in] substitute
             */
            void replaceInstOpr ( const std::string & substitute );

            /**
             * @brief
             * @param[in] substitute
             * @param[in] number
             */
            void replaceOpr ( const std::string & substitute,
                              unsigned int number = 0 );

            /// @brief
            std::string * m_line;

            /// @brief
            int m_label[2];

            /// @brief
            int m_instruction[2];

            /// @brief
            int m_comment[2];

            /// @brief
            std::vector<std::pair<int,int> > m_operands;
        };

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
                               bool secondPass = false ) = 0;

    ////    Public Attributes    ////
    public:
        /// @brief
        std::vector<std::string> m_prologue;

        /// @brief
        AsmTranslatorConfig * m_config;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] obj
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmTranslatorBase::LineFields & obj );
//@}

#endif // ASMTRANSLATORBASE_H
