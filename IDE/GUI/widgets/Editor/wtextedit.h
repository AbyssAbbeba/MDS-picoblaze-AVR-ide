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
        WTextEdit(QWidget *parent, SourceType type);
        void highlightCurrentLine();
        void highlightLine(int line, QColor *color);
        void reloadHighlighter(SourceType type);
        void setPosition(int pos);
        int getPosition();
        void scrollToLine(int line);

        /*WTextEdit & operator<< (QString const &str)
        {
            append(str);
            return *this;
        }*/


    signals:
        void focusIn();
        void breakpoint(int line);
        void bookmark(int line);
        void textChangedSignal(const QString& text, int pos);

    private:
        Highlighter *highlighter;
        SourceType sourceType;


    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
