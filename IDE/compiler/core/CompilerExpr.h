/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup Compiler
 * @file CompilerExpr.h
 */

#ifndef COMPILEREXPR_H
#define COMPILEREXPR_H

#include <sstream>

/**
 * @brief
 * @ingroup Compiler
 * @struct CompilerExpr
 */
struct CompilerExpr {
	struct Value {
		enum Type {
			TYPE_NULL = 0,
			TYPE_INT,
			TYPE_REAL,
			TYPE_EXPR,
			TYPE_SYMBOL
		} m_type;

		union Data {
			long long m_integer;
			double m_real;
			CompilerExpr * m_expr;
			char * m_symbol;
		} m_data;

		Value();
		~Value();

		Value(int value);
		Value(long long value);
		Value(float value);
		Value(double value);
		Value(CompilerExpr * value);
		Value(const char * value);

// 		friend std::ostringstream & operator << (std::ostringstream & out, const Value & val);
	};

	enum Operator {
		OPER_NONE = 0, ///< Makes only the m_lValue valid.
		OPER_ADD = '+',
		OPER_SUB = '-',
		OPER_MULT = '*',
		OPER_DIV = '/'
	} m_operator;

	Value m_lValue;
	Value m_rValue;

	CompilerExpr(Value value);
	CompilerExpr(Value lValue, Operator oper, Value rValue);
	CompilerExpr(Value lValue, char oper, Value rValue);

// 	friend std::ostringstream & operator << (std::ostringstream & out, const CompilerExpr * expr);
// 	friend std::ostringstream & operator << (std::ostringstream & out, const Operator & opr);

private:
	CompilerExpr();
};

#endif // COMPILEREXPR_H
