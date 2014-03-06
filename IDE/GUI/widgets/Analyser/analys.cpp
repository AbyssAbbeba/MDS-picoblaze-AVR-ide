/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file analys.cpp
 */


#include <QtGui>
#include <QRegExp>
#include <QTextBlock>
#include <QStringList>
#include <QString>

#include "analys.h"

Analys::Analys(const QTextDocument &text) {
    code = text.clone();
    reg.setMinimal(true);
}

QList<AnalysStruct *> *Analys::getVar() {
  QList<AnalysStruct *> *retList = new QList<AnalysStruct *>;
  QStringList strList;
  QTextBlock block = code->firstBlock();
  QStringList::iterator itList;
  int pos = 0;
  QString name;
  QString type;
  QString names;
  //AnalysStruct *tmp;
  QRegExp separeteEqual("([^=]+)");
  //              |dat.typ--------------|_____|promenne,inic-|______|;                              
  //reg.setPattern("([a-zA-Z][^ *=;{}()]+)[\\s]+([^;=()]+(=[^=,]+)?,?)+[\\s]*;");
  reg.setPattern("([a-zA-Z][^* =;,{}()]*)[\\s]+(([^=();]+(=[^=;]+)?,?)+)[\\s]*;");

   while (block.isValid()) { 
        pos = 0;
        while ((pos = reg.indexIn(block.text(), pos)) != -1) {
            type = reg.cap(1);
            names = QString(reg.cap(2)).remove(" ");
            strList = QString(names).split(",", QString::SkipEmptyParts);
	
            itList = strList.begin();
            while (itList != strList.end()) {
                if (separeteEqual.indexIn(*itList) != -1) {
                    name = separeteEqual.cap(1);
                    AnalysStruct *tmp = new AnalysStruct(block.blockNumber(), name, type);
                    retList->append(tmp);
                }
                ++itList;  
            }
        pos += reg.matchedLength();
        }
        block = block.next();
    } 
  
  return retList; 
}

QList<AnalysStruct *> *Analys::getFunc() {
  QList<AnalysStruct *> *retList = new QList<AnalysStruct *>;
  QTextBlock block = code->firstBlock();
  QString type;
  //tmp;
  int pos = 0;

  
  reg.setPattern("([a-zA-Z][^* =;,{}()]*)[\\s]+([^;,=()]+[\\s]*[(][^;()]*[)])");
  
     while (block.isValid()) { 
      pos = 0;
      while ((pos = reg.indexIn(block.text(), pos)) != -1) {
		AnalysStruct *tmp = new AnalysStruct(block.blockNumber(), reg.cap(2), reg.cap(1));
		retList->append(tmp);
		pos += reg.matchedLength();
      }
      block = block.next();
  } 
  
  return retList;
}
