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
 * @file analys_struct.h
 */


#ifndef ANALYS_STRUCT_H
#define ANALYS_STRUCT_H

#include <QString>

class AnalysStruct
 {
 
 QString name;
 QString type;
 int line;
 
 public:
    AnalysStruct(int l, QString n, QString t) {
      line = l;
      type = t;
      name = n;
    };
    
    QString getType () const {
      return type;
    };
    
    QString getName () const {
      return name;
    };
    
    int getLine () const {
      return line;
    };
    
 };

#endif
