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
 * @file analys.h
 */


#ifndef ANALYS_H
#define ANALYS_H

#include <QTextDocument>
#include <QRegExp>
#include "analys_struct.h"

class Analys
 {
 protected:
   QTextDocument *code; 
   QRegExp reg;
   
 public:
    Analys(const QTextDocument &text);
    QList<AnalysStruct *> *getVar();
    QList<AnalysStruct *> *getFunc();
 };

#endif
