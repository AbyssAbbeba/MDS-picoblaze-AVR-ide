/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file codeedit.h
 */



#ifndef CODEEDIT_H
#define CODEEDIT_H


#include <QTabWidget>
#include <QStatusBar>
#include <QGridLayout>
#include <QList>
#include "../../mainform/project.h"
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
        QTextEdit *getTextEdit();
        QWidget* getParent();
        void loadCodeEdit(CodeEdit* editor);
        QList<int> getBookmarkList();
        QList<int> getBreakpointList();
        CodeEdit* getParentCodeEdit();
        void setParentCodeEdit(CodeEdit *parentCodeEdit);

    public slots:
        void setChanged();
        void splitHorizontal();
        void splitVertical();
        void updateTextSlotOut();
        void updateTextSlotIn(const QString& textIn);
        void getFocus();
        void manageBreakpointEmit(int line);
        void manageBookmarkEmit(int line);

    signals:
        void splitSignal(Qt::Orientation orient, int line);
        void changedTabName(CodeEdit *editor, QString name);
        void updateText(const QString & text);
        void CodeEditChanged(CodeEdit* editor);
        void bookmarkListAdd(int line);
        void bookmarkListRemove(int line);
        void breakpointListAdd(int line);
        void breakpointListRemove(int line);
        void updateAnalysers(CodeEdit *editor);

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
        QList<int> breakpointList;
        QList<int> bookmarkList;
        CodeEdit *parentCodeEdit;

    protected:
        void contextMenuEvent(QContextMenuEvent *event);
        //bool eventFilter(QObject *target, QEvent *event);
};


#endif
