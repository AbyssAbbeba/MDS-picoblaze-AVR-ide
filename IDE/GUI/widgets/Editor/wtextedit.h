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
        bool isLineHighlighted(int line, QColor *color);
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
        void cursorPositionChangedSlot();
        void setTabToSpaces(bool enabled);
        void setSpacesInTab(int spacesInTab);
        //void editedUndo();
        //void editedRedo();
        //void editedCut();
        //void editedPaste();


    signals:
        void focusIn();
        void breakpoint(int line);
        void bookmark(int line);
        //void textChangedSignal(const QString& text, int pos);
        //void selectionRemovedSignal(int posStart, int posEnd);
        void updateLineCounter();
        void breakpointsAddLines(int line, int linesAdded);
        void breakpointsRemoveLines(int line, int linesRemoved);

    //private slots:
    //    void updateUndoRedo(int position, int charsRemoved, int charsAdded);

    private:
        void makeMenu();
        
        Highlighter *highlighter;
        SourceType sourceType;
        int prevBlock;
        QColor *cursorLineColor;
        QMenu *editorPopup;
        QAction *cutAct;
        QAction *copyAct;
        QAction *deselectAct;
        
        bool tabToSpaces;
        int spacesInTab;
        


    protected:
        bool eventFilter(QObject *target, QEvent *event);
        void contextMenuEvent(QContextMenuEvent *event);
};


#endif
