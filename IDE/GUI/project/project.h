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
 * @file project.h
 */


#ifndef PROJECT_H
#define PROJECT_H

//#define ERR_OK 500
//#define ERR_ASSIGN 501
//#define ERR_CONTENT 502

#include <set>
#include <QString>
#include <QList>
#include <QStringList>
#include <QPair>
//#include "mainform.h"
#include "../enums/enums.h"
#include "../widgets/RegWatcher/regwatcherexportstruct.h"


//class MainForm;
class CodeEdit;
class ProjectTree;

//simulation handle
//class McuMemoryView;
//class RegDisplayList;
class MCUSimControl;
//class McuSimCfgMgr;

class Project;
class QDockWidget;
class QFile;
class QTreeWidgetItem;
class QColor;
class DataFile;
class DbgFile;


/*typedef enum LangType {
    LANG_ASM = 0, LANG_C
} LangType;*/


/**
 * @brief Manager of Projects.
 * @ingroup GUI
 * @class ProjectMan
 */
class ProjectMan : public QObject
{
    Q_OBJECT
    public:
        ProjectMan(QWidget *parent);
        void addFile(QString path, QString name);
        QString addUntrackedFile(QString path, QString name);
        void addProject(QString name, QString path, QString architecture, LangType langType, QFile *file);
        void addUntrackedProject();
        void openProject(QFile *file);
        void setActive(Project *activePrj);
        void setActiveByIndex(int index);
        bool isActiveProject(Project *project);
        Project* getActive();
        Project* getUntracked();
        Project* getSimulated();
        void setSimulated(Project* project);
        bool isOpened(QString path);
        void createActiveMakefile();
        void closeProject(Project* project);
        QList<Project*> getOpenProjects();

    signals:
        void connectProject(Project *project);
        void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void tabifyDockWidget(QDockWidget *first, QDockWidget *second);
        void projectOpened();

    private:
        int projectCount;
        QList<Project*> openProjects;
        Project *activeProject;
        Project *untrackedProject;
        Project *simulatedProject;
    
};



/**
 * @brief Project class. Manages files and simulation.
 * @ingroup GUI
 * @class Project
 */
class Project : public QObject
{
    Q_OBJECT
    
    public:
        //konstruktor pro otevirani projektu
        Project(QFile *file, ProjectMan *parent);
        Project(ProjectMan *parent);
        //konstruktor pro prazdny projekt
        Project(QString name, QString path, QString arch, LangType langType, QFile *file, ProjectMan *parent);
        ~Project();

        void saveProject();
        void setupSim();
        void setupSim(QString family);
        int start(QString file = "", QString dumpFiles = "", DbgFile *dbgFile = NULL, DataFile *dataFile = NULL);
        void setBreakpoints(bool set);
        void stop();
        void reset();
        void step();
        void run();
        void animate();
        MCUSimControl* getSimControl();
        void setCompileOpt(QList<bool> opt);
        void setCompileIncPaths(QList<QString> paths);
        void setUseMainFile(bool enabled);
        void setIntVector(int value);
        void setHWBuild(int value);
        void setScratchpad(int value);
        void setProgMem(int value);
        void setName(QString name);
        void setFamily(QString family);
        void setTemplates(bool verilog, QString verilogTemplate, bool VHDL, QString VHDLTemplate);
        void setClock(double clock, int mult);
        int handleBreakpoint(QString file, int line);
        void moveBreakpointsAdd(QString file, int line, unsigned int linesAdded);
        void moveBreakpointsRemove(QString file, int line, unsigned int linesRemoved);
        int handleBookmark(QString file, int line);
        void moveBookmarksAdd(QString file, int line, unsigned int linesAdded);
        void moveBookmarksRemove(QString file, int line, unsigned int linesRemoved);
        QList<QPair<QString, QSet<unsigned int>>>* getBreakpointsListRef();
        QList<QPair<QString, QSet<unsigned int>>>* getBookmarksListRef();
        QList<unsigned int> getBreakpointsForFileAbsolute(QString file);
        QList<unsigned int> getBookmarksForFileAbsolute(QString file);
        void renameFile(QString oldPath, QString newPath);
        void setAsmType(int type);
        int getAsmType();
        void setFileOpened(QString path, bool opened);
        void setRegWatchers(QList<RegWatcherExportStruct> regWatchers);
        QList<RegWatcherExportStruct> getRegWatchers();

