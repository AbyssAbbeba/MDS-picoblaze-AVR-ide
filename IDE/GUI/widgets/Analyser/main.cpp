#include "analys_struct.h"
#include "analys.h"
#include <QtDebug>
#include <QTextDocument>

int main() {
    QString code("int main() { \n int promena, fa, a = print(); \n } \n string getString() { } ");
//  QString code("int promena;");
  QTextDocument *doc = new QTextDocument(code);
  qDebug() << doc->toPlainText();
  
  Analys *vars = new Analys(*doc);
  QList<AnalysStruct *> *list = vars->getVar();
  AnalysStruct *tmp;
  qDebug() << "VARIABLES: " << endl;
  foreach (tmp, list) {
    qDebug() << "\t" << "Name: " << tmp->getName() << ", type: " << tmp->getType() << ", line: " << tmp->getLine() << endl;
  }
  
  list = vars->getFunc();
    qDebug() << "FUNCTIONS: " << endl;
    foreach (tmp, list) {
    qDebug() << "\t" << "Name: " << tmp->getName() << ", type: " << tmp->getType() << ", line: " << tmp->getLine() << endl;
  }
  
}
