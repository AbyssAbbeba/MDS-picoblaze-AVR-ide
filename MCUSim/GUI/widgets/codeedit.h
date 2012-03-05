#ifndef CODEEDIT_H
#define CODEEDIT_H


#include <QPlainTextEdit>


//editor kodu s pouzitim QSyntaxHighlight a pocitanim radku
class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT   
    public:
        CodeEdit(QWidget *parent);

    private:
        
};


#endif
