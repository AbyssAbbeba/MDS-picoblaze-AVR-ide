// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3CodeListing.h
 */
// =============================================================================

#ifndef ASMKCPSM3CODELISTING_H
#define ASMKCPSM3CODELISTING_H

// Common compiler header files.
#include "../../CompilerSemanticInterface.h"
#include "../../CompilerOptions.h"
#include "../../CompilerMsgObserver.h"

// Standard headers.
#include <string>
#include <vector>
#include <ostream>
#include <utility>

/**
 * @brief
 * @ingroup Kcpsm3Asm
 * @class AsmKcpsm3CodeListing
 */
class AsmKcpsm3CodeListing : public CompilerMsgObserver
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmKcpsm3CodeListing * codeListing );

    ////    Public Static Constants    ////
    public:
        /// @brief
        static const unsigned int MAX_LINE_LENGTH = 1024;

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @return
         */
        AsmKcpsm3CodeListing ( CompilerSemanticInterface * compilerCore,
                               CompilerOptions * opts );

        /**
         * @brief
         */
        ~AsmKcpsm3CodeListing();

    ////    Public Datatypes    ////
    public:
        /**
         * @brief
         */
        struct LstLine
        {
            /**
             * @brief
             */
            LstLine();

            /**
             * @brief
             * @param[in] line
             */
            LstLine ( const char * line );

            ///
            int m_address;

            ///
            int m_macro;

            ///
            int m_inclusion;

            ///
            int m_value;

            ///
            int m_message;

            /// -1 == disable listing; 0 == neutral; 1 == (re)enable listing
            int m_noList;

            ///
            std::string m_line;

            ///
            std::vector<int> m_code;
        };

        /**
         * @brief
         */
        struct Message
        {
            /**
             * @brief
             */
            Message();

            /**
             * @brief
             * @param[in] type
             * @param[in] text
             */
            Message ( CompilerBase::MessageType type,
                      const std::string & text );

            ///
            CompilerBase::MessageType m_type;

            ///
            std::string m_text;
        };

    ////    Public Operations    ////
    public:
        /**
         * @brief
         */
        void loadSourceFiles();

        /**
         * @brief
         */
        void output();

        /**
         * @brief
         */
        void clear();

        /**
         * @brief
         * @param[in] location
         * @param[in] flag
         */
        void setNoList ( CompilerBase::SourceLocation location,
                         bool flag );

        /**
         * @brief
         * @param[in] title
         */
        void setTitle ( const std::string & title );

        /**
         * @brief
         * @param[in] location
         * @param[in] fileNumber
         */
        void setInclusion ( CompilerBase::SourceLocation location,
                            int fileNumber );

        /**
         * @brief
         * @param[in] location
         * @param[in] code
         * @param[in] address
         */
        void setCode ( CompilerBase::SourceLocation location,
                       int code,
                       int address );

        /**
         * @brief
         * @param[in] location
         * @param[in] value
         */
        void setValue ( CompilerBase::SourceLocation location,
                        int value );

        /**
         * @brief
         * @param[in] location
         * @param[in] definition
         * @param[in,out] expansion
         */
        void expandMacro ( CompilerBase::SourceLocation location,
                           const CompilerStatement * definition,
                           CompilerStatement * expansion );

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        virtual void message ( const CompilerBase::SourceLocation & location,
                               CompilerBase::MessageType type,
                               const std::string & text );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] out
         * @param[in,out] outputEnabled
         * @param[in,out] lineNumber
         * @param[in] fileNumber
         * @param[in] inclusionLevel
         * @param[in] macroLevel
         */
        void printCodeListing ( std::ostream & out,
                                bool & outputEnabled,
                                unsigned int & lineNumber,
                                unsigned int fileNumber = 0,
                                unsigned int inclusionLevel = 0,
                                unsigned int macroLevel = 0 ) const;

        /**
         * @brief
         * @param[in,out] lastLine
         * @param[in] definition
         */
        void copyMacroBody ( unsigned int * lastLine,
                             const CompilerStatement * definition );

        /**
         * @brief
         * @param[in,out] lineDiff
         * @param[in,out] macro
         */
        void rewriteMacroLoc ( unsigned int * lineDiff,
                               CompilerStatement * macro );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] location
         * @param[in] silent
         * @return
         */
        inline bool checkLocation ( const CompilerBase::SourceLocation & location,
                                    bool silent = false );

        /**
         * @brief
         * @return
         */
        inline void processMsgQueue();

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_numberOfFiles;

        ///
        unsigned int m_numberOfMacros;

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        std::string m_title;

        ///
        std::vector<std::vector<LstLine>> m_listing;

        ///
        std::vector<std::vector<Message>> m_messages;

        ///
        std::vector<std::pair<CompilerBase::SourceLocation,Message>> m_messageQueue;
};

/// @name Tracing operators
//@{
    /**
     * @brief
     * @param[in,out] out
     * @param[in] codeListing
     * @return
     */
    std::ostream & operator << ( std::ostream & out,
                                 const AsmKcpsm3CodeListing * codeListing );
//@}

#endif // ASMKCPSM3CODELISTING_H
