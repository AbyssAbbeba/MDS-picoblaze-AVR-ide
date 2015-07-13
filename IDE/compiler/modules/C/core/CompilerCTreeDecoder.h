// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCTreeDecoder.h
 */
// =============================================================================

#ifndef COMPILERCTREEDECODER_H
#define COMPILERCTREEDECODER_H

// Forward declarations.
class CompilerExpr;
class CompilerOptions;
class CompilerCBackend;
class CompilerStatement;
class CompilerCSymbolTable;
class CompilerCDeclaration;
class CompilerCExprProcessor;
class CompilerSemanticInterface;

// Compiler header files.
#include "CompilerValue.h"
#include "CompilerSourceLocation.h"

// C compiler header files.
#include "CompilerCType.h"

// Standard header files.
#include <map>
#include <string>
#include <vector>
#include <cstdint>

/**
 * @brief
 * @ingroup CompilerC
 * @class CompilerCTreeDecoder
 */
class CompilerCTreeDecoder
{
    ////    Private Datatypes    ////
    private:
        struct TemporaryDeclarations
        {
            void enterScope();
            void leaveScope();

            std::vector<std::map<std::string,std::pair<CompilerSourceLocation,int>>> m_intConstants;
            std::vector<std::map<std::string,std::pair<CompilerSourceLocation,const CompilerCDeclaration*>>> m_typedefs;
        };

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         */
        CompilerCTreeDecoder ( CompilerSemanticInterface * compilerCore,
                               CompilerOptions           * opts,
                               CompilerCBackend          * backend,
                               CompilerCSymbolTable      * symbolTable,
                               CompilerCExprProcessor    * exprProcessor );

        /**
         * @brief
         */
        virtual ~CompilerCTreeDecoder();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in,out] codeTree
         */
        void processCodeTree ( CompilerStatement * codeTree );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] exprTree
         * @param[in] inParamList
         * @return
         */
        CompilerCDeclaration * resolveDeclaration ( const CompilerExpr * exprTree,
                                                    bool inParamList = false );

        /**
         * @brief
         * @param[in] exprValue
         * @param[in] location
         * @return
         */
        CompilerCDeclaration * resolveType ( const CompilerValue & exprValue,
                                             const CompilerSourceLocation & location );

        /**
         * @brief
         * @param[in] leftValue
         * @param[in] rightValue
         * @param[in] location
         * @return
         */
        CompilerCDeclaration * resolveCompound ( const CompilerValue & leftValue,
                                                 const CompilerValue & rightValue,
                                                 const CompilerSourceLocation & location );

        /**
         * @brief
         * @param[in] exprTree
         * @return
         */
        CompilerCDeclaration * resolveDeclSpecifiers ( const CompilerExpr * exprTree );

        /**
         * @brief
         * @param[in] exprTree
         * @return
         */
        CompilerCDeclaration * resolveDeclarator ( const CompilerExpr * exprTree );

        /**
         * @brief
         * @param[in] exprTree
         * @param[in] inArray
         * @param[in] inFunction
         * @return
         */
        CompilerCDeclaration * resolveDirectDeclarator ( const CompilerExpr * exprTree,
                                                         bool inArray = false,
                                                         bool inFunction = false );

        /**
         * @brief
         * @param[in,out] with
         * @param[in] location
         * @return
         */
        CompilerCDeclaration * combineDeclarations ( const CompilerCDeclaration * base,
                                                     CompilerCDeclaration * with,
                                                     const CompilerSourceLocation & location );

        int64_t resolveIntConstExpr ( const CompilerExpr * expr );
        int64_t resolveIntConstValue ( const CompilerValue & value,
                                       const CompilerSourceLocation & location );
        int64_t getIntConstant ( const std::string & id,
                                 const CompilerSourceLocation & location );

        CompilerCType::Type * processType ( CompilerCDeclaration * declaration );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] declExpr
         */
        inline void processDeclaration ( CompilerExpr * declExpr );

        /**
         * @brief
         * @param[in,out] codeTree
         */
        inline void processFuncDef ( CompilerStatement * codeTree );

        /**
         * @brief
         * @param[in,out] expr
         */
        inline void processExpressions ( CompilerExpr * expr );

        /**
         * @brief
         * @param[in] location
         */
        inline void unexpectedNode ( const CompilerSourceLocation & location );

        /**
         * @brief
         * @param[in] typedefDecl
         */
        inline void newTypedef ( const CompilerCDeclaration * typedefDecl );

    ////    Private Attributes    ////
    private:
        ///
        CompilerSemanticInterface * const m_compilerCore;

        ///
        CompilerOptions * const m_opts;

        ///
        CompilerCBackend * const m_backend;

        ///
        CompilerCSymbolTable * const m_symbolTable;

        ///
        CompilerCExprProcessor * const m_exprProcessor;

        ///
        TemporaryDeclarations m_tmpDeclarations;
};

#endif // COMPILERCTREEDECODER_H
