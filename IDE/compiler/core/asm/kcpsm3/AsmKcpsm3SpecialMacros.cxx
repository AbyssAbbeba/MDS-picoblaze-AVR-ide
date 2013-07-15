// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup Kcpsm3Asm
 * @file AsmKcpsm3SpecialMacros.cxx
 */
// =============================================================================

// KCPSM3 assembler semantic analyzer header files.
#include "AsmKcpsm3SpecialMacros.h"
#include "AsmKcpsm3SymbolTable.h"
#include "AsmKcpsm3CodeListing.h"

// Common compiler header files.
#include "../../CompilerExpr.h"
#include "../../CompilerStatementTypes.h"
#include "../../CompilerStatement.h"
#include "../../CompilerSemanticInterface.h"

// Standard headers.
#include <cstdio>
#include <cstdint>
#include <iostream> // DEBUG
AsmKcpsm3SpecialMacros::AsmKcpsm3SpecialMacros ( CompilerSemanticInterface * compilerCore,
                                                 AsmKcpsm3SymbolTable * symbolTable,
                                                 AsmKcpsm3CodeListing * codeListing )
                                               : m_compilerCore ( compilerCore ),
                                                 m_symbolTable ( symbolTable ),
                                                 m_codeListing ( codeListing )
{
    m_labelNextCounter = 0;
}

AsmKcpsm3SpecialMacros::~AsmKcpsm3SpecialMacros()
{
}

CompilerStatement * AsmKcpsm3SpecialMacros::runTimeCondition ( CompilerStatement * rtIfTree )
{
    using namespace CompilerStatementTypes;

    bool elseBlock = false;
    std::string labelNext;
    std::string labelEnd;
    CompilerStatement * block;
    CompilerStatement * node2delete;
    CompilerStatement * nodeNext;
    CompilerStatement * result = rtIfTree->branch();

    generateLabel(labelEnd, true);
    generateLabel(labelNext);

    rtIfTree->m_branch = NULL;

    for ( CompilerStatement * node = result;
          NULL != node;
          node = node->next() )
    {
        switch ( (int) node->type() )
        {
            case ASMKCPSM3_DIR_RTIF:
            {
                block = evaluateCondition ( node->args(), labelNext );
                block->appendLink ( node->branch() );
                m_codeListing->generatedCode(node->location(), block);

                node2delete = node;
                nodeNext = block->last();
                node->insertLink ( block );
                node = nodeNext;

                node2delete->m_branch = NULL;
                delete node2delete;
                break;
            }
            case ASMKCPSM3_DIR_RTELSEIF:
            {
                block = jump(labelEnd);
                block->appendLink ( label(labelNext) );
                block->appendLink ( evaluateCondition ( node->args(), generateLabel(labelNext) ) );
                block->appendLink ( node->branch() );
                m_codeListing->generatedCode(node->location(), block);

                node2delete = node;
                nodeNext = block->last();
                node->insertLink ( block );
                node = nodeNext;

                node2delete->m_branch = NULL;
                delete node2delete;
                break;
            }
            case ASMKCPSM3_DIR_RTELSE:
            {
                elseBlock = true;

                block = jump(labelEnd);
                block->appendLink(label(labelNext));
                block->appendLink ( node->branch() );
                m_codeListing->generatedCode(node->location(), block);

                node2delete = node;
                nodeNext = block->last();
                node->insertLink ( block );
                node = nodeNext;

                node2delete->m_branch = NULL;
                delete node2delete;
                break;
            }
            case ASMKCPSM3_DIR_RTENDIF:
            {
                if ( false == elseBlock )
                {
                    block = label(labelNext);
                }
                block->appendLink(label(labelEnd));
                m_codeListing->generatedCode(node->location(), block);
                node->insertLink(block);
                node = node->next();
                delete node->prev();
                break;
            }
        }
    }

    return result;
}

