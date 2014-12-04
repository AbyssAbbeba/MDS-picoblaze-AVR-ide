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
#include "../../enums/enums.h"



class Highlighter;
class QShortcut;

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
        bool highlightLineAppend(int line, QColor *color);
        void clearHighlight();
        bool isLineHighlighted(int line, QColor *color);
        void reloadHighlighter(SourceType type);
        void setPosition(int pos);
        int getPosition();
        void scrollToLine(int line);
        void selectLine(int line);
        void setShortcuts();

        //only use in CodeEdit::loadCodeEdit(...)
        //SourceType getSourceType();
        //void setSourceType(SourceType type);
        void deleteHighlighter();
        void queryReadOnly(bool readOnly);
        void findAndMark(QString query, bool next, bool caseSensitive);
        void jumpToLine(int lineToJmp);
        void findAndReplace(QString query, QString replace, bool all, bool caseSensitive);

        


        

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
        void shortcutBreakpoint();
        void shortcutBookmark();
        void shortcutCopy();
        void shortcutPaste();
        void shortcutCut();
        void shortcutSelectAll();
        void shortcutDeselect();
        void shortcutComment();
        void shortcutDeleteComment();
        void shortcutJmpToBookmarkNext();
        void shortcutJmpToBookmarkPrev();
        void shortcutJmpToLine();
        void shortcutFind();
        void shortcutReplace();
        void shortcutMoveLineUp();
        void shortcutMoveLineDown();
        void shortcutMoveWordLeft();
        void shortcutMoveWordRight();
        void shortcutToUpper();
        void shortcutToLower();
        void shortcutFirstToUpper();
        void shortcutDeleteLine();
        void shortcutSwitchChars();
        void shortcutSelectWordUnder();
        void shortcutSelectWordLeft();
        void shortcutSelectWordRight();
        void shortcutUndo();
        void shortcutRedo();
//         void shortcutChangeTabLeft();
//         void shortcutChangeTabRight();
//         void shortcutCloseTab();
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
        void bookmarksAddLines(int line, int linesAdded);
        void bookmarksRemoveLines(int line, int linesRemoved);
        void requestScrollToBookmark(int currLine, bool next);
        void findDialog(QString query);
        void findAndReplaceDialog(QString query);
        void jumpToLineDialog(int line, int maxLines);
        void updateStatusBar();
        void editorReadOnly(bool readOnly);
//         void closeTab();
//         void changeTab(bool next);

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

        QShortcut *shctBreakpoint;
        QShortcut *shctBookmark;
        QShortcut *shctCopy;
        QShortcut *shctPaste;
        QShortcut *shctCut;
        QShortcut *shctSelectAll;
        QShortcut *shctDeselect;
        QShortcut *shctComment;
        QShortcut *shctDeleteComment;
        QShortcut *shctJmpToBookmarkNext;
        QShortcut *shctJmpToBookmarkPrev;
        QShortcut *shctJmpToLine;
        QShortcut *shctFind;
        QShortcut *shctReplace;
        QShortcut *shctMoveLineUp;
        QShortcut *shctMoveLineDown;
        QShortcut *shctMoveWordLeft;
        QShortcut *shctMoveWordRight;
        QShortcut *shctToUpper;
        QShortcut *shctToLower;
        QShortcut *shctFirstToUpper;
        QShortcut *shctDeleteLine;
        QShortcut *shctSwitchChars;
        QShortcut *shctSelectWordUnder;
        QShortcut *shctSelectWordLeft;
        QShortcut *shctSelectWordRight;
        QShortcut *shctUndo;
        QShortcut *shctRedo;
        /*QShortcut *shctCloseTab;
        QShortcut *shctChangeTabLeft;
        QShortcut *shctChangeTabRight;*/
        


    protected:
        bool eventFilter(QObject *target, QEvent *event);
        void contextMenuEvent(QContextMenuEvent *event);
};


#endif
