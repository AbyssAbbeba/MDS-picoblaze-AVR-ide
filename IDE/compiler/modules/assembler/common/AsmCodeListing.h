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
 * @ingroup Assembler
 * @file AsmCodeListing.h
 */
// =============================================================================

#ifndef ASMCODELISTING_H
#define ASMCODELISTING_H

// Forward declarations.
class AsmSymbolTable;

// Common compiler header files.
#include "CompilerOptions.h"
#include "CompilerMsgObserver.h"
#include "CompilerSemanticInterface.h"

//  assembler semantic analyzer header files.
#include "AsmCodeGenerator.h"

// Standard headers.
#include <set>
#include <map>
#include <string>
#include <vector>
#include <ostream>
#include <utility>

/**
 * @brief
 * @ingroup Assembler
 * @class AsmCodeListing
 */
class AsmCodeListing : public CompilerMsgObserver
{
    ////    Friends    ////
    friend std::ostream & operator << ( std::ostream & out,
                                        const AsmCodeListing * codeListing );

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] compilerCore
         * @param[in] opts
         * @param[in,out] symbolTable
         * @param[in,out] codeGenerator
         * @return
         */
        AsmCodeListing ( CompilerSemanticInterface * compilerCore,
                         const CompilerOptions * opts,
                         AsmSymbolTable * symbolTable,
                         AsmCodeGenerator * codeGenerator );

        /**
         * @brief
         */
        virtual ~AsmCodeListing();

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
         * @param[in] node
         * @param[in] fileNumber
         */
        void setInclusion ( CompilerStatement * node,
                            int fileNumber );

        /**
         * @brief
         * @param[in] location
         * @param[in] code
         * @param[in] address
         */
        void setCode ( const CompilerSourceLocation & location,
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
         * @param[in,out] expansion
         */
        void expandMacro ( const CompilerSourceLocation & location,
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
         * @param[in,out] macro
         * @param[in] origin
         * @param[in] lastLine
         */
        void copyMacroBody ( CompilerStatement * macro,
                             int origin,
                             int * lastLine = nullptr );

        /**
         * @brief
         * @param[in,out] codeTree
         * @param[in] fileNumber
         * @param[in] origin
         */
        void rewriteIncludeLoc ( CompilerStatement * codeTree,
                                 const int fileNumber,
                                 const int origin );

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

        /**
         * @brief
         * @param[in] location
         * @param[out] file
         * @param[out] line
         */
        inline bool translateLocation ( const CompilerSourceLocation & location,
                                        int & file,
                                        int & line);

        /**
         * @brief
         * @param[in] location
         * @param[in] silent
         * @return
         */
        inline bool checkLocation ( const CompilerSourceLocation & location,
                                    bool silent = false );

        /**
         * @brief
         * @param[in] file
         * @param[in] line
         * @param[in] silent
         * @return
         */
        inline bool checkLocation ( int file,
                                    int line,
                                    bool silent = false );

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
        const CompilerOptions * const m_opts;

        ///
        AsmSymbolTable * const m_symbolTable;

        ///
        AsmCodeGenerator * const m_codeGenerator;

        ///
        std::string m_title;

        ///
        std::set<int> m_files2skip;

        ///
        std::vector<std::vector<LstLine>> m_listing;

        ///
        std::vector<std::vector<Message>> m_messages;

        ///
        std::map<int,std::pair<int,int>> m_locationTransMap;

        ///
        std::vector<std::pair<CompilerSourceLocation,Message>> m_messageQueue;

        ///
        std::string m_lastMsgPrefix;
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
                                 const AsmCodeListing * codeListing );
//@}

#endif // ASMCODELISTING_H
