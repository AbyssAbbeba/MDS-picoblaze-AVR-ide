/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CompileInfo
 * @file compileinfo.cpp
 */


#include <QtGui>
#include "compileinfo.h"

CompileInfo::CompileInfo(QWidget *parent)
    : QTextEdit(parent)
{
    this->setFont(QFont("Andale Mono", 10));
    this->setMouseTracking(true);
    //this->installEventFilter(this);
    //this->setWordWrap();
}


void CompileInfo::mouseMoveEvent(QMouseEvent *e)
{
    //qDebug() << "CompileInfo: mouseMoveEvent";
    QTextCursor cur = this->cursorForPosition(e->pos());
    //qDebug() << "column:" << cur.positionInBlock();
    cur.select(QTextCursor::BlockUnderCursor);
    QStringList list = cur.selectedText().split(':');
    //QString line = list.at(2).
    bool ok = false;
    if (list.size() > 1)
    {
        //int line = list.at(1).section('.',0,0).toInt(&ok, 10);
        if (ok == true)
        {
            QWidget *viewport = this->viewport();
            viewport->setCursor(Qt::PointingHandCursor);
            this->setViewport(viewport);

            int start = cur.selectionStart();
            int end = cur.selectionEnd();
            cur.clearSelection();
            cur.setPosition(start+1);
            cur.setPosition(end, QTextCursor::KeepAnchor);
            //QTextCharFormat format = cur.blockCharFormat();
            //format.setFontUnderline(true);
            //format.setFontItalic(true);
            //cur.setBlockCharFormat(format);
            this->setTextCursor(cur);

            /*prevCur.select(QTextCursor::LineUnderCursor);
            QTextCharFormat prevFormat = prevCur.blockCharFormat();
            prevFormat.setFontUnderline(false);
            prevFormat.setFontItalic(false);
            prevCur.setBlockCharFormat(prevFormat);
            prevCur.clearSelection();
            setTextCursor(prevCur);*/
            
            prevCur = cur;
        }
        else
        {
            QWidget *viewport = this->viewport();
            viewport->setCursor(Qt::IBeamCursor);
            this->setViewport(viewport);
        }
    }
    else
    {
        QWidget *viewport = this->viewport();
        viewport->setCursor(Qt::IBeamCursor);
        this->setViewport(viewport);
    }
}


void CompileInfo::mouseDoubleClickEvent(QMouseEvent *e)
{
    //qDebug() << "CompileInfo: doubleclick";
    QTextCursor cur = this->cursorForPosition(e->pos());
    cur.select(QTextCursor::BlockUnderCursor);
    //QString selection = cur.selectedText();
    QStringList list = cur.selectedText().split(':');
    //QString line = list.at(2).
    bool ok = false;
    if (list.size() > 1)
    {
        int line = list.at(1).section('.',0,0).toInt(&ok, 10);
        if (ok == true)
        {
            emit errorClicked(list.at(0), line);
        }
    }
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
    //this->setTextColor(Qt::black);
    QString color = "rgb(0,0,0)";
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
            //this->setTextColor(Qt::darkRed);
            color = "rgb(230,0,0)";
            break;
        }
        case CompilerBase::MessageType::MT_WARNING:
        {
            //this->setTextColor(Qt::red);
            color = "rgb(30,0,0)";
            break;
        }
        case CompilerBase::MessageType::MT_REMARK:
        {
            //this->setTextColor(Qt::blue);
            color = "rgb(0,0,255)";
            break;
        }
    }
    this->insertHtml("<a style=\"color:" + color + "\">" + text + "</a>");
    this->insertPlainText("\n");
    //this->append(text);
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
