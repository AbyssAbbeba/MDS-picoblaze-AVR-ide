/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file compileinfo.h
 */


#ifndef COMPILEINFO_H
#define COMPILEINFO_H


#include <QTextEdit>
#include <QTextCursor>
#include "../../../compiler/core/CompilerBase.h"




/**
 * @brief QTextEdit with reimplemented eventFilter
 * @ingroup GUI
 * @class CompileInfo
 */
class CompileInfo : public QTextEdit
{
    Q_OBJECT   
    public:
        CompileInfo(QWidget *parent);
        void appendMessage(QString text, CompilerBase::MessageType type);
        void setFinished(bool status);
        void changeAsmMode(bool mdsAsm);
        void setProjectRelativePath(QString path);

        /*WTextEdit & operator<< (QString const &str)
        {
            append(str);
            return *this;
        }*/


    signals:
        void errorClicked(QString filepath, int line);

    private:
        QTextCursor prevCur;
        bool m_mode;
        QString m_projectPath;


    protected:
        //virtual bool eventFilter(QObject *target, QEvent *event);
        //virtual void keyPressEvent (QKeyEvent *e);
        virtual void mouseDoubleClickEvent(QMouseEvent *e);
        virtual void mouseMoveEvent(QMouseEvent *e);
};


#endif
