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
