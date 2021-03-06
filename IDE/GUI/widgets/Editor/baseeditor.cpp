/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup BaseEditor
 * @file baseeditor.cpp
 */


#include <QtGui>
#include "baseeditor.h"
#include "wtextedit.h"
#include "codeedit.h"
#include "../DockManager/wdockmanager.h"


BaseEditor::BaseEditor(QWidget *parent, WDockManager *dockParent, CodeEdit *edit, bool delCodeEdit)
    : QSplitter(parent)
{
    //qDebug() << "BaseEditor: BaseEditor()";
    if (dockParent != NULL)
    {
        this->dockMan = dockParent;
        //connect(this, SIGNAL(focusIn(CodeEdit*)), this->dockMan, SLOT(changeActiveCodeEdit(CodeEdit*)));
        //qDebug() << "BaseEditor: created and connected";
    }
    else
    {
        this->dockMan = NULL;
    }
    //qDebug() << "BaseEditor: creating codeEdit";
    this->codeEdit = new CodeEdit(this, false, edit->getName(), edit->getPath(), edit->getParentCodeEdit());
    //qDebug() << "BaseEditor: codeEdit created";
    if (edit->getTextEdit()->toPlainText() != "")
    {
        this->codeEdit->getTextEdit()->setPlainText(edit->getTextEdit()->toPlainText());
    }
    //qDebug() << "BaseEditor: text set";
    if (delCodeEdit == true)
    {
        delete edit;
        edit=NULL;
    }
    this->isSplit = false;
    connect(codeEdit,
            SIGNAL(splitSignal(Qt::Orientation, int)),
            this,
            SLOT(split(Qt::Orientation, int))
           );
    connect(codeEdit,
            SIGNAL(CodeEditChanged(CodeEdit*)),
            this,
            SLOT(reconnect(CodeEdit*))
           );
    connectCodeEdits(this->codeEdit, this->codeEdit->getParentCodeEdit());
    //this->show();
    //qDebug() << "BaseEditor: codeedit connected";
    //qDebug() << "BaseEditor: return BaseEditor()";
}


void BaseEditor::split(Qt::Orientation orient, int line)
{
    //qDebug() << "BaseEditor: split()";
    //qDebug() << "BaseEditor: split signal recieved";
    isSplit = true;
    this->setOrientation(orient);
    next = new BaseEditor(this, dockMan, codeEdit, false);
    next->getCodeEdit()->getTextEdit()->setPosition(codeEdit->getTextEdit()->getPosition());
    next2 = new BaseEditor(this, dockMan, codeEdit, true);
    next2->getCodeEdit()->getTextEdit()->setPosition(next->getCodeEdit()->getTextEdit()->getPosition());
    connectCodeEdits(next->getCodeEdit(), next->getCodeEdit()->getParentCodeEdit());
    connectCodeEdits(next2->getCodeEdit(), next2->getCodeEdit()->getParentCodeEdit());
    this->addWidget(next);
    this->addWidget(next2);
    //qDebug() << "BaseEditor: return split()";
}

void BaseEditor::unsplit()
{
    
}


CodeEdit* BaseEditor::getCodeEdit()
{
    return codeEdit;
}


void BaseEditor::connectCodeEdits(CodeEdit* editor1, CodeEdit* editor2)
{
    //qDebug() << "BaseEditor: connectCodeEdits()";
    if (editor1 != NULL && editor2 != NULL)
    {

        //if (editor1->getParentCodeEdit() != editor2)
        //{
            /*disconnect(editor1->getTextEdit(),
                       SIGNAL(bookmark(int)),
                       editor1->getParentCodeEdit(),
                       SLOT(manageBookmarkEmit(int))
                      );
            disconnect(editor1->getParentCodeEdit()->getTextEdit(),
                       SIGNAL(bookmark(int)),
                       editor1,
                       SLOT(manageBookmarkEmit(int))
                      );
            disconnect(editor1->getTextEdit(),
                       SIGNAL(breakpoint(int)),
                       editor1->getParentCodeEdit(),
                       SLOT(manageBreakpointEmit(int))
                      );
            disconnect(editor1->getParentCodeEdit()->getTextEdit(),
                       SIGNAL(breakpoint(int)),
                       editor1,
                       SLOT(manageBreakpointEmit(int))
                      );
            disconnect(editor1->getParentCodeEdit(),
                       SIGNAL(updateText(const QString&, int, CodeEdit*)),
                       editor1,
                       SLOT(updateTextSlotIn(const QString&,int, CodeEdit*))
                      );
            disconnect(editor1,
                       SIGNAL(updateText(const QString&, int, CodeEdit*)),
                       editor1->getParentCodeEdit(),
                       SLOT(updateTextSlotIn(const QString&, int, CodeEdit*))
                      );
            disconnect(editor1->getParentCodeEdit(),
                       SIGNAL(updateRemoveSelection(int, int, CodeEdit*)),
                       editor1,
                       SLOT(selectionRemovedIn(int, int, CodeEdit*))
                      );
            disconnect(editor1,
                       SIGNAL(updateText(const QString&, int, CodeEdit*)),
                       editor1->getParentCodeEdit(),
                       SLOT(selectionRemovedIn(int, int, CodeEdit*))
                      );*/

            editor1->setParentCodeEdit(editor2);
        //}

        //connect(editor1->getTextEdit(), SIGNAL(textChanged()), editor1, SLOT(updateTextSlotOut()));
        //connect(editor2->getTextEdit(), SIGNAL(textChanged()), editor2, SLOT(updateTextSlotOut()));
        /*connect(editor1,
                SIGNAL(updateText(const QString&, int, CodeEdit*)),
                editor2,
                SLOT(updateTextSlotIn(const QString&, int, CodeEdit*))
               );
        connect(editor2,
                SIGNAL(updateText(const QString&, int, CodeEdit*)),
                editor1,
                SLOT(updateTextSlotIn(const QString&, int, CodeEdit*))
               );
        connect(editor1,
                SIGNAL(updateRemoveSelection(int, int, CodeEdit*)),
                editor2,
                SLOT(selectionRemovedIn(int, int, CodeEdit*))
               );
        connect(editor2,
                SIGNAL(updateRemoveSelection(int, int, CodeEdit*)),
                editor1,
                SLOT(selectionRemovedIn(int, int, CodeEdit*))
               );
        connect(editor1->getTextEdit(),
                SIGNAL(bookmark(int)),
                editor2,
                SLOT(manageBookmarkEmit(int))
               );
        connect(editor2->getTextEdit(),
                SIGNAL(bookmark(int)),
                editor1,
                SLOT(manageBookmarkEmit(int))
               );
        connect(editor1->getTextEdit(),
                SIGNAL(breakpoint(int)),
                editor2,
                SLOT(manageBreakpointEmit(int))
               );
        connect(editor2->getTextEdit(),
                SIGNAL(breakpoint(int)),
                editor1,
                SLOT(manageBreakpointEmit(int))
               );*/
        
    }
    //qDebug() << "BaseEditor: return connectCodeEdits()";
}

/*void BaseEditor::connectBaseEditors(BaseEditor* editor1, BaseEditor* editor2)
{
    editor1->connectedCodeEdits.append(editor2->getCodeEdit());
    editor2->connectedCodeEdits.append(editor1->getCodeEdit());
}*/



void BaseEditor::reconnect(CodeEdit* editor)
{
    connectCodeEdits(this->codeEdit, editor->getParentCodeEdit());
}



void BaseEditor::focusIn()
{
    emit focusIn(codeEdit);
    //qDebug() << "BaseEditor: emit focus in";
}
