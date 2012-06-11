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

#include "CompilerCore.h"

#include <sstream>
#include <cstdio>
#include <iostream> // DEBUG

#include <QObject> // Used for i18n only

CompilerCore::CompilerCore() {
}

CompilerCore::~CompilerCore() {
	if ( NULL != m_rootStatement ) {
		m_rootStatement->completeDelete();
	}
}

bool CompilerCore::parseSourceFile(CompilerCore::TargetArch arch, const std::string & filename) {
	m_success = true;
	m_rootStatement = NULL;

	FILE * sourceFile = fopen(filename.c_str(), "r");
	yyscan_t yyscanner; // Pointer to the lexer context

	setFileName(filename);

	if ( NULL == sourceFile ) {
		message(QObject::tr("Unable to open: ").toStdString() + m_filename, MT_ERROR);
		return false;
	}

	switch ( arch ) {
		case TA_AVR8:
			avr8lexer_lex_init_extra(this, &yyscanner);
			avr8lexer_set_in(sourceFile, yyscanner);
			avr8parser_parse(yyscanner, this);
			avr8lexer_lex_destroy(yyscanner);
			break;
		case TA_PIC8:
			pic8lexer_lex_init_extra(this, &yyscanner);
			pic8lexer_set_in(sourceFile, yyscanner);
			pic8parser_parse(yyscanner, this);
			pic8lexer_lex_destroy(yyscanner);
			break;
		case TA_MCS51:
			mcs51lexer_lex_init_extra(this, &yyscanner);
			mcs51lexer_set_in(sourceFile, yyscanner);
			mcs51parser_parse(yyscanner, this);
			mcs51lexer_lex_destroy(yyscanner);
			break;
	}

	fclose(sourceFile);
	return m_success;
}

void CompilerCore::parserMessage(SourceLocation location, MessageType type, const std::string & text) {
	std::stringstream msgText;

	msgText	<< QObject::tr("Syntax error in ").toStdString()
		<< m_filename << ":"
		<< location.m_lineStart << ":"
		<< location.m_colStart << ": "
		<< text;

	message(msgText.str(), type);
}

void CompilerCore::lexerMessage(SourceLocation location, MessageType type, const std::string & text) {
	std::stringstream msgText;

	msgText	<< QObject::tr("Lexical error in ").toStdString()
		<< m_filename << ":"
		<< location.m_lineStart << ":"
		<< location.m_colStart << ": "
		<< text;

	message(msgText.str(), type);
}

void CompilerCore::setFileName(const std::string & filename) {
	int idx = getFileNumber(filename);

	m_filename = filename;
	if ( -1 != idx ) {
		m_fileNumber = idx;
	} else {
		m_fileNumber = m_fileNames.size();
		m_fileNames.push_back(filename);
	}
}

void CompilerCore::pushFileName(const std::string & filename) {
	setFileName(filename);
	m_fileNameStack.push(filename);
}
void CompilerCore::popFileName() {
	setFileName(m_fileNameStack.top());
	m_fileNameStack.pop();
}

int CompilerCore::getFileNumber() const {
	return m_fileNumber;
}

int CompilerCore::getFileNumber(const std::string & filename) const {
	int result = -1;
	for ( uint i = 0; i < m_fileNames.size(); i++ ) {
		if ( filename == m_fileNames[i] ) {
			result = i;
		}
	}
	return result;
}

void CompilerCore::syntaxAnalysisComplete(CompilerStatement * codeTree) {
	m_rootStatement = codeTree;
	if ( NULL != m_rootStatement ) {
		m_rootStatement = m_rootStatement->first();
	}

	std::cout << m_rootStatement;
}
