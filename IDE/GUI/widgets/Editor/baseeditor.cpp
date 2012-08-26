#include <QtGui>
#include "baseeditor.h"


BaseEditor::BaseEditor(QWidget *parent, CodeEdit *edit, bool delCodeEdit)
    : QSplitter(parent)
{
    this->codeEdit = new CodeEdit(this, false, edit->getName(), edit->getPath());
    this->codeEdit->getTextEdit()->setPlainText(edit->getTextEdit()->toPlainText());
    if (delCodeEdit == true)
        delete edit;
    this->isSplit = false;
    connect(codeEdit, SIGNAL(splitSignal(Qt::Orientation, int)), this, SLOT(split(Qt::Orientation, int)));
}


void BaseEditor::split(Qt::Orientation orient, int line)
{
    isSplit = true;
    this->setOrientation(orient);
    next = new BaseEditor(this, codeEdit, false);
    next2 = new BaseEditor(this, codeEdit, true);
    this->addWidget(next);
    this->addWidget(next2);
}

