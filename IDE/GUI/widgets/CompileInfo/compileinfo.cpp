/**
 * @brief Modif. QTextEdit
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
#include "compileinfo.h"

CompileInfo::CompileInfo(QWidget *parent)
    : QTextEdit(parent)
{
    this->setFont(QFont("Andale Mono", 10));
    //this->setWordWrap();
}


void CompileInfo::mouseDoubleClickEvent(QMouseEvent *e)
{
    QTextCursor cur = this->cursorForPosition(e->pos());
}


void CompileInfo::appendMessage(QString text, CompilerBase::MessageType type)
{
    this->setTextColor(Qt::black);
    switch (type)
    {
        case CompilerBase::MessageType::MT_INVALID:
        {
            return;
        }
        case CompilerBase::MessageType::MT_GENERAL:
        {
            break;
        }
        case CompilerBase::MessageType::MT_ERROR:
        {
            this->setTextColor(Qt::darkRed);
            break;
        }
        case CompilerBase::MessageType::MT_WARNING:
        {
            this->setTextColor(Qt::red);
            break;
        }
        case CompilerBase::MessageType::MT_REMARK:
        {
            this->setTextColor(Qt::blue);
            break;
        }
    }
    this->append(text);
}


void CompileInfo::setFinished(bool status)
{
    if (status == true)
    {
        this->setTextColor(Qt::darkGreen);
        if (this->document()->isEmpty())
        {
            this->append("Compilation Finished");
        }
        else
        {
            this->append("\nCompilation Finished");
        }
    }
    else
    {
        this->setTextColor(Qt::darkRed);
        if (this->document()->isEmpty())
        {
            this->append("Compilation Failed");
        }
        else
        {
            this->append("\nCompilation Failed");
        }
    }
}
