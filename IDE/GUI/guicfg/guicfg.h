#ifndef GUICFG_H
#define GUICFG_H


#include <QFont>
#include <QObject>
#include "guicfg_items.h"
#include "../enums/enums.h"
#include "../errordialog/errordlg.h"


class GuiCfg : public QObject, public GuiCfg_Items
{
    Q_OBJECT
    
    signals:
        void editorFontChanged(QFont font);
        

    public:
        GuiCfg();

        ~GuiCfg();
        
        static GuiCfg& getInstance();

        //setters
        void setEditorFont(QFont font);

        void setSimColor(QColor color);

        void setSimWidgetUpdatedColor(QColor color);

        void setBreakpointColor(QColor color);

        void setWarningsOpt(GuiCfg::WarningsOpt opts);

        void fileOpened(QString path, QString name);

        void setLastDialogPath(QString path);

        void setHighlightOpt(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role, GuiCfg::HighlightOpt opts);

        void setDefaultIDEGeneral();

        void setDefaultIDEShortcuts();

        void setDefaultEditFont();

        void setDefaultEditGeneral();

        void setDefaultEditSyntax();

        void setDefaultSimWarnings();

        void setDefaultSimOthers();

        void setDefaultProject();

        void setDefaultPaths(bool release);

        void setDefaultAll();

        void setTabWidth(int width);
        
        void setTabToSpaces(bool enabled);

        void setSpacesInTab(int count);

        void setEncoding(QString encoding);

        void setEOL(QString eol);
        
        
        //getters
        QFont getEditorFont();

        QColor getSimColor();

        QColor getSimWidgetChangedColor();

        QColor getBreakpointColor();

        GuiCfg::WarningsOpt getWarningsOpt();

        GuiCfg::RecentFiles getRecentFiles();

        QString getLastDialogPath();

        bool getHighlightEnabled();

        GuiCfg::HighlightOpt getHighlightOpts(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role);

        bool getSplash();
        
        bool getTipsOnStart();

        QString getLanguage();

        int getTabWidth();

        bool getTabToSpaces();

        int getSpacesInTabs();

        QString getEncoding();

        QString getEOL();

        QColor getCurrLineColor();

        QColor getPrevLineColor();

        QColor getPrevLine2Color();

        QString getProjectFamily();
        
        int getProjectIntVector();
        
        int getProjectHWBuild();
        
        int getProjectScratchpadSize();
        
        int getProjectProgMemSize();
        
        QList<bool> getProjectCompOpt();

        QString getCompilerPath();

        QString getExamplePath();

        QString getTemplatePath();

        QString getTempPath();

        QString getHelpPath();

        bool getProjectDefVHDL();
        
        bool getProjectDefVerilog();
        
        QString getProjectPathVHDL();
        
        QString getProjectPathVerilog();

        //xml parser
        void loadConfig();
        
        void saveConfig();
};


#endif