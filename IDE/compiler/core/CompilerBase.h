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
 * @file CompilerBase.h
 */

#ifndef COMPILERBASE_H
#define COMPILERBASE_H

/**
 * @brief
 * @ingroup Compiler
 * @class CompilerBase
 */
class CompilerBase {
public:
	virtual ~CompilerBase() {};

	enum MessageType {
		MT_GENERAL,
		MT_ERROR,
		MT_WARNING,
		MT_REMARK
	};

	enum TargetArch {
		TA_AVR8,
		TA_PIC8,
		TA_MCS51
	};

	enum LangId {
		LI_ASM
	};

	struct SourceLocation {
		SourceLocation(int fileNumber, int lineStart, int lineEnd, int colStart, int colEnd) {
			m_fileNumber = fileNumber;
			m_lineStart = lineStart;
			m_lineEnd = lineEnd;
			m_colStart = colStart;
			m_colEnd = colEnd;
		}

		int m_fileNumber;
		int m_lineStart;
		int m_lineEnd;
		int m_colStart;
		int m_colEnd;
	};

protected:
	CompilerBase() {};
};

#endif // COMPILERBASE_H
