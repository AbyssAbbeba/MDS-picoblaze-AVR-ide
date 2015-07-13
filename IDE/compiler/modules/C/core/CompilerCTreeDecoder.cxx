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
#include "CompilerCBackend.h"
#include "CompilerCSymbolTable.h"
#include "CompilerCDeclaration.h"

CompilerCTreeDecoder::CompilerCTreeDecoder ( CompilerSemanticInterface * compilerCore,
                                             CompilerOptions           * opts,
                                             CompilerCBackend          * backend,
                                             CompilerCSymbolTable      * symbolTable,
                                             CompilerCExprProcessor    * exprProcessor )
                                           :
                                             m_compilerCore ( compilerCore ),
                                             m_opts ( opts ),
                                             m_backend ( backend ),
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

    m_tmpDeclarations.enterScope();

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
                std::cout<<"UN:0\n";
                unexpectedNode(node->location());
                return;
        }
    }
}

inline void CompilerCTreeDecoder::processDeclaration ( CompilerExpr * declExpr )
{
    std::cout << "C_DECLARATION:" << declExpr << '\n';
    std::cout << "left="<<declExpr->lVal()<<'\n';
    std::cout << "op="<<declExpr->oper()<<'\n';
    std::cout << "right="<<declExpr->rVal()<<'\n'<<'\n';

    CompilerCDeclaration * declaration = resolveDeclaration(declExpr);
    processType(declaration);
}

