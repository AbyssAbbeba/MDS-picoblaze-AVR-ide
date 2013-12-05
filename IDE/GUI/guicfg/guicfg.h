#ifndef GUICFG_H
#define GUICFG_H

#include <QFont>
#include <QColor>
#include <QString>




class GuiCfg
{
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




    private:
        GuiCfg()
        {
        }

        //instance
        //static GuiCfg *instance = NULL;

        //recent files
        GuiCfg::RecentFiles recentFiles;

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
        GuiCfg::UndefinedValueOpt undefinedValue;

        //highlight options
        bool highlightEnabled;
        GuiCfg::HighlightOpt highComment;
        GuiCfg::HighlightOpt highLabel;
        GuiCfg::HighlightOpt highKeyword;
        GuiCfg::HighlightOpt highOperand;
        GuiCfg::HighlightOpt highOperator;
        GuiCfg::HighlightOpt highQuote;

        //warnings
        WarningsOpt warnings;

        

    public:
        static GuiCfg& getInstance()
        {
            static GuiCfg instance;
            return instance;
        }

        //setters
        void setEditorFont(QFont font)
        {
            editorFont = font;
        }

        void setSimColor(QColor color)
        {
            simColorLine = color;
        }

        void setSimWidgetUpdatedColor(QColor color)
        {
            simColorWidgetChanged = color;
        }

        void setBreakpointColor(QColor color)
        {
            simColorBreakpoint = color;
        }

        void setWarningsOpt(GuiCfg::WarningsOpt opts)
        {
            warnings = opts;
        }

        void fileOpened(QString path, QString name)
        {
            for (int i = 5; i > 1; i--)
            {
                recentFiles.fileNames[i] = recentFiles.fileNames[i-1];
                recentFiles.filePaths[i] = recentFiles.filePaths[i-1];
            }
            recentFiles.fileNames[0] = name;
            recentFiles.filePaths[0] = path;
        }

        void setHighlightOpt(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role, GuiCfg::HighlightOpt opts)
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

        void setDefaultIDEGeneral()
        {
        }

        void setDefaultIDEShortcuts()
        {
        }

        void setDefaultEditFont()
        {
        }

        void setDefaultEditGeneral()
        {
        }

        void setDefaultEditSyntax()
        {
        }

        void setDefaultSimWarnings()
        {
        }

        void setDefaultSimOthers()
        {
        }

        void setDefaultAll()
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
        QFont getEditorFont()
        {
            return editorFont;
        }

        QColor getSimColor()
        {
            return simColorLine;
        }

        QColor getSimWidgetChangedColor()
        {
            return simColorWidgetChanged;
        }

        QColor getBreakpointColor()
        {
            return simColorBreakpoint;
        }

        GuiCfg::WarningsOpt getWarningsOpt()
        {
            return warnings;
        }

        GuiCfg::RecentFiles getRecentFiles()
        {
            return recentFiles;
        }

        GuiCfg::HighlightOpt getHighlightOpts(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role)
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
        
    
};


#endif