        QDockWidget *prjDockWidget;
        ProjectTree *prjTreeWidget;

        QString prjName;
        QString prjPath;
        int fileCount;

        LangType langType;
        QString family;
        int scratchpadSize;
        int progMemSize;
        int intVector;
        int hwBuild;
        QString mainFileName;
        QString mainFilePath;
        bool useMainFile;
        QList<QString> fileNames;
        QList<QString> filePaths;
        QList<bool> m_fileStats;
        //Symbol, Macro, Debug, Code, List, Hex, Bin, SRec, Mem, RawHex, Verilog, VHDL
        QList<bool> compileOpt;
        QStringList compileIncPaths;

        //templates
        bool defaultVerilog;
        bool defaultVHDL;
        QString templateVerilog;
        QString templateVHDL;

        //simulation handle
        //McuMemoryView *m_mcuMemoryView;
        //RegDisplayList *m_regList;
        MCUSimControl *m_simControlUnit;
        QString simulatedFile;
        //simulation others
        double clock;
        int clockMult;

        int m_untitledCounter;

    public slots:
        void setMainFile(QString path, QString name);
        void removeFile(QString path, QString name);
        QString addFile(QString path, QString name);
        void reloadProjectTree();
        
    private slots:
        void setActive();
        void openItem();
        void openUntrackedItem();
        void emitFileCount();
        void startCfgDlgCore();
        void handleUpdateRequest(int mask);
        void closeProjectSlot();
        void breakpointReachedSlot();
        void requestProjectPath();

    signals:
        //void highlightLine(QString file, int line, QColor *color);
        void simHighlightLines(std::vector<std::pair<const std::string *, unsigned int>> curr,
                               std::vector<std::pair<const std::string *, unsigned int>> prev,
                               std::vector<std::pair<const std::string *, unsigned int>> prev2,
                               QList<QColor*> colors);
        void setCentralByName(QString file);
        void scrollToLine(int line);
        void addUntrackedFile(QString name, QString path);
        //void addDockWidget(Qt::DockWidgetArea area, QDockWidget* dockWidget);
        void openFilePath(QString path);
        void setEditorReadOnly(bool readOnly);
        //void setCentralChanged();
        void fileCountSignal(int fileCount);
        void startConfig(Project *project);
        void changeFamily(QString family);
        void closeProject();
        void breakpointReached();
        
    private:
        //void xmlBreakpointAdd(QString file, unsigned int line);
        //void xmlBreakpointRemove(QString file, unsigned int line);
        //void xmlBreakpointFileReplace(QString file);
        
        ProjectMan *parentManager;
        QList<QPair<QString, QSet<unsigned int>>> breakPoints;
        QList<QPair<QString, QSet<unsigned int>>> bookmarks;
        std::vector<std::pair<const std::string *, unsigned int>> m_currSim;
        std::vector<std::pair<const std::string *, unsigned int>> m_prevSim;
        std::vector<std::pair<const std::string *, unsigned int>> m_prevSim2;
        QList<QColor*> simColors;

        int m_asmType;
        QList<RegWatcherExportStruct> m_regWatchers;
        
        QTreeWidgetItem *treeProjName;
        QTreeWidgetItem *treeProjSource;
        QTreeWidgetItem *treeProjInclude;
        QTreeWidgetItem *treeProjCompiled;
        QTreeWidgetItem *treeProjOther;
        //QTreeWidgetItem *treeProjUntitled;
        
       
};

#endif
