/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file errordlg.h
 */


#ifndef ERRORDLG_H
#define ERRORDLG_H

//error codes
//#define ERR_OPENFILE 2000
//#define ERR_XML_CONTENT 2001
//#define ERR_XML_ASSIGN 2002
//#define ERR_NO_PROJECT 2003


#include "../enums/enums.h"
#include <QString>

void error(int errCode, QString detail = QString());

#endif
