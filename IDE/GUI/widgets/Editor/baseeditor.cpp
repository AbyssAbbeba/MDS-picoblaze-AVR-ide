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
    this->dockMan = dockParent;
    //this->codeEdit = edit;
    this->codeEdit = new CodeEdit(this, false, edit->getName(), edit->getPath());
    this->codeEdit->getTextEdit()->setPlainText(edit->getTextEdit()->toPlainText());
    if (delCodeEdit == true)
        delete edit;
    this->isSplit = false;
    connect(codeEdit, SIGNAL(splitSignal(Qt::Orientation, int)), this, SLOT(split(Qt::Orientation, int)));
    //this->setFocusPolicy(Qt::ClickFocus);
}


void BaseEditor::split(Qt::Orientation orient, int line)
{
    isSplit = true;
    this->setOrientation(orient);
    next = new BaseEditor(this, dockMan, codeEdit, false);
    next2 = new BaseEditor(this, dockMan, codeEdit, true);
    connectCodeEdits(next->getCodeEdit(), next2->getCodeEdit());
    for (int i=0; i<connectedCodeEdits.count(); i++)
    {
        connectCodeEdits(next->getCodeEdit(), connectedCodeEdits.at(i));
        connectCodeEdits(next2->getCodeEdit(), connectedCodeEdits.at(i));
    }
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
    connect(editor1->getTextEdit(), SIGNAL(textChanged()), editor1, SLOT(updateTextSlotOut()));
    connect(editor2->getTextEdit(), SIGNAL(textChanged()), editor2, SLOT(updateTextSlotOut()));
    connect(editor1, SIGNAL(updateText(const QString&)), editor2, SLOT(updateTextSlotIn(const QString&)));
    connect(editor2, SIGNAL(updateText(const QString&)), editor1, SLOT(updateTextSlotIn(const QString&)));
}

void BaseEditor::connectBaseEditors(BaseEditor* editor1, BaseEditor* editor2)
{
    editor1->connectedCodeEdits.append(editor2->getCodeEdit());
    editor2->connectedCodeEdits.append(editor1->getCodeEdit());
}

