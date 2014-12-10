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
#include <map>
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
            //
            friend std::ostream & operator << ( std::ostream & out,
                                                const LineFields & obj );

            /**
             * @brief
             * @param[in,out] line
             */
            LineFields ( std::string & line );

            /**
             * @brief
             * @param[in] keepCase
             * @return
             */
            std::string getLabel ( bool keepCase = false ) const;

            /**
             * @brief
             * @param[in] keepCase
             * @return
             */
            std::string getInstruction ( bool keepCase = false ) const;

            /**
             * @brief
             * @param[in] number
             * @param[in] keepCase
             * @return
             */
            std::string getOperand ( unsigned int number = 0,
                                     bool keepCase = false ) const;

            /**
             * @brief
             * @return
             */
            std::string getComment() const;

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

            /**
             * @brief
             * @param[in] substitute
             */
            void replaceAll ( const std::string & substitute );

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

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        virtual ~AsmTranslatorBase() {}

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
                               bool secondPass = false ) = 0;

        /**
         * @brief
         * @param[in,out] messages
         * @param[in,out] lineMap
         * @param[in,out] includedFiles
         * @return
         */
        void setup ( std::vector<std::pair<unsigned int, std::string> > * messages,
                     std::map<unsigned int, unsigned int> * lineMap,
                     std::vector<std::string> * includedFiles );

    ////    Protected Operations    ////
    protected:
        /**
         * @brief
         * @param[in] string
         * @param[in] letterCase
         * @return
         */
        std::string changeLetterCase ( const std::string & string,
                                       AsmTranslatorConfig::LetterCase letterCase ) const;

        /**
         * @brief
         * @param[in] line
         * @param[in] tabSizes
         * @param[in] useLabelCol
         * @return
         */
        std::string autoIndent ( const std::string * line,
                                 unsigned int tabSizes,
                                 bool useLabelCol = false ) const;

    ////    Public Attributes    ////
    public:
        /// @brief
        std::vector<std::string> m_prologue;

        /// @brief
        AsmTranslatorConfig * m_config;

    ////    Protected Attributes    ////
    public:
        /// @brief
        std::vector<std::pair<unsigned int, std::string> > * m_messages;

        /// @brief
        std::map<unsigned int, unsigned int> * m_lineMap;

        /// @brief
        std::vector<std::string> * m_includedFiles;
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
