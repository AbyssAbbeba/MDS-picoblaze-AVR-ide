/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */



#include <QtGui>
#include "baseeditor.h"


BaseEditor::BaseEditor(QWidget *parent, WDockManager *dockParent, CodeEdit *edit, bool delCodeEdit)
    : QSplitter(parent)
{
    if (dockParent != NULL)
    {
        this->dockMan = dockParent;
        connect(this, SIGNAL(focusIn(CodeEdit*)), this->dockMan, SLOT(changeActiveCodeEdit(CodeEdit*)));
        qDebug() << "BaseEditor: created and connected";
    }
    else
        this->dockMan = NULL;
    this->codeEdit = new CodeEdit(this, false, edit->getName(), edit->getPath());
    this->codeEdit->getTextEdit()->setPlainText(edit->getTextEdit()->toPlainText());
    if (delCodeEdit == true)
    {
        delete edit;
        edit=NULL;
    }
    this->isSplit = false;
    connect(codeEdit, SIGNAL(splitSignal(Qt::Orientation, int)), this, SLOT(split(Qt::Orientation, int)));
    connect(codeEdit, SIGNAL(CodeEditChanged(CodeEdit*)), this, SLOT(reconnect(CodeEdit*)));
}


void BaseEditor::split(Qt::Orientation orient, int line)
{
    qDebug() << "BaseEditor: split signal recieved";
    isSplit = true;
    this->setOrientation(orient);
    next = new BaseEditor(this, dockMan, codeEdit, false);
    next2 = new BaseEditor(this, dockMan, codeEdit, true);
    connectCodeEdits(next->getCodeEdit(), next2->getCodeEdit());
    qDebug() << "BaseEditor: connecting code edits";
    for (int i=0; i<connectedCodeEdits.count(); i++)
    {
        connectCodeEdits(next->getCodeEdit(), connectedCodeEdits.at(i));
        connectCodeEdits(next2->getCodeEdit(), connectedCodeEdits.at(i));
    }
    qDebug() << "BaseEditor: codeEdits connected";
    connectBaseEditors(next, next2);
    this->addWidget(next);
    this->addWidget(next2);
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
    if (editor1 != NULL && editor2 != NULL)
    {
        connect(editor1->getTextEdit(), SIGNAL(textChanged()), editor1, SLOT(updateTextSlotOut()));
        connect(editor2->getTextEdit(), SIGNAL(textChanged()), editor2, SLOT(updateTextSlotOut()));
        connect(editor1, SIGNAL(updateText(const QString&)), editor2, SLOT(updateTextSlotIn(const QString&)));
        connect(editor2, SIGNAL(updateText(const QString&)), editor1, SLOT(updateTextSlotIn(const QString&)));
        connect(editor1->getTextEdit(), SIGNAL(breakpoint(int)), editor2, SLOT(manageBreakpointEmit(int)));
        connect(editor2->getTextEdit(), SIGNAL(breakpoint(int)), editor1, SLOT(manageBreakpointEmit(int)));
    }
}

void BaseEditor::connectBaseEditors(BaseEditor* editor1, BaseEditor* editor2)
{
    editor1->connectedCodeEdits.append(editor2->getCodeEdit());
    editor2->connectedCodeEdits.append(editor1->getCodeEdit());
}



void BaseEditor::reconnect(CodeEdit* editor)
{
    qDebug() << "BaseEditor: reconnect";
    connectedCodeEdits.clear();
    BaseEditor* parentBase = (BaseEditor*)editor->getParent();
    connectCodeEdits(this->codeEdit, editor);
    for (int i=0; i<parentBase->connectedCodeEdits.count(); i++)
    {
        this->connectedCodeEdits.append(parentBase->connectedCodeEdits.at(i));
        connectCodeEdits(this->codeEdit, parentBase->connectedCodeEdits.at(i));
    }
    this->connectedCodeEdits.append(editor);
    //parentBase->connectedCodeEdits.append(this->codeEdit);
    qDebug() << "BaseEditor: reconnect done";
}



void BaseEditor::focusIn()
{
        emit focusIn(codeEdit);
        qDebug() << "BaseEditor: emit focus in";
}
