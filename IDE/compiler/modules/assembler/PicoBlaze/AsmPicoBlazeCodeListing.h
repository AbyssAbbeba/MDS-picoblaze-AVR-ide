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
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeCodeListing.h
 */
// =============================================================================

#ifndef ASMPICOBLAZECODELISTING_H
#define ASMPICOBLAZECODELISTING_H

// Forward declarations.
class AsmPicoBlazeSymbolTable;

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerMsgObserver.h"
#include "CompilerSemanticInterface.h"

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeCodeGenerator.h"

// Standard headers.
#include <set>
#include <string>
#include <vector>
#include <ostream>
#include <utility>

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeCodeListing
 */
class AsmPicoBlazeCodeListing : public CompilerMsgObserver
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmPicoBlazeCodeListing * codeListing );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @param[in,out] symbolTable
         * @return
         */
        AsmPicoBlazeCodeListing ( CompilerSemanticInterface * compilerCore,
                                  CompilerOptions * opts,
                                  AsmPicoBlazeSymbolTable * symbolTable );

        /**
         * @brief
         */
        virtual ~AsmPicoBlazeCodeListing();

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
            std::string m_line;

            ///
            std::vector<int> m_code;

            ///
            std::vector<int> m_macro;

            ///
            int m_address;

            ///
            int m_inclusion;

            ///
            int m_value;

            ///
            int m_message;

            /// -1 == disable listing; 0 == neutral; 1 == (re)enable listing
            int m_noList;
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
             * @param[in] subsequent
             */
            Message ( CompilerBase::MessageType type,
                      const std::string & text,
                      bool subsequent = false );

            ///
            CompilerBase::MessageType m_type;

            ///
            std::string m_text;

            ///
            bool m_subsequent;
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
        void setNoList ( CompilerSourceLocation location,
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
        void setInclusion ( CompilerSourceLocation location,
                            int fileNumber );

        /**
         * @brief
         * @param[in] location
         * @param[in] code
         * @param[in] address
         */
        void setCode ( CompilerSourceLocation location,
                       int code,
                       int address );

        /**
         * @brief
         * @param[in] location
         * @param[in] value
         */
        void setValue ( CompilerSourceLocation location,
                        int value );

        /**
         * @brief
         * @param[in] location
         * @param[in] definition
         * @param[in,out] expansion
         */
        void expandMacro ( CompilerSourceLocation location,
                           const CompilerStatement * definition,
                           CompilerStatement * expansion );

        /**
         * @brief
         * @param[in] location
         * @param[in,out] code
         * @param[in] first
         */
        void repeatCode ( CompilerSourceLocation location,
                          CompilerStatement * code,
                          bool first );

        /**
         * @brief
         * @param[in] location
         * @param[in,out] code
         */
        void generatedCode ( CompilerSourceLocation location,
                             CompilerStatement * code );

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         * @param[in] subsequent
         */
        virtual void message ( const CompilerSourceLocation & location,
                               CompilerBase::MessageType type,
                               const std::string & text,
                               bool subsequent = false ) override;

        /**
         * @brief
         * @param[in] limit
         */
        virtual void setMaxNumberOfMessages ( unsigned int limit ) override;

        /**
         * @brief
         */
        virtual void reset() override;

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
         * @param[in] origin
         */
        void rewriteMacroLoc ( unsigned int * lineDiff,
                               CompilerStatement * macro,
                               int origin );

        /**
         * @brief
         * @param[in,out] lineCounter
         * @param[in,out] code
         */
        void rewriteRepeatLoc ( unsigned int * lineCounter,
                                CompilerStatement * code );

        /**
         * @brief
         * @param[in] location
         * @param[in] silent
         * @return
         */
        bool checkLocation ( const CompilerSourceLocation & location,
                             bool silent = false );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @return
         */
        inline void processMsgQueue();

        /**
         * @brief
         * @param[in] location
         * @param[in] type
         * @param[in] text
         */
        inline void insertMessage ( const CompilerSourceLocation & location,
                                    CompilerBase::MessageType type,
                                    const std::string & text );

    ////    Private Attributes    ////
    private:
        ///
        unsigned int m_numberOfFiles;

        ///
        unsigned int m_numberOfMacros;

        ///
        unsigned int m_messageLimit;

        ///
        unsigned int m_msgCounter;

        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        AsmPicoBlazeSymbolTable * const m_symbolTable;

        ///
        AsmPicoBlazeCodeGenerator m_codeGenerator;

        ///
        std::string m_title;

        ///
        std::set<int> m_files2skip;

        ///
        std::vector<std::vector<LstLine>> m_listing;

        ///
        std::vector<std::vector<Message>> m_messages;

        ///
        std::vector<std::pair<CompilerSourceLocation,Message>> m_messageQueue;

        ///
        std::string m_lastMsgPrefix;

        ///
        bool m_lastMsgSubsequent;

        ///
        CompilerSourceLocation m_lastMsgLocation;
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
                                 const AsmPicoBlazeCodeListing * codeListing );
//@}

#endif // ASMPICOBLAZECODELISTING_H
