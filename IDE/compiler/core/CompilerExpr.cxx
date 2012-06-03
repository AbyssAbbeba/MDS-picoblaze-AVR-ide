/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "CompilerExpr.h"

#include <cstring>

CompilerExpr::Value::Value() {
	m_type = TYPE_NULL;
}

CompilerExpr::Value::~Value() {
	if ( TYPE_EXPR == m_type ) {
		delete m_data.m_expr;
	} else if ( TYPE_SYMBOL == m_type ) {
		delete[] m_data.m_symbol;
	}
}

CompilerExpr::Value::Value(int value) {
	m_type = TYPE_INT;
	m_data.m_integer = (long long)value;
}

CompilerExpr::Value::Value(long long value) {
	m_type = TYPE_INT;
	m_data.m_integer = value;
}

CompilerExpr::Value::Value(float value) {
	m_type = TYPE_REAL;
	m_data.m_real = (double)value;
}

CompilerExpr::Value::Value(double value) {
	m_type = TYPE_REAL;
	m_data.m_real = value;
}

CompilerExpr::Value::Value(CompilerExpr * value) {
	m_type = TYPE_EXPR;
	m_data.m_expr = value;
}

CompilerExpr::Value::Value(const char * value) {
	int maxSize = ( 1 + strlen(value) );

	m_type = TYPE_SYMBOL;
	m_data.m_symbol = new char[maxSize];
	memcpy(m_data.m_symbol, value, maxSize);
}

CompilerExpr::CompilerExpr(Value value) {
	m_lValue = value;
	m_operator = OPER_NONE;
}

CompilerExpr::CompilerExpr(Value lValue, Operator oper, Value rValue) {
	m_lValue = lValue;
	m_operator = oper;
	m_rValue = rValue;
}

CompilerExpr::CompilerExpr(Value lValue, char oper, Value rValue) {
	m_lValue = lValue;
	m_operator = Operator(oper);
	m_rValue = rValue;
}

std::ostringstream & operator << (std::ostringstream & out, const CompilerExpr::Value & val) {
	switch ( val.m_type ) {
		case CompilerExpr::Value::TYPE_NULL:
			out << std::string("NULL");
			break;
		case CompilerExpr::Value::TYPE_INT:
			out << std::string("I:") << val.m_data.m_integer;
			break;
		case CompilerExpr::Value::TYPE_REAL:
			out << std::string("R:") << val.m_data.m_real;
			break;
		case CompilerExpr::Value::TYPE_EXPR:
			out << std::string("E:") << val.m_data.m_expr;
			break;
		case CompilerExpr::Value::TYPE_SYMBOL:
			out << std::string("S:") << val.m_data.m_symbol;
			break;
	}
	return out;
}

std::ostringstream & operator << (std::ostringstream & out, const CompilerExpr::Operator & opr) {
	switch ( opr ) {
		case CompilerExpr::OPER_NONE:
			break;
		case CompilerExpr::OPER_ADD:
			out << std::string("+");
			break;
		case CompilerExpr::OPER_SUB:
			out << std::string("-");
			break;
		case CompilerExpr::OPER_MULT:
			out << std::string("*");
			break;
		case CompilerExpr::OPER_DIV:
			out << std::string("/");
			break;
	}
	return out;
}

std::ostringstream & operator << (std::ostringstream & out, const CompilerExpr * expr) {
	if ( CompilerExpr::OPER_NONE == expr->m_operator ) {
		out << std::string("(");
		out << expr->m_lValue;
		out << std::string(")");
	} else {
		out << std::string("(");
		out << expr->m_lValue;
		out << expr->m_operator;
		out << expr->m_rValue;
		out << std::string(")");
	}
	return out;
}
