/**
 * @brief Modif. QTextEdit
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2013
 * @ingroup GUI
 * @file compileinfo.h
 */



#ifndef COMPILEINFO_H
#define COMPILEINFO_H


#include <QTextEdit>
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

        /*WTextEdit & operator<< (QString const &str)
        {
            append(str);
            return *this;
        }*/


    signals:
        void errorClicked(int line);


    protected:
        virtual void mouseDoubleClickEvent(QMouseEvent *e);
};


#endif
