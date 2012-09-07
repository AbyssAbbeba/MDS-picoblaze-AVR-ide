#ifndef BASEEDITOR_H
#define BASEEDITOR_H


#include <QSplitter>
#include <QMenu>
#include "codeedit.h"

class CodeEdit;


class BaseEditor : public QSplitter
{
    Q_OBJECT
    public:
        BaseEditor(QWidget *parent, CodeEdit *edit, bool delCodeEdit);
        CodeEdit* getCodeEdit();

    private slots:
        void split(Qt::Orientation orient, int line);
        void unsplit();
        
    private:
        void connectCodeEdits(CodeEdit* editor1, CodeEdit* editor2);
        void connectBaseEditors(BaseEditor* editor1, BaseEditor* editor2);

        bool isSplit;
        BaseEditor *next; 
        BaseEditor *next2;
        CodeEdit *codeEdit;
        QList<CodeEdit*> connectedCodeEdits;
        QMenu *basePopup;
     
};


#endif
