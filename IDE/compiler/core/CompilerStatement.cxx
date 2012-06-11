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

#include "CompilerStatement.h"

CompilerStatement::~CompilerStatement() {
}

CompilerStatement::CompilerStatement() {
	m_type = StatementTypes::EMPTY_STATEMENT;
	m_userData = 0;
	m_serialNumber = -1;
	m_prev = NULL;
	m_next = NULL;
	m_branch = NULL;
	m_args = NULL;
}

CompilerStatement::CompilerStatement(CompilerBase::SourceLocation location, StatementTypes::StatementType type, CompilerExpr * args) {
	m_type = type;
	m_location = location;

	m_userData = 0;
	m_serialNumber = -1;

	m_prev = NULL;
	m_next = NULL;
	m_branch = NULL;
	if ( NULL != args ) {
		m_args = args->first();
	} else {
		m_args = NULL;
	}
}

CompilerStatement * CompilerStatement::createBranch(CompilerStatement * branch) {
	if ( NULL == branch ) {
		return this;
	}

	m_branch = branch->first();
	return this;
}

CompilerStatement * CompilerStatement::first() {
	CompilerStatement * stmt = this;
	while ( NULL != stmt->m_prev ) {
		stmt = stmt->m_prev;
	}
	return stmt;
}

CompilerStatement * CompilerStatement::addLink(CompilerStatement * next) {
	if ( NULL == next ) {
		return this;
	}

	next = next->first();

	CompilerStatement * stmt = this;
	while ( NULL != stmt->m_next ) {
		stmt = stmt->m_next;
	}
	stmt->m_next = next;
	next->m_prev = stmt;

	return next;
}

void CompilerStatement::completeDelete() {
	if ( NULL != m_args ) {
		m_args->completeDelete();
	}
	if ( NULL != m_branch ) {
		m_branch->completeDelete();
	}
	if ( NULL != m_next ) {
		m_next->completeDelete();
	}
	delete this;
}

std::ostream & CompilerStatement::print(std::ostream & out, int level, std::string lineString) const {
	for ( int i = 0; i < level; i++ ) {
		if ( '0' == lineString[i] ) {
			out << "    ";
		} else {
			out << "  │ ";
		}
	}
	if ( NULL == m_prev ) {
		if ( NULL == m_next ) {
			lineString[level] = '0';
		} 
		out << "  █─ ";
	} else {
		if ( NULL == m_next ) {
			out << "  └─ ";
			lineString[level] = '0';
		} else {
			out << "  ├─ ";
		}
	}

	out << m_type;
	if ( NULL != m_args ) {
		out << " [ ";
		out << m_args;
		out << " ]";
	}
	out << "\n";

	if ( NULL != m_branch ) {
		lineString += "1";
		m_branch->print(out, level + 1, lineString);
	}

	if ( NULL != m_next ) {
		m_next->print(out, level, lineString);
	}

	return out;
}

std::ostream & operator << (std::ostream & out, const CompilerStatement * stmt) {
// 	out << "  ▄ \n";
	return stmt->print(out);
}
