/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file errordlg.h
 */



#ifndef ERRORDLG_H
#define ERRORDLG_H

#define ERR_OPENFILE 2000
#define ERR_XML_CONTENT 2001
#define ERR_XML_ASSIGN 2002

#include <QMessageBox>

void error(int errCode);

#endif
