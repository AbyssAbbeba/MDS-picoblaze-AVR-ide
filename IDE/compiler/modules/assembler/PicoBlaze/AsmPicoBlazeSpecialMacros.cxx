// =============================================================================
/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeSpecialMacros.cxx
 */
// =============================================================================

// PicoBlaze assembler semantic analyzer header files.
#include "AsmPicoBlazeSpecialMacros.h"
#include "AsmPicoBlazeCodeListing.h"

// Common compiler header files.
#include "CompilerExpr.h"
#include "CompilerStatement.h"
#include "CompilerStatementTypes.h"
#include "CompilerSourceLocation.h"
#include "CompilerSemanticInterface.h"

// Standard headers.
#include <cstdio>
#include <cstdint>

AsmPicoBlazeSpecialMacros::AsmPicoBlazeSpecialMacros ( CompilerSemanticInterface * compilerCore,
                                                       AsmPicoBlazeSymbolTable      * symbolTable,
                                                       AsmPicoBlazeCodeListing      * codeListing )
                                                     : m_compilerCore ( compilerCore ),
                                                       m_symbolTable  ( symbolTable  ),
                                                       m_codeListing  ( codeListing  )
{
    m_labelNextCounter [ (int) LT_IF    ] = 0;
    m_labelNextCounter [ (int) LT_FOR   ] = 0;
    m_labelNextCounter [ (int) LT_WHILE ] = 0;
}

AsmPicoBlazeSpecialMacros::~AsmPicoBlazeSpecialMacros()
{
}

CompilerStatement * AsmPicoBlazeSpecialMacros::runTimeWhile ( CompilerStatement * rtWhile )
{
    CompilerStatement * result;
    CompilerStatement * resultEnd;
    std::string labelBreak;
    std::string labelContinue;

    generateLabel(labelBreak, LT_WHILE, true);
    generateLabel(labelContinue, LT_WHILE);

    result = label(labelContinue);
    result->appendLink ( evaluateCondition(rtWhile->args(), labelBreak) );
    m_codeListing->generatedCode(rtWhile->location(), result);

    resultEnd = jump(labelContinue);
    resultEnd->appendLink ( label(labelBreak) );
    m_codeListing->generatedCode(rtWhile->branch()->last()->location(), resultEnd);

    // Remove `RT_ENDW' directive.
    delete rtWhile->branch()->last();

    result->appendLink(rtWhile->branch());
    result->appendLink(resultEnd);
    rtWhile->m_branch = NULL;

    return result;
}

void AsmPicoBlazeSpecialMacros::runTimeForLeave()
{
    if ( false == m_forLoopIterRegs.empty() )
    {
        m_forLoopIterRegs.pop_back();
    }
}

