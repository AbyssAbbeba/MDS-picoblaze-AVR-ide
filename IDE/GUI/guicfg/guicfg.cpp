#include <QtGui>
#include "guicfg.h"


GuiCfg::GuiCfg()
    : QObject(0), GuiCfg_Items()
{
}

GuiCfg::~GuiCfg()
{
}


GuiCfg& GuiCfg::getInstance()
{
    static GuiCfg instance;
    return instance;
}

//setters
void GuiCfg::setEditorFont(QFont font)
{
    editorFont = font;
    emit editorFontChanged(font);
}

void GuiCfg::setSimColor(QColor color)
{
    simColorLine = color;
}

void GuiCfg::setSimWidgetUpdatedColor(QColor color)
{
    simColorWidgetChanged = color;
}

void GuiCfg::setBreakpointColor(QColor color)
{
    simColorBreakpoint = color;
}

void GuiCfg::setWarningsOpt(GuiCfg::WarningsOpt opts)
{
    warnings = opts;
}

void GuiCfg::fileOpened(QString path, QString name)
{
    for (int i = 5; i > 1; i--)
    {
        recentFiles.fileNames[i] = recentFiles.fileNames[i-1];
        recentFiles.filePaths[i] = recentFiles.filePaths[i-1];
    }
    recentFiles.fileNames[0] = name;
    recentFiles.filePaths[0] = path;
}

void GuiCfg::setLastDialogPath(QString path)
{
    lastDialogPath = path;
}

void GuiCfg::setHighlightOpt(GuiCfg_Items::HighlightLang lang, GuiCfg_Items::HighlightRole role, GuiCfg_Items::HighlightOpt opts)
{
    //check for language!
    switch (role)
    {
        case HighlightRole::COMMENT:
        {
            highComment = opts;
        }
        case GuiCfg::HighlightRole::LABEL:
        {
            highLabel = opts;
        }
        case GuiCfg::HighlightRole::KEYWORD:
        {
            highKeyword = opts;
        }
        case GuiCfg::HighlightRole::OPERAND:
        {
            highOperand = opts;
        }
        case GuiCfg::HighlightRole::OPERATOR:
        {
            highOperator = opts;
        }
        case GuiCfg::HighlightRole::QUOTE:
        {
            highQuote = opts;
        }
        default:
        {
            qDebug() << "GuiCfg: setHighlightOpt role out of bounds!";
        }
    }
}

void GuiCfg::setDefaultIDEGeneral()
{
    this->splash = false;
    this->tipsOnStart = false;
    this->language = "English";
}

void GuiCfg::setDefaultIDEShortcuts()
{
}

void GuiCfg::setDefaultEditFont()
{
    this->editorFont.setFamily("Monospace");
    this->editorFont.setPointSize(9);
}

void GuiCfg::setDefaultEditGeneral()
{
}

void GuiCfg::setDefaultEditSyntax()
{
}

void GuiCfg::setDefaultSimWarnings()
{
}

void GuiCfg::setDefaultSimOthers()
{
}

void GuiCfg::setDefaultAll()
{
    this->setDefaultIDEGeneral();
    this->setDefaultIDEShortcuts();
    this->setDefaultEditFont();
    this->setDefaultEditGeneral();
    this->setDefaultEditSyntax();
    this->setDefaultSimWarnings();
    this->setDefaultSimOthers();
}


//getters
QFont GuiCfg::getEditorFont()
{
    return editorFont;
}

QColor GuiCfg::getSimColor()
{
    return simColorLine;
}

QColor GuiCfg::getSimWidgetChangedColor()
{
    return simColorWidgetChanged;
}

QColor GuiCfg::getBreakpointColor()
{
    return simColorBreakpoint;
}

GuiCfg_Items::WarningsOpt GuiCfg::getWarningsOpt()
{
    return warnings;
}

GuiCfg_Items::RecentFiles GuiCfg::getRecentFiles()
{
    return recentFiles;
}

QString GuiCfg::getLastDialogPath()
{
    return lastDialogPath;
}

GuiCfg_Items::HighlightOpt GuiCfg::getHighlightOpts(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role)
{
    //check for language!
    switch (role)
    {
        case GuiCfg::HighlightRole::COMMENT:
        {
            return highComment;
        }
        case GuiCfg::HighlightRole::LABEL:
        {
            return highLabel;
        }
        case GuiCfg::HighlightRole::KEYWORD:
        {
            return highKeyword;
        }
        case GuiCfg::HighlightRole::OPERAND:
        {
            return highOperand;
        }
        case GuiCfg::HighlightRole::OPERATOR:
        {
            return highOperator;
        }
        case GuiCfg::HighlightRole::QUOTE:
        {
            return highQuote;
        }
        default:
        {
            qDebug() << "GuiCfg: getHighlightOpt role out of bounds! Returning comment opts";
        }
    }
    return highComment;
}