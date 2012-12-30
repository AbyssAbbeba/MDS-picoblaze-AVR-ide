#include "analyserwidget.h"
#include <QtGui>

AnalyserWidget::AnalyserWidget( QWidget * parent )
: QListWidget ( parent ) {
}

void AnalyserWidget::fill ( QList<AnalysStruct*> *list ) {
    
  this->clear();
  QString lineNum;  
  QString line;
  
  for (int i = 0; i < list->count(); i++) {
    AnalysStruct * s = list->at(i);
    lineNum.setNum(s->getLine()+1);
    line = s->getType() + "   " + s->getName() + "\t[" + lineNum + "]";
    //this->addItem(s->getType());
    //this->addItem(s->getName());
    //lineNum.setNum(s->getLine());
    //this->addItem(lineNum);
    this->addItem(line); 
     
  }  
}

