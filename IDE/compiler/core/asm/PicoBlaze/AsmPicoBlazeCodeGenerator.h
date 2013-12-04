// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup PicoBlazeAsm
 * @file AsmPicoBlazeCodeGenerator.h
 */
// =============================================================================

#ifndef ASMPICOBLAZECODEGENERATOR_H
#define ASMPICOBLAZECODEGENERATOR_H

// Forward declarations.
class CompilerStatement;
class CompilerValue;
class CompilerExpr;

// Standard headers.
#include <string>

/**
 * @brief
 * @ingroup PicoBlazeAsm
 * @class AsmPicoBlazeCodeGenerator
 */
class AsmPicoBlazeCodeGenerator
{
    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[out] result
         * @param[in] node
         */
        static void toSourceLine ( std::string & result,
                                   const CompilerStatement * node );

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in,out] result
         * @param[in] args
         * @param[in] immediate
         * @param[in] indirect
         */
        static void operands ( std::string & result,
                               const CompilerExpr * args,
                               bool immediate = false,
                               bool indirect = false );

        /**
         * @brief
         * @param[in,out] result
         * @param[in] expr
         */
        static void expr2str ( std::string & result,
                               const CompilerExpr * expr );

        /**
         * @brief
         * @param[in,out] result
         * @param[in] val
         */
        static void value2str ( std::string & result,
                                const CompilerValue & val );

        /**
         * @brief
         * @param[in,out] result
         * @param[in] oper
         */
        static void oper2str ( std::string & result,
                               int oper );
};

#endif // ASMPICOBLAZECODEGENERATOR_H