CompilerStatement * AsmPicoBlazeSpecialMacros::runTimeFor ( CompilerStatement * rtFor )
{
    CompilerStatement * result;
    CompilerStatement * resultEnd;
    std::string labelBreak;
    std::string labelContinue;

    generateLabel(labelBreak, LT_FOR, true);
    generateLabel(labelContinue, LT_FOR);

    CompilerExpr * args[4];
    args[0] = rtFor->args();
    for ( int i = 0; i < 3; i++ )
    {
        args [ i + 1 ] = args[i]->next();
    }

    int reg = (int) m_symbolTable->resolveExpr(args[0]);
    int start = 0;
    int end;
    int by = 1;

    bool regInUse = false;
    for ( std::vector<int>::const_iterator it = m_forLoopIterRegs.cbegin();
          it != m_forLoopIterRegs.cend();
          it++ )
    {
        if ( *it == reg )
        {
            regInUse = true;
            break;
        }
    }
    if ( true == regInUse )
    {
        m_compilerCore->semanticMessage ( args[0]->location(),
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "reuse of iterator register in nested for loop (the two loops "
                                                        "will affect each other via their iterator registers)" )
                                                      .toStdString(),
                                          true );
    }
    m_forLoopIterRegs.push_back(reg);

    checkType ( true, m_symbolTable->getType(args[0]), args[0]->location() );
    for ( int i = 1; i < 4; i++ )
    {
        if ( NULL == args[i] )
        {
            break;
        }
        checkType ( false, m_symbolTable->getType(args[i]), args[i]->location() );
    }

    if ( NULL == args[2] )
    {
        // RT_FOR <REG>, <NUMBER>
        end = (int) m_symbolTable->resolveExpr(args[1], 8);
    }
    else if ( NULL == args[3] )
    {
        // RT_FOR <REG>, <START> .. <END>
        start = (int) m_symbolTable->resolveExpr(args[1], 8);
        end   = (int) m_symbolTable->resolveExpr(args[2], 8);
    }
    else
    {
        // RT_FOR <REG>, <START> .. <END>, <BY>
        start = (int) m_symbolTable->resolveExpr(args[1], 8);
        end   = (int) m_symbolTable->resolveExpr(args[2], 8);
        by    = (int) m_symbolTable->resolveExpr(args[3], 9);

        // `by' belongs to interval [-256;255]
        if ( by > 0xff )
        {
            by = ( 0x200 - by );
        }
    }

    /*
     * Generate the loop's code.
     */
    // reg = start
    result = new CompilerStatement ( CompilerSourceLocation(),
                                     CompilerStatementTypes::ASMPICOBLAZE_INS_LOAD_SX_KK,
                                     (new CompilerExpr(reg))->appendLink(new CompilerExpr(start)) );
    // continue:
    result->appendLink ( label(labelContinue) );
    // if ( reg == end ) { jump break }
    result->appendLink ( compare_sx_kk(reg, end) );
    result->appendLink ( jump(labelBreak, JC_Z) );
    m_codeListing->generatedCode(rtFor->location(), result);

    // increment / decrement the iterator register
    if ( by > 0 )
    {
        resultEnd = new CompilerStatement ( CompilerSourceLocation(),
                                            CompilerStatementTypes::ASMPICOBLAZE_INS_ADD_SX_KK,
                                            (new CompilerExpr(reg))->appendLink(new CompilerExpr(by)) );
    }
    else if ( by < 0 )
    {
        resultEnd = new CompilerStatement ( CompilerSourceLocation(),
                                            CompilerStatementTypes::ASMPICOBLAZE_INS_SUB_SX_KK,
                                            (new CompilerExpr(reg))->appendLink(new CompilerExpr(-by)) );
    }
    else
    {
        resultEnd = new CompilerStatement();
    }
    // jump continue
    resultEnd->appendLink ( jump(labelContinue) );
    // break:
    resultEnd->appendLink ( label(labelBreak) );
    m_codeListing->generatedCode(rtFor->branch()->last()->location(), resultEnd);

    // Remove `RT_ENDF' directive.
    delete rtFor->branch()->last();

    result->appendLink(rtFor->branch());
    result->appendLink(resultEnd);
    rtFor->m_branch = NULL;

    return result;
}

CompilerStatement * AsmPicoBlazeSpecialMacros::runTimeCondition ( CompilerStatement * rtIfTree )
{
    using namespace CompilerStatementTypes;

    bool elseBlock = false;
    std::string labelEnd;
    std::string labelNext;
    CompilerStatement * block;
    CompilerStatement * nodeNext;
    CompilerStatement * node2delete;
    CompilerStatement * result = NULL;

    generateLabel(labelEnd, LT_IF, true);
    generateLabel(labelNext, LT_IF);

    for ( CompilerStatement * node = rtIfTree->branch();
          NULL != node;
          node = node->next() )
    {
        block = NULL;
        switch ( (int) node->type() )
        {
            case ASMPICOBLAZE_DIR_RTIF:
            {
                block = evaluateCondition ( node->args(), labelNext );
                m_codeListing->generatedCode(node->location(), block);
                block->appendLink ( node->branch() );
                break;
            }
            case ASMPICOBLAZE_DIR_RTELSEIF:
            {
                block = jump(labelEnd);
                block->appendLink ( label(labelNext) );
                block->appendLink ( evaluateCondition ( node->args(), generateLabel(labelNext, LT_IF) ) );
                m_codeListing->generatedCode(node->location(), block);
                block->appendLink ( node->branch() );
                break;
            }
            case ASMPICOBLAZE_DIR_RTELSE:
            {
                elseBlock = true;

                block = jump(labelEnd);
                block->appendLink(label(labelNext));
                m_codeListing->generatedCode(node->location(), block);
                block->appendLink ( node->branch() );
                break;
            }
            case ASMPICOBLAZE_DIR_RTENDIF:
            {
                block = label(labelEnd);
                if ( false == elseBlock )
                {
                    block->appendLink(label(labelNext));
                }
                m_codeListing->generatedCode(node->location(), block);
                break;
            }
        }

        if ( NULL == result )
        {
            result = block;
        }

        node2delete = node;
        nodeNext = block->last();
        node->insertLink ( block );
        node = nodeNext;

        node2delete->m_branch = NULL;
        delete node2delete;
    }

    rtIfTree->m_branch = NULL;
    return result;
}

