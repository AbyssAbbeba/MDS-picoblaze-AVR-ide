/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GuiCfg
 * @file guicfg.cpp
 */


#include <QtGui>
#include <QtXml>
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
    this->sessionRestoration = true;
    this->language = "English";
}


void GuiCfg::setDefaultIDEShortcuts()
{
}

void GuiCfg::setDefaultEditFont()
{
    #ifdef Q_OS_LINUX
        this->editorFont.setFamily("Monospace");
    #elif defined(Q_OS_WIN32)
        this->editorFont.setFamily("Courier");
    #else
        qDebug() << "GuiCfg: setDefaultEditFont: Platform not supported";
    #endif
    this->editorFont.setPointSize(9);
}


void GuiCfg::setDefaultEditGeneral()
{
    this->tabWidth = 4;
    this->tabToSpaces = true;
    this->spacesInTab = 4;
    this->encoding = "utf-8";
    this->eol = "lf";
}


void GuiCfg::setDefaultEditSyntax()
{
    this->highlightEnabled = true;
    
    this->highComment.color.setNamedColor("#FFFFFF");
    this->highComment.italic = false;
    this->highComment.underline = false;
    this->highComment.strike = false;
    this->highComment.bold = false;

    this->highLabel.color.setNamedColor("#FFFFFF");
    this->highLabel.italic = false;
    this->highLabel.underline = false;
    this->highLabel.strike = false;
    this->highLabel.bold = false;

    this->highKeyword.color.setNamedColor("#FFFFFF");
    this->highKeyword.italic = false;
    this->highKeyword.underline = false;
    this->highKeyword.strike = false;
    this->highKeyword.bold = false;

    this->highOperand.color.setNamedColor("#FFFFFF");
    this->highOperand.italic = false;
    this->highOperand.underline = false;
    this->highOperand.strike = false;
    this->highOperand.bold = false;

    this->highOperator.color.setNamedColor("#FFFFFF");
    this->highOperator.italic = false;
    this->highOperator.underline = false;
    this->highOperator.strike = false;
    this->highOperator.bold = false;

    this->highQuote.color.setNamedColor("#FFFFFF");
    this->highQuote.italic = false;
    this->highQuote.underline = false;
    this->highQuote.strike = false;
    this->highQuote.bold = false;
}


void GuiCfg::setDefaultSimWarnings()
{
    this->warnings.memReadNonexist = true;
    this->warnings.memWriteNonexist = true;
    this->warnings.memReadUnimplement = true;
    this->warnings.memWriteUnimplement = true;
    this->warnings.memReadAccess = true;
    this->warnings.memWriteAccess = true;
    this->warnings.memReadUndef = true;
    this->warnings.memReadDefault = true;
    this->warnings.memReadWriteOnly = true;
    this->warnings.memWriteReadOnly = true;
    this->warnings.memReadPartWriteOnly = true;
    this->warnings.memWritePartReadOnly = true;
    this->warnings.memReadReserved = true;
    this->warnings.memWriteReserved = true;
    this->warnings.stackOverflow = true;
    this->warnings.stackUnderflow = true;
    this->warnings.cpuOpcode = true;
    this->warnings.cpuJump = true;
    this->warnings.cpuCall = true;
    this->warnings.cpuIRQ = true;
    this->warnings.cpuRet = true;
    this->warnings.cpuReti = true;
    this->warnings.cpuOpset = true;
    this->warnings.cpuInsUnsupported = true;
    this->warnings.cpuInsIgnored = true;
    this->warnings.cpuPcOverflow = true;
    this->warnings.cpuPcUnderflow = true;
}


void GuiCfg::setDefaultSimOthers()
{
    this->undefinedValue = (GuiCfg_Items::UndefinedValueOpt)0;
    this->simColorLine.setRgb(102,204,255, 255);
    this->simColorBreakpoint.setNamedColor("#00FF00");
    this->simColorWidgetChanged.setNamedColor("#FFFF00");
}


void GuiCfg::setDefaultProject()
{
    this->family = "kcpsm3";
    this->scratchpadSize = 64;
    this->progMemSize = 1024;
    this->intVector = 0x3ff;
    this->hwBuild = -1;
    this->mainFileName = "";
    this->mainFilePath = "";
    this->useMainFile = false;
    //Symbol, Macro, Debug, Code, List, Hex, Bin, SRec
    this->compileOpt.append(false);
    this->compileOpt.append(false);
    this->compileOpt.append(true);
    this->compileOpt.append(false);
    this->compileOpt.append(true);
    this->compileOpt.append(true);
    this->compileOpt.append(false);
    this->compileOpt.append(false);
    this->compileOpt.append(false);
    this->compileOpt.append(false);
    this->compileOpt.append(true);
    this->compileOpt.append(true);

    this->defaultVerilog = true;
    this->defaultVHDL = true;
    this->templateVerilog = "";
    this->templateVHDL = "";
}


void GuiCfg::setDefaultPaths(bool release)
{
    if (true == release)
    {
        QByteArray rootArray;
        #ifdef Q_OS_LINUX
            rootArray = qgetenv("HOME");
        #elif defined(Q_OS_WIN32)
            rootArray = qgetenv("USERPROFILE");
        #endif
        QString root(rootArray);
        QDir homeDir(root + "/.mds");
        //if (false == homeDir.exists())
        //{
        //    homeDir.mkpath(".");
        //}   
        this->configPath = homeDir.absolutePath() + "/config.xml";
        this->sessionPath = homeDir.absolutePath() + "/session.xml";
        this->compilerPath = "../include/mds";
        this->examplePath = "../share/mds/demoproject";
        this->templatePath = this->compilerPath + "/assembler/PicoBlaze";
        this->tempPath = "../share/mds/temp";
        this->helpPath = "../doc/mds";
    }
    else
    {
        #ifdef Q_OS_LINUX
            this->configPath = "./resources/xml/config.xml";
            this->sessionPath = "./resources/xml/session.xml";
            this->compilerPath = "../compiler/include";
            this->examplePath = "./demoprojekt/Example";
            this->templatePath = this->compilerPath + "/assembler/PicoBlaze";
            this->tempPath = "./temp";
            this->helpPath = "../docs/manual";
        #elif defined(Q_OS_WIN32)
            this->configPath = "./GUI/resources/xml/config.xml";
            this->sessionPath = "./GUI/resources/xml/session.xml";
            this->compilerPath = "./compiler/include";
            this->examplePath = "./GUI/demoprojekt/Example";
            this->templatePath = this->compilerPath + "/assembler/PicoBlaze";
            this->tempPath = "./GUI/temp";
            this->helpPath = "./docs/manual";
        #endif
    }
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
    this->setDefaultProject();
}