CompilerStatement * AsmKcpsm3SpecialMacros::evaluateCondition ( const CompilerExpr * cnd,
                                                                const std::string & label )
{
    using namespace CompilerStatementTypes;

    struct
    {
        bool m_reg;
        CompilerExpr * m_val;
    } cndVal[2];

    CompilerStatement * result = NULL;
    CompilerExpr * lblExpr = new CompilerExpr(label);

    const CompilerExpr * const lVal = cnd->lVal().m_data.m_expr;
    const CompilerExpr * const rVal = cnd->rVal().m_data.m_expr;

    int resultKnownInAdvance = 0;

    cndVal[0].m_val = new CompilerExpr ( (int) m_symbolTable->resolveExpr(lVal->lVal().m_data.m_expr, 8) );
    cndVal[1].m_val = new CompilerExpr ( (int) m_symbolTable->resolveExpr(rVal->lVal().m_data.m_expr, 8) );

    if ( CompilerExpr::OPER_HASH == lVal->oper() )
    {
        // <IMMEDIATE> <OPERATOR> <?>
        cndVal[0].m_reg = false;

        if ( CompilerExpr::OPER_HASH == rVal->oper() )
        {
            // <IMMEDIATE> <OPERATOR> <IMMEDIATE>
            cndVal[1].m_reg = false;
        }
        else
        {
            // <IMMEDIATE> <OPERATOR> <DIRECT>
            cndVal[1].m_reg = true;
        }
    }
    else
    {
        // <DIRECT> <OPERATOR> <?>
        cndVal[0].m_reg = true;

        if ( CompilerExpr::OPER_HASH == rVal->oper() )
        {
            // <DIRECT> <OPERATOR> <IMMEDIATE>
            cndVal[1].m_reg = false;
        }
        else
        {
            // <DIRECT> <OPERATOR> <DIRECT>
            cndVal[1].m_reg = true;
        }
    }

    CompilerSourceLocation emptyLocation;

    switch ( (int) cnd->oper() )
    {
        case CompilerExpr::OPER_EQ:
        case CompilerExpr::OPER_NE:
            if ( true == cndVal[0].m_reg )
            {
                if ( true == cndVal[1].m_reg )
                {
                    result = new CompilerStatement ( emptyLocation,
                                                     ASMKCPSM3_INS_COMPARE_SX_SY,
                                                     cndVal[0].m_val->appendLink(cndVal[1].m_val) );
                }
                else
                {
                    result = new CompilerStatement ( emptyLocation,
                                                     ASMKCPSM3_INS_COMPARE_SX_KK,
                                                     cndVal[0].m_val->appendLink(cndVal[1].m_val) );
                }
            }
            else
            {
                if ( true == cndVal[1].m_reg )
                {
                    result = new CompilerStatement ( emptyLocation,
                                                     ASMKCPSM3_INS_COMPARE_SX_KK,
                                                     cndVal[1].m_val->appendLink(cndVal[0].m_val) );
                }
                else
                {
                    if ( cndVal[0].m_reg == cndVal[1].m_reg )
                    {
                        resultKnownInAdvance = 1;
                        result = new CompilerStatement(emptyLocation, ASMKCPSM3_INS_JUMP_AAA, lblExpr);
                    }
                    else
                    {
                        resultKnownInAdvance = -1;
                    }
                }
            }

            if ( true == cndVal[0].m_reg || true == cndVal[1].m_reg )
            {
                StatementType statementType;
                if ( CompilerExpr::OPER_EQ == cnd->oper() )
                {
                    statementType = ASMKCPSM3_INS_JUMP_NZ_AAA;
                }
                else
                {
                    statementType = ASMKCPSM3_INS_JUMP_Z_AAA;
                }
                result->appendLink ( new CompilerStatement(emptyLocation, statementType, lblExpr) );
            }

            break;

        case CompilerExpr::OPER_LT:
//             COMPARE
//             JUMP C
        case CompilerExpr::OPER_LE:
//             COMPARE
//             JUMP C
//             JUMP Z
        case CompilerExpr::OPER_GE:
//             COMPARE
//             JUMP NC
        case CompilerExpr::OPER_GT:
//             COMPARE
//             JUMP NC && NZ
        case CompilerExpr::OPER_BAND:

//             ASMKCPSM3_INS_TEST_SX_KK
//             ASMKCPSM3_INS_TEST_SX_SY


//             TEST
//             JUMP NZ
        case CompilerExpr::OPER_NAND:
//             TEST
//             JUMP Z
            break;
    }

    if ( 1 == resultKnownInAdvance )
    {
        m_compilerCore->compilerMessage ( cnd->location(),
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "comparing two immediate constants, result is always positive" )
                                                      . toStdString() );
    }
    else if ( -1 == resultKnownInAdvance )
    {
        m_compilerCore->compilerMessage ( cnd->location(),
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "comparing two immediate constants, result is always negative" )
                                                      . toStdString() );
    }

    if ( NULL == result )
    {
        return new CompilerStatement();
    }
    else
    {
        return result;
    }
}

inline CompilerStatement * AsmKcpsm3SpecialMacros::jump ( const std::string & label )
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMKCPSM3_INS_JUMP_AAA,
                                   new CompilerExpr(label) );
}

inline CompilerStatement * AsmKcpsm3SpecialMacros::label ( const std::string & label )
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMKCPSM3_LABEL,
                                   new CompilerExpr(label) );
}

inline const std::string & AsmKcpsm3SpecialMacros::generateLabel ( std::string & label,
                                                                   bool end )
{
    char buf[5];
    sprintf(buf, "%d", m_labelNextCounter);

    label.clear();
    label = "IF-";
    label += buf;

    if ( true == end )
    {
        label += "-END";
    }
    else
    {
        m_labelNextCounter++;
    }

    return label;
}
