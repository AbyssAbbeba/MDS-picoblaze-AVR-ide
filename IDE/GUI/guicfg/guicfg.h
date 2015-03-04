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
 * @file guicfg.h
 */


#ifndef GUICFG_H
#define GUICFG_H


#include <QFont>
#include <QObject>
#include "guicfg_items.h"
#include "../enums/enums.h"


class GuiCfg : public QObject, public GuiCfg_Items
{
    Q_OBJECT
    
    signals:
        void editorFontChanged(QFont font);
        void tabWidthChanged(int width);
        void tabToSpacesChanged(bool enabled);
        void spacesInTabChanged(int width);
        void externalAppsChanged();
        

    public:
        GuiCfg();

        ~GuiCfg();
        
        static GuiCfg& getInstance();

        //setters
        void setEditorFont(QFont font);

        void setSimColor(QColor color);

        void setSimWidgetUpdatedColor(QColor color);

        void setWarningsOpt(GuiCfg::WarningsOpt opts);

        void fileOpened(QString path);

        void projectOpened(QString path);

        void setLastDialogPath(QString path);

        void setHighlightOpt(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role, GuiCfg::HighlightOpt opts);

        void setDefaultIDEGeneral(bool trial);

        void setDefaultIDEShortcuts();

        void setDefaultEditFont();

        void setDefaultEditGeneral();

        void setDefaultEditSyntax();

        void setDefaultSimWarnings();

        void setDefaultSimOthers();

        void setDefaultProject();

        void setDefaultPaths(bool release);

        void setDefaultAll(bool trial = false);

        void setTabWidth(int width);
        
        void setTabToSpaces(bool enabled);

        void setSpacesInTab(int count);

        void setEncoding(QString encoding);

        void setEOL(QString eol);

        void setSplash(bool enabled);

        void setExternalApps(QList<GuiCfg::ExternalApp> apps);

        void setSessionRestoration(bool enabled);

        void sessionAppendProject(QString path);

        void sessionAppendFile(QString path);

        void sessionAppendFileParentProject(QString path);

        void sessionClear();
        
        
        //getters
        QFont getEditorFont();

        QColor getSimColor();

        QColor getSimWidgetChangedColor();

        GuiCfg::WarningsOpt getWarningsOpt();

        QList<QString> getRecentFiles();
        
        QList<QString> getRecentProjects();

        QString getLastDialogPath();

        bool getHighlightEnabled();

        GuiCfg::HighlightOpt getHighlightOpts(GuiCfg::HighlightLang lang, GuiCfg::HighlightRole role);

        bool getSplash();
        
        QList<GuiCfg::ExternalApp> getExternalApps();
        
        bool getTipsOnStart();

        bool getSessionRestoration();

        QString getLanguage();

        int getTabWidth();

        bool getTabToSpaces();

        int getSpacesInTab();

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

        QList<int> getProjectCompDepth();

        QString getConfigPath();

        QString getCompilerPath();

        QString getExamplePath();

        QString getTemplatePath();

        QString getTempPath();

        QString getHelpPath();

        QString getLicensePath();

        QString getUserGuidePath();

        bool getProjectDefVHDL();
        
        bool getProjectDefVerilog();
        
        QString getProjectPathVHDL();
        
        QString getProjectPathVerilog();

        QList<QString> getSessionProjectPaths();

        QList<QString> getSessionFilePaths();

        QList<QString> getSessionFileParentProjects();

        QDateTime getTrial();

        //xml parser
        bool loadConfig();
        
        void saveConfig();

        void saveSession();

        bool loadSession();
};


#endif