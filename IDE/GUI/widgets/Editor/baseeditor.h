#ifndef BASEEDITOR_H
#define BASEEDITOR_H


#include <QSplitter>
#include <QMenu>
#include "codeedit.h"


class BaseEditor : public QSplitter
{
    Q_OBJECT
    public:
        BaseEditor(QWidget *parent, CodeEdit *edit, bool delCodeEdit);

    private slots:
        void split(Qt::Orientation orient, int line);
        
    private:

        bool isSplit;
        BaseEditor *next; 
        BaseEditor *next2;
        CodeEdit *codeEdit;
        QMenu *basePopup;
     
};


#endif
