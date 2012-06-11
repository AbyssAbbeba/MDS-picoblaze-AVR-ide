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

#include <ostream>

/**
 * @brief
 * @ingroup Compiler
 * @struct CompilerExpr
 */
class CompilerExpr {
public:
	class Value {
	public:
		enum Type {
			TYPE_EMPTY = 0,
			TYPE_INT,
			TYPE_REAL,
			TYPE_EXPR,
			TYPE_SYMBOL,
			TYPE_ARRAY
		} m_type;

		union Data {
			long long m_integer;
			double m_real;
			CompilerExpr * m_expr;
			char * m_symbol;
			struct {
				unsigned char * m_data;
				int m_size;
			} m_array;
		} m_data;

		int m_size; // Used only for TYPE_STRING

		Value();
		Value(int value);
		Value(long long value);
		Value(float value);
		Value(double value);
		Value(CompilerExpr * expr);
		Value(const char * string);
		Value(const unsigned char * array, unsigned int size);
		Value(unsigned char * array, unsigned int size, bool copy = false);

		void completeDelete();
	};

	enum Operator {
		OPER_NONE = 0, ///< Makes only the m_lValue valid.
		OPER_ADD = '+',
		OPER_SUB = '-',
		OPER_MULT = '*',
		OPER_DIV = '/',
		OPER_DOT = '.',
		OPER_CALL = 'C'
	} m_operator;

	Value m_lValue;
	Value m_rValue;

	CompilerExpr * m_next;
	CompilerExpr * m_prev;

	CompilerExpr();
	CompilerExpr(Value value);
	CompilerExpr(Value lValue, Operator oper, Value rValue);
	CompilerExpr(Value lValue, char oper, Value rValue);

	CompilerExpr * first();
	CompilerExpr * addLink(CompilerExpr * next);
	void completeDelete();
};

// Debugging operators
std::ostream & operator << (std::ostream & out, const CompilerExpr::Value & val);
std::ostream & operator << (std::ostream & out, const CompilerExpr::Operator & opr);
std::ostream & operator << (std::ostream & out, const CompilerExpr * expr);

#endif // COMPILEREXPR_H
