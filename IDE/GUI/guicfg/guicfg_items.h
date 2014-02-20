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


        typedef struct RecentFiles
        {
            QString fileNames[5];
            QString filePaths[5];
        } RecentFiles;


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

        
        //recent files
        GuiCfg_Items::RecentFiles recentFiles;

        //last dialog (open, save) path
        QString lastDialogPath;

        //ide general
        bool splash;
        bool tipsOnStart;
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
        QColor simColorLine;
        QColor simColorBreakpoint;
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
        //Symbol, Macro, Debug, Code, List, Hex, Bin, SRec
        QList<bool> compileOpt;
        
};


#endif