void GuiCfg::setTabWidth(int width)
{
    this->tabWidth = width;
}


void GuiCfg::setTabToSpaces(bool enabled)
{
    this->tabToSpaces = enabled;
}

        
void GuiCfg::setSpacesInTab(int count)
{
    this->spacesInTab = count;
}

        
void GuiCfg::setEncoding(QString encoding)
{
    this->encoding = encoding;
}

        
void GuiCfg::setEOL(QString eol)
{
    this->eol = eol;
}


void GuiCfg::setSessionRestoration(bool enabled)
{
    this->sessionRestoration = enabled;
}


void GuiCfg::sessionAppendProject(QString path)
{
    this->sessionProjectPaths.append(path);
}


void GuiCfg::sessionAppendFile(QString path)
{
    this->sessionFilePaths.append(path);
}


void GuiCfg::sessionAppendFileParentProject(QString path)
{
    this->sessionFileParentProjects.append(path);
}


void GuiCfg::sessionClear()
{
    this->sessionProjectPaths.clear();
    this->sessionFilePaths.clear();
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

bool GuiCfg::getSplash()
{
    return this->splash;
}

bool GuiCfg::getTipsOnStart()
{
    return this->tipsOnStart;
}

bool GuiCfg::getSessionRestoration()
{
    return this->sessionRestoration;
}

QString GuiCfg::getLanguage()
{
    return this->language;
}

int GuiCfg::getTabWidth()
{
    return this->tabWidth;
}

bool GuiCfg::getTabToSpaces()
{
    return this->tabToSpaces;
}

int GuiCfg::getSpacesInTabs()
{
    return this->spacesInTab;
}

QString GuiCfg::getEncoding()
{
    return this->encoding;
}

QString GuiCfg::getEOL()
{
    return this->eol;
}


bool GuiCfg::getHighlightEnabled()
{
    return this->highlightEnabled;
}


QColor GuiCfg::getCurrLineColor()
{
    this->simColorLine.setAlpha(255);
    return this->simColorLine;
}


QColor GuiCfg::getPrevLineColor()
{
    this->simColorLine.setAlpha(125);
    return this->simColorLine;
}


QColor GuiCfg::getPrevLine2Color()
{
    this->simColorLine.setAlpha(50);
    return this->simColorLine;
}


QString GuiCfg::getProjectFamily()
{
    return this->family;
}


int GuiCfg::getProjectIntVector()
{
    return this->intVector;
}


int GuiCfg::getProjectHWBuild()
{
    return this->hwBuild;
}


int GuiCfg::getProjectScratchpadSize()
{
    return this->scratchpadSize;
}


int GuiCfg::getProjectProgMemSize()
{
    return this->progMemSize;
}


QList<bool> GuiCfg::getProjectCompOpt()
{
    return this->compileOpt;
}


QString GuiCfg::getCompilerPath()
{
    return this->compilerPath;
}


QString GuiCfg::getExamplePath()
{
    return this->examplePath;
}


QString GuiCfg::getTemplatePath()
{
    return this->templatePath;
}


QString GuiCfg::getTempPath()
{
    return this->tempPath;
}


QString GuiCfg::getHelpPath()
{
    return this->helpPath;
}


bool GuiCfg::getProjectDefVHDL()
{
    return this->defaultVHDL;
}


bool GuiCfg::getProjectDefVerilog()
{
    return this->defaultVerilog;
}


QString GuiCfg::getProjectPathVHDL()
{
    return this->templateVHDL;
}


QString GuiCfg::getProjectPathVerilog()
{
    return this->templateVerilog;
}


QList<QString> GuiCfg::getSessionProjectPaths()
{
    return this->sessionProjectPaths;
}


QList<QString> GuiCfg::getSessionFilePaths()
{
    return this->sessionFilePaths;
}


QList<QString> GuiCfg::getSessionFileParentProjects()
{
    return this->sessionFileParentProjects;
}













//xml parsers


void GuiCfg::loadConfig()
{
    QDomDocument domDoc("config");
    //QFile cfgFile("./resources/xml/config.xml");
    QFile cfgFile(this->configPath);
    if (!cfgFile.open(QIODevice::ReadOnly))
    {
        qDebug() << "GuiCfg: config file not found";
        this->setDefaultAll();
        this->saveConfig();
        return;
    }
    if (!domDoc.setContent(&cfgFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "config")
        {
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "IDEGeneral")
                    {
                        QDomNode xmlIDEGeneralNode = xmlElement.firstChild();
                        QDomElement xmlIDEGeneralElement;
                        while (!xmlIDEGeneralNode.isNull())
                        {
                            xmlIDEGeneralElement = xmlIDEGeneralNode.toElement();
                            if (xmlIDEGeneralElement.tagName() == "Option")
                            {
                                if (xmlIDEGeneralElement.attribute("name", "") == "splash")
                                {
                                    this->splash = (xmlIDEGeneralElement.attribute("param", "") == "true");
                                }
                                else if (xmlIDEGeneralElement.attribute("name", "") == "tips")
                                {
                                    this->tipsOnStart = (xmlIDEGeneralElement.attribute("param", "") == "true");
                                }
                                else if (xmlIDEGeneralElement.attribute("name", "") == "session")
                                {
                                    this->sessionRestoration = (xmlIDEGeneralElement.attribute("param", "") == "true");
                                }
                                else if (xmlIDEGeneralElement.attribute("name", "") == "language")
                                {
                                    this->language = xmlIDEGeneralElement.attribute("param", "");
                                }
                            }
                            xmlIDEGeneralNode = xmlIDEGeneralNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "IDEShortcuts")
                    {
                        QDomNode xmlIDEShortcutsNode = xmlElement.firstChild();
                        QDomElement xmlIDEShortcutsElement;
                        while (!xmlIDEShortcutsNode.isNull())
                        {
                            xmlIDEShortcutsElement = xmlIDEShortcutsNode.toElement();
                            if (xmlIDEShortcutsElement.tagName() == "Shortcut")
                            {
                            }
                            xmlIDEShortcutsNode = xmlIDEShortcutsNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "EditorGeneral")
                    {
                        QDomNode xmlEditorGeneralNode = xmlElement.firstChild();
                        QDomElement xmlEditorGeneralElement;
                        while (!xmlEditorGeneralNode.isNull())
                        {
                            xmlEditorGeneralElement = xmlEditorGeneralNode.toElement();
                            if (xmlEditorGeneralElement.tagName() == "Option")
                            {
                                if (xmlEditorGeneralElement.attribute("name", "") == "tabwidth")
                                {
                                    this->tabWidth = xmlEditorGeneralElement.attribute("param", "").toInt();
                                }
                                else if (xmlEditorGeneralElement.attribute("name", "") == "spaces")
                                {
                                    this->tabToSpaces = (xmlEditorGeneralElement.attribute("param", "") == "true");
                                }
                                else if (xmlEditorGeneralElement.attribute("name", "") == "spaceswidth")
                                {
                                    this->spacesInTab = xmlEditorGeneralElement.attribute("param", "").toInt();
                                }
                                else if (xmlEditorGeneralElement.attribute("name", "") == "encoding")
                                {
                                    this->encoding = xmlEditorGeneralElement.attribute("param", "");
                                }
                                else if (xmlEditorGeneralElement.attribute("name", "") == "eol")
                                {
                                    this->eol = xmlEditorGeneralElement.attribute("param", "");
                                }
                            }
                            xmlEditorGeneralNode = xmlEditorGeneralNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "EditorHighlighter")
                    {
                        QDomNode xmlEditorHighlightNode = xmlElement.firstChild();
                        QDomElement xmlEditorHighlightElement;
                        while (!xmlEditorHighlightNode.isNull())
                        {
                            xmlEditorHighlightElement = xmlEditorHighlightNode.toElement();
                            if (xmlEditorHighlightElement.tagName() == "Option")
                            {
                                if (xmlEditorHighlightElement.attribute("name", "") == "enabled")
                                {
                                    this->highlightEnabled = (xmlEditorHighlightElement.attribute("param", "") == "true");
                                }
                                else if (xmlEditorHighlightElement.attribute("name", "") == "highlight")
                                {
                                    if (xmlEditorHighlightElement.attribute("type", "") == "comment")
                                    {
                                        this->highComment.color = QColor(xmlEditorHighlightElement.attribute("color", ""));
                                        this->highComment.italic = (xmlEditorHighlightElement.attribute("i", "") == "true");
                                        this->highComment.underline = (xmlEditorHighlightElement.attribute("u", "") == "true");
                                        this->highComment.strike = (xmlEditorHighlightElement.attribute("s", "") == "true");
                                        this->highComment.bold = (xmlEditorHighlightElement.attribute("b", "") == "true");
                                    }
                                    else if (xmlEditorHighlightElement.attribute("type", "") == "label")
                                    {
                                        this->highLabel.color = QColor(xmlEditorHighlightElement.attribute("color", ""));
                                        this->highLabel.italic = (xmlEditorHighlightElement.attribute("i", "") == "true");
                                        this->highLabel.underline = (xmlEditorHighlightElement.attribute("u", "") == "true");
                                        this->highLabel.strike = (xmlEditorHighlightElement.attribute("s", "") == "true");
                                        this->highLabel.bold = (xmlEditorHighlightElement.attribute("b", "") == "true");
                                    }
                                    else if (xmlEditorHighlightElement.attribute("type", "") == "keyword")
                                    {
                                        this->highKeyword.color = QColor(xmlEditorHighlightElement.attribute("color", ""));
                                        this->highKeyword.italic = (xmlEditorHighlightElement.attribute("i", "") == "true");
                                        this->highKeyword.underline = (xmlEditorHighlightElement.attribute("u", "") == "true");
                                        this->highKeyword.strike = (xmlEditorHighlightElement.attribute("s", "") == "true");
                                        this->highKeyword.bold = (xmlEditorHighlightElement.attribute("b", "") == "true");
                                    }
                                    else if (xmlEditorHighlightElement.attribute("type", "") == "operand")
                                    {
                                        this->highOperand.color = QColor(xmlEditorHighlightElement.attribute("color", ""));
                                        this->highOperand.italic = (xmlEditorHighlightElement.attribute("i", "") == "true");
                                        this->highOperand.underline = (xmlEditorHighlightElement.attribute("u", "") == "true");
                                        this->highOperand.strike = (xmlEditorHighlightElement.attribute("s", "") == "true");
                                        this->highOperand.bold = (xmlEditorHighlightElement.attribute("b", "") == "true");
                                    }
                                    else if (xmlEditorHighlightElement.attribute("type", "") == "operator")
                                    {
                                        this->highOperator.color = QColor(xmlEditorHighlightElement.attribute("color", ""));
                                        this->highOperator.italic = (xmlEditorHighlightElement.attribute("i", "") == "true");
                                        this->highOperator.underline = (xmlEditorHighlightElement.attribute("u", "") == "true");
                                        this->highOperator.strike = (xmlEditorHighlightElement.attribute("s", "") == "true");
                                        this->highOperator.bold = (xmlEditorHighlightElement.attribute("b", "") == "true");
                                    }
                                    else if (xmlEditorHighlightElement.attribute("type", "") == "quote")
                                    {
                                        this->highQuote.color = QColor(xmlEditorHighlightElement.attribute("color", ""));
                                        this->highQuote.italic = (xmlEditorHighlightElement.attribute("i", "") == "true");
                                        this->highQuote.underline = (xmlEditorHighlightElement.attribute("u", "") == "true");
                                        this->highQuote.strike = (xmlEditorHighlightElement.attribute("s", "") == "true");
                                        this->highQuote.bold = (xmlEditorHighlightElement.attribute("b", "") == "true");
                                    }
                                }
                            }
                            xmlEditorHighlightNode = xmlEditorHighlightNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "EditorFonts")
                    {
                        QDomNode xmlEditorFontsNode = xmlElement.firstChild();
                        QDomElement xmlEditorFontsElement;
                        while (!xmlEditorFontsNode.isNull())
                        {
                            xmlEditorFontsElement = xmlEditorFontsNode.toElement();
                            if (xmlEditorFontsElement.tagName() == "Option")
                            {
                                if (xmlEditorFontsElement.attribute("name", "") == "font")
                                {
                                    this->editorFont.setFamily(xmlEditorFontsElement.attribute("family", ""));
                                    this->editorFont.setPointSize(xmlEditorFontsElement.attribute("size", "").toInt());
                                }
                            }
                            xmlEditorFontsNode = xmlEditorFontsNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "SimWarnings")
                    {
                        QDomNode xmlSimWarningsNode = xmlElement.firstChild();
                        QDomElement xmlSimWarningsElement;
                        while (!xmlSimWarningsNode.isNull())
                        {
                            xmlSimWarningsElement = xmlSimWarningsNode.toElement();
                            if (xmlSimWarningsElement.tagName() == "Option")
                            {
                                if (xmlSimWarningsElement.attribute("name", "") == "mem_rnonexist")
                                {
                                    this->warnings.memReadNonexist = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_wnonexist")
                                {
                                    this->warnings.memWriteNonexist = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_runimplement")
                                {
                                    this->warnings.memReadUnimplement = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_wunimplement")
                                {
                                    this->warnings.memWriteUnimplement = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_rdenied")
                                {
                                    this->warnings.memReadAccess = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_wdenied")
                                {
                                    this->warnings.memWriteAccess = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_runidentified")
                                {
                                    this->warnings.memReadUndef = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_rdefault")
                                {
                                    this->warnings.memReadDefault = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_rwonly")
                                {
                                    this->warnings.memReadWriteOnly = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_wronly")
                                {
                                    this->warnings.memWriteReadOnly = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_rpartwonly")
                                {
                                    this->warnings.memReadPartWriteOnly = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_wpartronly")
                                {
                                    this->warnings.memWritePartReadOnly = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_rreserved")
                                {
                                    this->warnings.memReadReserved = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "mem_wreserved")
                                {
                                    this->warnings.memWriteReserved = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "stack_over")
                                {
                                    this->warnings.stackOverflow = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "stack_under")
                                {
                                    this->warnings.stackUnderflow = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_opcode")
                                {
                                    this->warnings.cpuOpcode = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_jump")
                                {
                                    this->warnings.cpuJump = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_call")
                                {
                                    this->warnings.cpuCall = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_irq")
                                {
                                    this->warnings.cpuIRQ = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_ret")
                                {
                                    this->warnings.cpuRet = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_reti")
                                {
                                    this->warnings.cpuReti = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_opset")
                                {
                                    this->warnings.cpuOpset = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_instunsupported")
                                {
                                    this->warnings.cpuInsUnsupported = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_instignored")
                                {
                                    this->warnings.cpuInsIgnored = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_pcover")
                                {
                                    this->warnings.cpuPcOverflow = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                                else if (xmlSimWarningsElement.attribute("name", "") == "cpu_pcunder")
                                {
                                    this->warnings.cpuPcUnderflow = (xmlSimWarningsElement.attribute("param", "") == "true");
                                }
                            }
                            xmlSimWarningsNode = xmlSimWarningsNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "SimOthers")
                    {
                        QDomNode xmlSimOthersNode = xmlElement.firstChild();
                        QDomElement xmlSimOthersElement;
                        while (!xmlSimOthersNode.isNull())
                        {
                            xmlSimOthersElement = xmlSimOthersNode.toElement();
                            if (xmlSimOthersElement.tagName() == "Option")
                            {
                                if (xmlSimOthersElement.attribute("name", "") == "undef")
                                {
                                    if (xmlSimOthersElement.attribute("param", "") == "random")
                                    {
                                        this->undefinedValue = (GuiCfg_Items::UndefinedValueOpt)0;
                                    }
                                    else if (xmlSimOthersElement.attribute("param", "") == "zero")
                                    {
                                        this->undefinedValue = (GuiCfg_Items::UndefinedValueOpt)1;
                                    }
                                    else
                                    {
                                        //highest
                                        this->undefinedValue = (GuiCfg_Items::UndefinedValueOpt)2;
                                    }
                                }
                                else if (xmlSimOthersElement.attribute("name", "") == "color")
                                {
                                    if (xmlSimOthersElement.attribute("type", "") == "line")
                                    {
                                        this->simColorLine.setNamedColor(xmlSimOthersElement.attribute("color", ""));
                                    }
                                    else if (xmlSimOthersElement.attribute("type", "") == "breakpoint")
                                    {
                                        this->simColorBreakpoint.setNamedColor(xmlSimOthersElement.attribute("color", ""));
                                    }
                                    else if (xmlSimOthersElement.attribute("type", "") == "update")
                                    {
                                        this->simColorWidgetChanged.setNamedColor(xmlSimOthersElement.attribute("color", ""));
                                    }
                                }
                            }
                            xmlSimOthersNode = xmlSimOthersNode.nextSibling();
                        }
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
        }
    }
    cfgFile.close();
    this->setDefaultProject();
}


void GuiCfg::saveConfig()
{
    QDomDocument domDoc("config");
    QDomElement xmlRoot = domDoc.createElement("config");
    domDoc.appendChild(xmlRoot);

    //IDEGeneral
    QDomElement xmlIDEGeneral = domDoc.createElement("IDEGeneral");
    QDomElement xmlSplash = domDoc.createElement("Option");
    xmlSplash.setAttribute("name", "splash");
    if (this->splash == true)
    {
        xmlSplash.setAttribute("param", "true");
    }
    else
    {
        xmlSplash.setAttribute("param", "false");
    }
    xmlIDEGeneral.appendChild(xmlSplash);
    QDomElement xmlTips = domDoc.createElement("Option");
    xmlTips.setAttribute("name", "tips");
    if (this->tipsOnStart == true)
    {
        xmlTips.setAttribute("param", "true");
    }
    else
    {
        xmlTips.setAttribute("param", "false");
    }
    xmlIDEGeneral.appendChild(xmlTips);
    QDomElement xmlSession = domDoc.createElement("Option");
    xmlSession.setAttribute("name", "session");
    if (this->sessionRestoration == true)
    {
        xmlSession.setAttribute("param", "true");
    }
    else
    {
        xmlSession.setAttribute("param", "false");
    }
    xmlIDEGeneral.appendChild(xmlSession);
    QDomElement xmlLanguage = domDoc.createElement("Option");
    xmlLanguage.setAttribute("name", "language");
    xmlLanguage.setAttribute("param", this->language);
    xmlIDEGeneral.appendChild(xmlLanguage);
    xmlRoot.appendChild(xmlIDEGeneral);

    //IDEShortcuts
    QDomElement xmlIDEShortcuts = domDoc.createElement("IDEShortcuts");
    xmlRoot.appendChild(xmlIDEShortcuts);

    //EditorGeneral
    QDomElement xmlEditorGeneral = domDoc.createElement("EditorGeneral");
    QDomElement xmlTabWidth = domDoc.createElement("Option");
    xmlTabWidth.setAttribute("name", "tabwidth");
    xmlTabWidth.setAttribute("param", QString::number(this->tabWidth));
    xmlEditorGeneral.appendChild(xmlTabWidth);
    QDomElement xmlSpaces = domDoc.createElement("Option");
    xmlSpaces.setAttribute("name", "spaces");
    if (this->tabToSpaces == true)
    {
        xmlSpaces.setAttribute("param", "true");
    }
    else
    {
        xmlSpaces.setAttribute("param", "false");
    }
    xmlEditorGeneral.appendChild(xmlSpaces);
    QDomElement xmlSpacesWidth = domDoc.createElement("Option");
    xmlSpacesWidth.setAttribute("name", "spaceswidth");
    xmlSpacesWidth.setAttribute("param", QString::number(this->spacesInTab));
    xmlEditorGeneral.appendChild(xmlSpacesWidth);
    QDomElement xmlEncoding = domDoc.createElement("Option");
    xmlEncoding.setAttribute("name", "encoding");
    xmlEncoding.setAttribute("param", this->encoding);
    xmlEditorGeneral.appendChild(xmlEncoding);
    QDomElement xmlEol = domDoc.createElement("Option");
    xmlEol.setAttribute("name", "eol");
    xmlEol.setAttribute("param", this->eol);
    xmlEditorGeneral.appendChild(xmlEol);
    xmlRoot.appendChild(xmlEditorGeneral);

    //EditorHighlighter
    QDomElement xmlEditorHighlighter = domDoc.createElement("EditorHighlighter");
    QDomElement xmlEnabled = domDoc.createElement("Option");
    xmlEnabled.setAttribute("name", "enabled");
    if (this->highlightEnabled == true)
    {
        xmlEnabled.setAttribute("param", "true");
    }
    else
    {
        xmlEnabled.setAttribute("param", "false");
    }
    xmlEditorHighlighter.appendChild(xmlEnabled);
    QDomElement xmlComment = domDoc.createElement("Option");
    xmlComment.setAttribute("name", "highlight");
    xmlComment.setAttribute("type", "comment");
    xmlComment.setAttribute("color", this->highComment.color.name());
    if (this->highComment.italic == true)
    {
        xmlComment.setAttribute("i", "true");
    }
    else
    {
        xmlComment.setAttribute("i", "false");
    }
    if (this->highComment.underline == true)
    {
        xmlComment.setAttribute("u", "true");
    }
    else
    {
        xmlComment.setAttribute("u", "false");
    }
    if (this->highComment.strike == true)
    {
        xmlComment.setAttribute("s", "true");
    }
    else
    {
        xmlComment.setAttribute("s", "false");
    }
    if (this->highComment.bold == true)
    {
        xmlComment.setAttribute("b", "true");
    }
    else
    {
        xmlComment.setAttribute("b", "false");
    }
    xmlEditorHighlighter.appendChild(xmlComment);
    QDomElement xmlLabel = domDoc.createElement("Option");
    xmlLabel.setAttribute("name", "highlight");
    xmlLabel.setAttribute("type", "label");
    xmlLabel.setAttribute("color", this->highLabel.color.name());
    if (this->highLabel.italic == true)
    {
        xmlLabel.setAttribute("i", "true");
    }
    else
    {
        xmlLabel.setAttribute("i", "false");
    }
    if (this->highLabel.underline == true)
    {
        xmlLabel.setAttribute("u", "true");
    }
    else
    {
        xmlLabel.setAttribute("u", "false");
    }
    if (this->highLabel.strike == true)
    {
        xmlLabel.setAttribute("s", "true");
    }
    else
    {
        xmlLabel.setAttribute("s", "false");
    }
    if (this->highLabel.bold == true)
    {
        xmlLabel.setAttribute("b", "true");
    }
    else
    {
        xmlLabel.setAttribute("b", "false");
    }
    xmlEditorHighlighter.appendChild(xmlLabel);
    QDomElement xmlKeyword = domDoc.createElement("Option");
    xmlKeyword.setAttribute("name", "highlight");
    xmlKeyword.setAttribute("type", "keyword");
    xmlKeyword.setAttribute("color", this->highKeyword.color.name());
    if (this->highKeyword.italic == true)
    {
        xmlKeyword.setAttribute("i", "true");
    }
    else
    {
        xmlKeyword.setAttribute("i", "false");
    }
    if (this->highKeyword.underline == true)
    {
        xmlKeyword.setAttribute("u", "true");
    }
    else
    {
        xmlKeyword.setAttribute("u", "false");
    }
    if (this->highKeyword.strike == true)
    {
        xmlKeyword.setAttribute("s", "true");
    }
    else
    {
        xmlKeyword.setAttribute("s", "false");
    }
    if (this->highKeyword.bold == true)
    {
        xmlKeyword.setAttribute("b", "true");
    }
    else
    {
        xmlKeyword.setAttribute("b", "false");
    }
    xmlEditorHighlighter.appendChild(xmlKeyword);
    QDomElement xmlOperand = domDoc.createElement("Option");
    xmlOperand.setAttribute("name", "highlight");
    xmlOperand.setAttribute("type", "operand");
    xmlOperand.setAttribute("color", this->highOperand.color.name());
    if (this->highOperand.italic == true)
    {
        xmlOperand.setAttribute("i", "true");
    }
    else
    {
        xmlOperand.setAttribute("i", "false");
    }
    if (this->highOperand.underline == true)
    {
        xmlOperand.setAttribute("u", "true");
    }
    else
    {
        xmlOperand.setAttribute("u", "false");
    }
    if (this->highOperand.strike == true)
    {
        xmlOperand.setAttribute("s", "true");
    }
    else
    {
        xmlOperand.setAttribute("s", "false");
    }
    if (this->highOperand.bold == true)
    {
        xmlOperand.setAttribute("b", "true");
    }
    else
    {
        xmlOperand.setAttribute("b", "false");
    }
    xmlEditorHighlighter.appendChild(xmlOperand);
    QDomElement xmlOperator = domDoc.createElement("Option");
    xmlOperator.setAttribute("name", "highlight");
    xmlOperator.setAttribute("type", "operator");
    xmlOperator.setAttribute("color", this->highOperator.color.name());
    if (this->highOperator.italic == true)
    {
        xmlOperator.setAttribute("i", "true");
    }
    else
    {
        xmlOperator.setAttribute("i", "false");
    }
    if (this->highOperator.underline == true)
    {
        xmlOperator.setAttribute("u", "true");
    }
    else
    {
        xmlOperator.setAttribute("u", "false");
    }
    if (this->highOperator.strike == true)
    {
        xmlOperator.setAttribute("s", "true");
    }
    else
    {
        xmlOperator.setAttribute("s", "false");
    }
    if (this->highOperator.bold == true)
    {
        xmlOperator.setAttribute("b", "true");
    }
    else
    {
        xmlOperator.setAttribute("b", "false");
    }
    xmlEditorHighlighter.appendChild(xmlOperator);
    QDomElement xmlQuote = domDoc.createElement("Option");
    xmlQuote.setAttribute("name", "highlight");
    xmlQuote.setAttribute("type", "quote");
    xmlQuote.setAttribute("color", this->highQuote.color.name());
    if (this->highQuote.italic == true)
    {
        xmlQuote.setAttribute("i", "true");
    }
    else
    {
        xmlQuote.setAttribute("i", "false");
    }
    if (this->highQuote.underline == true)
    {
        xmlQuote.setAttribute("u", "true");
    }
    else
    {
        xmlQuote.setAttribute("u", "false");
    }
    if (this->highQuote.strike == true)
    {
        xmlQuote.setAttribute("s", "true");
    }
    else
    {
        xmlQuote.setAttribute("s", "false");
    }
    if (this->highQuote.bold == true)
    {
        xmlQuote.setAttribute("b", "true");
    }
    else
    {
        xmlQuote.setAttribute("b", "false");
    }
    xmlEditorHighlighter.appendChild(xmlQuote);
    xmlRoot.appendChild(xmlEditorHighlighter);

    //EditorFonts
    QDomElement xmlEditorFonts = domDoc.createElement("EditorFonts");
    QDomElement xmlFont = domDoc.createElement("Option");
    xmlFont.setAttribute("name", "font");
    xmlFont.setAttribute("family", this->editorFont.family());
    xmlFont.setAttribute("size", QString::number(this->editorFont.pointSize()));
    xmlEditorFonts.appendChild(xmlFont);
    xmlRoot.appendChild(xmlEditorFonts);

    //SimWarnings
    QDomElement xmlSimWarnings = domDoc.createElement("SimWarnings");
    QDomElement xmlRNonExist = domDoc.createElement("Option");
    xmlRNonExist.setAttribute("name", "mem_rnonexist");
    if (this->warnings.memReadNonexist == true)
    {
        xmlRNonExist.setAttribute("param", "true");
    }
    else
    {
        xmlRNonExist.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRNonExist);
    QDomElement xmlWNonExist = domDoc.createElement("Option");
    xmlWNonExist.setAttribute("name", "mem_wnonexist");
    if (this->warnings.memWriteNonexist == true)
    {
        xmlWNonExist.setAttribute("param", "true");
    }
    else
    {
        xmlWNonExist.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlWNonExist);
    QDomElement xmlRUnimplement = domDoc.createElement("Option");
    xmlRUnimplement.setAttribute("name", "mem_runimplement");
    if (this->warnings.memReadUnimplement == true)
    {
        xmlRUnimplement.setAttribute("param", "true");
    }
    else
    {
        xmlRUnimplement.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRUnimplement);
    QDomElement xmlWUnimplement = domDoc.createElement("Option");
    xmlWUnimplement.setAttribute("name", "mem_wunimplement");
    if (this->warnings.memWriteUnimplement == true)
    {
        xmlWUnimplement.setAttribute("param", "true");
    }
    else
    {
        xmlWUnimplement.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlWUnimplement);
    QDomElement xmlRDenied = domDoc.createElement("Option");
    xmlRDenied.setAttribute("name", "mem_rdenied");
    if (this->warnings.memReadAccess == true)
    {
        xmlRDenied.setAttribute("param", "true");
    }
    else
    {
        xmlRDenied.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRDenied);
    QDomElement xmlWDenied = domDoc.createElement("Option");
    xmlWDenied.setAttribute("name", "mem_wdenied");
    if (this->warnings.memWriteAccess == true)
    {
        xmlWDenied.setAttribute("param", "true");
    }
    else
    {
        xmlWDenied.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlWDenied);
    QDomElement xmlRUnidentified = domDoc.createElement("Option");
    xmlRUnidentified.setAttribute("name", "mem_runidentified");
    if (this->warnings.memReadUndef == true)
    {
        xmlRUnidentified.setAttribute("param", "true");
    }
    else
    {
        xmlRUnidentified.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRUnidentified);
    QDomElement xmlRDefault = domDoc.createElement("Option");
    xmlRDefault.setAttribute("name", "mem_rdefault");
    if (this->warnings.memReadDefault == true)
    {
        xmlRDefault.setAttribute("param", "true");
    }
    else
    {
        xmlRDefault.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRDefault);
    QDomElement xmlRWOnly = domDoc.createElement("Option");
    xmlRWOnly.setAttribute("name", "mem_rwonly");
    if (this->warnings.memReadWriteOnly == true)
    {
        xmlRWOnly.setAttribute("param", "true");
    }
    else
    {
        xmlRWOnly.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRWOnly);
    QDomElement xmlWROnly = domDoc.createElement("Option");
    xmlWROnly.setAttribute("name", "mem_wronly");
    if (this->warnings.memWriteReadOnly == true)
    {
        xmlWROnly.setAttribute("param", "true");
    }
    else
    {
        xmlWROnly.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlWROnly);
    QDomElement xmlRPartWOnly = domDoc.createElement("Option");
    xmlRPartWOnly.setAttribute("name", "mem_rpartwonly");
    if (this->warnings.memReadPartWriteOnly == true)
    {
        xmlRPartWOnly.setAttribute("param", "true");
    }
    else
    {
        xmlRPartWOnly.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRPartWOnly);
    QDomElement xmlWPartROnly = domDoc.createElement("Option");
    xmlWPartROnly.setAttribute("name", "mem_wpartronly");
    if (this->warnings.memWritePartReadOnly == true)
    {
        xmlWPartROnly.setAttribute("param", "true");
    }
    else
    {
        xmlWPartROnly.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlWPartROnly);
    QDomElement xmlRReserved = domDoc.createElement("Option");
    xmlRReserved.setAttribute("name", "mem_rreserved");
    if (this->warnings.memReadReserved == true)
    {
        xmlRReserved.setAttribute("param", "true");
    }
    else
    {
        xmlRReserved.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRReserved);
    QDomElement xmlWReserved = domDoc.createElement("Option");
    xmlWReserved.setAttribute("name", "mem_wreserved");
    if (this->warnings.memWriteReserved == true)
    {
        xmlWReserved.setAttribute("param", "true");
    }
    else
    {
        xmlWReserved.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlWReserved);
    QDomElement xmlStackOver = domDoc.createElement("Option");
    xmlStackOver.setAttribute("name", "stack_over");
    if (this->warnings.stackOverflow == true)
    {
        xmlStackOver.setAttribute("param", "true");
    }
    else
    {
        xmlStackOver.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlStackOver);
    QDomElement xmlStackUnder = domDoc.createElement("Option");
    xmlStackUnder.setAttribute("name", "stack_under");
    if (this->warnings.stackUnderflow == true)
    {
        xmlStackUnder.setAttribute("param", "true");
    }
    else
    {
        xmlStackUnder.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlStackUnder);
    QDomElement xmlOpCode = domDoc.createElement("Option");
    xmlOpCode.setAttribute("name", "cpu_opcode");
    if (this->warnings.cpuOpcode == true)
    {
        xmlOpCode.setAttribute("param", "true");
    }
    else
    {
        xmlOpCode.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlOpCode);
    QDomElement xmlJump = domDoc.createElement("Option");
    xmlJump.setAttribute("name", "cpu_jump");
    if (this->warnings.cpuJump == true)
    {
        xmlJump.setAttribute("param", "true");
    }
    else
    {
        xmlJump.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlJump);
    QDomElement xmlCall = domDoc.createElement("Option");
    xmlCall.setAttribute("name", "cpu_call");
    if (this->warnings.cpuCall == true)
    {
        xmlCall.setAttribute("param", "true");
    }
    else
    {
        xmlCall.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlCall);
    QDomElement xmlIRQ = domDoc.createElement("Option");
    xmlIRQ.setAttribute("name", "cpu_irq");
    if (this->warnings.cpuIRQ == true)
    {
        xmlIRQ.setAttribute("param", "true");
    }
    else
    {
        xmlIRQ.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlIRQ);
    QDomElement xmlRet = domDoc.createElement("Option");
    xmlRet.setAttribute("name", "cpu_ret");
    if (this->warnings.cpuRet == true)
    {
        xmlRet.setAttribute("param", "true");
    }
    else
    {
        xmlRet.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlRet);
    QDomElement xmlReti = domDoc.createElement("Option");
    xmlReti.setAttribute("name", "cpu_reti");
    if (this->warnings.cpuReti == true)
    {
        xmlReti.setAttribute("param", "true");
    }
    else
    {
        xmlReti.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmlReti);
    QDomElement xmOpSet = domDoc.createElement("Option");
    xmOpSet.setAttribute("name", "cpu_opset");
    if (this->warnings.cpuOpset == true)
    {
        xmOpSet.setAttribute("param", "true");
    }
    else
    {
        xmOpSet.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmOpSet);
    QDomElement xmInstUnsup = domDoc.createElement("Option");
    xmInstUnsup.setAttribute("name", "cpu_instunsupported");
    if (this->warnings.cpuInsUnsupported == true)
    {
        xmInstUnsup.setAttribute("param", "true");
    }
    else
    {
        xmInstUnsup.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmInstUnsup);
    QDomElement xmInstIgnor = domDoc.createElement("Option");
    xmInstIgnor.setAttribute("name", "cpu_instignored");
    if (this->warnings.cpuInsIgnored == true)
    {
        xmInstIgnor.setAttribute("param", "true");
    }
    else
    {
        xmInstIgnor.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmInstIgnor);
    QDomElement xmPCOver = domDoc.createElement("Option");
    xmPCOver.setAttribute("name", "cpu_pcover");
    if (this->warnings.cpuPcOverflow == true)
    {
        xmPCOver.setAttribute("param", "true");
    }
    else
    {
        xmPCOver.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmPCOver);
    QDomElement xmPCUnder = domDoc.createElement("Option");
    xmPCUnder.setAttribute("name", "cpu_pcunder");
    if (this->warnings.cpuPcUnderflow == true)
    {
        xmPCUnder.setAttribute("param", "true");
    }
    else
    {
        xmPCUnder.setAttribute("param", "false");
    }
    xmlSimWarnings.appendChild(xmPCUnder);
    xmlRoot.appendChild(xmlSimWarnings);

    //SimOthers
    QDomElement xmlSimOthers = domDoc.createElement("SimOthers");
    QDomElement xmlUndef = domDoc.createElement("Option");
    xmlUndef.setAttribute("name", "undef");
    if (this->undefinedValue == 0)
    {
        xmlUndef.setAttribute("param", "random");
    }
    else if (this->undefinedValue == 1)
    {
        xmlUndef.setAttribute("param", "zero");
    }
    else
    {
        xmlUndef.setAttribute("param", "highest");
    }
    xmlSimOthers.appendChild(xmlUndef);
    QDomElement xmlLine = domDoc.createElement("Option");
    xmlLine.setAttribute("name", "color");
    xmlLine.setAttribute("type", "line");
    xmlLine.setAttribute("color", this->simColorLine.name());
    xmlSimOthers.appendChild(xmlLine);
    QDomElement xmlBreak = domDoc.createElement("Option");
    xmlBreak.setAttribute("name", "color");
    xmlBreak.setAttribute("type", "breakpoint");
    xmlBreak.setAttribute("color", this->simColorBreakpoint.name());
    xmlSimOthers.appendChild(xmlBreak);
    QDomElement xmlUpdate = domDoc.createElement("Option");
    xmlUpdate.setAttribute("name", "color");
    xmlUpdate.setAttribute("type", "update");
    xmlUpdate.setAttribute("color", this->simColorWidgetChanged.name());
    xmlSimOthers.appendChild(xmlUpdate);
    xmlRoot.appendChild(xmlSimOthers);
    QFile cfgFile(this->configPath);
    cfgFile.open(QIODevice::WriteOnly);
    QTextStream xmlStream(&cfgFile);
    xmlStream << domDoc.toString();
    cfgFile.close();
}


void GuiCfg::saveSession()
{
    if (this->sessionProjectPaths.count() > 0)
    {
        QDomDocument domDoc("sessionRestoration");
        QDomElement xmlRoot = domDoc.createElement("sessionRestoration");
        domDoc.appendChild(xmlRoot);

        QDomElement xmlProjects = domDoc.createElement("Projects");
        for (int i = 0; i < this->sessionProjectPaths.count(); i++)
        {
            QDomElement xmlProject = domDoc.createElement("Project");
            xmlProject.setAttribute("path", this->sessionProjectPaths.at(i));
            xmlProjects.appendChild(xmlProject);
        }
        xmlRoot.appendChild(xmlProjects);

        QDomElement xmlFiles = domDoc.createElement("Files");
        for (int i = 0; i < this->sessionFilePaths.count(); i++)
        {
            QDomElement xmlFile = domDoc.createElement("File");
            xmlFile.setAttribute("path", this->sessionFilePaths.at(i));
            xmlFile.setAttribute("parent", this->sessionFileParentProjects.at(i));
            xmlFiles.appendChild(xmlFile);
        }
        xmlRoot.appendChild(xmlFiles);

        QFile sessionFile(this->sessionPath);
        sessionFile.open(QIODevice::WriteOnly);
        QTextStream xmlStream(&sessionFile);
        xmlStream << domDoc.toString();
        sessionFile.close();
    }
    else
    {
        QFile sessionFile(this->sessionPath);
        sessionFile.open(QIODevice::WriteOnly);
        sessionFile.close();
    }
}


bool GuiCfg::loadSession()
{
    QDomDocument domDoc("sessionRestoration");
    QFile sessionFile(this->sessionPath);
    if (!sessionFile.open(QIODevice::ReadOnly))
    {
        return false;
    }
    if (!domDoc.setContent(&sessionFile))
    {
        return false;
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "sessionRestoration")
        {
            return false;
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "Projects")
                    {
                        QDomNode xmlProjectsNode = xmlElement.firstChild();
                        QDomElement xmlProjectsElement;
                        while (!xmlProjectsNode.isNull())
                        {
                            xmlProjectsElement = xmlProjectsNode.toElement();
                            if (xmlProjectsElement.tagName() == "Project")
                            {

                                this->sessionProjectPaths.append(xmlProjectsElement.attribute("path", ""));

                            }
                            xmlProjectsNode = xmlProjectsNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "Files")
                    {
                        QDomNode xmlFilesNode = xmlElement.firstChild();
                        QDomElement xmlFilesElement;
                        while (!xmlFilesNode.isNull())
                        {
                            xmlFilesElement = xmlFilesNode.toElement();
                            if (xmlFilesElement.tagName() == "File")
                            {
                                this->sessionFilePaths.append(xmlFilesElement.attribute("path", ""));
                                this->sessionFileParentProjects.append(xmlFilesElement.attribute("parent", ""));
                            }
                            xmlFilesNode = xmlFilesNode.nextSibling();
                        }
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
        }
    }
    sessionFile.close();
    return true;
}