CompilerStatement * AsmPicoBlazeSpecialMacros::evaluateCondition ( const CompilerExpr * cnd,
                                                                   const std::string & label )
{
    struct
    {
        AsmPicoBlazeSymbolTable::SymbolType m_type;
        int m_val;
        bool m_reg;
    } cndVal[2];

    int resultKnownInAdvance = 0;

    CompilerStatement * result = NULL;

    const CompilerExpr * const lVal = cnd->lVal().m_data.m_expr;
    const CompilerExpr * const rVal = cnd->rVal().m_data.m_expr;

    cndVal[0].m_type = m_symbolTable->getType(lVal->lVal().m_data.m_expr);
    cndVal[1].m_type = m_symbolTable->getType(rVal->lVal().m_data.m_expr);
    cndVal[0].m_val  = (int) m_symbolTable->resolveExpr(lVal->lVal().m_data.m_expr, 8);
    cndVal[1].m_val  = (int) m_symbolTable->resolveExpr(rVal->lVal().m_data.m_expr, 8);

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

    {
        const CompilerExpr * e = lVal;
        for ( int  i = 0; i < 2; i++ )
        {
            checkType ( cndVal[i].m_reg, cndVal[i].m_type, e->location() );
            e = rVal;
        }
    }

    switch ( (int) cnd->oper() )
    {
        case CompilerExpr::OPER_EQ:
        case CompilerExpr::OPER_NE:
            if ( true == cndVal[0].m_reg )
            {
                // <DIRECT> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <DIRECT> <OPERATOR> <DIRECT>
                    if ( cndVal[0].m_val == cndVal[1].m_val )
                    {
                        if ( CompilerExpr::OPER_EQ == cnd->oper() )
                        {
                            resultKnownInAdvance = 2;
                            result = new CompilerStatement();
                        }
                        else
                        {
                            resultKnownInAdvance = -2;
                            result = jump(label);
                        }

                        // Do not append the label, break right here.
                        break;
                    }
                    else
                    {
                        result = compare_sx_sy(cndVal[0].m_val, cndVal[1].m_val);
                    }
                }
                else
                {
                    // <DIRECT> <OPERATOR> <IMMEDIATE>
                    result = compare_sx_kk(cndVal[0].m_val, cndVal[1].m_val);
                }
            }
            else
            {
                // <IMMEDIATE> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <IMMEDIATE> <OPERATOR> <DIRECT>
                    result = compare_sx_kk(cndVal[1].m_val, cndVal[0].m_val);
                }
                else
                {
                    // <IMMEDIATE> <OPERATOR> <IMMEDIATE>
                    if (
                           ( ( cndVal[0].m_val == cndVal[1].m_val ) && ( CompilerExpr::OPER_EQ == cnd->oper() ) )
                               ||
                           ( ( cndVal[0].m_val != cndVal[1].m_val ) && ( CompilerExpr::OPER_NE == cnd->oper() ) )
                       )
                    {
                        resultKnownInAdvance = 1;
                        result = new CompilerStatement();
                    }
                    else
                    {
                        resultKnownInAdvance = -1;
                        result = jump(label);
                    }

                    // Do not append the label, break right here.
                    break;
                }
            }

            // Append conditional jump after the comparison instruction.
            if ( CompilerExpr::OPER_EQ == cnd->oper() )
            {
                result->appendLink(jump(label, JC_NZ));
            }
            else
            {
                result->appendLink(jump(label, JC_Z));
            }
            break;

        case CompilerExpr::OPER_LE:
        {
            // Swap cndVal [0] <--> [1].

            bool auxFlag = cndVal[0].m_reg;
            int auxVal   = cndVal[0].m_val;

            cndVal[0].m_reg = cndVal[1].m_reg;
            cndVal[0].m_val = cndVal[1].m_val;

            cndVal[1].m_reg = auxFlag;
            cndVal[1].m_val = auxVal;
        }
        case CompilerExpr::OPER_GE:
            if ( true == cndVal[0].m_reg )
            {
                // <DIRECT> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <DIRECT> <OPERATOR> <DIRECT>
                    if ( cndVal[0].m_val == cndVal[1].m_val )
                    {
                        if ( CompilerExpr::OPER_GE == cnd->oper() )
                        {
                            resultKnownInAdvance = 2;
                            result = new CompilerStatement();
                        }
                        else
                        {
                            resultKnownInAdvance = -2;
                            result = jump(label);
                        }

                        // Do not append the label, break right here.
                        break;
                    }
                    else
                    {
                        result = compare_sx_sy(cndVal[0].m_val, cndVal[1].m_val);
                    }
                }
                else
                {
                    // <DIRECT> <OPERATOR> <IMMEDIATE>
                    result = compare_sx_kk(cndVal[0].m_val, cndVal[1].m_val);
                }

                result->appendLink(jump(label, JC_C));
            }
            else
            {
                // <IMMEDIATE> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <IMMEDIATE> <OPERATOR> <DIRECT>
                    result = compare_sx_kk(cndVal[1].m_val, cndVal[0].m_val);
                    result->appendLink ( new CompilerStatement ( CompilerSourceLocation(),
                                                                 CompilerStatementTypes::ASMPICOBLAZE_INS_JUMP_Z_AAA,
                                                                 new CompilerExpr ( "$", '+', 2 ) ) );
                    result->appendLink(jump(label, JC_NC));
                }
                else
                {
                    // <IMMEDIATE> <OPERATOR> <IMMEDIATE>
                    if (
                           ( ( cndVal[0].m_val >= cndVal[1].m_val ) && ( CompilerExpr::OPER_GE == cnd->oper() ) )
                               ||
                           ( ( cndVal[0].m_val <= cndVal[1].m_val ) && ( CompilerExpr::OPER_LE == cnd->oper() ) )
                       )
                    {
                        resultKnownInAdvance = 1;
                        result = new CompilerStatement();
                    }
                    else
                    {
                        resultKnownInAdvance = -1;
                        result = jump(label);
                    }
                }
            }
            break;
        case CompilerExpr::OPER_LT:
        {
            // Swap cndVal [0] <--> [1].

            bool auxFlag = cndVal[0].m_reg;
            int auxVal   = cndVal[0].m_val;

            cndVal[0].m_reg = cndVal[1].m_reg;
            cndVal[0].m_val = cndVal[1].m_val;

            cndVal[1].m_reg = auxFlag;
            cndVal[1].m_val = auxVal;
        }
        case CompilerExpr::OPER_GT:
            if ( true == cndVal[0].m_reg )
            {
                // <DIRECT> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <DIRECT> <OPERATOR> <DIRECT>
                    if ( cndVal[0].m_val == cndVal[1].m_val )
                    {
                        if (
                               ( ( cndVal[0].m_val > cndVal[1].m_val ) && ( CompilerExpr::OPER_GT == cnd->oper() ) )
                                   ||
                               ( ( cndVal[0].m_val < cndVal[1].m_val ) && ( CompilerExpr::OPER_LT == cnd->oper() ) )
                           )
                        {
                            resultKnownInAdvance = 2;
                            result = new CompilerStatement();
                        }
                        else
                        {
                            resultKnownInAdvance = -2;
                            result = jump(label);
                        }

                        // Do not append the label, break right here.
                        break;
                    }
                    else
                    {
                        result = compare_sx_sy(cndVal[0].m_val, cndVal[1].m_val);
                    }
                }
                else
                {
                    // <DIRECT> <OPERATOR> <IMMEDIATE>
                    result = compare_sx_kk(cndVal[0].m_val, cndVal[1].m_val);
                }

                result->appendLink ( jump(label, JC_C) );
                result->appendLink ( jump(label, JC_Z) );
            }
            else
            {
                // <IMMEDIATE> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <IMMEDIATE> <OPERATOR> <DIRECT>
                    result = compare_sx_kk(cndVal[1].m_val, cndVal[0].m_val);
                    result->appendLink ( jump(label, JC_NC) );
                }
                else
                {
                    // <IMMEDIATE> <OPERATOR> <IMMEDIATE>
                    if (
                           ( ( cndVal[0].m_val > cndVal[1].m_val ) && ( CompilerExpr::OPER_GT == cnd->oper() ) )
                               ||
                           ( ( cndVal[0].m_val < cndVal[1].m_val ) && ( CompilerExpr::OPER_LT == cnd->oper() ) )
                       )
                    {
                        resultKnownInAdvance = 1;
                        result = new CompilerStatement();
                    }
                    else
                    {
                        resultKnownInAdvance = -1;
                        result = jump(label);
                    }
                }
            }
            break;
        case CompilerExpr::OPER_BAND:
        case CompilerExpr::OPER_NAND:
            if ( true == cndVal[0].m_reg )
            {
                // <DIRECT> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <DIRECT> <OPERATOR> <DIRECT>
                    if ( cndVal[0].m_val == cndVal[1].m_val )
                    {
                        if ( CompilerExpr::OPER_BAND == cnd->oper() )
                        {
                            resultKnownInAdvance = 2;
                            result = new CompilerStatement();
                        }
                        else
                        {
                            resultKnownInAdvance = -2;
                            result = jump(label);
                        }

                        // Do not append the label, break right here.
                        break;
                    }
                    else
                    {
                        result = test_sx_sy(cndVal[0].m_val, cndVal[1].m_val);
                    }
                }
                else
                {
                    // <DIRECT> <OPERATOR> <IMMEDIATE>
                    result = test_sx_kk(cndVal[0].m_val, cndVal[1].m_val);
                }
            }
            else
            {
                // <IMMEDIATE> <OPERATOR> ...
                if ( true == cndVal[1].m_reg )
                {
                    // <IMMEDIATE> <OPERATOR> <DIRECT>
                    result = test_sx_kk(cndVal[1].m_val, cndVal[0].m_val);
                }
                else
                {
                    // <IMMEDIATE> <OPERATOR> <IMMEDIATE>
                    if (
                           ( !( cndVal[0].m_val & cndVal[1].m_val ) && ( CompilerExpr::OPER_BAND == cnd->oper() ) )
                               ||
                           (  ( cndVal[0].m_val & cndVal[1].m_val ) && ( CompilerExpr::OPER_NAND == cnd->oper() ) )
                       )
                    {
                        resultKnownInAdvance = 1;
                        result = new CompilerStatement();
                    }
                    else
                    {
                        resultKnownInAdvance = -1;
                        result = jump(label);
                    }

                    // Do not append the label, break right here.
                    break;
                }
            }

            // Append conditional jump after the comparison instruction.
            if ( CompilerExpr::OPER_BAND == cnd->oper() )
            {
                result->appendLink(jump(label, JC_Z));
            }
            else
            {
                result->appendLink(jump(label, JC_NZ));
            }
            break;
    }

    switch ( resultKnownInAdvance )
    {
        case -2:
            m_compilerCore->semanticMessage(cnd->location(),
                                            CompilerBase::MT_WARNING,
                                            QObject::tr("comparing a register with itself, result is always negative")
                                                       .toStdString(),
                                            true );
            break;
        case -1:
            m_compilerCore->semanticMessage(cnd->location(),
                                            CompilerBase::MT_WARNING,
                                            QObject::tr("comparing two immediate constants, result is always negative")
                                                       .toStdString(),
                                            true );
            break;
        case 1:
            m_compilerCore->semanticMessage(cnd->location(),
                                            CompilerBase::MT_WARNING,
                                            QObject::tr("comparing two immediate constants, result is always positive")
                                                        .toStdString(),
                                            true);
            break;
        case 2:
            m_compilerCore->semanticMessage(cnd->location(),
                                            CompilerBase::MT_WARNING,
                                            QObject::tr("comparing a register with itself, result is always positive")
                                                       .toStdString(),
                                            true);
            break;
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

inline CompilerStatement * AsmPicoBlazeSpecialMacros::compare_sx_sy ( int sx,
                                                                      int sy ) const
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMPICOBLAZE_INS_COMPARE_SX_SY,
                                   (new CompilerExpr(sx))->appendLink(new CompilerExpr(sy)) );
}

