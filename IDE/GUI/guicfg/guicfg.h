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

        void setDefaultAll();
        
        
        //getters
        QFont getEditorFont();

        QColor getSimColor();

        QColor getSimWidgetChangedColor();

        QColor getBreakpointColor();

        GuiCfg::WarningsOpt getWarningsOpt();

        GuiCfg::RecentFiles getRecentFiles();

        QString getLastDialogPath();

        GuiCfg::HighlightOpt getHighlightOpts(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role);

        bool getSplash();
        
        bool getTipsOnStart();

        QString getLanguage();

        int getTabWidth();

        bool getTabToSpaces();

        int getSpacesInTabs();

        QString getEncoding();

        QString getEOL();

        //xml parser
        void loadConfig();
        
        void saveConfig();
};


#endif