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
#include <cctype>
#include <cstdlib>
#include <iostream> // DEBUG

CompilerExpr::Value::Value() {
	m_type = TYPE_EMPTY;
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

CompilerExpr::Value::Value(CompilerExpr * expr) {
	m_type = TYPE_EXPR;
	m_data.m_expr = expr->first();
}

CompilerExpr::Value::Value(const char * string) {
	m_type = TYPE_SYMBOL;
	int length = ( 1 + strlen(string) );
	m_data.m_symbol = new char[length];
	memcpy(m_data.m_symbol, string, length);
}

CompilerExpr::Value::Value(const unsigned char * array, unsigned int size) {
	m_type = TYPE_ARRAY;
	m_data.m_array.m_size = size;
	m_data.m_array.m_data = (unsigned char*) malloc(size);
	memcpy(m_data.m_array.m_data, array, size);
}

CompilerExpr::Value::Value(unsigned char * array, unsigned int size, bool copy) {
	m_type = TYPE_ARRAY;
	m_data.m_array.m_size = size;

	if ( true == copy ) {
		m_data.m_array.m_data = (unsigned char*) malloc(size);
		memcpy(m_data.m_array.m_data, array, size);
	} else {
		m_data.m_array.m_data = array;
	}
}

void CompilerExpr::Value::completeDelete() {
	if ( TYPE_SYMBOL == m_type ) {
		delete[] m_data.m_symbol;
	} else if ( TYPE_ARRAY == m_type ) {
		free(m_data.m_array.m_data);
	} else if ( TYPE_EXPR == m_type ) {
		m_data.m_expr->completeDelete();
	}
}

CompilerExpr::CompilerExpr() {
	m_operator = OPER_NONE;

	m_next = NULL;
	m_prev = NULL;
}

CompilerExpr::CompilerExpr(Value value) {
	m_lValue = value;
	m_operator = OPER_NONE;

	m_next = NULL;
	m_prev = NULL;
}

CompilerExpr::CompilerExpr(Value lValue, Operator oper, Value rValue) {
	m_lValue = lValue;
	m_operator = oper;
	m_rValue = rValue;

	m_next = NULL;
	m_prev = NULL;
}

CompilerExpr::CompilerExpr(Value lValue, char oper, Value rValue) {
	m_lValue = lValue;
	m_operator = Operator(oper);
	m_rValue = rValue;

	m_next = NULL;
	m_prev = NULL;
}


CompilerExpr * CompilerExpr::first() {
	CompilerExpr * expr = this;
	while ( NULL != expr->m_prev ) {
		expr = expr->m_prev;
	}
	return expr;
}

CompilerExpr * CompilerExpr::addLink(CompilerExpr * next) {
	if ( NULL == next ) {
		return this;
	}

	next = next->first();

	CompilerExpr * expr = this;
	while ( NULL != expr->m_next ) {
		expr = expr->m_next;
	}
	expr->m_next = next;
	next->m_prev = expr;

	return next;
}

void CompilerExpr::completeDelete() {
	m_lValue.completeDelete();
	m_rValue.completeDelete();
	if ( NULL != m_next ) {
		m_next->completeDelete();
	}
	delete this;
}

std::ostream & operator << (std::ostream & out, const CompilerExpr::Value & val) {
	switch ( val.m_type ) {
		case CompilerExpr::Value::TYPE_EMPTY:
			out << "<EMPTY>";
			break;
		case CompilerExpr::Value::TYPE_INT:
			out << std::dec << val.m_data.m_integer;
			break;
		case CompilerExpr::Value::TYPE_REAL:
			out << std::scientific << val.m_data.m_real;
			break;
		case CompilerExpr::Value::TYPE_EXPR:
			out << val.m_data.m_expr;
			break;
		case CompilerExpr::Value::TYPE_SYMBOL:
			out << "'" << val.m_data.m_symbol << "'";
			break;
		case CompilerExpr::Value::TYPE_ARRAY:
			out << "{" << std::dec << val.m_data.m_array.m_size << "}:\"";
			for ( int i = 0; i < val.m_data.m_array.m_size; i++ ) {
				if ( 0 == isprint(int(val.m_data.m_array.m_data[i])) ) {
					if ( val.m_data.m_array.m_data[i] < 16 ) {
						out << "\\x0";
					} else {
						out << "\\x";
					}
					out << std::hex << int(val.m_data.m_array.m_data[i]) << std::dec;
				} else {
					out << val.m_data.m_array.m_data[i];
				}
			}
			out << "\"";
			break;
	}
	return out;
}

std::ostream & operator << (std::ostream & out, const CompilerExpr::Operator & opr) {
	switch ( opr ) {
		case CompilerExpr::OPER_NONE:
			break;
		case CompilerExpr::OPER_ADD:
			out << "+";
			break;
		case CompilerExpr::OPER_SUB:
			out << "-";
			break;
		case CompilerExpr::OPER_MULT:
			out << "*";
			break;
		case CompilerExpr::OPER_DIV:
			out << "/";
			break;
		case CompilerExpr::OPER_DOT:
			out << ".";
			break;
		case CompilerExpr::OPER_CALL:
			out << "<CALL>";
			break;
	}
	return out;
}

std::ostream & operator << (std::ostream & out, const CompilerExpr * expr) {
	if ( CompilerExpr::OPER_NONE == expr->m_operator ) {
		out << "(" << expr->m_lValue << ")";
	} else {
		out << "(" << expr->m_lValue << expr->m_operator << expr->m_rValue << std::string(")");
	}
	if ( NULL != expr->m_next ) {
		out << " | " << expr->m_next;
	}
	return out;
}
