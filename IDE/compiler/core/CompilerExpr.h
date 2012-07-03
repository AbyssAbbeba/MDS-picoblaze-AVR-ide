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
		Value(const unsigned char * array, int size);
		Value(unsigned char * array, int size, bool copy = false);

		void completeDelete();
	};

	/**
	 *
	 */
	enum Operator {
		OPER_NONE	= 0,	///< This makes only the m_lValue valid.
		OPER_ADD	= L'+',	///<
		OPER_SUB	= L'-',	///<
		OPER_MULT	= L'*',	///<
		OPER_DIV	= L'/',	///<
		OPER_MOD	= L'%',	///<
		OPER_DOT	= L'.',	///<
		OPER_CALL	= L'C',	///<
		OPER_BOR	= L'|',	///< Bitwise OR
		OPER_BXOR	= L'^',	///< Bitwise XOR
		OPER_BAND	= L'&',	///< Bitwise AND
		OPER_LOR	= L'⋁',	///< Logical OR
		OPER_LXOR	= L'⊕',	///< Logical XOR
		OPER_LAND	= L'⋀',	///< Logical AND
		OPER_LOW	= L'l',	///<
		OPER_HIGH	= L'h',	///<
		OPER_EQ		= L'≡',	///<
		OPER_NE		= L'≠',	///<
		OPER_LT		= L'<',	///<
		OPER_LE		= L'≤',	///<
		OPER_GE		= L'≥',	///<
		OPER_GT		= L'>',	///<
		OPER_SHR	= L'≫',	///<
		OPER_SHL	= L'≪',	///<
		OPER_CMPL	= L'~'	///<
	} m_operator;

	Value m_lValue;
	Value m_rValue;

	CompilerExpr * m_next;
	CompilerExpr * m_prev;

	CompilerExpr();
	CompilerExpr(Value value);
	CompilerExpr(Operator oper, Value value);
	CompilerExpr(char oper, Value value);
	CompilerExpr(Value lValue, Operator oper, Value rValue);
	CompilerExpr(Value lValue, char oper, Value rValue);

	CompilerExpr * first();
	CompilerExpr * addLink(CompilerExpr * next);
	
	void completeDelete();
	static void completeDelete(CompilerExpr * expr);
};

// Tracing operators
std::ostream & operator << (std::ostream & out, const CompilerExpr::Value & val);
std::ostream & operator << (std::ostream & out, const CompilerExpr::Operator & opr);
std::ostream & operator << (std::ostream & out, const CompilerExpr * expr);

#endif // COMPILEREXPR_H
