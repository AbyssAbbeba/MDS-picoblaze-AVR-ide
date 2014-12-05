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
 * @file codeedit.h
 */


#ifndef CODEEDIT_H
#define CODEEDIT_H


//#include <QTabWidget>
#include <QWidget>
#include <QList>
#include <QTextCursor>
#include "../../enums/enums.h"
//#include "../../project/project.h"


class Project;
class WLineCounter;
class WTextEdit;
class QStatusBar;
class QGridLayout;
class Find;
class FindAndReplace;
class JumpToLine;

/**
 * @brief Code Editor, include splitter, wtextedit and line counter
 * @ingroup GUI
 * @class CodeEdit
 */
class CodeEdit : public QWidget
{
    Q_OBJECT   
    public:
        CodeEdit(QWidget *parent, bool tabs, QString wName, QString wPath, CodeEdit *parentCodeEdit);
        CodeEdit(QWidget *parent, bool tabs, Project* parentPrj, QString wName, QString wPath, CodeEdit *parentCodeEdit);
        ~CodeEdit();
        QString getName();
        QString getPath();
        void setName(QString wName);
        void setPath(QString wPath);
        void setSaved();
        bool isChanged();
        void connectAct();
        bool isChild(Project* project);
        void setParentProject(Project* project);
        WTextEdit* getTextEdit();
        QWidget* getParent();
        void loadCodeEdit(CodeEdit* editor);
        QList<int> getBookmarkList();
        QList<int> getBreakpointList();
        CodeEdit* getParentCodeEdit();
        void setParentCodeEdit(CodeEdit *parentCodeEdit);
        Project *getParentProject();
        void addBreakpointLine(int line);
        void removeBreakpointLine(int line);
        void moveBreakpointsLines(int line, int linesChanged, bool added);
        QList<int>* getBreakpointsLines();
        void addBookmarkLine(int line);
        void removeBookmarkLine(int line);
        void moveBookmarksLines(int line, int linesChanged, bool added);
        QList<int>* getBookmarksLines();
        void setBreakpointsLines(QList<unsigned int> breakpoints);
        void setBreakpointsLines(QList<int> breakpoints);
        void setBookmarksLines(QList<unsigned int> bookmarks);
        void setBookmarksLines(QList<int> bookmarks);
        int getScrollValue();
        void setScrollValue(int value);
        QTextCursor getCursorValue();
        void setCursorValue(QTextCursor value);
        void setHidden(bool hide);

    public slots:
        void setChanged();
        void splitHorizontal();
        void splitVertical();
        //void updateTextSlotOut(const QString& text, int pos);
        //void updateTextSlotIn(const QString& textIn, int pos, CodeEdit *editor);
        //void selectionRemovedOut(int posStart, int posEnd);
        //void selectionRemovedIn(int posStart, int posEnd, CodeEdit *editor);
        void getFocus();
        void manageBreakpointEmit(int line);
        void manageBookmarkEmit(int line);
        void changeFont(QFont font);
        void changeTabStopWidth(int width);
        void changeHeight();
        void updateLineCounter();
        void breakpointsAddLines(int line, int linesAdded);
        void breakpointsRemoveLines(int line, int linesRemoved);
        void bookmarksAddLines(int line, int linesAdded);
        void bookmarksRemoveLines(int line, int linesRemoved);
        

    signals:
        void splitSignal(Qt::Orientation orient, int line);
        void changedTabName(QString name, QString path, bool changed);
        void changedTabStatus(QString name, QString path, bool changed);
        //void updateText(const QString& text, int pos, CodeEdit *editor);
        void CodeEditChanged(CodeEdit* editor);
        //void breakpointListStateChanged(QString file, int line);
        void breakpointEmit(QString file, int line);
        void breakpointsAddLines(QString file, int line, int linesAdded);
        void breakpointsRemoveLines(QString file, int line, int linesRemoved);
        void bookmarkEmit(QString file, int line);
        void bookmarksAddLines(QString file, int line, int linesAdded);
        void bookmarksRemoveLines(QString file, int line, int linesRemoved);
        //void breakpointListRemove(QString file, int line);
        void updateAnalysers(CodeEdit *editor);
        void updateRemoveSelection(int posStart, int posEnd, CodeEdit *editor);
//         void changeTab(bool next);
//         void closeTab();

    private slots:
        void updateStatusBar();
        void textEditChanged(bool modified);
        void textEditReadOnly(bool readOnly);
        void requestScrollToBookmark(int currLine, bool next);
        void findDialog(QString query);
        void findAndReplaceDialog(QString query);
        void jumpToLineDialog(int line, int maxLines);
        void closeBottomWidget(CodeEditBottomWidget widget);
        void find(QString query, bool next, bool caseSensitive);
        void jumpToLine(int line);
        void findAndReplace(QString find, QString replace, bool all, bool caseSensitive);
//         void changeTabSlot();
//         void closeTabSlot();

    private:
        QString m_name;
        QString m_path;
        bool m_changed;
        bool m_tabs;
        Project *m_parentProject;
        QWidget *m_parentWidget;
        QStatusBar *m_statusBar;
        WTextEdit *m_textEdit;
        QGridLayout *m_layout;
        WLineCounter *m_lineCount;
        //QList<int> breakpointList;
        //QList<int> bookmarkList;
        CodeEdit *m_parentCodeEdit;
        QColor *m_breakpointColor;
        int m_prevBlockCount;
        QList<int> *m_bookmarksLines;
        QList<int> *m_breakpointsLines;
        CodeEdit *m_curCodeEdit;
        int m_curScrollValue;
        QTextCursor m_curCursorPos;
        bool m_hidden;

        Find *m_findDialog;
        FindAndReplace *m_findAndReplaceDialog;
        JumpToLine *m_jumpToLineDialog;
};


#endif
