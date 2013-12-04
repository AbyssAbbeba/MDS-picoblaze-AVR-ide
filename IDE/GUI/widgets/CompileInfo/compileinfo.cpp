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
    //this->installEventFilter(this);
    //this->setWordWrap();
}


void CompileInfo::mouseDoubleClickEvent(QMouseEvent *e)
{
    //qDebug() << "CompileInfo: doubleclick";
    QTextCursor cur = this->cursorForPosition(e->pos());
    cur.select(QTextCursor::LineUnderCursor);
    //QString selection = cur.selectedText();
    QStringList list = cur.selectedText().split(':');
    //QString line = list.at(2).
    emit errorClicked(list.at(0), list.at(1).section('.',0,0).toInt());
    //QString line;
    //QString wordBegin;
    //QString wordEnd;
    //qDebug() << "CompileInfo: selection: " << list.at(1).section('.',0,0).toInt();
}


/*void CompileInfo::keyPressEvent(QKeyEvent *e)
{
    qDebug() << "CompileInfo: key press event";
}

bool CompileInfo::eventFilter(QObject *target, QEvent *event)
{
    qDebug() << "CompileInfo: event";
    if (target == this && event->type() == QEvent::KeyPress)
    {
        return true;
    }
    else if (target == this && event->type() == QEvent::MouseButtonPress)
    {
        qDebug() << "CompileInfo: click event";
        QMouseEvent *mouse = static_cast<QMouseEvent *>(event);
        QTextCursor cur = this->cursorForPosition(mouse->pos());
        //cur.select(QTextCursor::BlockUnderCursor);
        //QString selection = cur.selectedText();
        return true;
    }
    else if (target == this && event->type() == QEvent::MouseButtonDblClick)
    {
        qDebug() << "CompileInfo: double click event";
        QMouseEvent *mouse = static_cast<QMouseEvent *>(event);
        QTextCursor cur = this->cursorForPosition(mouse->pos());
        cur.select(QTextCursor::BlockUnderCursor);
        QString selection = cur.selectedText();
        return true;
    }
    return QWidget::eventFilter(target, event);
}*/


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
