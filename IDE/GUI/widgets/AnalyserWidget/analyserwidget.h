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
 * @file analyser_widget.h
 */


#ifndef ANALYSERWIDGET_H
#define ANALYSERWIDGET_H

#include <QListWidget>
#include <QWidget>
#include "../Analyser/analys.h"
#include "../Analyser/analys_struct.h"

class AnalyserWidget: public QListWidget {
  public:AnalyserWidget( QWidget * parent );
  
  void fill (QList<AnalysStruct*> *list);
};

#endif
