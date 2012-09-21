/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file baseeditor.h
 */


#ifndef BASEEDITOR_H
#define BASEEDITOR_H


#include <QSplitter>
#include <QMenu>
#include "codeedit.h"
#include "../DockManager/wdockmanager.h"

class CodeEdit;
class WDockManager;


/**
 * @brief
 * @ingroup GUI
 * @class BaseEditor
 */
class BaseEditor : public QSplitter
{
    Q_OBJECT
    public:
        BaseEditor(QWidget *parent, WDockManager *dockParent, CodeEdit *edit, bool delCodeEdit);
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
        WDockManager *dockMan;
     
};


#endif
