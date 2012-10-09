/**
 * @brief Modif. QTextEdit
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file wtextedit.h
 */



#ifndef WTEXTEDIT_H
#define WTEXTEDIT_H


#include <QTextEdit>
#include "../Highlighter/highlighter.h"

class Highlighter;

/**
 * @brief QTextEdit with reimplemented eventFilter
 * @ingroup GUI
 * @class WTextEdit
 */
class WTextEdit : public QTextEdit
{
    Q_OBJECT   
    public:
        WTextEdit(QWidget *parent);
        void highlightCurrentLine();
        void highlightLine(int line);

    signals:
        void focusIn();
        void breakpoint(int line);

    private:
        Highlighter *highlighter;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
