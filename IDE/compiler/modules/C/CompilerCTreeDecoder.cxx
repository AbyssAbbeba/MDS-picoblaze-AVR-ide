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

    CompilerCDeclaration * absDeclar = resolveDeclaration(declaration);
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
    const CompilerValue & leftValue = exprTree->lVal();
    const CompilerValue & rightValue = exprTree->rVal();

    switch ( exprTree->oper() )
    {
        case CompilerExpr::OPER_DATATYPE:
        {
            CompilerCDeclaration * a = resolveType(leftValue, location);
            CompilerCDeclaration * b = resolveType(rightValue, location);
            return a->combine(b);
        }

        case CompilerExpr::OPER_COMPOUND:
            return resolveCompound(leftValue, rightValue, location);

        case CompilerExpr::OPER_DECLARATION:

        default:
            unexpectedNode(location);
            return nullptr;
    }
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveCompound ( const CompilerValue & leftValue,
                                                               const CompilerValue & rightValue,
                                                               const CompilerSourceLocation & location )
{
    if ( ( CompilerValue::TYPE_INT != leftValue.m_type ) || ( CompilerValue::TYPE_EXPR != rightValue.m_type ) )
    {
        unexpectedNode(location);
        return nullptr;
    }

    CompilerCDeclaration * result;
    CompilerExpr * spec = rightValue.m_data.m_expr;
    CompilerCDeclaration::Type type = (CompilerCDeclaration::Type) leftValue.m_data.m_integer;

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

    if ( CompilerValue::TYPE_SYMBOL != spec->lVal().m_type )
    {
        unexpectedNode(spec->location());
        return nullptr;
    }

    if ( CompilerExpr::OPER_PAIR == spec->oper() )
    {
        result = new CompilerCDeclaration(location);
        result->m_type = type;
        result->m_id = new std::string(spec->lVal().m_data.m_symbol);

        if ( CompilerValue::TYPE_EXPR != spec->rVal().m_type )
        {
            unexpectedNode(spec->location());
            return nullptr;
        }

        for ( CompilerExpr * expr = spec->rVal().m_data.m_expr;
              nullptr != expr;
              expr = expr->next() )
        {
            result->addMember(resolveDeclaration(expr));
        }

        return result;
    }
    else if ( CompilerExpr::OPER_NONE == spec->oper() )
    {
        result = m_symbolTable->findDataType(spec->lVal().m_data.m_symbol, type)->copy(location);

        if ( nullptr == result )
        {
            const char * keyword;
            if ( CompilerCDeclaration::DT_UNION == type )
            {
                keyword = "union";
            }
            else if ( CompilerCDeclaration::DT_STRUCT == type )
            {
                keyword = "struct";
            }
            else
            {
                keyword = "enum";
            }

            m_compilerCore->semanticMessage ( location,
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "%1 %2 has not been declared in this scope" )
                                                          .arg(keyword)
                                                          .arg(spec->lVal().m_data.m_symbol)
                                                          .toStdString() );
        }

        return result;
    }
    else
    {
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
            result = m_symbolTable->findDataType ( exprValue.m_data.m_symbol,
                                                   CompilerCDeclaration::A_TYPEDEF )->copy(location);

            if ( nullptr == result )
            {
                m_compilerCore->semanticMessage ( location,
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "typedef name %1 has not been declared in this scope" )
                                                              .arg(exprValue.m_data.m_symbol).toStdString() );
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