inline CompilerStatement * AsmPicoBlazeSpecialMacros::compare_sx_kk ( int sx,
                                                                      int kk ) const
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMPICOBLAZE_INS_COMPARE_SX_KK,
                                   (new CompilerExpr(sx))->appendLink(new CompilerExpr(kk)) );
}

inline CompilerStatement * AsmPicoBlazeSpecialMacros::test_sx_sy ( int sx,
                                                                   int sy ) const
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMPICOBLAZE_INS_TEST_SX_SY,
                                   (new CompilerExpr(sx))->appendLink(new CompilerExpr(sy)) );
}

inline CompilerStatement * AsmPicoBlazeSpecialMacros::test_sx_kk ( int sx,
                                                                   int kk ) const
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMPICOBLAZE_INS_TEST_SX_KK,
                                   (new CompilerExpr(sx))->appendLink(new CompilerExpr(kk)) );
}

inline CompilerStatement * AsmPicoBlazeSpecialMacros::jump ( const std::string & label,
                                                             JumpCondition cnd ) const
{
    using namespace CompilerStatementTypes;

    StatementType statementType;

    switch ( cnd )
    {
        case JC_NONE: statementType = ASMPICOBLAZE_INS_JUMP_AAA;    break;
        case JC_Z:    statementType = ASMPICOBLAZE_INS_JUMP_Z_AAA;  break;
        case JC_C:    statementType = ASMPICOBLAZE_INS_JUMP_C_AAA;  break;
        case JC_NZ:   statementType = ASMPICOBLAZE_INS_JUMP_NZ_AAA; break;
        case JC_NC:   statementType = ASMPICOBLAZE_INS_JUMP_NC_AAA; break;
    }

    return new CompilerStatement ( CompilerSourceLocation(),
                                   statementType,
                                   new CompilerExpr(label) );
}

