/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CodeEdit
 * @file codeedit.cpp
 */


#include <QtGui>
#include "../EditorWidgets/JumpToLine/jumptoline.h"
#include "../EditorWidgets/Find/find.h"
#include "../EditorWidgets/FindAndReplace/findandreplace.h"
#include "codeedit.h"
#include "../DockManager/wdockmanager.h"
#include "baseeditor.h"
#include "../../guicfg/guicfg.h"
#include "../Counters/wlinecounter.h"
#include "wtextedit.h"


CodeEdit::CodeEdit(QWidget *parent, bool tabs, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    //qDebug() << "CodeEdit: CodeEdit()";
    m_parentCodeEdit = parentCodeEdit;
    m_curCodeEdit = NULL;
    m_hidden = false;

    m_breakpointsLines = new QList<int>();
    m_bookmarksLines = new QList<int>();

    if (wName == NULL || wPath == NULL || wPath == "untracked")
    {
        //qDebug() << "CodeEdit: untracked";
        if (wPath == "untracked")
        {
            if (wName == "ASM Translator" || wName == "ASM Translator error")
            {
                m_textEdit = new WTextEdit(this, PICOBLAZEASM);
            }
            else if (wName == "disasm")
            {
                m_textEdit = new WTextEdit(this, PICOBLAZEASM);
            }
            else
            {
                m_textEdit = new WTextEdit(this, PLAIN);
            }
        }
        else
        {
            m_textEdit = new WTextEdit(this, PLAIN);
        }
        m_textEdit->setPlainText("");
    }
    else
    {
        int index = wName.lastIndexOf(".");
        if (index > 0)
        {
            QString text(wName.right(wName.size() - index));
            if (text.toLower() == ".h")
            {
                m_textEdit = new WTextEdit(this, PLAIN);
                //m_textEdit = new WTextEdit(this, C);
            }
            else if (text.toLower() == ".cpp" || text.toLower() == ".cxx" || text.toLower() == ".cc")
            {
                m_textEdit = new WTextEdit(this, PLAIN);
                //m_textEdit = new WTextEdit(this, CPP);
            }
            else if (text.toLower() == ".c")
            {
                m_textEdit = new WTextEdit(this, PLAIN);
                //m_textEdit = new WTextEdit(this, C);
            }
            else if (text.toLower() == ".asm" || text.toLower() == ".psm")
            {
                //!!!DO NOT FORGET TO CHECK IF ASM IS AVR OR PIC TYPE!!!
                m_textEdit = new WTextEdit(this, PICOBLAZEASM);
            }
            else
            {
                m_textEdit = new WTextEdit(this, PLAIN);
            }
        }
        else
        {
            m_textEdit = new WTextEdit(this, PLAIN);
        }
    }
    //textEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_textEdit->setFont(GuiCfg::getInstance().getEditorFont());
    QFontMetrics fontMetrics(m_textEdit->font());
    m_textEdit->setTabStopWidth(GuiCfg::getInstance().getTabWidth() * fontMetrics.width(' '));
    m_textEdit->setTabToSpaces(GuiCfg::getInstance().getTabToSpaces());
    m_textEdit->setSpacesInTab(GuiCfg::getInstance().getSpacesInTab());
    m_lineCount = new WLineCounter(m_textEdit, true, false, 0, m_textEdit->font());
    m_statusBar = new QStatusBar(this);
    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_lineCount, 0, 0);
    m_layout->addWidget(m_textEdit, 0, 1);
    m_layout->addWidget(m_statusBar, 1, 1);
    setLayout(m_layout);
    m_name = wName;
    m_path = wPath;
    m_parentWidget = parent;
    m_tabs = tabs;
    m_parentProject = NULL;
    m_breakpointColor = new QColor(0,255,0);
    m_curScrollValue = 0;
    //textEdit->setWordWrapMode(QTextOption::NoWrap);
    m_textEdit->setWordWrapMode(QTextOption::WordWrap);
    m_textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    m_findDialog = NULL;
    m_findAndReplaceDialog = NULL;
    m_jumpToLineDialog = NULL;
    //this->makeMenu();
    //this->setFocusPolicy(Qt::StrongFocus);
    //this->textEdit->setFocusPolicy(Qt::NoFocus);
    //this->installEventFilter(this);
    if (wPath != NULL && wPath != "untracked")
    {
        //qDebug() << "CodeEdit: Not untracked";
        QFile file(m_path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_textEdit->setPlainText(file.readAll());
            file.close();
            //QApplication::processEvents();
            m_textEdit->setPositionToStart();
        }
    }
    m_changed = false;
    connect(m_textEdit,
            SIGNAL(focusIn()),
            this,
            SLOT(getFocus())
           );
    connect(m_textEdit,
            SIGNAL(breakpoint(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(m_textEdit,
            SIGNAL(breakpointsAddLines(int, int)),
            this,
            SLOT(breakpointsAddLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(breakpointsRemoveLines(int, int)),
            this,
            SLOT(breakpointsRemoveLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(bookmark(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
    connect(m_textEdit,
            SIGNAL(bookmarksAddLines(int, int)),
            this,
            SLOT(bookmarksAddLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(bookmarksRemoveLines(int, int)),
            this,
            SLOT(bookmarksRemoveLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(updateLineCounter()),
            this,
            SLOT(updateLineCounter())
           );
    connect(m_lineCount,
            SIGNAL(breakpoint(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(m_lineCount,
            SIGNAL(bookmark(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            this,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            m_lineCount,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabWidthChanged(int)),
            this,
            SLOT(changeTabStopWidth(int))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabToSpacesChanged(bool)),
            m_textEdit,
            SLOT(setTabToSpaces(bool))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(spacesInTabChanged(int)),
            m_textEdit,
            SLOT(setSpacesInTab(int))
           );
    connect(m_textEdit,
            SIGNAL(updateStatusBar()),
            this,
            SLOT(updateStatusBar())
           );
    connect(m_textEdit,
            SIGNAL(editorReadOnly(bool)),
            this,
            SLOT(textEditReadOnly(bool))
           );
    connect(m_textEdit,
            SIGNAL(requestScrollToBookmark(int, bool)),
            this,
            SLOT(requestScrollToBookmark(int, bool))
           );
    connect(m_textEdit,
            SIGNAL(findDialog(QString)),
            this,
            SLOT(findDialog(QString))
           );
    connect(m_textEdit,
            SIGNAL(findAndReplaceDialog(QString)),
            this,
            SLOT(findAndReplaceDialog(QString))
           );
    connect(m_textEdit,
            SIGNAL(jumpToLineDialog(int, int)),
            this,
            SLOT(jumpToLineDialog(int, int))
           );
    //this->connectAct();
    m_prevBlockCount = m_textEdit->document()->blockCount();
    //this->show();
    changeHeight();
    m_textEdit->setShortcuts();
    //qDebug() << "CodeEdit: return CodeEdit()";
}



/*CodeEdit::CodeEdit(QWidget *parent, bool tabs, Project* parentPrj, QString wName, QString wPath, CodeEdit *parentCodeEdit)
    : QWidget(parent)
{
    qDebug() << "CodeEdit: CodeEdit()2";
    m_parentCodeEdit = parentCodeEdit;
    m_curCodeEdit = NULL;
    m_hidden = false;

    m_breakpointsLines = new QList<int>();
    m_bookmarksLines = new QList<int>();

    
    int index = wName.lastIndexOf(".");
    if (index > 0)
    {
        QString text(wName.right(wName.size() - index));
        if (text.toLower() == ".h")
        {
            m_textEdit = new WTextEdit(this, PLAIN);
            //m_textEdit = new WTextEdit(this, C);
        }
        else if (text.toLower() == ".cpp" || text.toLower() == ".cxx" || text.toLower() == ".cc")
        {
            m_textEdit = new WTextEdit(this, PLAIN);
            //m_textEdit = new WTextEdit(this, CPP);
        }
        else if (text.toLower() == ".c")
        {
            m_textEdit = new WTextEdit(this, PLAIN);
            //m_textEdit = new WTextEdit(this, C);
        }
        else if (text.toLower() == ".asm" || text.toLower() == ".psm")
        {
            //!!!DO NOT FORGET TO CHECK IF ASM IS AVR OR PIC TYPE!!!
            m_textEdit = new WTextEdit(this, PICOBLAZEASM);
        }
        else
        {
            m_textEdit = new WTextEdit(this, PLAIN);
        }
    }
    else
    {
        m_textEdit = new WTextEdit(this, PLAIN);
    }
    //textEdit->setContextMenuPolicy(Qt::NoContextMenu);
    m_textEdit->setFont(GuiCfg::getInstance().getEditorFont());
    QFontMetrics fontMetrics(m_textEdit->font());
    m_textEdit->setTabStopWidth(GuiCfg::getInstance().getTabWidth() * fontMetrics.width(' '));
    m_textEdit->setTabToSpaces(GuiCfg::getInstance().getTabToSpaces());
    m_textEdit->setSpacesInTab(GuiCfg::getInstance().getSpacesInTab());
    m_lineCount = new WLineCounter(m_textEdit, true, false, 0, m_textEdit->font());
    m_statusBar = new QStatusBar(this);
    m_layout = new QGridLayout(this);
    m_layout->addWidget(m_lineCount, 0, 0);
    m_layout->addWidget(m_textEdit, 0, 1);
    m_layout->addWidget(m_statusBar, 1, 1);
    m_name = wName;
    m_path = wPath;
    m_parentWidget = parent;
    m_tabs = tabs;
    m_parentProject = parentPrj;
    m_breakpointColor = new QColor(0,255,0);
    m_textEdit->setWordWrapMode(QTextOption::WordWrap);
    m_textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    m_findDialog = NULL;
    m_findAndReplaceDialog = NULL;
    m_jumpToLineDialog = NULL;
    //this->makeMenu();
    //this->setFocusPolicy(Qt::StrongFocus);
    //this->installEventFilter(this);
    if (wPath != NULL)
    {
        QFile file(m_path);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_textEdit->setPlainText(file.readAll());
            file.close();
        }
    }
    m_changed = false;
    connect(m_textEdit,
            SIGNAL(focusIn()),
            this,
            SLOT(getFocus())
           );
    connect(m_textEdit,
            SIGNAL(breakpoint(int)),
            this,
            SLOT(manageBreakpointEmit(int))
           );
    connect(m_textEdit,
            SIGNAL(breakpointsAddLines(int, int)),
            this,
            SLOT(breakpointsAddLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(breakpointsRemoveLines(int, int)),
            this,
            SLOT(breakpointsRemoveLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(bookmark(int)),
            this,
            SLOT(manageBookmarkEmit(int))
           );
    connect(m_textEdit,
            SIGNAL(bookmarksAddLines(int, int)),
            this,
            SLOT(bookmarksAddLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(bookmarksRemoveLines(int, int)),
            this,
            SLOT(bookmarksRemoveLines(int, int))
           );
    connect(m_textEdit,
            SIGNAL(updateLineCounter()),
            this,
            SLOT(updateLineCounter())
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            this,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(editorFontChanged(QFont)),
            m_lineCount,
            SLOT(changeFont(QFont))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabWidthChanged(int)),
            this,
            SLOT(changeTabStopWidth(int))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(tabToSpacesChanged(bool)),
            m_textEdit,
            SLOT(setTabToSpaces(bool))
           );
    connect(&GuiCfg::getInstance(),
            SIGNAL(spacesInTabChanged(int)),
            m_textEdit,
            SLOT(setSpacesInTab(int))
           );
    connect(m_textEdit,
            SIGNAL(updateStatusBar()),
            this,
            SLOT(updateStatusBar())
           );
    connect(m_textEdit,
            SIGNAL(editorReadOnly(bool)),
            this,
            SLOT(textEditReadOnly(bool))
           );
    connect(m_textEdit,
            SIGNAL(requestScrollToBookmark(int, bool)),
            this,
            SLOT(requestScrollToBookmark(int, bool))
           );
    connect(m_textEdit,
            SIGNAL(findDialog(QString)),
            this,
            SLOT(findDialog(QString))
           );
    connect(m_textEdit,
            SIGNAL(findAndReplaceDialog(QString)),
            this,
            SLOT(findAndReplaceDialog(QString))
           );
    connect(m_textEdit,
            SIGNAL(jumpToLineDialog(int, int)),
            this,
            SLOT(jumpToLineDialog(int, int))
           );
    //this->connectAct();
    m_prevBlockCount = m_textEdit->document()->blockCount();
    this->changeHeight();
    m_textEdit->setShortcuts();
    //qDebug() << "CodeEdit: return CodeEdit()2";
}*/


CodeEdit::~CodeEdit()
{
    if (m_parentCodeEdit == NULL)
    {
        delete m_breakpointsLines;
        delete m_bookmarksLines;
    }
}




void CodeEdit::connectAct()
{
    //qDebug() << "CodeEdit: connectAct()";
    connect(m_textEdit, SIGNAL(modificationChanged(bool)), this, SLOT(textEditChanged(bool)));
    //qDebug() << "CodeEdit: return connectAct()";
}


void CodeEdit::textEditChanged(bool modified)
{
    qDebug() << "modification";
    if (true == modified)
    {
        this->setChanged();
    }
    else
    {
        this->setSaved();
    }
}


//najit efektivnejsi reseni, neco jako signal disable pri zmene a enable pri savu
void CodeEdit::setChanged()
{
    m_changed = true;
    emit changedTabStatus(m_name, m_path, true);
}


void CodeEdit::setSaved()
{
    m_changed = false;
    emit changedTabStatus(m_name, m_path, false);
    m_textEdit->document()->setModified(false);
}


bool CodeEdit::isChanged()
{
    //qDebug() << "CodeEdit: isChanged()";
    //qDebug() << "CodeEdit: return isChanged()";
    return (m_changed == true);
}

void CodeEdit::setName(QString wName)
{
    //qDebug() << "CodeEdit: setName()";
    m_name = wName;
    //qDebug() << "CodeEdit: return setName()";
}

void CodeEdit::setPath(QString wPath)
{
    //qDebug() << "CodeEdit: setPath()";
    m_path = wPath;
    //qDebug() << "CodeEdit: return setPath()";
}


QString CodeEdit::getName()
{
    return m_name;
}

QString CodeEdit::getPath()
{
    return m_path;
}


bool CodeEdit::isChild(Project* project)
{
    return (project == m_parentProject);
}



void CodeEdit::setParentProject(Project* project)
{
    //qDebug() << "CodeEdit: setParentProject()";
    m_parentProject = project;
    //TODO: refresh breakpointLines and bookmarkLines
    //qDebug() << "CodeEdit: return setParentProject()";
}



WTextEdit* CodeEdit::getTextEdit()
{
    return m_textEdit;
}


void CodeEdit::splitHorizontal()
{
    //qDebug() << "CodeEdit: splitHorizontal()";
    //qDebug() << "Code Edit: split signal - horizontal";
    emit splitSignal(Qt::Horizontal, 0);
    //qDebug() << "CodeEdit: return splitHorizontal()";
}

void CodeEdit::splitVertical()
{
    //qDebug() << "CodeEdit: splitVertical()";
    //qDebug() << "Code Edit: split signal - vertical";
    emit splitSignal(Qt::Vertical, 0);
    //qDebug() << "CodeEdit: return splitVertical()";
}


void CodeEdit::loadCodeEdit(CodeEdit* editor)
{
    if (m_curCodeEdit != NULL)
    {
        if (true == m_changed)
        {
            m_curCodeEdit->setChanged();
        }
        else
        {
            m_curCodeEdit->setSaved();
        }
        m_curCodeEdit->setScrollValue(m_textEdit->verticalScrollBar()->value());
        m_curCodeEdit->setCursorValue(m_textEdit->textCursor());
        m_curCodeEdit->getTextEdit()->setExtraSelections(m_textEdit->extraSelections());
    }
    
    bool prevChanged = editor->isChanged();
    this->setName(editor->getName());
    this->setPath(editor->getPath());
    m_textEdit->deleteHighlighter();
    m_textEdit->setDocument(editor->getTextEdit()->document());
    m_textEdit->clearHighlight();
    //qDebug() << "CodeEdit: loading CodeEdit for path" << editor->getPath();
    m_textEdit->setExtraSelections(editor->getTextEdit()->extraSelections());
    /*for (int i = 0; i < m_textEdit->extraSelections().count(); i++)
    {
        qDebug() << "CodeEdit: extra selection" <<  m_textEdit->extraSelections().at(i).cursor.position();
    }*/
    m_breakpointsLines = editor->getBreakpointsLines();
    m_bookmarksLines = editor->getBookmarksLines();
    if (m_breakpointsLines == NULL)
    {
        qDebug() << "CodeEdit: breakpointsLines == NULL";
    }
    if (m_bookmarksLines == NULL)
    {
        qDebug() << "CodeEdit: bookmarskLines == NULL";
    }
    m_lineCount->getWidget()->setBreakpointList(m_breakpointsLines);
    m_lineCount->getWidget()->setBookmarkList(m_bookmarksLines);
    emit CodeEditChanged(editor);
    changeHeight();
    m_changed = prevChanged;
    if (false == m_changed)
    {
        emit changedTabStatus(m_name, m_path, false);
    }
    m_curCodeEdit = editor;
    if (false == m_curCodeEdit->getCursorValue().isNull())
    {
        m_textEdit->setTextCursor(m_curCodeEdit->getCursorValue());
    }
    //qDebug() << "CodeEdit: scroll value" << m_curCodeEdit->getScrollValue();
    m_textEdit->verticalScrollBar()->setValue(m_curCodeEdit->getScrollValue());

}


QWidget* CodeEdit::getParent()
{
    return m_parentWidget;
}



void CodeEdit::getFocus()
{
    if (m_parentWidget != NULL)
    {
        ((BaseEditor*)m_parentWidget)->focusIn();
    }
}

void CodeEdit::manageBreakpointEmit(int line)
{
    emit breakpointEmit(m_path, line);
}

void CodeEdit::manageBookmarkEmit(int line)
{
    emit bookmarkEmit(m_path, line);
}


CodeEdit* CodeEdit::getParentCodeEdit()
{
    if (m_parentCodeEdit == NULL)
    {
        return this;
    }
    else
    {
        return m_parentCodeEdit;
    }
}


void CodeEdit::setParentCodeEdit(CodeEdit *parentCodeEdit)
{
    m_parentCodeEdit = parentCodeEdit;
}


void CodeEdit::changeHeight()
{
    m_lineCount->getWidget()->changeHeight();
}


void CodeEdit::updateLineCounter()
{
    m_lineCount->getWidget()->update();
}


void CodeEdit::changeFont(QFont font)
{
    m_textEdit->setFont(font);
}


Project* CodeEdit::getParentProject()
{
    return m_parentProject;
}


void CodeEdit::addBreakpointLine(int line)
{
    m_breakpointsLines->append(line);
    qSort(m_breakpointsLines->begin(), m_breakpointsLines->end());
    m_lineCount->getWidget()->update();
}


void CodeEdit::removeBreakpointLine(int line)
{
    m_breakpointsLines->removeAll(line);
    m_lineCount->getWidget()->update();
}


QList<int>* CodeEdit::getBreakpointsLines()
{
    return m_breakpointsLines;
}


void CodeEdit::addBookmarkLine(int line)
{
    m_bookmarksLines->append(line);
    qSort(m_bookmarksLines->begin(), m_bookmarksLines->end());
    m_lineCount->getWidget()->update();
}


void CodeEdit::removeBookmarkLine(int line)
{
    m_bookmarksLines->removeAll(line);
    m_lineCount->getWidget()->update();
}


QList<int>* CodeEdit::getBookmarksLines()
{
    return m_bookmarksLines;
}


void CodeEdit::breakpointsAddLines(int line, int linesAdded)
{
    //qDebug() << "CodeEdit: breakpointsAddLines";
    emit breakpointsAddLines(m_path, line, linesAdded);
}


void CodeEdit::breakpointsRemoveLines(int line, int linesRemoved)
{
    emit breakpointsRemoveLines(m_path, line, linesRemoved);
}


void CodeEdit::moveBreakpointsLines(int line, int linesChanged, bool added)
{
    bool update = false;
    if (true == added)
    {
        for (int i = 0; i < m_breakpointsLines->count(); i++)
        {
            if (m_breakpointsLines->at(i) > line)
            {
                (*(m_breakpointsLines))[i] += linesChanged;
                update = true;
            }
        }
    }
    else
    {
        QList<int>::iterator i;
        for (i = m_breakpointsLines->begin(); i != m_breakpointsLines->end(); i++)
        {
            if ( *i >= line
            && *i < line + linesChanged
            )
            {
                *i = -1;
                update = true;
            }
            else
            {
                if (*i >= line + linesChanged)
                {
                    *i -= linesChanged;
                    update = true;
                }
            }
        }
        m_breakpointsLines->removeAll(-1);
        /*if (this->breakpointsLines->contains(line))
        {
            this->breakpointsLines->removeAll(line);
            update = true;
        }*/
        /*
        for (int i = 0; i < this->breakpointsLines->count(); i++)
        {
            if ( this->breakpointsLines->at(i) >= line
              && this->breakpointsLines->at(i) <= line + linesChanged;
               )
            {
                this->breakpointsLines->removeAll(line);
                update = true;
            }
            else
            {
                if (this->breakpointsLines->at(i) > line + linesChanged)
                {
                    (*(this->breakpointsLines))[i] -= linesChanged;
                    update = true;
                }
            }
        }*/
    }
    if (true == update)
    {
        m_lineCount->getWidget()->update();
    }
}


void CodeEdit::bookmarksAddLines(int line, int linesAdded)
{
    //qDebug() << "CodeEdit: bookmarksAddLines";
    emit bookmarksAddLines(m_path, line, linesAdded);
}


void CodeEdit::bookmarksRemoveLines(int line, int linesRemoved)
{
    emit bookmarksRemoveLines(m_path, line, linesRemoved);
}


void CodeEdit::moveBookmarksLines(int line, int linesChanged, bool added)
{
    bool update = false;
    if (true == added)
    {
        for (int i = 0; i < m_bookmarksLines->count(); i++)
        {
            if (m_bookmarksLines->at(i) > line)
            {
                (*(m_bookmarksLines))[i] += linesChanged;
                update = true;
            }
        }
    }
    else
    {
        QList<int>::iterator i;
        for (i = m_bookmarksLines->begin(); i != m_bookmarksLines->end(); i++)
        {
            if ( *i >= line
            && *i < line + linesChanged
            )
            {
                *i = -1;
                update = true;
            }
            else
            {
                if (*i >= line + linesChanged)
                {
                    *i -= linesChanged;
                    update = true;
                }
            }
        }
        m_bookmarksLines->removeAll(-1);
    }
    if (true == update)
    {
        m_lineCount->getWidget()->update();
    }
}


void CodeEdit::changeTabStopWidth(int width)
{
    QFontMetrics fontMetrics(m_textEdit->font());
    m_textEdit->setTabStopWidth(width * fontMetrics.width(' '));
}


void CodeEdit::setBreakpointsLines(QList<unsigned int> breakpoints)
{
    //qDebug() << "CodeEdit: setBreakpointsLines(unsigned int)";
    /*if (breakpoints == NULL)
    {
        qDebug() << "CodeEdit: breakpoints NULL";
    }*/
    //qDebug() << "breakpoints count" << breakpoints.count();
    //if (breakpoints != NULL && breakpointsLines != NULL)
    if (m_breakpointsLines != NULL)
    {
        m_breakpointsLines->clear();
        for (int i = 0; i < breakpoints.count(); i++)
        {
            //qDebug() << "CodeEdit: appending breakpoint";
            m_breakpointsLines->append((int)breakpoints.at(i));
        }
        //qDebug() << "CodeEdit: setBreakpoints - updateWidget()";
        qSort(m_breakpointsLines->begin(), m_breakpointsLines->end());
        m_lineCount->getWidget()->update();
    }
}


void CodeEdit::setBreakpointsLines(QList<int> breakpoints)
{
    if (m_breakpointsLines != NULL)
    {
        m_breakpointsLines->clear();
        for (int i = 0; i < breakpoints.count(); i++)
        {
            m_breakpointsLines->append(breakpoints.at(i));
        }
        qSort(m_breakpointsLines->begin(), m_breakpointsLines->end());
        m_lineCount->getWidget()->update();
    }
}


void CodeEdit::setBookmarksLines(QList<unsigned int> bookmarks)
{
    if (m_bookmarksLines != NULL)
    {
        m_bookmarksLines->clear();
        for (int i = 0; i < bookmarks.count(); i++)
        {
            m_bookmarksLines->append((int)bookmarks.at(i));
        }
        qSort(m_bookmarksLines->begin(), m_bookmarksLines->end());
        m_lineCount->getWidget()->update();
    } 
}


void CodeEdit::setBookmarksLines(QList<int> bookmarks)
{
    if (m_bookmarksLines != NULL)
    {
        m_bookmarksLines->clear();
        for (int i = 0; i < bookmarks.count(); i++)
        {
            m_bookmarksLines->append(bookmarks.at(i));
        }
        qSort(m_bookmarksLines->begin(), m_bookmarksLines->end());
        m_lineCount->getWidget()->update();
    }
}


int CodeEdit::getScrollValue()
{
    return m_curScrollValue;
}


void CodeEdit::setScrollValue(int value)
{
    m_curScrollValue = value;
}


QTextCursor CodeEdit::getCursorValue()
{
    return m_curCursorPos;
}


void CodeEdit::setCursorValue(QTextCursor value)
{
    m_curCursorPos = value;
}


void CodeEdit::setHidden(bool hide)
{
    m_hidden = hide;
}


void CodeEdit::updateStatusBar()
{
    QString message = "Line: " + QString::number(m_textEdit->textCursor().blockNumber()+1);
    message += " Column: " + QString::number(m_textEdit->textCursor().positionInBlock()+1);
    m_statusBar->showMessage(message);
}


void CodeEdit::textEditReadOnly(bool readOnly)
{
    if (true == readOnly)
    {
        m_statusBar->showMessage("Read Only");
    }
    else
    {
        updateStatusBar();
    }
}


void CodeEdit::requestScrollToBookmark(int currLine, bool next)
{
    int line = currLine+1;
    if (true == next)
    {
        for (int i = 0; i < m_bookmarksLines->count(); i++)
        {
            line = m_bookmarksLines->at(i);
            if (line  > currLine+1)
            {
                break;
            }

        }
        //qDebug() << "CodeEdit: currLine+1" << currLine+1;
        //qDebug() << "CodeEdit: line" << line;
        if (currLine+1 == line && m_bookmarksLines->count() > 0)
        {
            line = m_bookmarksLines->at(0);
        }
    }
    else
    {
        for (int i = m_bookmarksLines->count()-1; i >= 0; i--)
        {
            line = m_bookmarksLines->at(i);
            if (line < currLine+1)
            {
                break;
            }
        }
        //qDebug() << "CodeEdit: currLine+1" << currLine+1;
        //qDebug() << "CodeEdit: line" << line;
        if (currLine+1 == line && m_bookmarksLines->count() > 0)
        {
            line = m_bookmarksLines->at(m_bookmarksLines->count()-1);
        }
    }
    m_textEdit->jumpToLine(line-1);
}


void CodeEdit::findDialog(QString query)
{
    //FindDialog
    if (NULL != m_findAndReplaceDialog)
    {
        delete m_findAndReplaceDialog;
        m_findAndReplaceDialog = NULL;
    }
    if (NULL != m_jumpToLineDialog)
    {
        delete m_jumpToLineDialog;
        m_jumpToLineDialog = NULL;
    }
    
    if (NULL == m_findDialog)
    {
        m_findDialog = new Find(this, query);
        m_findDialog->show();
        m_findDialog->reloadFocus();
        m_layout->addWidget(m_findDialog, 2, 1);
        connect(m_findDialog,
                SIGNAL(closeWidget(CodeEditBottomWidget)),
                this,
                SLOT(closeBottomWidget(CodeEditBottomWidget))
               );
        connect(m_findDialog,
                SIGNAL(find(QString, bool, bool)),
                this,
                SLOT(find(QString, bool, bool))
               );
        this->changeHeight();
        //QLabel *label = new QLabel("ASDSDASASASSDA", this);
        //m_layout->addWidget(label, 2, 1);
    }
    else
    {
        m_findDialog->setQuery(query);
        m_findDialog->reloadFocus();
    }
}


void CodeEdit::findAndReplaceDialog(QString query)
{
    //FindDialog
    if (NULL != m_findDialog)
    {
        delete m_findDialog;
        m_findDialog = NULL;
    }
    if (NULL != m_jumpToLineDialog)
    {
        delete m_jumpToLineDialog;
        m_jumpToLineDialog = NULL;
    }

    if (NULL == m_findAndReplaceDialog)
    {
        m_findAndReplaceDialog = new FindAndReplace(this, query);
        m_findAndReplaceDialog->show();
        m_layout->addWidget(m_findAndReplaceDialog, 2, 1);
        connect(m_findAndReplaceDialog,
                SIGNAL(closeWidget(CodeEditBottomWidget)),
                this,
                SLOT(closeBottomWidget(CodeEditBottomWidget))
               );
        connect(m_findAndReplaceDialog,
                SIGNAL(find(QString, bool, bool)),
                this,
                SLOT(find(QString, bool, bool))
               );
        connect(m_findAndReplaceDialog,
                SIGNAL(findAndReplace(QString, QString, bool, bool)),
                this,
                SLOT(findAndReplace(QString, QString, bool, bool))
               );
        this->changeHeight();
        //QLabel *label = new QLabel("ASDSDASASASSDA", this);
        //m_layout->addWidget(label, 2, 1);
    }
    else
    {
        m_findAndReplaceDialog->setQuery(query);
    }
}


void CodeEdit::jumpToLineDialog(int line, int maxLines)
{
    //FindDialog
    if (NULL != m_findAndReplaceDialog)
    {
        delete m_findAndReplaceDialog;
        m_findAndReplaceDialog = NULL;
    }
    if (NULL != m_findDialog)
    {
        delete m_findDialog;
        m_findDialog = NULL;
    }

    if (NULL == m_jumpToLineDialog)
    {
        m_jumpToLineDialog = new JumpToLine(this, line+1, maxLines);
        m_jumpToLineDialog->show();
        m_layout->addWidget(m_jumpToLineDialog, 2, 1);
        connect(m_jumpToLineDialog,
                SIGNAL(closeWidget(CodeEditBottomWidget)),
                this,
                SLOT(closeBottomWidget(CodeEditBottomWidget))
               );
        connect(m_jumpToLineDialog,
                SIGNAL(jmpToLine(int)),
                this,
                SLOT(jumpToLine(int))
               );
        this->changeHeight();
        //QLabel *label = new QLabel("ASDSDASASASSDA", this);
        //m_layout->addWidget(label, 2, 1);
    }
}


void CodeEdit::closeBottomWidget(CodeEditBottomWidget widget)
{
    switch (widget)
    {
        case CodeEditBottomWidget::WFIND:
        {
            delete m_findDialog;
            m_findDialog = NULL;
            break;
        }
        case CodeEditBottomWidget::WFINDANDREPLACE:
        {
            delete m_findAndReplaceDialog;
            m_findAndReplaceDialog = NULL;
            break;
        }
        case CodeEditBottomWidget::WJUMPTOLINE:
        {
            delete m_jumpToLineDialog;
            m_jumpToLineDialog = NULL;
            break;
        }
    }
    this->changeHeight();
}


void CodeEdit::find(QString query, bool next, bool caseSensitive)
{
    m_textEdit->findAndMark(query, next, caseSensitive);
}


void CodeEdit::jumpToLine(int line)
{
    m_textEdit->jumpToLine(line-1);
    delete m_jumpToLineDialog;
    m_jumpToLineDialog = NULL;
    m_textEdit->setFocus();
}


void CodeEdit::findAndReplace(QString find, QString replace, bool all, bool caseSensitive)
{
    m_textEdit->findAndReplace(find, replace, all, caseSensitive);
}