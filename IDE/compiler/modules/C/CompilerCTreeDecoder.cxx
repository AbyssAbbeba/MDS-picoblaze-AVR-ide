// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2015 Moravia Microsystems, s.r.o.
 *
 * @ingroup CompilerC
 * @file CompilerCTreeDecoder.cxx
 */
// =============================================================================

#include "CompilerCTreeDecoder.h"

// Standard headers.
#include<iostream>//DEBUG

// Compiler header files.
#include "CompilerOptions.h"
#include "CompilerStatement.h"
#include "CompilerSemanticInterface.h"

// C compiler header files.
#include "CompilerCSymbolTable.h"
#include "CompilerCDeclaration.h"

CompilerCTreeDecoder::CompilerCTreeDecoder ( CompilerSemanticInterface * compilerCore,
                                             CompilerOptions           * opts,
                                             CompilerCSymbolTable      * symbolTable,
                                             CompilerCExprProcessor    * exprProcessor )
                                           :
                                             m_compilerCore ( compilerCore ),
                                             m_opts ( opts ),
                                             m_symbolTable ( symbolTable ),
                                             m_exprProcessor ( exprProcessor )
{
}

CompilerCTreeDecoder::~CompilerCTreeDecoder()
{
}

void CompilerCTreeDecoder::processCodeTree ( CompilerStatement * codeTree )
{
    using namespace CompilerStatementTypes;

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case C_DECLARATION:
                processDeclaration(node->args());
                break;
            case C_FUNCTION_DEF:
                processFuncDef(node);
                break;
            default:
                unexpectedNode(node->location());
                return;
        }
    }
}

/*
(
    ( datatype
        (0x10000 datatype
            ( 's' pair
                (
                    (0x8 datatype) declaration
                        (pair 'n')
                ) | (
                    (0x80 datatype) declaration
                        (pair
                            (
                                ('d') [array]
                                    ( declaration )
                            )
                        )
                )
            )
        )
    ) declaration
)
*/

inline void CompilerCTreeDecoder::processDeclaration ( CompilerExpr * declaration )
{
    std::cout << "C_DECLARATION:" << declaration << '\n';
    std::cout << "left="<<declaration->lVal()<<'\n';
    std::cout << "op="<<declaration->oper()<<'\n';
    std::cout << "right="<<declaration->rVal()<<'\n'<<'\n';

    bool isTypedef;
    CompilerCDeclaration * absDeclar = resolveDeclaration(isTypedef, declaration);
//     if ( nullptr != absDeclar )
//     {
//         if ( true == isTypedef )
//         {
//             m_symbolTable->newDataType(absDeclar);
//         }
//         else
//         {
//             m_symbolTable->newSymbol(absDeclar);
//         }
//     }
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveDeclaration ( const CompilerExpr * exprTree )
{
    const CompilerSourceLocation & location = exprTree->location();
    const CompilerValue & lVal = exprTree->lVal();
    const CompilerValue & rVal = exprTree->rVal();

    switch ( exprTree->oper() )
    {
        case CompilerExpr::OPER_DATATYPE:
        {
            CompilerCDeclaration * a = resolveType(lVal, location);
            CompilerCDeclaration * b = resolveType(rVal, location);
            return a->combine(b);
        }

        case CompilerExpr::OPER_COMPOUND:
        {
            CompilerCDeclaration::Type type = (CompilerCDeclaration::Type) lVal.m_data.m_integer;

            if (
                   ( type != CompilerCDeclaration::DT_ENUM )
                       &&
                   ( type != CompilerCDeclaration::DT_UNION )
                       &&
                   ( type != CompilerCDeclaration::DT_STRUCT )
               )
            {
                unexpectedNode(location);
                return nullptr;
            }

            if ( CompilerValue::TYPE_EXPR != rVal.m_type )
            {
                unexpectedNode(location);
                return nullptr;
            }

            CompilerExpr * spec = rVal.m_data.m_expr;
            switch ( spec->oper() )
            {
                case CompilerExpr::OPER_NONE:
                case CompilerExpr::OPER_PAIR:
            }
        }

        case CompilerExpr::OPER_DECLARATION:

        default:
            unexpectedNode(location);
            return nullptr;
    }
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveType ( const CompilerValue & exprValue,
                                                           const CompilerSourceLocation & location )
{
    CompilerCDeclaration * result = nullptr;

    switch ( exprValue.m_type )
    {
        case CompilerValue::TYPE_INT:
            result = new CompilerCDeclaration(location);
            result->m_type = (CompilerCDeclaration::Type) exprValue.m_data.m_integer;
            break;

        case CompilerValue::TYPE_EXPR:
            result = resolveDeclaration(exprValue.m_data.m_expr);
            break;

        case CompilerValue::TYPE_SYMBOL:
            result = m_symbolTable->findDataType(exprValue.m_data.m_symbol)->copy(location);
            if ( nullptr == result )
            {
                m_compilerCore->semanticMessage ( location,
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "type %1 has not been declared in this scope" )
                                                              .arg(exprValue.m_data.m_symbol).toStdString() );
            }
            else if ( CompilerCDeclaration::A_TYPEDEF != result->m_type )
            {
                const char * keyword = "<!SYMBOL-TABLE-ERROR-INVALID-ENTRY!>";
                if ( CompilerCDeclaration::DT_ENUM & result->m_type )
                {
                    keyword = "enum";
                }
                else if ( CompilerCDeclaration::DT_UNION & result->m_type )
                {
                    keyword = "union";
                }
                else if ( CompilerCDeclaration::DT_STRUCT & result->m_type )
                {
                    keyword = "struct";
                }

                m_compilerCore->semanticMessage ( location,
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "%1 is not a typedef name, maybe you forgot the %2 "
                                                                "keyword" )
                                                              .arg(exprValue.m_data.m_symbol)
                                                              .arg(keyword)
                                                              .toStdString() );
            }
            break;

        default:
            unexpectedNode(location);
            break;
    }

    return result;
}

inline void CompilerCTreeDecoder::processFuncDef ( CompilerStatement * definition )
{
    std::cout << "C_FUNCTION_DEF:"<<definition->args()<<'\n';
}

inline void CompilerCTreeDecoder::processExpressions ( CompilerExpr * expr )
{
    std::cout << "processExpressions\n";
//     m_exprProcessor->something(...);
}

inline void CompilerCTreeDecoder::unexpectedNode ( const CompilerSourceLocation & location )
{
    m_compilerCore->semanticMessage ( location,
                                      CompilerBase::MT_ERROR,
                                      QObject::tr ( "internal error, please report this as a bug in the compiler "
                                                    "implementation" ).toStdString() );
}
