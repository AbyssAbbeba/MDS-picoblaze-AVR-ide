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
 * @file guicfg_items.h
 */


#ifndef GUICFG_ITEMS_H
#define GUICFG_ITEMS_H


#include <QFont>
#include <QColor>
#include <QString>


class GuiCfg_Items
{
    public:
        GuiCfg_Items()
        {
        }

        ~GuiCfg_Items()
        {
        }

        
        typedef struct HighlightOpt
        {
            QColor color;
            bool italic;
            bool underline;
            bool strike;
            bool bold;
        } HighlightOpt;


        typedef struct WarningsOpt
        {
                bool memReadNonexist;
                bool memWriteNonexist;
                bool memReadUnimplement;
                bool memWriteUnimplement;
                bool memReadAccess;
                bool memWriteAccess;
                bool memReadUndef;
                bool memReadDefault;
                bool memReadWriteOnly;
                bool memWriteReadOnly;
                bool memReadPartWriteOnly;
                bool memWritePartReadOnly;
                bool memReadReserved;
                bool memWriteReserved;

                bool stackOverflow;
                bool stackUnderflow;

                bool cpuOpcode;
                bool cpuJump;
                bool cpuCall;
                bool cpuIRQ;
                bool cpuRet;
                bool cpuReti;
                bool cpuOpset;
                bool cpuInsUnsupported;
                bool cpuInsIgnored;
                bool cpuPcOverflow;
                bool cpuPcUnderflow;
        } WarningsOpt;


        /*typedef struct RecentFiles
        {
            QList<QString> filePaths;
        } RecentFiles;


        typedef struct RecentProjects
        {
            QList<QString> projectPaths;
        } RecentProjects;*/


        typedef enum
        {
            RANDOM = 0,
            ZERO,
            HIGHEST
        } UndefinedValueOpt;


        typedef enum
        {
            PICOBLAZEASM = 0
        } HighlightLang;

        typedef enum
        {
            COMMENT = 0,
            LABEL,
            KEYWORD,
            OPERAND,
            OPERATOR,
            QUOTE
        } HighlightRole;

        typedef struct
        {
            QString path;
            QString args;
            bool toolBar;
        } ExternalApp;

        QList<ExternalApp> externalApps;

        
        //recent files
        QList<QString> recentFiles;
        QList<QString> recentProjects;

        //last dialog (open, save) path
        QString lastDialogPath;

        //ide general
        bool splash;
        bool tipsOnStart;
        bool sessionRestoration;
        QString language;

        //editor general
        int tabWidth;
        bool tabToSpaces;
        int spacesInTab;
        QString encoding;
        QString eol;


        //fonts
        QFont editorFont;

        //sim options
        QColor simColorCurrLine;
        QColor simColorPrevLine;
        QColor simColorPrev2Line;
        QColor simColorWidgetChanged;
        GuiCfg_Items::UndefinedValueOpt undefinedValue;

        //highlight options
        bool highlightEnabled;
        GuiCfg_Items::HighlightOpt highComment;
        GuiCfg_Items::HighlightOpt highLabel;
        GuiCfg_Items::HighlightOpt highKeyword;
        GuiCfg_Items::HighlightOpt highOperand;
        GuiCfg_Items::HighlightOpt highOperator;
        GuiCfg_Items::HighlightOpt highQuote;

        //warnings
        WarningsOpt warnings;

        //compiler options
        QString family;
        int scratchpadSize;
        int progMemSize;
        int intVector;
        int hwBuild;
        QString mainFileName;
        QString mainFilePath;
        bool useMainFile;
        //Symbol, Macro, Debug, Code, List, Hex, Bin, SRec, Mem, RawHex, Verilog, VHDL
        QList<bool> compileOpt;
        QList<int> compileDepths;

        bool defaultVerilog;
        bool defaultVHDL;
        QString templateVerilog;
        QString templateVHDL;

        //paths
        QString configPath;
        QString compilerPath;
        QString templatePath;
        QString examplePath;
        QString tempPath;
        QString helpPath;
        QString sessionPath;
        QString licensePath;
        QString userGuidePath;

        //session
        QList<QString> sessionProjectPaths;
        QList<QString> sessionFilePaths;
        QList<QString> sessionFileParentProjects;

        QString version;
        
};


#endif