inline CompilerStatement * AsmPicoBlazeSpecialMacros::label ( const std::string & label ) const
{
    return new CompilerStatement ( CompilerSourceLocation(),
                                   CompilerStatementTypes::ASMPICOBLAZE_LABEL,
                                   new CompilerExpr(label) );
}

inline const std::string & AsmPicoBlazeSpecialMacros::generateLabel ( std::string & label,
                                                                      LabelType labelType,
                                                                      bool end )
{
    char buf[5];
    sprintf(buf, "%d", m_labelNextCounter[(int)labelType]);

    label.clear();
    switch ( labelType )
    {
        case LT_IF:    label = "IF-";    break;
        case LT_FOR:   label = "FOR-";   break;
        case LT_WHILE: label = "WHILE-"; break;
    }
    label += buf;

    if ( true == end )
    {
        label += "-END";
    }
    else
    {
        m_labelNextCounter[(int)labelType]++;
    }

    return label;
}

inline void AsmPicoBlazeSpecialMacros::checkType ( bool regOrNumber,
                                                   AsmPicoBlazeSymbolTable::SymbolType type,
                                                   const CompilerSourceLocation & location )
{
    if ( AsmPicoBlazeSymbolTable::STYPE_EXPRESSION == type )
    {
        return;
    }

    if ( true == regOrNumber )
    {
        if ( AsmPicoBlazeSymbolTable::STYPE_REGISTER != type )
        {
            m_compilerCore->semanticMessage ( location,
                                              CompilerBase::MT_WARNING,
                                              QObject::tr ( "register is expected here but given type is: " )
                                                          . toStdString()
                                                          + m_symbolTable->symType2Str(type),
                                              true );
        }
    }
    else if ( AsmPicoBlazeSymbolTable::STYPE_NUMBER != type )
    {
        m_compilerCore->semanticMessage ( location,
                                          CompilerBase::MT_WARNING,
                                          QObject::tr ( "generic number is expected here but given type is: " )
                                                      . toStdString()
                                                      + m_symbolTable->symType2Str(type),
                                          true );
    }
}
