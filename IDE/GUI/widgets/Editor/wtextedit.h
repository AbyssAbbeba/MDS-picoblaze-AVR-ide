/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup WTextEdit
 * @file wtextedit.h
 */


#ifndef WTEXTEDIT_H
#define WTEXTEDIT_H


#include <QPlainTextEdit>
#include "../Highlighter/highlighter.h"



class Highlighter;

/**
 * @brief QTextEdit with reimplemented eventFilter
 * @ingroup GUI
 * @class WTextEdit
 */
class WTextEdit : public QPlainTextEdit
{
    Q_OBJECT   
    public:
        WTextEdit(QWidget *parent, SourceType type);
        void highlightCurrentLine();
        bool highlightLine(int line, QColor *color);
        void reloadHighlighter(SourceType type);
        void setPosition(int pos);
        int getPosition();
        void scrollToLine(int line);
        void selectLine(int line);

        /*WTextEdit & operator<< (QString const &str)
        {
            append(str);
            return *this;
        }*/

    public slots:
        void deselect();


    signals:
        void focusIn();
        void breakpoint(int line);
        void bookmark(int line);
        void textChangedSignal(const QString& text, int pos);
        void selectionRemovedSignal(int posStart, int posEnd);

    private:
        Highlighter *highlighter;
        SourceType sourceType;


    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