CompilerCType::Type * CompilerCTreeDecoder::processType ( CompilerCDeclaration * declaration )
{
    using namespace CompilerCType;

    std::cout << "\n>>> resolveDeclaration -->\n" << declaration<<'\n';

    Type * type = nullptr;

    if ( CompilerCDeclaration::TS_ENUM & declaration->m_type )
    {
        std::cout<<"########################################################\n CompilerCDeclaration::TS_ENUM\n########################################################\n";
        if ( nullptr != declaration->m_members )
        {
            Enum * enumeration = new Enum();
            type = enumeration;

            if ( nullptr != declaration->m_name.m_id )
            {
                enumeration->m_tag = *(declaration->m_name.m_id);
            }

            auto & intConstants = m_tmpDeclarations.m_intConstants.back();

            int value = 0;
            for ( CompilerCDeclaration * member = declaration->m_members;
                  nullptr != member;
                  member = member->m_next, value++ )
            {
                const std::string & id = *(member->m_name.m_id);
                if ( nullptr != member->m_value )
                {
                    value = resolveIntConstExpr(member->m_value);
                }
std::cout << "enum " << id << " = " << value << '\n';

                if ( intConstants.cend() != intConstants.find(id) )
                {
                    // TODO: eclipse
                    std::cout << "conflict\n";
                }

                intConstants[id] = { member->m_location, value };
                enumeration->m_members.push_back ( { id, value } );
            }
        }
    }
    else if ( ( CompilerCDeclaration::TS_STRUCT | CompilerCDeclaration::TS_UNION ) & declaration->m_type )
    {
        std::cout<<"########################################################\n CompilerCDeclaration::TS_STRUCT | CompilerCDeclaration::TS_UNION\n########################################################\n";
        if ( nullptr != declaration->m_members )
        {
            Struct * structure = new Struct();
            type = structure;

            if ( nullptr != declaration->m_name.m_id )
            {
                structure->m_tag = *(declaration->m_name.m_id);
            }

            structure->m_union = ( CompilerCDeclaration::TS_UNION & declaration->m_type ) ? true : false;

            for ( CompilerCDeclaration * member = declaration->m_members;
                  nullptr != member;
                  member = member->m_next )
            {
                std::cout << "MEMBER = \n" <<member<<'\n';
                Type * memberType = processType(member);
                std::cout << "MEMBER type = \n" <<memberType<<'\n';

                for ( CompilerCDeclaration * instance = member->m_instances;
                      nullptr != instance;
                      instance = instance->m_next )
                {
                    // TODO: handle pointers, array, etc.
                    std::string id;
                    if ( nullptr != instance->m_name.m_id )
                    {
                        id = *( instance->m_name.m_id );
                    }
                    structure->m_members.push_back ( { id, memberType } );
                }
            }
        }
    }
    else
    {
        Basic * basicType = new Basic();
        type = basicType;

        constexpr int INTEGER_TYPES =
        (
              CompilerCDeclaration::TS_CHAR
            | CompilerCDeclaration::TS_SHORT
            | CompilerCDeclaration::TS_INT
            | CompilerCDeclaration::TS_LONG
            | CompilerCDeclaration::TS_LONG_LONG
        );

        constexpr int FLOATING_TYPES =
        (
              CompilerCDeclaration::TS_FLOAT
            | CompilerCDeclaration::TS_DOUBLE
            | CompilerCDeclaration::TS_LONG_DOUBLE
        );

        if ( CompilerCDeclaration::TS_VOID & declaration->m_type )
        {
            basicType->m_type = Basic::VOID;
            basicType->m_size = 0;
        }
        else if ( CompilerCDeclaration::TS_BOOL & declaration->m_type )
        {
            basicType->m_type = Basic::BOOLEAN;
            basicType->m_size = m_backend->dataTypeSize().m_bool;
        }
        else if ( INTEGER_TYPES & declaration->m_type )
        {
            basicType->m_type = ( CompilerCDeclaration::TS_UNSIGNED & declaration->m_type )
                                ? Basic::UNSIGNED_INT : Basic::SIGNED_INT;

            unsigned int size = 0;
            const CompilerCBackend::DataTypeSizeTable & dtst = m_backend->dataTypeSize();

            if ( CompilerCDeclaration::TS_CHAR & declaration->m_type )
            {
                size = dtst.m_char;
            }
            else if ( CompilerCDeclaration::TS_SHORT & declaration->m_type )
            {
                size = dtst.m_shortInt;
            }
            else if ( CompilerCDeclaration::TS_INT & declaration->m_type )
            {
                size = dtst.m_int;
            }
            else if ( CompilerCDeclaration::TS_LONG & declaration->m_type )
            {
                size = dtst.m_longInt;
            }
            else if ( CompilerCDeclaration::TS_LONG_LONG & declaration->m_type )
            {
                size = dtst.m_longLongInt;
            }

            basicType->m_size = size;
        }
        else if ( FLOATING_TYPES & declaration->m_type )
        {
            unsigned int size = 0;
            const CompilerCBackend::DataTypeSizeTable & dtst = m_backend->dataTypeSize();

            if ( CompilerCDeclaration::TS_COMPLEX & declaration->m_type )
            {
                basicType->m_type = Basic::COMPLEX_FLOAT;

                if ( CompilerCDeclaration::TS_FLOAT & declaration->m_type )
                {
                    size = dtst.m_complexFloat;
                }
                else if ( CompilerCDeclaration::TS_DOUBLE & declaration->m_type )
                {
                    size = dtst.m_complexDouble;
                }
                else if ( CompilerCDeclaration::TS_LONG_DOUBLE & declaration->m_type )
                {
                    size = dtst.m_complexLongDouble;
                }
            }
            else
            {
                if ( CompilerCDeclaration::TS_IMAGINARY & declaration->m_type )
                {
                    basicType->m_type = Basic::IMAGINARY_FLOAT;
                }
                else
                {
                    basicType->m_type = Basic::REAL_FLOAT;
                }

                if ( CompilerCDeclaration::TS_FLOAT & declaration->m_type )
                {
                    size = dtst.m_float;
                }
                else if ( CompilerCDeclaration::TS_DOUBLE & declaration->m_type )
                {
                    size = dtst.m_double;
                }
                else if ( CompilerCDeclaration::TS_LONG_DOUBLE & declaration->m_type )
                {
                    size = dtst.m_longDouble;
                }
            }

            basicType->m_size = size;
        }
    }

std::cout << "\n### TYPE >>>\n";
std::cerr << type << '\n';

    if ( CompilerCDeclaration::SC_TYPEDEF & declaration->m_type )
    {
        declaration->m_type = CompilerCDeclaration::Type ( declaration->m_type & ~(CompilerCDeclaration::SC_TYPEDEF) );
        std::cout<<"########################################################\n CompilerCDeclaration::SC_TYPEDEF\n########################################################\n";
        for ( CompilerCDeclaration * instance = declaration->m_instances;
              nullptr != instance;
              instance = instance->m_next )
        {
            newTypedef(combineDeclarations(declaration, instance, instance->m_location));
        }
    }
    else
    {

    }

    return type;
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveDeclaration ( const CompilerExpr * exprTree,
                                                                  bool inParamList )
{
    if ( nullptr == exprTree )
    {
        return nullptr;
    }

    const CompilerSourceLocation & location = exprTree->location();
    const CompilerValue & leftValue = exprTree->lVal();
    const CompilerValue & rightValue = exprTree->rVal();

    if (
           ( CompilerExpr::OPER_DECLARATION != exprTree->oper() )
               ||
           ( CompilerValue::TYPE_EXPR != leftValue.m_type )
               ||
           ( CompilerValue::TYPE_EXPR != rightValue.m_type )
       )
    {
        if (
               ( true == inParamList )
                   &&
               ( CompilerExpr::OPER_NONE == exprTree->oper() )
                   &&
               ( CompilerValue::TYPE_EMPTY == leftValue.m_type )
                   &&
               ( CompilerValue::TYPE_EMPTY == rightValue.m_type )
           )
        {
            return new CompilerCDeclaration(exprTree->location());
        }

        std::cout<<"UN:1"<<exprTree<<"\n";
        unexpectedNode(location);
        return nullptr;
    }

    CompilerCDeclaration * result = resolveDeclSpecifiers(leftValue.m_data.m_expr);
    std::cout << "decl spec: " << result << '\n';
    if ( nullptr == result )
    {
        return nullptr;
    }

    for ( CompilerExpr * expr = rightValue.m_data.m_expr;
          nullptr != expr;
          expr = expr->next() )
    {
        CompilerExpr * declarator = expr;
        CompilerExpr * allocator = nullptr;
        CompilerExpr * bitWidth = nullptr;
        CompilerExpr * initializer = nullptr;

        if ( CompilerExpr::OPER_AT == declarator->oper() )
        {
            allocator = declarator->lVal().m_data.m_expr;
            declarator = declarator->rVal().m_data.m_expr;
        }

        if ( CompilerExpr::OPER_COLON == declarator->oper() )
        {
            bitWidth = declarator->rVal().m_data.m_expr;
            declarator = declarator->lVal().m_data.m_expr;
        }

        if ( CompilerExpr::OPER_INIT == declarator->oper() )
        {
            initializer = declarator->rVal().m_data.m_expr;
            declarator = declarator->lVal().m_data.m_expr;
        }

        if ( nullptr != allocator )
        {
            std::cout << "@@@@@ allocator: " << allocator << '\n';
        }
        if ( nullptr != bitWidth )
        {
            std::cout << "@@@@@ bitWidth: " << bitWidth << '\n';
        }
        if ( nullptr != initializer )
        {
            std::cout << "@@@@@ initializer: " << initializer << '\n';
        }

        CompilerCDeclaration * instance = resolveDeclarator(declarator);
        result->addInstance(instance);
    }

    return result;
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveDeclarator ( const CompilerExpr * exprTree )
{
    if ( nullptr == exprTree )
    {
        return nullptr;
    }

    std::cout << "resolveDeclarator ( "<<exprTree<<" )\n";

    if ( CompilerExpr::OPER_PAIR != exprTree->oper() )
    {
        std::cout<<"UN:X0"<<exprTree<<"\n";
        unexpectedNode(exprTree->location());
        return nullptr;
    }

    CompilerCDeclaration * result = new CompilerCDeclaration(exprTree->location());

    // Resolve pointer.
    for ( const CompilerExpr * ptr = exprTree->lVal().m_data.m_expr;
          nullptr != ptr;
          ptr = ptr->lVal().m_data.m_expr )
    {
        if ( CompilerExpr::OPER_POINTER != ptr->oper() )
        {
            std::cout<<"UN:X1"<<exprTree<<"\n";
            unexpectedNode(exprTree->location());
            return nullptr;
        }

        CompilerCDeclaration * ptrSpec = resolveDeclSpecifiers(ptr->rVal().m_data.m_expr);
        result->m_pointer.push_back( ( nullptr == ptrSpec ) ? CompilerCDeclaration::F_EMPTY : ptrSpec->m_type );
    }

    auto xxx = combineDeclarations(resolveDirectDeclarator(exprTree->rVal().m_data.m_expr), result, exprTree->location());
    std::cout << "resolveDeclarator --> " << xxx;
    return xxx;
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveDirectDeclarator ( const CompilerExpr * exprTree,
                                                                       bool inArray,
                                                                       bool inFunction )
{
    std::cout << "resolveDirectDeclarator ( "<<exprTree<<" )\n";
    CompilerCDeclaration * result = new CompilerCDeclaration(exprTree->location());

    switch ( exprTree->oper() )
    {
        case CompilerExpr::OPER_NONE:
            result->m_name.m_id = new std::string(exprTree->lVal().m_data.m_symbol);
            break;

        case CompilerExpr::OPER_ARRAY:
            if ( true == inFunction )
            {
                m_compilerCore->semanticMessage ( exprTree->location(),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "invalid array declaration" )
                                                              .toStdString() );
                return nullptr;
            }

            for ( const CompilerExpr * arr = exprTree;
                  nullptr != arr;
                  arr = arr->lVal().m_data.m_expr )
            {
                if ( CompilerExpr::OPER_ARRAY != arr->oper() )
                {
                    std::cout << "IAD: " << arr << '\n';
                    return combineDeclarations(resolveDirectDeclarator(arr, true, inFunction), result, exprTree->location());
                }

                CompilerExpr * specExpr = arr->rVal().m_data.m_expr;

                CompilerCDeclaration * arrSpec;
                switch ( specExpr->oper() )
                {
                    case CompilerExpr::OPER_DECLARATION:
                        arrSpec = resolveDeclSpecifiers(specExpr->lVal().m_data.m_expr);
                        if ( nullptr == arrSpec )
                        {
                            arrSpec = new CompilerCDeclaration(specExpr->location());
                        }
                        arrSpec->m_value = specExpr->rVal().m_data.m_expr->copyChainLink();
                        break;

                    case CompilerExpr::OPER_POINTER:
                        arrSpec = resolveDeclSpecifiers(specExpr->lVal().m_data.m_expr);
                        if ( nullptr == arrSpec )
                        {
                            arrSpec = new CompilerCDeclaration(specExpr->location());
                        }
                        arrSpec->m_pointer.push_back(CompilerCDeclaration::F_EMPTY);
                        break;

                    default:
                        std::cout<<"UN:X3"<<exprTree<<"\n";
                        unexpectedNode(exprTree->location());
                        return nullptr;
                }

                result->m_array->prependLink(arrSpec);
                result->m_array = arrSpec;
            }
            break;

        case CompilerExpr::OPER_FUNCTION:
            if ( true == inArray )
            {
                m_compilerCore->semanticMessage ( exprTree->location(),
                                                  CompilerBase::MT_ERROR,
                                                  QObject::tr ( "invalid function declaration" )
                                                              .toStdString() );
                return nullptr;
            }

            result->m_type = CompilerCDeclaration::A_FUNCTION;
            for ( const CompilerExpr * parm = exprTree->rVal().m_data.m_expr;
                  nullptr != parm;
                  parm = parm->next() )
            {
                result->addParameter(resolveDeclaration(parm, true));
            }

            return combineDeclarations ( resolveDirectDeclarator(exprTree->lVal().m_data.m_expr, inArray, true), result, exprTree->location());

        case CompilerExpr::OPER_COMPOUND:
            result->m_overrule = resolveDeclarator(exprTree->lVal().m_data.m_expr);
            break;

        default:
            std::cout<<"UN:X2"<<exprTree<<"\n";
            unexpectedNode(exprTree->location());
            return nullptr;
    }

    std::cout << "resolveDirectDeclarator --> " << result;

    return result;
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveDeclSpecifiers ( const CompilerExpr * exprTree )
{
    if ( nullptr == exprTree )
    {
        return nullptr;
    }

    const CompilerSourceLocation & location = exprTree->location();
    const CompilerValue & leftValue = exprTree->lVal();
    const CompilerValue & rightValue = exprTree->rVal();

    switch ( exprTree->oper() )
    {
        case CompilerExpr::OPER_DATATYPE:
        {
            CompilerCDeclaration * left = resolveType(leftValue,  location);
            CompilerCDeclaration * right = resolveType(rightValue, location);
    std::cout << "left = "<<left;
    std::cout << "right = "<<right;
            CompilerCDeclaration * result = combineDeclarations(left, right, location);
    std::cout << "resolveDeclSpecifiers ("<<exprTree<<") --> \n\t(*"<<(void*)result<<") " << result<<'\n';
            return result;
        }

        case CompilerExpr::OPER_COMPOUND:
            return resolveCompound(leftValue, rightValue, location);

        case CompilerExpr::OPER_ALIGNAS:
            if ( CompilerValue::TYPE_EMPTY == leftValue.m_type )
            {
                CompilerCDeclaration * result = new CompilerCDeclaration(location);
                result->m_alloc.m_alignment = resolveIntConstExpr(rightValue.m_data.m_expr);
                return result;
            }
            else
            {
                // TODO:
                // type-name
//                 leftValue
            }
            return nullptr;

        default:
                std::cout<<"UN:2\n"<<exprTree<<'\n';
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
                std::cout<<"UN:3\n";
        unexpectedNode(location);
        return nullptr;
    }

    CompilerCDeclaration * result;
    CompilerExpr * spec = rightValue.m_data.m_expr;
    CompilerCDeclaration::Type type = (CompilerCDeclaration::Type) leftValue.m_data.m_integer;

    if (
           ( type != CompilerCDeclaration::TS_ENUM )
               &&
           ( type != CompilerCDeclaration::TS_UNION )
               &&
           ( type != CompilerCDeclaration::TS_STRUCT )
        )
    {
                std::cout<<"UN:4\n";
        unexpectedNode(location);
        return nullptr;
    }

    result = new CompilerCDeclaration(location);
    result->m_type = type;

    switch (  spec->oper() )
    {
        case CompilerExpr::OPER_PAIR:
        {
                std::cout << "XXX='"<<spec->lVal()<<"'\n"<<std::flush;

                std::cout << "######## = " << spec->lVal() << '\n';
            {
                CompilerExpr * id = spec->lVal().m_data.m_expr;
                if ( nullptr != id )
                {
                    result->m_name.m_type = new std::string(id->lVal().m_data.m_symbol);
                }
            }

            if ( CompilerValue::TYPE_EXPR != spec->rVal().m_type )
            {
                std::cout<<"UN:6\n";
                unexpectedNode(spec->location());
                return nullptr;
            }

            for ( const CompilerExpr * expr = spec->rVal().m_data.m_expr;
                  nullptr != expr;
                  expr = expr->next() )
            {
                CompilerCDeclaration * member;
                if ( CompilerCDeclaration::TS_ENUM == type )
                {
                    member = new CompilerCDeclaration(expr->location());
                    if ( CompilerExpr::OPER_ASSIGN == expr->oper() )
                    {
                        member->m_name.m_id = new std::string(expr->lVal().m_data.m_expr->lVal().m_data.m_symbol);
                        member->m_value = expr->rVal().m_data.m_expr->copyChainLink();
                    }
                    else if ( CompilerExpr::OPER_NONE == expr->oper() )
                    {
                        member->m_name.m_id = new std::string(expr->lVal().m_data.m_symbol);
                    }
                    else
                    {
                        unexpectedNode(expr->location());
                    }
                }
                else
                {
                    member = resolveDeclaration(expr);
                }

                result->addMembers(member);
            }

            return result;
        }
        case CompilerExpr::OPER_NONE:
        {
            result->m_name.m_type = new std::string(spec->lVal().m_data.m_expr->lVal().m_data.m_symbol);
            return result;
        }
        default:
        {
                std::cout<<"UN:7\n";
            unexpectedNode(location);
            return nullptr;
        }
    }
}

CompilerCDeclaration * CompilerCTreeDecoder::resolveType ( const CompilerValue & exprValue,
                                                           const CompilerSourceLocation & location )
{
    CompilerCDeclaration * result = nullptr;

    switch ( exprValue.m_type )
    {
        case CompilerValue::TYPE_EMPTY:
            return nullptr;

        case CompilerValue::TYPE_INT:
            result = new CompilerCDeclaration(location);
            result->m_type = (CompilerCDeclaration::Type) exprValue.m_data.m_integer;
            break;

        case CompilerValue::TYPE_EXPR:
        {
            const CompilerExpr * expr = exprValue.m_data.m_expr;
            if ( nullptr != expr )
            {
                if ( CompilerExpr::OPER_NONE == expr->oper() )
                {
                    return resolveType(expr->lVal(), expr->location());
                }
                return resolveDeclSpecifiers(expr);
            }
            break;
        }

        case CompilerValue::TYPE_SYMBOL:
        {
            const std::string symbol = exprValue.m_data.m_symbol;
            for ( int i = m_tmpDeclarations.m_typedefs.size() - 1; i >= 0; i-- )
            {
                const auto iter = m_tmpDeclarations.m_typedefs[i].find(symbol);
                if ( m_tmpDeclarations.m_typedefs[i].cend() != iter )
                {
                    result = iter->second.second->copyChainLink(location);
                    delete result->m_name.m_id;
                    result->m_name.m_id = nullptr;
                    return result;
                }
            }

            m_compilerCore->semanticMessage ( location,
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "typedef name %1 has not been declared in this scope" )
                                                          .arg(exprValue.m_data.m_symbol).toStdString() );

            break;
        }

        default:
                std::cout<<"UN:8 "<<exprValue.m_type<<"\n";
            unexpectedNode(location);
            break;
    }

    std::cout << "resolveType ("<<exprValue<<") --> " << result;
    return result;
}

CompilerCDeclaration * CompilerCTreeDecoder::combineDeclarations ( const CompilerCDeclaration * base,
                                                                   CompilerCDeclaration * with,
                                                                   const CompilerSourceLocation & location )
{
    CompilerCDeclaration * result = with->copyChainLink(location);

    std::cout << "combineDeclarations\n";
    std::cout << "    [base] = "<<base;
    std::cout << "    [with] = "<<with;

    if ( nullptr == base )
    {
        auto r = with->copyChainLink(location);
    std::cout << "combineDeclarations --> (*"<<(void*)r<<") "<<r;
    return r;
    }
    else if ( nullptr == with )
    {
        auto r = base->copyChainLink(location);
    std::cout << "combineDeclarations --> (*"<<(void*)r<<") "<<r;
    return r;
    }

    result->m_location = location;
    if ( result->m_type & base->m_type )
    {
        std::cout << "cannot combine: "<<base<<" + "<<with<<"\n";
        std::cout << "reason: " << std::hex << result->m_type << " & " << with->m_type << std::dec << '\n';
        return nullptr;
    }

    result->m_type = CompilerCDeclaration::Type ( result->m_type | base->m_type );

    if ( nullptr == result->m_name.m_id )
    {
        if ( nullptr != base->m_name.m_id )
        {
            result->m_name.m_id = new std::string(*base->m_name.m_id);
        }
    }
    if ( true == result->m_pointer.empty() )
    {
        result->m_pointer = base->m_pointer;
    }
    if ( nullptr == result->m_array )
    {
        result->m_array = base->m_array;
    }
    if ( nullptr == result->m_members )
    {
        result->m_members = base->m_members;
    }
    if ( nullptr == result->m_overrule )
    {
        result->m_overrule = base->m_overrule;
    }
    if ( nullptr == result->m_value )
    {
        result->m_value = base->m_value->copyEntireChain();
    }

    std::cout << "combineDeclarations --> (*"<<(void*)result<<")"<<result;
    return result;
}

int64_t CompilerCTreeDecoder::resolveIntConstExpr ( const CompilerExpr * expr )
{
    int64_t leftValue = resolveIntConstValue(expr->lVal(), expr->location());
    switch ( expr->oper() )
    {
        case CompilerExpr::OPER_NONE:           return  leftValue;
        case CompilerExpr::OPER_NOT:            return !leftValue;
        case CompilerExpr::OPER_CMPL:           return ~leftValue;
        case CompilerExpr::OPER_ADD_INV:        return -leftValue;
        case CompilerExpr::OPER_INT_PROM:       return +leftValue;

        case CompilerExpr::OPER_CONDITION:
        {
            const CompilerExpr * subExpr = expr->rVal().m_data.m_expr;
            if ( CompilerExpr::OPER_COLON != subExpr->oper() )
            {
                unexpectedNode(subExpr->location());
            }

            if ( 0 != leftValue )
            {
                return resolveIntConstExpr(subExpr->lVal().m_data.m_expr);
            }
            else
            {
                return resolveIntConstExpr(subExpr->lVal().m_data.m_expr);
            }
        }

        default:
        {
            int64_t rightValue = resolveIntConstValue(expr->rVal(), expr->location());
            switch ( expr->oper() )
            {
                case CompilerExpr::OPER_ADD:    return leftValue +  rightValue;
                case CompilerExpr::OPER_SUB:    return leftValue -  rightValue;
                case CompilerExpr::OPER_MULT:   return leftValue *  rightValue;
                case CompilerExpr::OPER_BOR:    return leftValue |  rightValue;
                case CompilerExpr::OPER_BXOR:   return leftValue ^  rightValue;
                case CompilerExpr::OPER_BAND:   return leftValue &  rightValue;
                case CompilerExpr::OPER_LOR:    return leftValue || rightValue;
                case CompilerExpr::OPER_LAND:   return leftValue && rightValue;
                case CompilerExpr::OPER_EQ:     return leftValue == rightValue;
                case CompilerExpr::OPER_NE:     return leftValue != rightValue;
                case CompilerExpr::OPER_LT:     return leftValue <  rightValue;
                case CompilerExpr::OPER_LE:     return leftValue <= rightValue;
                case CompilerExpr::OPER_GE:     return leftValue >= rightValue;
                case CompilerExpr::OPER_GT:     return leftValue >  rightValue;
                case CompilerExpr::OPER_SHR:    return leftValue >> rightValue;
                case CompilerExpr::OPER_SHL:    return leftValue << rightValue;

                case CompilerExpr::OPER_DIV:
                case CompilerExpr::OPER_MOD:
                    if ( 0 == rightValue)
                    {
                        m_compilerCore->semanticMessage ( expr->location(),
                                                          CompilerBase::MT_ERROR,
                                                          QObject::tr ( "division by zero" ).toStdString() );
                        return 1;
                    }
                    if ( expr->oper() == CompilerExpr::OPER_DIV )
                    {
                        return leftValue / rightValue;
                    }
                    else
                    {
                        return leftValue % rightValue;
                    }

                case CompilerExpr::OPER_INDEX:
                case CompilerExpr::OPER_SIZEOF:
                case CompilerExpr::OPER_FIXED_DATATYPE:
                case CompilerExpr::OPER_CAST:
                case CompilerExpr::OPER_ADDR:
                case CompilerExpr::OPER_GENERIC:
                case CompilerExpr::OPER_ALIGNOF:
                    std::cout << "NOT SUPPORTED, YET!\n";
                    return 1;

                default:
                    m_compilerCore->semanticMessage ( expr->location(),
                                                      CompilerBase::MT_ERROR,
                                                      QObject::tr ( "invalid operation in integer constant expression" )
                                                                  .toStdString() );
                    return 1;
            }
        }
    }
}

int64_t CompilerCTreeDecoder::resolveIntConstValue ( const CompilerValue & value,
                                                     const CompilerSourceLocation & location )
{
    switch ( value.m_type )
    {
        case CompilerValue::TYPE_EMPTY:
            unexpectedNode(location);
            return 1;

        case CompilerValue::TYPE_INT:
            return value.m_data.m_integer;

        case CompilerValue::TYPE_REAL:
            m_compilerCore->semanticMessage ( location,
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "floating point value in integer constant expression" )
                                                          .toStdString() );
            return 1;

        case CompilerValue::TYPE_EXPR:
            return resolveIntConstExpr(value.m_data.m_expr);

        case CompilerValue::TYPE_SYMBOL:
            return getIntConstant(value.m_data.m_symbol, location);

        case CompilerValue::TYPE_ARRAY:
            m_compilerCore->semanticMessage ( location,
                                              CompilerBase::MT_ERROR,
                                              QObject::tr ( "string literal in integer constant expression" )
                                                          .toStdString() );
            return 1;
    }
}

int64_t CompilerCTreeDecoder::getIntConstant ( const std::string & id,
                                               const CompilerSourceLocation & location )
{
    for ( unsigned int i = m_tmpDeclarations.m_intConstants.size() - 1; i >= 0; i-- )
    {
        auto end = m_tmpDeclarations.m_intConstants[i].end();
        auto iter = m_tmpDeclarations.m_intConstants[i].find(id);

        if ( end != iter )
        {
            return iter->second.second;
        }
    }

    m_compilerCore->semanticMessage ( location,
                                      CompilerBase::MT_ERROR,
                                      QObject::tr("value of %1 has not been defined").arg(id.c_str()).toStdString() );

    return 1;
}

inline void CompilerCTreeDecoder::processFuncDef ( CompilerStatement * codeTree )
{
    std::cout << "C_FUNCTION_DEF:"<<codeTree->args()<<'\n';
    using namespace CompilerStatementTypes;

    m_tmpDeclarations.enterScope();

    for ( CompilerStatement * node = codeTree->next();
          nullptr != node;
          node = node->next() )
    {
        switch ( node->type() )
        {
            case C_DECLARATION:
                processDeclaration(node->args());
                break;

//             C_LABEL,
//             C_CASE,
//             C_DEFAULT,
//
//             C_COMPOUND,
//             C_CRITICAL_COMPOUND,
//
//             C_DECLARATION,
//             C_EXPRESSION,
//             C_NULL,
//
//             C_IF_ELSE,
//             C_IF,
//             C_ELSE,
//             C_SWITCH,
//
//             C_WHILE,
//             C_DO_WHILE,
//             C_FOR,
//             C_FOR_DECL,
//
//             C_GOTO,
//             C_CONTINUE,
//             C_BREAK,
//             C_RETURN,
//
//             C_INLINE_ASSEMBLY,

            default:
                std::cout<<"UN:0\n";
                unexpectedNode(node->location());
                return;
        }
    }

    m_tmpDeclarations.leaveScope();
}

inline void CompilerCTreeDecoder::processExpressions ( CompilerExpr * /*expr*/ )
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

void CompilerCTreeDecoder::TemporaryDeclarations::enterScope()
{
    m_intConstants.resize ( m_intConstants.size() + 1 );
    m_typedefs.resize ( m_typedefs.size() + 1 );
}

void CompilerCTreeDecoder::TemporaryDeclarations::leaveScope()
{
    m_intConstants.pop_back();
    m_typedefs.pop_back();
}

inline void CompilerCTreeDecoder::newTypedef ( const CompilerCDeclaration * typedefDecl )
{
    if ( nullptr == typedefDecl )
    {
        return;
    }

    const std::string & id = *(typedefDecl->m_name.m_id);
    // TODO: process eclipse
    if ( m_tmpDeclarations.m_typedefs.back().cend() != m_tmpDeclarations.m_typedefs.back().find(id) )
    {
        m_compilerCore->semanticMessage ( typedefDecl->m_location,
                                          CompilerBase::MT_ERROR,
                                          QObject::tr ( "sdfsfdsf" ).toStdString() );
        delete typedefDecl;
        return;
    }

    m_tmpDeclarations.m_typedefs.back()[id] = { typedefDecl->m_location, typedefDecl };
}
