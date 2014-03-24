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


#include <QTabWidget>
#include <QStatusBar>
#include <QGridLayout>
#include <QList>
#include "../../project/project.h"
#include "../Counters/wlinecounter.h"
#include "wtextedit.h"


class Project;

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
        void changeHeight();

    signals:
        void splitSignal(Qt::Orientation orient, int line);
        void changedTabName(QString name, QString path, bool changed);
        void changedTabStatus(QString name, QString path, bool changed);
        void updateText(const QString& text, int pos, CodeEdit *editor);
        void CodeEditChanged(CodeEdit* editor);
        void bookmarkListStateChanged(QString file, int line);
        void breakpointEmit(QString file, int line);
        //void breakpointListRemove(QString file, int line);
        void updateAnalysers(CodeEdit *editor);
        void updateRemoveSelection(int posStart, int posEnd, CodeEdit *editor);

    private:
        void makeMenu();

        QString name;
        QString path;
        bool changed;
        bool tabs;
        Project* parentProject;
        QWidget *parentWidget;
        QStatusBar *statusBar;
        WTextEdit *textEdit;
        QGridLayout *layout;
        WLineCounter *lineCount;
        QMenu *editorPopup;
        QAction *cutAct;
        QAction *copyAct;
        QAction *deselectAct;
        //QList<int> breakpointList;
        //QList<int> bookmarkList;
        CodeEdit *parentCodeEdit;
        QColor *breakpointColor;
        int prevBlockCount;

    protected:
        void contextMenuEvent(QContextMenuEvent *event);
        //bool eventFilter(QObject *target, QEvent *event);
};


#endif
