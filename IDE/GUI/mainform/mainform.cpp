/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup MainForm
 * @file mainform.cpp
 */
/*#ifdef MDS_FEATURE_FILECONVERTER
#endif
#ifdef MDS_FEATURE_DISASSEMBLER
#endif
#ifdef MDS_FEATURE_TRANSLATOR
#endif
#ifdef MDS_FEATURE_LICENCE_CERTIFICATE
#endif
#ifdef MDS_FEATURE_CONVERTER_TOOL
#endif
#ifdef MDS_FEATURE_LOOP_GENERATOR
#endif
#ifdef MDS_FEATURE_8_SEGMENT_EDITOR
#endif
#ifdef MDS_FEATURE_SIM_LED_PANEL
#endif
#ifdef MDS_FEATURE_SIM_7_SEGMENT
#endif*/

#include <QtGui>
#include <unistd.h>
#include "../../mds.h"
//#include <QtHelp/QHelpEngineCore>
//pozdeji zamenit QtGui za mensi celky
#include "mainform.h"
#include "../dialogs/projectdlg/projectdlg.h"
#include "../dialogs/savedlg/savedlg.h"
#include "../project/project.h"
#include "../errordialog/errordlg.h"
//#include "pluginman_gui.h"
#include "../dialogs/projectcfg/projectcfgdlg_core.h"
#include "../dialogs/interfacecfg/interfacecfgdlg_core.h"
//#include "../widgets/CompileWidget/compilewidget.h"
#include "../widgets/HelpWidget/helpwidget.h"
#include "../widgets/AsmMacroAnalyser/asmmacroanalyser.h"
#include "../widgets/BookmarkList/bookmarklist.h"
#include "../widgets/BreakpointList/breakpointlist.h"
#include "../widgets/CompileInfo/compileinfo.h"
#include "../widgets/DockManager/wdockmanager.h"
#include "../widgets/Editor/codeedit.h"
#include "../widgets/PicoBlazeGrid/picoblazegrid.h"
#include "../widgets/Editor/wtextedit.h"

#ifdef Q_OS_WIN
    #include <windows.h> // for Sleep
#endif


#ifdef MDS_FEATURE_DISASSEMBLER
    #include "../dialogs/disasmdlg/disasmdlg.h"
#endif
#ifdef MDS_FEATURE_TRANSLATOR
    #include "../dialogs/translatordlg/translatordlg.h"
#endif
#ifdef MDS_FEATURE_FILECONVERTER
    #include "../dialogs/fileconvertdlg/fileconvertdlg.h"
#endif

#ifdef MDS_FEATURE_LICENCE_CERTIFICATE
    #include "../widgets/LicenseWidget/licensewidget.h"
#endif
#ifdef MDS_FEATURE_LOOP_GENERATOR
    #include "../widgets/LoopGen/loop_gen.h"
#endif
#include "../widgets/AboutWidget/aboutwidget.h"
#ifdef MDS_FEATURE_8_SEGMENT_EDITOR
    #include "../widgets/Tools/Display/displaytool.h"
#endif
#ifdef MDS_FEATURE_CONVERTER_TOOL
    #include "../widgets/Tools/Convertor/convertortool.h"
#endif
#ifdef MDS_FEATURE_SIM_LED_PANEL
    #define MDS_SIM_FEATURES
    #include "../widgets/Tools/SimLed/simled.h"
#endif
#ifdef MDS_FEATURE_SIM_7_SEGMENT
    #define MDS_SIM_FEATURES
    #include "../widgets/Tools/Sim7Seg/sim7seg.h"
#endif
#ifdef MDS_FEATURE_SIM_SWITCH
    #define MDS_SIM_FEATURES
    #include "../widgets/Tools/SimSwitch/simswitch.h"
#endif
#ifdef MDS_FEATURE_SIM_PORT_LOGGER
    //#define MDS_SIM_FEATURES
    //#include "../widgets/Tools/SimPortLogger/simportlogger.h"
#endif

#include "../guicfg/guicfg.h"

#include "../widgets/WelcomeWidget/WelcomeWidget.h"





    Q_DECLARE_METATYPE (std::string);
    Q_DECLARE_METATYPE (CompilerBase::MessageType);
/**
 * @brief Constructor, inits project and dock widget manager and creates menus/toolbars
 */
MainForm::MainForm()
{
    //this->statusBar()->showMessage("Loading", 0);
    //qDebug() << "MainForm: MainForm()";
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setWindowTitle("Multitarget Development System");
    this->setWindowIcon(QIcon(":resources/icons/mainIcon_64x64.png"));
    #ifdef MDS_VARIANT_NONCOMMERCIAL
        this->setWindowTitle("MDS NON-COMMERCIAL");
    #endif
    #ifdef MDS_VARIANT_TRIAL
        QFileInfo mdsInfo(GuiCfg::getInstance().getConfigPath());
        QString trial = QString("MDS TRIAL, ") +  QString::number(MDS_TRIAL_PERIOD - mdsInfo.lastModified().daysTo(QDateTime::currentDateTime())) + QString(" days left");
        this->setWindowTitle(trial);
    #endif
    this->projectTabConnected = false;
    this->simulationStatus = false;
    this->simulationRunStatus = false;
    this->simulationAnimateStatus = false;
    this->simulationRequest = false;
    this->simulationBreakpointsReload = true;
    this->simulationBreakpointsEnabled = true;
    this->projectTabs = NULL;

    projectMan = new ProjectMan(this);
    connect(projectMan,
            SIGNAL(addDockWidget(Qt::DockWidgetArea, QDockWidget*)),
            this,
            SLOT(addDockWidgetSlot(Qt::DockWidgetArea, QDockWidget*))
           );
    connect(projectMan,
            SIGNAL(tabifyDockWidget(QDockWidget*, QDockWidget*)),
            this,
            SLOT(tabifyDockWidgetSlot(QDockWidget*, QDockWidget*))
           );
    connect(projectMan,
            SIGNAL(connectProject(Project*)),
            this,
            SLOT(connectProjectSlot(Project*))
           );
    connect(projectMan,
            SIGNAL(projectOpened()),
            this,
            SLOT(projectOpened())
           );
    QWidget *centralWidget = new QWidget(this);
    wDockManager = new WDockManager(this, centralWidget);
    this->setCentralWidget(centralWidget);
    centralWidget->show();
    //connect(this, SIGNAL(dockWidgetsCreated()), wDockManager, SLOT(dockWidgetsCreated()));
    /*connect(wDockManager,
            SIGNAL(createDockWidgets()),
            this,
            SLOT(createDockWidgets())
           );*/
    connect(wDockManager,
            SIGNAL(tabifyDockWidget(QDockWidget*, QDockWidget*)),
            this,
            SLOT(tabifyDockWidgetSlot(QDockWidget*, QDockWidget*))
           );
    connect(wDockManager,
            SIGNAL(addDockWidget(Qt::DockWidgetArea, QDockWidget*)),
            this,
            SLOT(addDockWidgetSlot(Qt::DockWidgetArea, QDockWidget*))
           );
    connect(wDockManager,
            SIGNAL(getSimProjectData()),
            this,
            SLOT(simProjectData())
           );
    connect(this,
            SIGNAL(unhighlightSim()),
            wDockManager,
            SLOT(unhighlightSimWidget())
           );
    connect(wDockManager,
            SIGNAL(stopSimSig()),
            this,
            SLOT(stopSimSlot())
           );
    connect(wDockManager,
            SIGNAL(clockChangedSig(double, int)),
            this,
            SLOT(clockChangedSlot(double, int))
           );
    connect(wDockManager,
            SIGNAL(centralCreated()),
            this,
            SLOT(enableSimActs())
           );
    connect(wDockManager,
            SIGNAL(centralClosed()),
            this,
            SLOT(disableSimActs())
           );
    connect(wDockManager,
            SIGNAL(breakpointEmit(QString, int)),
            this,
            SLOT(manageBreakpointEmit(QString, int))
           );
    connect(wDockManager,
            SIGNAL(breakpointsAddLines(QString, int, int)),
            this,
            SLOT(breakpointsAddLines(QString, int, int))
           );
    connect(wDockManager,
            SIGNAL(breakpointsRemoveLines(QString, int, int)),
            this,
            SLOT(breakpointsRemoveLines(QString, int, int))
           );
    connect(wDockManager,
            SIGNAL(bookmarkEmit(QString, int)),
            this,
            SLOT(manageBookmarkEmit(QString, int))
           );
    connect(wDockManager,
            SIGNAL(bookmarksAddLines(QString, int, int)),
            this,
            SLOT(bookmarksAddLines(QString, int, int))
           );
    connect(wDockManager,
            SIGNAL(bookmarksRemoveLines(QString, int, int)),
            this,
            SLOT(bookmarksRemoveLines(QString, int, int))
           );
    connect(wDockManager,
            SIGNAL(welcomeScrExampleSig()),
            this,
            SLOT(exampleOpen())
           );
    connect(wDockManager,
            SIGNAL(welcomeScrOpenPrjSig()),
            this,
            SLOT(openProject())
           );
    connect(wDockManager,
            SIGNAL(welcomeScrNewPrjSig()),
            this,
            SLOT(newProject())
           );
    connect(wDockManager,
            SIGNAL(welcomeScrManualSig()),
            this,
            SLOT(help())
           );
    connect(wDockManager,
            SIGNAL(welcomeScrGuideSig()),
            this,
            SLOT(userGuide())
           );
    connect(wDockManager,
            SIGNAL(welcomeScrRecentSig(QString)),
            this,
            SLOT(openProject(QString))
           );
    /*connect(wDockManager,
            SIGNAL(breakpointListRemove(QString, int)),
            this,
            SLOT(manageBreakpointRemove(QString, int))
           );*/


    /*connect(this,
            SIGNAL(sessionRestorationSignal()),
            this,
            SLOT(sessionRestorationSlot())
           );*/
    //this->dockWidgets = false;
    createActions();
    createMenu();
    createShortcuts();
    createToolbar();
    //this->welcomeDialog();
    //CreateDockWidgets();
    //CreateWelcome();
    //qDebug() << "MainForm: return MainForm()";
    //this->statusBar()->showMessage("Loaded", 5000);
}


MainForm::~MainForm()
{
    if (true == simulationStatus)
    {
        projectMan->getActive()->stop();
    }


    delete this->pm_simFlowStart;
    delete this->pm_simFlowStop;
    delete this->pm_simRun;
    delete this->pm_simAnimate;
    delete this->pm_cross;

    delete this->icon_simFlow;
    delete this->icon_simRun;
    delete this->icon_simAnimate;

    if (true == GuiCfg::getInstance().getSessionRestoration())
    {
        if (projectMan->getOpenProjects().count() > 0)
        {
            bool fileFound;
            QList<Project*> projects = projectMan->getOpenProjects();
            //qDebug() << "Mainform: prepare to project session restoration";
            for (int i = 0; i < projects.count(); i++)
            {
                fileFound = false;
                //qDebug() << "Mainform: saving project" << projects.at(i)->prjName;
                if (wDockManager->getTabCount() > 0)
                {
                    //qDebug() << "Mainform: prepare to files session restoration";
                    for (int j = 0; j < wDockManager->getTabCount(); j++)
                    {
                        if (true == wDockManager->getTabWidget(j)->isChild(projects.at(i)))
                        {
                            if ("untracked" == wDockManager->getTabWidget(j)->getPath())
                            {
                                continue;
                            }
                            fileFound = true;
                            //qDebug() << "Mainform: saving file" << wDockManager->getTabWidget(j)->getName();
                            //qDebug() << "MainForm: with parent" << projects.at(i)->prjPath;
                            GuiCfg::getInstance().sessionAppendFile(wDockManager->getTabWidget(j)->getPath());
                            GuiCfg::getInstance().sessionAppendFileParentProject(projects.at(i)->prjPath);
                        }
                    }
                }
                if ("untracked" == projects.at(i)->prjPath)
                {
                    if (true == fileFound)
                    {
                        GuiCfg::getInstance().sessionAppendProject(projects.at(i)->prjPath);
                    }
                }
                else
                {
                    GuiCfg::getInstance().sessionAppendProject(projects.at(i)->prjPath);
                }
            }
        }
        GuiCfg::getInstance().saveSession();
    }

    while (projectMan->getOpenProjects().count() > 0)
    {
        projectMan->closeProject(projectMan->getActive());
    }
    GuiCfg::getInstance().saveConfig();
    QApplication::closeAllWindows();
}



/**
 * @brief Creates menus in main menu
 */
void MainForm::createMenu()
{
    //qDebug() << "MainForm: createMenu()";
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAddAct);
    fileMenu->addAction(newAct);
    fileMenu->addSeparator();
    fileMenu->addAction(openAct);
    recentFilesMenu = fileMenu->addMenu(tr("Open Recent"));
    QList<QString> recentFiles = GuiCfg::getInstance().getRecentFiles();
    for (int i = recentFiles.count() -1; i >= 0 ; i--)
    {
        QAction *recent = new QAction(recentFiles.at(i), recentFilesMenu);
        recentFilesMenu->addAction(recent);
    }
    connect(recentFilesMenu,
            SIGNAL(triggered(QAction*)),
            this,
            SLOT(openRecentFileSlot(QAction*))
           );
    fileMenu->addSeparator();
    //fileMenu->addAction(addAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAllAct);
    fileMenu->addSeparator();
    fileMenu->addAction(closeFileAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    editMenu->addSeparator();
    editMenu->addAction(selectAllAct);
    editMenu->addAction(deselectAct);
    editMenu->addSeparator();
    editMenu->addAction(findAct);
    editMenu->addAction(findNextAct);
    editMenu->addAction(findPreviousAct);
    editMenu->addAction(replaceAct);
    editMenu->addSeparator();
    editMenu->addAction(jmpToLineAct);
    editMenu->addSeparator();
    editMenu->addAction(commentAct);
    editMenu->addAction(deleteCommentAct);
    editMenu->addSeparator();
    editMenu->addAction(jmpToBookmarkNextAct);
    editMenu->addAction(jmpToBookmarkPrevAct);


    interfaceMenu = menuBar()->addMenu(tr("&Interface"));
    interfaceMenu->addAction(interfaceConfigAct);
    //interfaceMenu->addAction(pluginAct);

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newProjAct);
    projectMenu->addAction(openProjAct);
    recentProjectsMenu = projectMenu->addMenu(tr("Open Recent"));
    QList<QString> recentProjects = GuiCfg::getInstance().getRecentProjects();
    for (int i = recentProjects.count() -1; i >= 0 ; i--)
    {
        QAction *recent = new QAction(recentProjects.at(i), recentProjectsMenu);
        recentProjectsMenu->addAction(recent);
    }
    connect(recentProjectsMenu,
            SIGNAL(triggered(QAction*)),
            this,
            SLOT(openRecentProjectSlot(QAction*))
           );

    projectMenu->addAction(saveProjAct);
    projectMenu->addAction(closeProjectAct);
    projectMenu->addSeparator();
    projectMenu->addAction(projectCompileAct);
    projectMenu->addAction(saveProjConfigAct);
    projectMenu->addAction(projectConfigAct);

    simulationMenu = menuBar()->addMenu(tr("&Simulation"));
    simulationMenu->addAction(simulationFlowAct);
    simulationMenu->addAction(simulationRunAct);
    simulationMenu->addAction(simulationAnimateAct);
    simulationMenu->addAction(simulationStepAct);
    simulationMenu->addAction(simulationResetAct);
    simulationMenu->addAction(simulationUnhighlightAct);
    simulationMenu->addSeparator();
    simulationMenu->addAction(simulationBreakpointAct);
    simulationMenu->addAction(simulationDisableBreakpointsAct);


    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    #ifdef MDS_FEATURE_DISASSEMBLER
        toolsMenu->addAction(toolDisassemblerAct);
    #endif
    #ifdef MDS_FEATURE_TRANSLATOR
        toolsMenu->addAction(toolTranslatorAct);
    #endif
    #ifdef MDS_FEATURE_FILECONVERTER
        toolsMenu->addAction(toolFileConvertAct);
    #endif
    #ifdef MDS_FEATURE_CONVERTER_TOOL
        toolsMenu->addAction(toolConvertorAct);
    #endif
    #ifdef MDS_FEATURE_8_SEGMENT_EDITOR
        toolsMenu->addAction(toolDisplayAct);
    #endif
    #ifdef MDS_FEATURE_LOOP_GENERATOR
        toolsMenu->addAction(toolLoopGenAct);
    #endif

    #if defined(MDS_SIM_FEATURES)
        simToolsMenu = menuBar()->addMenu(tr("Simulation tools"));
        #ifdef MDS_FEATURE_SIM_LED_PANEL
            simToolsMenu->addAction(toolSimLedsAct);
        #endif
        #ifdef MDS_FEATURE_SIM_7_SEGMENT
            simToolsMenu->addAction(toolSim7SegAct);
        #endif
        #ifdef MDS_FEATURE_SIM_SWITCH
            simToolsMenu->addAction(toolSimSwitchAct);
        #endif
        #ifdef MDS_FEATURE_SIM_PORT_LOGGER
        //    simToolsMenu->addAction(toolSimLoggerAct);
        #endif
    #endif

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(helpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(example1Act);
    helpMenu->addAction(welcomeAct);
    helpMenu->addAction(aboutQTAct);
    #ifdef MDS_FEATURE_LICENCE_CERTIFICATE
        helpMenu->addAction(licenseAct);
    #endif
    //qDebug() << "MainForm: return createMenu()";
}



/**
 * @brief Creates actions used in menus and toolbars and connects them to appropriate slots
 *
 */
void MainForm::createActions()
{
    //qDebug() << "MainForm: CreateActions()";


    newAddAct = new QAction(QIcon(":resources/icons/projNewAdd.png"), tr("New File"), this);
    newAddAct->setDisabled(true);
    newAddAct->setShortcut(QKeySequence("Ctrl+N"));
    connect(newAddAct, SIGNAL(triggered()), this, SLOT(newAddFile()));

    newAct = new QAction(QIcon(":resources/icons/page.png"), tr("New Untracked File"), this);
    newAct->setStatusTip("Create a new file");
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":resources/icons/folder.png"), tr("Open File"), this);
    openAct->setShortcut(QKeySequence("Ctrl+O"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));


    /*QPixmap *pm_projAdd = new QPixmap(":resources/icons/projAdd.png");
    QIcon *icon_projAdd = new QIcon(*pm_projAdd);
    addAct = new QAction(*icon_projAdd, tr("Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));
    addAct->setDisabled(true);*/


    /*QPixmap *pm_projRemoveFile = new QPixmap("src//resources/icons/projDelete.png");
    QIcon *icon_projRemoveFile = new QIcon(*pm_projRemoveFile);
    removeFileAct = new QAction(*icon_projRemoveFile, tr("&Remove file"), this);
    connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeProjFile()));*/


    saveAct = new QAction(QIcon(":resources/icons/disk.png"), tr("Save File"), this);
    saveAct->setDisabled(true);
    saveAct->setShortcut(QKeySequence("Ctrl+S"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(QIcon(":resources/icons/disk2.png"), tr("Save As"), this);
    saveAsAct->setDisabled(true);
    saveAsAct->setShortcut(QKeySequence("Ctrl+Shift+S"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    saveAllAct = new QAction(QIcon(":resources/icons/disk_multiple.png"), tr("Save All"), this);
    saveAllAct->setDisabled(true);
    saveAllAct->setShortcut(QKeySequence("Ctrl+L"));
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));

    closeFileAct = new QAction(tr("Close File"), this);
    closeFileAct->setDisabled(true);
    closeFileAct->setShortcut(QKeySequence("Ctrl+W"));
    connect(closeFileAct, SIGNAL(triggered()), this, SLOT(shortcutCloseTab()));
    


    //EDIT
    undoAct = new QAction(QIcon(":resources/icons/arrow_undo.png"), tr("Undo"), this);
    undoAct->setShortcut(QKeySequence("Ctrl+Z"));
    undoAct->setDisabled(true);
    undoAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undoSlot()));
    redoAct = new QAction(QIcon(":resources/icons/arrow_redo.png"), tr("Redo"), this);
    redoAct->setShortcut(QKeySequence("Ctrl+Shift+Z"));
    redoAct->setDisabled(true);
    redoAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redoSlot()));
    cutAct = new QAction(QIcon(":resources/icons/cut.png"), tr("Cut"), this);
    cutAct->setShortcut(QKeySequence("Ctrl+X"));
    cutAct->setDisabled(true);
    cutAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cutSlot()));
    copyAct = new QAction(QIcon(":resources/icons/page_copy.png"), tr("Copy"), this);
    copyAct->setShortcut(QKeySequence("Ctrl+C"));
    copyAct->setDisabled(true);
    copyAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySlot()));
    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setShortcut(QKeySequence("Ctrl+V"));
    pasteAct->setDisabled(true);
    pasteAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    selectAllAct = new QAction(tr("Select All"), this);
    selectAllAct->setShortcut(QKeySequence("Ctrl+A"));
    selectAllAct->setDisabled(true);
    selectAllAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(selectAllAct, SIGNAL(triggered()), this, SLOT(selectAllSlot()));
    deselectAct = new QAction(tr("Deselect"), this);
    deselectAct->setDisabled(true);
    deselectAct->setShortcut(QKeySequence("Ctrl+Shift+A"));
    deselectAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(deselectAct, SIGNAL(triggered()), this, SLOT(deselectSlot()));
    findAct = new QAction(tr("Find"), this);
    findAct->setShortcut(QKeySequence("Ctrl+F"));
    findAct->setDisabled(true);
    findAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(findAct, SIGNAL(triggered()), this, SLOT(findSlot()));
    findNextAct = new QAction(tr("Find Next"), this);
    findNextAct->setShortcut(QKeySequence("F3"));
    findNextAct->setDisabled(true);
    findNextAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(findNextAct, SIGNAL(triggered()), this, SLOT(findNextSlot()));
    findPreviousAct = new QAction(tr("Find Previous"), this);
    findPreviousAct->setShortcut(QKeySequence("Shift+F3"));
    findPreviousAct->setDisabled(true);
    findPreviousAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(findPreviousAct, SIGNAL(triggered()), this, SLOT(findPrevSlot()));
    replaceAct = new QAction(tr("Replace"), this);
    replaceAct->setShortcut(QKeySequence("Ctrl+R"));
    replaceAct->setDisabled(true);
    replaceAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(replaceAct, SIGNAL(triggered()), this, SLOT(replaceSlot()));
    jmpToLineAct = new QAction(tr("Go to Line"), this);
    jmpToLineAct->setShortcut(QKeySequence("Ctrl+G"));
    jmpToLineAct->setDisabled(true);
    jmpToLineAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(jmpToLineAct, SIGNAL(triggered()), this, SLOT(jmpToLineSlot()));
    commentAct = new QAction(tr("Comment"), this);
    commentAct->setShortcut(QKeySequence("Ctrl+D"));
    commentAct->setDisabled(true);
    commentAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(commentAct, SIGNAL(triggered()), this, SLOT(commentSlot()));
    deleteCommentAct = new QAction(tr("Uncomment"), this);
    deleteCommentAct->setShortcut(QKeySequence("Ctrl+Shift+D"));
    deleteCommentAct->setDisabled(true);
    deleteCommentAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(deleteCommentAct, SIGNAL(triggered()), this, SLOT(deleteCommentSlot()));
    jmpToBookmarkNextAct = new QAction(tr("Jump to Next Bookmark"), this);
    jmpToBookmarkNextAct->setShortcut(QKeySequence(Qt::Key_Alt + Qt::Key_PageDown));
    jmpToBookmarkNextAct->setDisabled(true);
    jmpToBookmarkNextAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(jmpToBookmarkNextAct, SIGNAL(triggered()), this, SLOT(jmpToBookmarkNextSlot()));
    jmpToBookmarkPrevAct = new QAction(tr("Jump to Previous Bookmark"), this);
    jmpToBookmarkPrevAct->setShortcut(QKeySequence(Qt::Key_Alt + Qt::Key_PageUp));
    jmpToBookmarkPrevAct->setDisabled(true);
    jmpToBookmarkPrevAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(jmpToBookmarkPrevAct, SIGNAL(triggered()), this, SLOT(jmpToBookmarkPrevSlot()));



    //INTERFACE
    interfaceConfigAct = new QAction(QIcon(":resources/icons/page_white_wrench.png"), tr("Configure"), this);
    connect(interfaceConfigAct, SIGNAL(triggered()), this, SLOT(interfaceConfig()));
    //pluginAct = new QAction(tr("Plugins"), this);
    //connect(pluginAct, SIGNAL(triggered()), this, SLOT(showPlugins()));




    newProjAct = new QAction(QIcon(":resources/icons/projNew.png"), tr("New Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    openProjAct = new QAction(QIcon(":resources/icons/projOpen.png"), tr("Open Project"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    saveProjAct = new QAction(QIcon(":resources/icons/disk.png"), tr("Save project"), this);
    connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));
    saveProjAct->setDisabled(true);

    closeProjectAct = new QAction(QIcon(":resources/icons/breakpoint_disable.png"), tr("Close Project"), this);
    connect(closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));
    closeProjectAct->setDisabled(true);

    saveProjConfigAct = new QAction(tr("Save Project Config"), this);
    saveProjConfigAct->setDisabled(true);

    exitAct = new QAction(QIcon(":resources/icons/cancel.png"), tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    projectCompileAct = new QAction(QIcon(":resources/icons/compile.png"), tr("Compile"), this);
    connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));
    projectCompileAct->setDisabled(true);
    projectCompileAct->setShortcut(Qt::Key_F5);

    projectConfigAct = new QAction(QIcon(":resources/icons/page_white_wrench.png"), tr("Configure"), this);
    connect(projectConfigAct, SIGNAL(triggered()), this, SLOT(projectConfig()));
    projectConfigAct->setDisabled(true);
    //connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));


    this->pm_simFlowStart = new QPixmap(":resources/icons/simulationStart.png");
    this->pm_simFlowStop = new QPixmap(":resources/icons/simulationStop.png");
    this->icon_simFlow = new QIcon(*pm_simFlowStart);
    simulationFlowAct = new QAction(*icon_simFlow, tr("Start Simulation"), this);
    connect(simulationFlowAct, SIGNAL(triggered()), this, SLOT(simulationFlowHandle()));
    simulationStatus = false;
    simulationRunStatus = false;
    simulationAnimateStatus = false;
    simulationFlowAct->setDisabled(true);
    simulationFlowAct->setShortcut(Qt::Key_F6);

    this->pm_simRun = new QPixmap(":resources/icons/simulationRun.png");
    this->icon_simRun = new QIcon(*pm_simRun);
    simulationRunAct = new QAction(*icon_simRun, tr("Run"), this);
    connect(simulationRunAct, SIGNAL(triggered()), this, SLOT(simulationRunHandle()));
    simulationRunAct->setDisabled(true);
    simulationRunAct->setShortcut(Qt::Key_F7);

    this->pm_simAnimate = new QPixmap(":resources/icons/simulationAnimate.png");
    this->icon_simAnimate = new QIcon(*pm_simAnimate);
    simulationAnimateAct = new QAction(*icon_simAnimate, tr("Animate"), this);
    connect(simulationAnimateAct, SIGNAL(triggered()), this, SLOT(simulationAnimateHandle()));
    simulationAnimateAct->setDisabled(true);
    simulationAnimateAct->setShortcut(Qt::Key_F8);

    simulationStepAct = new QAction(QIcon(":resources/icons/simulationStep.png"), tr("Step"), this);
    connect(simulationStepAct, SIGNAL(triggered()), this, SLOT(simulationStep()));
    simulationStepAct->setDisabled(true);
    simulationStepAct->setShortcut(Qt::Key_F9);

    simulationResetAct = new QAction(QIcon(":resources/icons/simulationReset.png"), tr("Reset"), this);
    connect(simulationResetAct, SIGNAL(triggered()), this, SLOT(simulationReset()));
    simulationResetAct->setDisabled(true);
    simulationResetAct->setShortcut(Qt::Key_F10);

    simulationUnhighlightAct = new QAction(QIcon(":resources/icons/unhighlight.png"), tr("Unhighlight"), this);
    connect(simulationUnhighlightAct, SIGNAL(triggered()), this, SLOT(unhighlight()));
    simulationUnhighlightAct->setDisabled(true);

    simulationBreakpointAct = new QAction(QIcon(":resources/icons/breakpoint.png"), tr("Breakpoint"), this);
    simulationBreakpointAct->setDisabled(true);
    connect(simulationBreakpointAct, SIGNAL(triggered()), this, SLOT(breakpointActHandle()));

    simulationDisableBreakpointsAct = new QAction(tr("Disable Breakpoints"), this);
    simulationDisableBreakpointsAct->setDisabled(true);
    connect(simulationDisableBreakpointsAct, SIGNAL(triggered()), this, SLOT(disableBreakpointsHandle()));

    #ifdef MDS_FEATURE_DISASSEMBLER
        toolDisassemblerAct = new QAction(QIcon(":resources/icons/disassemble.png"), tr("Disassembler"), this);
        connect(toolDisassemblerAct, SIGNAL(triggered()), this, SLOT(toolDisassemble()));
    #endif
    #ifdef MDS_FEATURE_TRANSLATOR
        toolTranslatorAct = new QAction(QIcon(":resources/icons/arrow_switch.png"), tr("Assembler Translator"), this);
        connect(toolTranslatorAct, SIGNAL(triggered()), this, SLOT(toolTranslate()));
    #endif
    #ifdef MDS_FEATURE_FILECONVERTER
        toolFileConvertAct = new QAction(QIcon(":resources/icons/page_white_gear.png"), tr("Data File Converter"), this);
        connect(toolFileConvertAct, SIGNAL(triggered()), this, SLOT(toolFileConvert()));
    #endif
    #ifdef MDS_FEATURE_CONVERTER_TOOL
        toolConvertorAct = new QAction(tr("Radix Converter"), this);
        connect(toolConvertorAct, SIGNAL(triggered()), this, SLOT(toolConvertor()));
    #endif
    #ifdef MDS_FEATURE_8_SEGMENT_EDITOR
        toolDisplayAct = new QAction(QIcon(":resources/icons/8segedit.png"), tr("8 Segment Editor"), this);
        connect(toolDisplayAct, SIGNAL(triggered()), this, SLOT(toolDisplay()));
    #endif
    #ifdef MDS_FEATURE_LOOP_GENERATOR
        toolLoopGenAct = new QAction(QIcon(":resources/icons/page_white_lightning.png"), tr("Loop Generator"), this);
        connect(toolLoopGenAct, SIGNAL(triggered()), this, SLOT(loopGen()));
    #endif
    #ifdef MDS_FEATURE_SIM_LED_PANEL
        toolSimLedsAct = new QAction(QIcon(":resources/icons/ledpanel.png"), tr("LED Panel"), this);
        toolSimLedsAct->setDisabled(true);
        connect(toolSimLedsAct, SIGNAL(triggered()), this, SLOT(simLeds()));
    #endif
    #ifdef MDS_FEATURE_SIM_7_SEGMENT
        toolSim7SegAct = new QAction(QIcon(":resources/icons/8seg.png"), tr("7 Segment Display"), this);
        toolSim7SegAct->setDisabled(true);
        connect(toolSim7SegAct, SIGNAL(triggered()), this, SLOT(sim7Seg()));
    #endif
    #ifdef MDS_FEATURE_SIM_SWITCH
        toolSimSwitchAct = new QAction(QIcon(":resources/icons/simplekeypad.png"), tr("Switch panel"), this);
        toolSimSwitchAct->setDisabled(true);
        connect(toolSimSwitchAct, SIGNAL(triggered()), this, SLOT(simSwitch()));
    #endif
    #ifdef MDS_FEATURE_SIM_PORT_LOGGER
        //toolSimLoggerAct = new QAction(tr("Port Logger"), this);
        //toolSimLoggerAct->setDisabled(true);
        //connect(toolSimLoggerAct, SIGNAL(triggered()), this, SLOT(simPortLogger()));
    #endif

    #ifdef MDS_FEATURE_LICENCE_CERTIFICATE
        licenseAct = new QAction(tr("License"), this);
        connect(licenseAct, SIGNAL(triggered()), this, SLOT(manageLicense()));
    #endif

    aboutAct = new QAction(QIcon(":resources/icons/mainIcon.png"), tr("About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    aboutQTAct = new QAction(tr("About QT"), this);
    connect(aboutQTAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    welcomeAct = new QAction(tr("Welcome Dialog"), this);
    connect(welcomeAct, SIGNAL(triggered()), this, SLOT(welcomeDialog()));
    helpAct = new QAction(QIcon(":resources/icons/help.png"), tr("User Manual"), this);
    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
    example1Act = new QAction(tr("Open Tutorial Project"), this);
    connect(example1Act, SIGNAL(triggered()), this, SLOT(exampleOpen()));

    this->pm_cross = new QPixmap(":resources/icons/pause.png");
    //qDebug() << "MainForm: return CreateActions()";
}


void MainForm::createShortcuts()
{
    //QShortcut *shctCloseTab = new QShortcut(this);
    //shctCloseTab->setKey(Qt::CTRL + Qt::Key_W);
    //connect(shctCloseTab, SIGNAL(activated()), this, SLOT(shortcutCloseTab()));
    QShortcut *shctChangeTabLeft = new QShortcut(this);
    shctChangeTabLeft->setKey(Qt::ALT + Qt::Key_Left);
    connect(shctChangeTabLeft, SIGNAL(activated()), this, SLOT(shortcutChangeTabLeft()));
    QShortcut *shctChangeTabRight = new QShortcut(this);
    shctChangeTabRight->setKey(Qt::ALT + Qt::Key_Right);
    connect(shctChangeTabRight, SIGNAL(activated()), this, SLOT(shortcutChangeTabRight()));
}



/**
 * @brief Creates toolbars used in main window
 */
void MainForm::createToolbar()
{
    //qDebug() << "MainForm: CreateToolbar()";
    fileToolBar = addToolBar(tr("File Toolbar"));
//     projectToolBar = addToolBar(tr("Project Toolbar"));
    simulationToolBar = addToolBar(tr("Simulation Toolbar"));
    m_toolToolBar = addToolBar(tr("Tools Toolbar"));
    m_simtoolToolBar = addToolBar(tr("Simulation Tools Toolbar"));
    m_helpToolBar = addToolBar(tr("Help Toolbar"));

    #ifdef MDS_FEATURE_DISASSEMBLER
        m_toolToolBar->addAction(toolDisassemblerAct);
    #endif
    #ifdef MDS_FEATURE_TRANSLATOR
        m_toolToolBar->addAction(toolTranslatorAct);
    #endif
    #ifdef MDS_FEATURE_FILECONVERTER
        m_toolToolBar->addAction(toolFileConvertAct);
    #endif
    #ifdef MDS_FEATURE_8_SEGMENT_EDITOR
        m_toolToolBar->addAction(toolDisplayAct);
    #endif
    #ifdef MDS_FEATURE_LOOP_GENERATOR
        m_toolToolBar->addAction(toolLoopGenAct);
    #endif
    #ifdef MDS_FEATURE_SIM_LED_PANEL
        m_simtoolToolBar->addAction(toolSimLedsAct);
    #endif
    #ifdef MDS_FEATURE_SIM_7_SEGMENT
        m_simtoolToolBar->addAction(toolSim7SegAct);
    #endif
    #ifdef MDS_FEATURE_SIM_SWITCH
        m_simtoolToolBar->addAction(toolSimSwitchAct);
    #endif

    m_helpToolBar->addAction(aboutAct);
    m_helpToolBar->addAction(helpAct);

    fileToolBar->addAction(newAddAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(saveAsAct);
    fileToolBar->addAction(saveAllAct);

//     projectToolBar->addAction(newProjAct);
//     projectToolBar->addAction(openProjAct);
//     projectToolBar->addAction(saveProjAct);
//     projectToolBar->addSeparator();
//     projectToolBar->addAction(newAct);

    //projectToolBar->addAction(removeFileAct);
    //projectToolBar->addAction(addAct);

    //fileToolBar->addAction(saveAct);


    simulationToolBar->addAction(projectCompileAct);
    simulationToolBar->addSeparator();
    simulationToolBar->addAction(simulationFlowAct);
    simulationToolBar->addAction(simulationStepAct);
    simulationToolBar->addAction(simulationAnimateAct);
    simulationToolBar->addAction(simulationRunAct);
    simulationToolBar->addAction(simulationResetAct);
//     simulationToolBar->addAction(simulationUnhighlightAct);
//     simulationToolBar->addAction(simulationBreakpointAct);
//     simulationToolBar->addAction(simulationDisableBreakpointsAct);



    fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
//     projectToolBar->setAllowedAreas(Qt::TopToolBarArea);
    simulationToolBar->setAllowedAreas(Qt::TopToolBarArea);
    m_toolToolBar->setAllowedAreas(Qt::TopToolBarArea);
    m_simtoolToolBar->setAllowedAreas(Qt::TopToolBarArea);
    m_helpToolBar->setAllowedAreas(Qt::TopToolBarArea);
    fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
//     addToolBar(Qt::TopToolBarArea, projectToolBar);
//     addToolBar(Qt::TopToolBarArea, simulationToolBar);
    #ifdef MDS_VARIANT_NONCOMMERCIAL
        QToolBar *toolBar = addToolBar("NON-COMMERCIAL");
        toolBar->setFloatable(false);
        QWidget* spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // toolBar is a pointer to an existing toolbar
        toolBar->addWidget(spacer);
        toolBar->addAction("NON-COMMERCIAL");
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        addToolBar(Qt::TopToolBarArea, toolBar);
        connect(toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(showWebSite(QAction*)));
    #endif
    #ifdef MDS_VARIANT_TRIAL
        QFileInfo mdsInfo(GuiCfg::getInstance().getConfigPath());
        QString trial = QString("TRIAL, ") +  QString::number(MDS_TRIAL_PERIOD - mdsInfo.lastModified().daysTo(QDateTime::currentDateTime())) + QString(" days left");
        QToolBar *toolBar = addToolBar(trial);
        toolBar->setFloatable(false);
        QWidget* spacer = new QWidget();
        spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        // toolBar is a pointer to an existing toolbar
        toolBar->addWidget(spacer);
        toolBar->addAction(trial);
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        addToolBar(Qt::TopToolBarArea, toolBar);
        connect(toolBar, SIGNAL(actionTriggered(QAction*)), this, SLOT(showWebSite(QAction*)));
    #endif
    //addToolBar(Qt::TopToolBarArea, fileToolBar);
    //qDebug() << "MainForm: return CreateToolbar()";
}





/**
 * @brief Inits default or user-saved layout of basic dock widgets
 * @bug Only default layout
 */
void MainForm::createDockWidgets()
{
    //qDebug() << "MainForm: CreateDockWidgets()";
    //setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    if (false == wDockManager->dockWidgets)
    {
        setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
        setTabPosition(Qt::RightDockWidgetArea, QTabWidget::East);

        //mozno stejne jako u WDockManager - ulozit si ptr na okno
        //projectMan->addProject(NULL, NULL, NULL);

        QList<QTabBar*> tabList;
        //wDockManager->addDockWidget(wListCode);
        //wDockManager->addDockWidget(wListCode2);
        /*
        tabList= this->findChildren<QTabBar*>();
        wDockManager->leftAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                wDockManager,
                SLOT(handleShowHideLeft(int))
               );
         */
        wDockManager->addDockWidget(wCompileInfo);
        wDockManager->addDockWidget(wSimulationInfo);
        wDockManager->addDockWidget(wBottomHide);
        /*#ifdef Q_OS_WIN
            Sleep(50);
        #else
            usleep(50000);
        #endif*/
        tabList= this->findChildren<QTabBar*>();
        wDockManager->bottomAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                wDockManager,
                SLOT(handleShowHideBottom(int))
               );

        wDockManager->addDockWidget(wBreakpointList);
        wDockManager->addDockWidget(wBookmarkList);
        wDockManager->addDockWidget(wAsmMacroAnalyser);
        AsmMacroAnalyser *analys = (AsmMacroAnalyser*)(wDockManager->getDockWidget(wAsmMacroAnalyser)->widget());
        connect(analys,
                SIGNAL(requestCodeEdits()),
                this,
                SLOT(requestMacrosCodeEdits())
            );
        connect(analys,
                SIGNAL(macroClicked(QString, int)),
                this,
                SLOT(scrollToFileLine(QString, int))
            );
        connect(this,
                SIGNAL(provideMacroCodeEdits(QList<CodeEdit*>)),
                analys,
                SLOT(reload(QList<CodeEdit*>))
            );
        connect(this,
                SIGNAL(mtblCompiled(QString)),
                analys,
                SLOT(reload(QString))
            );
        wDockManager->addDockWidget(wRightHide);
        /*#ifdef Q_OS_WIN
            Sleep(50);
        #else
            usleep(50000);
        #endif*/
        tabList= this->findChildren<QTabBar*>();
        wDockManager->rightAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                wDockManager,
                SLOT(handleShowHideRight(int))
               );
        //wDockManager->addDockWidget(wAnalysVar);
        //wDockManager->addDockWidget(wAnalysFunc);
        //addAct->setEnabled(true);
        //projectCompileAct->setEnabled(true);
        //simulationFlowAct->setEnabled(true);
        wDockManager->dockWidgets = true;
        if (wDockManager->getBreakpointList() != NULL)
        {
            connect(wDockManager->getBreakpointList(),
                    SIGNAL(breakpointClicked(QString, int)),
                    this,
                    SLOT(scrollToFileLine(QString, int))
                   );
        }
        if (wDockManager->getBookmarkList() != NULL)
        {
            connect(wDockManager->getBookmarkList(),
                    SIGNAL(bookmarkClicked(QString, int)),
                    this,
                    SLOT(scrollToFileLine(QString, int))
                   );
        }

        wDockManager->hideDockWidgetArea(1);
        wDockManager->hideDockWidgetArea(2);
        QApplication::processEvents();
        reloadTabIcons();
    }
    else
    {
        wDockManager->addDockWidget(wSimulationInfo);
    }
    //QTimer::singleShot(50, this, SLOT(reloadTabIcons()));
    //emit dockWidgetsCreated;
    //qDebug() << "MainForm: return CreateDockWidgets()";
}


/**
 * @brief Creates welcome screen with tips and basic functions (create project etc.)
 * @details Still not implemented
 */
void MainForm::createWelcome()
{
    wDockManager->setCentralWelcome();
}



/**
 * @brief Slot. Opens new blank file.
 */
void MainForm::newFile()
{
    //qDebug() << "MainForm: newFile()";
    //jen se vytvori novy tab na code editoru
    //prepta se dialogem, zda pridat do aktivniho projektu
    projectMan->addUntrackedFile(NULL, NULL);
    //qDebug() << "MainForm: return newFile()";
}


/**
 * @brief Slot. Opens new blank file and adds it to the active project.
 */
void MainForm::newAddFile()
{
    //jen se vytvori novy tab na code editoru
    //a soubor se prida k projektu
    QString path ;
    bool done = false;
    while (false == done)
    {
        if (projectMan->getActive() != NULL && projectMan->getActive()->prjPath != "untracked")
        {
            path = QFileDialog::getSaveFileName(this, tr("New File"), QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
        }
        else
        {
            path = QFileDialog::getSaveFileName(this, tr("New File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
        }
        if (path == NULL)
        {
            break;
        }
        int index = path.lastIndexOf(".");
        if (index > 0)
        {
            QString text(path.right(path.size() - index));
            if (text == ".asm" || text == ".psm")
            {
                done = true;
            }
            else
            {
                QMessageBox dialog(this);
                dialog.setWindowTitle("Highlight note");
                dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                dialog.setIcon(QMessageBox::Warning);
                dialog.setModal(true);
                dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                int result = dialog.exec();
                if (QMessageBox::Yes == result)
                {
                    done = true;
                }
                else
                {
                    if (QMessageBox::Cancel == result)
                    {
                        path = "";
                        break;
                    }
                }
            }
        }
        else
        {
            QMessageBox dialog(this);
            dialog.setWindowTitle("Highlight note");
            dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
            dialog.setIcon(QMessageBox::Warning);
            dialog.setModal(true);
            dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            int result = dialog.exec();
            if (QMessageBox::Yes == result)
            {
                done = true;
            }
            else
            {
                if (QMessageBox::Cancel == result)
                {
                    path = "";
                    break;
                }
            }
        }
    }
    if (path != NULL)
    {
        wDockManager->addCentralWidget(path.section('/', -1), path);
        wDockManager->getCentralWidget()->setChanged();
        //wDockManager->getCentralWidget()->connectAct();

        //je sice prehlednejsi zavolat saveFile(), ale
        //vlozeni kodu pro ulozeni je rychlejsi a efektivnejsi
        //...ale necham volani saveFile()...
        saveFile();
        //pridani do projektu
        projectMan->addFile(path, path.section('/', -1));
        wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
    }
    //qDebug() << "MainForm: return newAddFile()";
}


/**
 * @brief Slot. Opens file selected by user in dialog.
 */
void MainForm::openFile()
{
//     qDebug() << "MainForm: openFile()";
    QString path;
    if (projectMan->getActive() != NULL && projectMan->getActive()->prjPath != "untracked")
    {
        path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
    }
    else
    {
        path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
    }
    if (path != NULL)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE, path);
        }
        else
        {
            //wDockManager->addCentralWidget(path.section('/', -1), path);
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            file.close();
            if (projectMan->getOpenProjects().count() > 0)
            {
                projectMan->getActive()->addFile(path, path.section('/', -1));
                wDockManager->addCentralWidget(path.section('/', -1), path);
                wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
                wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getActive());
            }
            else
            {
                //wDockManager->getCentralWidget()->connectAct();
                projectMan->addUntrackedFile(path, path.section('/', -1));
                wDockManager->addUntrackedCentralWidget(path.section('/', -1), path);
                wDockManager->getCentralWidget()->setParentProject(projectMan->getUntracked());
                wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getUntracked());
            }
            GuiCfg::getInstance().fileOpened(path);
            QTimer::singleShot(100, this->wDockManager->getCentralWidget(), SLOT(changeHeight()));
        }
    }
    //qDebug() << "MainForm: return openFile()";
}


/**
 * @brief Opens file selected by programmer.
 * @param path Path to the file.
 */
void MainForm::openFilePath(QString path, QString parentProjectPath)
{
    //qDebug() << "MainForm: openFilePath()";
    //QDir thisDir(".");
    //QDir projectDir(QFileInfo(projectMan->activeProject->prjPath).dir());
    //QString absoluteFilePath = QFileInfo(projectMan->getActive()->prjPath).dir().path() + "/" + path;
    //qDebug() << path;
    if (NULL != path)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            //qDebug() << "Mainform: openFilePath error" << path;
            error(ERR_OPENFILE, path);
        }
        else
        {
            file.close();
            //qDebug() << "MainForm: addCentralWidget";
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            //qDebug() << "MainForm: connect";
            //qDebug() << "MainForm: set parent";
            if (parentProjectPath != "")
            {
                for (int i = 0; i < projectMan->getOpenProjects().count(); i++)
                {
                    if (projectMan->getOpenProjects().at(i)->prjPath == parentProjectPath)
                    {
                        wDockManager->addCentralWidget(path.section('/', -1), path);
                        CodeEdit *centralCodeEdit = wDockManager->getCentralWidget();
                        //centralCodeEdit->connectAct();
                        centralCodeEdit->setParentProject(projectMan->getOpenProjects().at(i));
                        wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getOpenProjects().at(i));
                        centralCodeEdit->setBreakpointsLines(projectMan->getActive()->getBreakpointsForFileAbsolute(centralCodeEdit->getPath()));
                        centralCodeEdit->setBookmarksLines(projectMan->getActive()->getBookmarksForFileAbsolute(centralCodeEdit->getPath()));
                        //wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->updateLineCounter();
                        break;
                    }
                }
                if (parentProjectPath == "untracked")
                {
                    projectMan->addUntrackedFile(path, path.section('/', -1));
                    wDockManager->addUntrackedCentralWidget(path.section('/', -1), path);
                    //wDockManager->getCentralWidget()->connectAct();
                    wDockManager->getCentralWidget()->setParentProject(projectMan->getUntracked());
                    wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getUntracked());
                    GuiCfg::getInstance().fileOpened(path);
                }
            }
            else
            {
                wDockManager->addCentralWidget(path.section('/', -1), path);
                CodeEdit *centralCodeEdit = wDockManager->getCentralWidget();
                //centralCodeEdit->connectAct();
                centralCodeEdit->setParentProject(projectMan->getActive());
                wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getActive());
                centralCodeEdit->setBreakpointsLines(projectMan->getActive()->getBreakpointsForFileAbsolute(centralCodeEdit->getPath()));
                centralCodeEdit->setBookmarksLines(projectMan->getActive()->getBookmarksForFileAbsolute(centralCodeEdit->getPath()));
                //wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->updateLineCounter();

            }
            //wDockManager->getCentralWidget()->setSaved();
            /*if (true == centralCodeEdit->isChanged())
            {
                qDebug() << "MainForm: openfilepath - some error here";
            }*/
            QTimer::singleShot(100, this->wDockManager->getCentralWidget(), SLOT(changeHeight()));
        }
    }
    //qDebug() << "MainForm: return openFilePath()";
}


/**
 * @brief Slot. Adds opened file to the active project.
 */
void MainForm::addFile()
{
    //qDebug() << "MainForm: addFile()";
    if (wDockManager->isEmpty() == false)
    {
        QString path;
        if (wDockManager->getCentralPath() == NULL)
        {
            if (projectMan->getActive() != NULL && projectMan->getActive()->prjPath != "untracked")
            {
                path = QFileDialog::getSaveFileName(this, tr("Save File"), QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
            }
            else
            {
                path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
            }
            wDockManager->setCentralPath(path);
            wDockManager->setCentralName(path.section('/', -1));
        }
        else
        {
            path = wDockManager->getCentralPath();
        }

        //je sice prehlednejsi zavolat saveFile(), ale
        //vlozeni kodu pro ulozeni je rychlejsi a efektivnejsi
        //...ale necham volani saveFile()...
        saveFile();
        //pridani do projektu

        projectMan->addFile(path, path.section('/', -1));
        wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
    }
    //qDebug() << "MainForm: return addFile()";
}

/**
 * @brief Slot. Saves the active file.
 */
void MainForm::saveFile()
{
    //qDebug() << "MainForm: saveFile()";
    if (wDockManager->getCentralWidget()->isChanged() == true || wDockManager->getCentralPath() == "untracked")
    {
        /*if (wDockManager->getCentralWidget()->isChanged() == true )
        {
            qDebug() << "MainForm: central is changed";
        }
        qDebug() << "Mainform: saving file";*/
        QString path;
        if (wDockManager->getCentralPath() == NULL || wDockManager->getCentralPath() == "untracked")
        {
            //path = QFileDialog::getSaveFileName(this, tr("Source File");
            bool done = false;
            while (false == done)
            {
                if (projectMan->getActive() != NULL && projectMan->getActive()->prjPath != "untracked")
                {
                    path = QFileDialog::getSaveFileName (this, tr("Save File"), QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
                }
                else
                {
                    path = QFileDialog::getSaveFileName (this, tr("Save File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
                }
                if (path == NULL)
                {
                    break;
                }
                else
                {
                    int index = path.lastIndexOf(".");
                    if (index > 0)
                    {
                        QString text(path.right(path.size() - index));
                        if (text == ".asm" || text == ".psm")
                        {
                            done = true;
                        }
                        else
                        {
                            QMessageBox dialog(this);
                            dialog.setWindowTitle("Highlight note");
                            dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                            dialog.setIcon(QMessageBox::Warning);
                            dialog.setModal(true);
                            dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                            int result = dialog.exec();
                            if (QMessageBox::Yes == result)
                            {
                                done = true;
                            }
                            else
                            {
                                if (QMessageBox::Cancel == result)
                                {
                                    path = "";
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        QMessageBox dialog(this);
                        dialog.setWindowTitle("Highlight note");
                        dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                        dialog.setIcon(QMessageBox::Warning);
                        dialog.setModal(true);
                        dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                        int result = dialog.exec();
                        if (QMessageBox::Yes == result)
                        {
                            done = true;
                        }
                        else
                        {
                            if (QMessageBox::Cancel == result)
                            {
                                path = "";
                                break;
                            }
                        }
                    }
                }
            }
            if (path != NULL)
            {
                wDockManager->setCentralPath(path);
                wDockManager->setCentralName(path.section('/', -1));
            }
        }
        else
        {
            path = wDockManager->getCentralPath();
        }

        if (path != NULL)
        {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                error(ERR_OPENFILE, path);
            }
            else
            {
                QTextStream fout(&file);
                fout << wDockManager->getCentralTextEdit()->toPlainText();
                file.close();
                wDockManager->setTabSaved();
                wDockManager->getCentralWidget()->setSaved();
                //qDebug() << "mainform: file saved";
            }
        }
    }
    //qDebug() << "MainForm: return saveFile()";
}


/**
 * @brief Slot. Saves the active file by the name selected by user.
 */
void MainForm::saveFileAs()
{
    //qDebug() << "MainForm: saveFileAs()";
    //QString path = QFileDialog::getSaveFileName(this, tr("Source File");
    QString path;
    bool done = false;
    while (false == done)
    {
        if (projectMan->getActive() != NULL && projectMan->getActive()->prjPath != "untracked")
        {
            path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
        }
        else
        {
            path = QFileDialog::getSaveFileName(this, tr("Save File As"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
        }
        if (path == NULL)
        {
            break;
        }
        else
        {
            int index = path.lastIndexOf(".");
            if (index > 0)
            {
                QString text(path.right(path.size() - index));
                if (text == ".asm" || text == ".psm")
                {
                    done = true;
                }
                else
                {
                    QMessageBox dialog(this);
                    dialog.setWindowTitle("Highlight note");
                    dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                    dialog.setIcon(QMessageBox::Warning);
                    dialog.setModal(true);
                    dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                    int result = dialog.exec();
                    if (QMessageBox::Yes == result)
                    {
                        done = true;
                    }
                    else
                    {
                        if (QMessageBox::Cancel == result)
                        {
                            path = "";
                            break;
                        }
                    }
                }
            }
            else
            {
                QMessageBox dialog(this);
                dialog.setWindowTitle("Highlight note");
                dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                dialog.setIcon(QMessageBox::Warning);
                dialog.setModal(true);
                dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                int result = dialog.exec();
                if (QMessageBox::Yes == result)
                {
                    done = true;
                }
                else
                {
                    if (QMessageBox::Cancel == result)
                    {
                        break;
                    }
                }
            }
        }
    }
    if (path != NULL)
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE, path);
        }
        else
        {
            QTextStream fout(&file);
            fout << wDockManager->getCentralTextEdit()->toPlainText();
            file.close();
            wDockManager->getCentralWidget()->getParentProject()->renameFile(wDockManager->getCentralPath(), path);
            wDockManager->setCentralPath(path);
            wDockManager->setCentralName(path.section('/', -1));
            wDockManager->getCentralWidget()->setSaved();
        }
    }
    //qDebug() << "MainForm: return saveFileAs()";
}


/**
 * @brief Saves file from selected CodeEditor.
 */
void MainForm::saveFile(CodeEdit *editor)
{
    //qDebug() << "MainForm: saveFile()";
    //if (editor->isChanged() == true)
    //{
        QString path;
        if (editor->getPath() == NULL || editor->getPath() == "untracked")
        {
            //path = QFileDialog::getSaveFileName(this, tr("Source File");
            bool done = false;
            while (false == done)
            {
                if (projectMan->getActive() != NULL && projectMan->getActive()->prjPath != "untracked")
                {
                    path = QFileDialog::getSaveFileName(this, tr("Save File"), QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
                }
                else
                {
                    path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
                }
                if (path == NULL)
                {
                    break;
                }
                else
                {
                    int index = path.lastIndexOf(".");
                    if (index > 0)
                    {
                        QString text(path.right(path.size() - index));
                        if (text == ".asm" || text == ".psm")
                        {
                            done = true;
                        }
                        else
                        {
                            QMessageBox dialog(this);
                            dialog.setWindowTitle("Highlight note");
                            dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                            dialog.setIcon(QMessageBox::Warning);
                            dialog.setModal(true);
                            dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                            int result = dialog.exec();
                            if (QMessageBox::Yes == result)
                            {
                                done = true;
                            }
                            else
                            {
                                if (QMessageBox::Cancel == result)
                                {
                                    path = "";
                                    break;
                                }
                            }
                        }
                    }
                    else
                    {
                        QMessageBox dialog(this);
                        dialog.setWindowTitle("Highlight note");
                        dialog.setText("Note: Only with .asm or .psm file extension will source code be highlighted. Do you wish to continue?");
                        dialog.setIcon(QMessageBox::Warning);
                        dialog.setModal(true);
                        dialog.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
                        int result = dialog.exec();
                        if (QMessageBox::Yes == result)
                        {
                            done = true;
                        }
                        else
                        {
                            if (QMessageBox::Cancel == result)
                            {
                                path = "";
                                break;
                            }
                        }
                    }
                }
            }
            if (path != NULL)
            {
                editor->setPath(path);
                editor->setName(path.section('/', -1));
            }
        }
        else
        {
            path = editor->getPath();
        }
        if (path != NULL)
        {
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                error(ERR_OPENFILE, path);
            }
            else
            {
                QTextStream fout(&file);
                fout << editor->getTextEdit()->toPlainText();
                file.close();
                editor->setSaved();
                //qDebug() << "mainform: editor saved";
            }
        }
    //}
    //qDebug() << "MainForm: return saveFile()";
}



/**
 * @brief Slot. Saves all opened files which are changed (or unnamed).
 */
void MainForm::saveAll()
{
    //qDebug() << "MainForm: saveAll()";
    //ulozi vsechny zmenene a nepojmenovane
    for (int i = 0; i < wDockManager->getTabCount(); i++)
    {
        if (wDockManager->getTabStatusChanged(i) == true
            || wDockManager->getTabWidget(i)->getPath() == "untracked")
        {
            //qDebug() << "MainForm: saveFile codeedit";
            saveFile(wDockManager->getTabWidget(i));
        }
    }
    //qDebug() << "MainForm: return saveAll()";
}

/**
 * @brief Slot. Creates a new project.
 */
void MainForm::newProject()
{
    //qDebug() << "MainForm: newProject()";
    ProjectDialog *projectEdit = new ProjectDialog(this, projectMan);
    projectEdit->show();
    //qDebug() << "MainForm: return newProject()";
}

/**
 * @brief Slot. Opens project selected by user in dialog.
 */
void MainForm::openProject()
{
    //qDebug() << "MainForm: openProject()";
    //nalezeni projektu
    QFileDialog dialog;
    QString path = QFileDialog::getOpenFileName (this, tr("Open Project"), QDir::homePath(), tr("Project (*.mds-project)"), 0, QFileDialog::DontUseNativeDialog);

    if (path.isEmpty() == false && projectMan->isOpened(path) == false)
    {
    //nacteni projektu
        //qDebug() << path;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE, path);
        }
        else
        {
            //nacteni obsahu do widgetu
            projectMan->openProject(&file);
            file.close();
        }
    }
    //qDebug() << "MainForm: return openProject()";
}


/**
 * @brief Opens selected project.
 * @param path Path to the project.
 */
bool MainForm::openProject(QString path)
{
    //qDebug() << "MainForm: openProject()";
    if (false == projectMan->isOpened(path))
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE, path);
            return false;
        }
        else
        {
            //nacteni obsahu do widgetu
            projectMan->openProject(&file);
            file.close();
            //qDebug() << "MainForm: return openProject()";
            return true;
        }
    }
    //qDebug() << "MainForm: return openProject() false";
    return false;
}


void MainForm::projectOpened()
{
    newAddAct->setEnabled(true);

    //qDebug() << "MainForm: projectOpened";
    wDockManager->deleteCentralWelcome();
    if (false == projectConfigAct->isEnabled())
    {
        projectConfigAct->setEnabled(true);
    }
    if (false == closeProjectAct->isEnabled())
    {
        closeProjectAct->setEnabled(true);
    }
    if (false == saveProjConfigAct->isEnabled())
    {
        saveProjConfigAct->setEnabled(true);
    }
    if (false == projectTabConnected)
    {
        QList<QTabBar*> tabList = this->findChildren<QTabBar*>();
        if (tabList.size() > 1)
        {
            //qDebug() << "MainForm: tab connected";
            projectTabConnected = true;
            connect(tabList.at(tabList.size()-1),
                    SIGNAL(currentChanged(int)),
                    this,
                    SLOT(activeProjectChanged(int))
                   );
            //qDebug() << "projectTabs = ";
            projectTabs = tabList.at(tabList.size()-1);
            //qDebug() << "projectTabs = done";
        }
    }
    this->createDockWidgets();
    if (wDockManager->getBreakpointList() != NULL)
    {
        wDockManager->getBreakpointList()->reload(projectMan->getActive()->getBreakpointsListRef());
        /*if (wDockManager->getCentralWidget() != NULL)
        {
            wDockManager->getCentralWidget()->setBreakpointsLines(projectMan->getActive()->getBreakpointsForFileAbsolute(wDockManager->getCentralWidget()->getPath()));
        }*/
    }
    if (wDockManager->getBookmarkList() != NULL)
    {
        wDockManager->getBookmarkList()->reload(projectMan->getActive()->getBookmarksListRef());
        /*if (wDockManager->getCentralWidget() != NULL)
        {
            wDockManager->getCentralWidget()->setBookmarksLines(projectMan->getActive()->getBookmarksForFileAbsolute(wDockManager->getCentralWidget()->getPath()));
        }*/
    }
    if (projectTabs != NULL )
    {
        //qDebug() << "count: " << projectTabs->count();
        //projectTabs->setCurrentIndex(projectTabs->count()-1);
        QTimer::singleShot(50, this, SLOT(refreshProjectTree()));
    }

    if (projectMan->getActive()->prjPath != "untracked")
    {
        GuiCfg::getInstance().projectOpened(projectMan->getActive()->prjPath);
    }
    //qDebug() << "MainForm: projectOpened done";
}


/**
 * @brief Slot. Saves all changed files in active project.
 */
void MainForm::saveProject()
{
    //qDebug() << "MainForm: saveProject()";
    for (int i = 0; i < wDockManager->getTabCount(); i++)
    {
        if (wDockManager->getTabWidget(i)->isChild(projectMan->getActive()) == true
            && true == wDockManager->getTabWidget(i)->isChanged())
        {
            saveFile(wDockManager->getTabWidget(i));
        }
    }
    //qDebug() << "MainForm: return saveProject()";
}


/**
 * @brief Slot. Compiles active project.
 */
void MainForm::compileProject()
{

    if (projectMan->getActive() == NULL)
    {
        //should never happen, error
        error(ERR_NO_PROJECT);
        return;
    }
    if (true == simulationStatus)
    {
        error(ERR_COMPILE_DURING_SIMULATION);
        return;
    }

    ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->clear();
    this->options = new CompilerOptions();
    QString mainFile;

    if (projectMan->getActive()->prjPath == "untracked" && projectMan->getActive()->prjName == "untracked")
    {
        //compile actual file with global settings, not error
        //error(ERR_UNTRACKED_PROJECT);
        QMessageBox warningBox(QMessageBox::Warning,
                               "Compilation Warning",
                               "Note: You are going to compile untracked project, do you wish to continue?",
                               QMessageBox::Yes|QMessageBox::No,
                               this
                              );
        if (warningBox.exec() != QMessageBox::Yes)
        {
            return;
        }
        //qDebug() << "MainForm: compiled untracked project, untracked actual file";

        if (wDockManager->getCentralWidget() == NULL)
        {
            //qDebug() << "MainForm: returned";
            return;
        }

        this->saveFile();
        this->projectMan->getUntracked()->addFile(wDockManager->getCentralPath(),wDockManager->getCentralName());

        options->m_sourceFiles.push_back(wDockManager->getCentralPath().toLocal8Bit().constData());
        //options->m_sourceFiles.push_back(wDockManager->getCentralPath().toStdString());

        options->m_device = this->projectMan->getActive()->family.toStdString();


        QDir templateDir(GuiCfg::getInstance().getTemplatePath());
        if (true == this->projectMan->getActive()->defaultVerilog)
        {
            options->m_verilogTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + this->projectMan->getActive()->family
                                        + ".v"
                                        ).toLocal8Bit().constData();
        }
        else
        {
            options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
        }

        if (true == this->projectMan->getActive()->defaultVHDL)
        {
            options->m_vhdlTemplate = ( templateDir.absolutePath()
                                    + "/"
                                    + this->projectMan->getActive()->family
                                    + ".vhd"
                                    ).toLocal8Bit().constData();
        }
        else
        {
            options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
        }



        QDir pathDir(GuiCfg::getInstance().getTempPath());
        if (false == pathDir.exists())
        {
            pathDir.mkpath(".");
        }

        QDir prjDir;
        if ("" == wDockManager->getCentralName().section('.',0,-2))
        {
            mainFile = pathDir.absolutePath()
                    + "/"
                    +  wDockManager->getCentralName();
        }
        else
        {
            mainFile = pathDir.absolutePath()
                    + "/"
                    +  wDockManager->getCentralName().section('.',0,-2);
        }

        //qDebug() << "MainForm: file" << mainFile;


        CompileInfo *compileInfo = ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()));
        compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Compilation settings:",
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Project:      untracked",
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("File:         " + wDockManager->getCentralName(),
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Architecture: Picoblaze",
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Family:       " + projectMan->getUntracked()->family,
                                    CompilerBase::MessageType::MT_REMARK);


        if (projectMan->getActive()->compileOpt.at(0))
        {
            //options->m_symbolTable = (mainFile + ".stbl").toStdString();
            options->m_symbolTable = ( mainFile
                                     + ".stbl"
                                     ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(1))
        {
            options->m_macroTable = ( mainFile
                                    + ".mtbl"
                                    ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(2))
        {
            options->m_mdsDebugFile = ( mainFile
                                      + ".dbg"
                                      ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(3))
        {
            options->m_codeTree = ( mainFile
                                  + ".ctr"
                                  ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(4))
        {
            options->m_lstFile = ( mainFile
                                  + ".lst"
                                 ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(5))
        {
            options->m_hexFile = ( mainFile
                                 + ".ihex"
                                 ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(6))
        {
            options->m_binFile = ( mainFile
                                 + ".bin"
                                 ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(7))
        {
            options->m_srecFile = ( mainFile
                                  + ".srec"
                                  ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(8))
        {
            options->m_memFile = ( mainFile
                                 + ".mem"
                                 ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(9))
        {
            options->m_rawHexDumpFile = ( mainFile
                                        + ".rawhex"
                                        ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(10))
        {
            options->m_verilogFile = ( mainFile
                                     + ".v"
                                     ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(11))
        {
            options->m_vhdlFile = ( mainFile
                                  + ".vhd"
                                  ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(12))
        {
            options->m_stringTable = ( mainFile
                                  + ".strtbl"
                                  ).toLocal8Bit().constData();
        }
        //return;
    }
    else if ( false == projectMan->getActive()->useMainFile )
    {
        //not error
        //check if enabled, if it isnt, compile actual file
        //(but find whether it is in act project for compile settings)
        //error(ERR_NO_MAINFILE);
        if (wDockManager->getCentralWidget() == NULL)
        {
            return;
        }

        this->saveFile();

        QDir prjDir(projectMan->getActive()->prjPath.section('/',0, -2));
        QDir fileDir;
        QString filePath;
        bool found = false;

        for (int i = 0; i < projectMan->getActive()->filePaths.count(); i++)
        {
            //fileDir.setPath(QDir::cleanPath(prjDir.absolutePath() + "/" + projectMan->getActive()->filePaths.at(i)));
            filePath = QDir::cleanPath(prjDir.absolutePath() + "/" + projectMan->getActive()->filePaths.at(i));
            //qDebug() << prjDir.relativeFilePath(projectMan->getActive()->filePaths.at(i));
            //qDebug() << projectMan->getActive()->filePaths.at(i);
            //qDebug() << "fixed?" << prjDir.absolutePath() + "/" + projectMan->getActive()->filePaths.at(i);
            //qDebug() << "MainForm: file dir" << filePath;
            if (filePath == wDockManager->getCentralPath())
            {
                found = true;
                break;
            }
        }
        if (found == true)
        {
            //qDebug() << "MainForm: compiled actual project, actual file" << filePath;
            options->m_sourceFiles.push_back(filePath.toLocal8Bit().constData());


            CompileInfo *compileInfo = ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()));
            compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                        CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Compilation settings:",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Project:      " + projectMan->getActive()->prjName,
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("File:         " + wDockManager->getCentralName(),
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Architecture: Picoblaze",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Family:       " + projectMan->getActive()->family + "\n\n",
                                    CompilerBase::MessageType::MT_REMARK);


        if ("" == wDockManager->getCentralName().section('.',0,-2))
        {
            mainFile = prjDir.absolutePath() + "/" + wDockManager->getCentralName();
        }
        else
        {
            mainFile = prjDir.absolutePath() + "/" + wDockManager->getCentralName().section('.',0,-2);
        }
            //qDebug() << mainFile;

            options->m_device = this->projectMan->getActive()->family.toStdString();


            QDir templateDir(GuiCfg::getInstance().getTemplatePath());
            if (true == this->projectMan->getActive()->defaultVerilog)
            {
                options->m_verilogTemplate = ( templateDir.absolutePath()
                                            + "/"
                                            + this->projectMan->getActive()->family
                                            + ".v"
                                            ).toLocal8Bit().constData();
            }
            else
            {
                options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
            }

            if (true == this->projectMan->getActive()->defaultVHDL)
            {
                options->m_vhdlTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + this->projectMan->getActive()->family
                                        + ".vhd"
                                        ).toLocal8Bit().constData();
            }
            else
            {
                options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
            }

            if (projectMan->getActive()->compileOpt.at(0))
            {
                options->m_symbolTable = (mainFile + ".stbl").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(1))
            {
                options->m_macroTable = (mainFile + ".mtbl").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(2))
            {
                options->m_mdsDebugFile = (mainFile + ".dbg").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(3))
            {
                options->m_codeTree = (mainFile + ".ctr").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(4))
            {
                options->m_lstFile = (mainFile + ".lst").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(5))
            {
                options->m_hexFile = (mainFile + ".ihex").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(6))
            {
                options->m_binFile = (mainFile + ".bin").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(7))
            {
                options->m_srecFile = (mainFile + ".srec").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(8))
            {
                options->m_memFile = ( mainFile + ".mem").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(9))
            {
                options->m_rawHexDumpFile = ( mainFile + ".rawhex" ).toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(10))
            {
                options->m_verilogFile = ( mainFile  + ".v").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(11))
            {
                options->m_vhdlFile = ( mainFile + ".vhd").toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(12))
            {
                options->m_stringTable = ( mainFile + ".strtbl").toLocal8Bit().constData();
            }
        }
        else
        {
            QMessageBox warningBox(QMessageBox::Warning,
                                   "Compilation Warning",
                                   "Note: You are going to compile with configuration of the untracked project, do you wish to continue?",
                                   QMessageBox::Yes|QMessageBox::No,
                                   this
                                  );
            if (warningBox.exec() != QMessageBox::Yes)
            {
                return;
            }

            //qDebug() << "MainForm: compiled untracked project, actual file";
            if (this->projectMan->getUntracked() != NULL)
            {
                this->projectMan->getUntracked()->addFile(wDockManager->getCentralPath(),wDockManager->getCentralName());
                this->projectMan->setActive(this->projectMan->getUntracked());
                if (projectTabs != NULL )
                {
                    QList<Project*> projects = projectMan->getOpenProjects();
                    int index = 0;
                    for (;index < projects.count(); index++)
                    {
                        if (projects.at(index) == projectMan->getUntracked())
                        {
                            break;
                        }
                    }
                    if (projectTabs->count() <= index)
                    {
                        //qDebug() << "MainForm: projectTabs overflow";
                        projectTabs->setCurrentIndex(projectTabs->count()-1);
                    }
                    else
                    {
                        projectTabs->setCurrentIndex(index);
                    }
                }
            }
            else
            {
                this->projectMan->addUntrackedProject();
                this->projectMan->getUntracked()->addFile(wDockManager->getCentralPath(),wDockManager->getCentralName());
                this->projectMan->setActive(this->projectMan->getUntracked());
                QTimer::singleShot(50, this, SLOT(refreshProjectTree()));
            }
            options->m_sourceFiles.push_back(wDockManager->getCentralPath().toLocal8Bit().constData());

            options->m_device = this->projectMan->getActive()->family.toStdString();


            QDir templateDir(GuiCfg::getInstance().getTemplatePath());
            if (true == this->projectMan->getActive()->defaultVerilog)
            {
                options->m_verilogTemplate = ( templateDir.absolutePath()
                                             + "/"
                                             + this->projectMan->getActive()->family
                                             + ".v"
                                             ).toLocal8Bit().constData();
            }
            else
            {
                options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
            }

            if (true == this->projectMan->getActive()->defaultVHDL)
            {
                options->m_vhdlTemplate = ( templateDir.absolutePath()
                                          + "/"
                                          + this->projectMan->getActive()->family
                                          + ".vhd"
                                          ).toLocal8Bit().constData();
            }
            else
            {
                options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
            }


            CompileInfo *compileInfo = ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()));
            compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                        CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Compilation settings:",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Project:      untracked",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("File:         " + wDockManager->getCentralName(),
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Architecture: Picoblaze",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Family:       " + projectMan->getUntracked()->family + "\n\n",
                                    CompilerBase::MessageType::MT_REMARK);

            QDir pathDir(GuiCfg::getInstance().getTempPath());
            if (false == pathDir.exists())
            {
                pathDir.mkpath(".");
            }

            QDir prjDir;
            if ("" == wDockManager->getCentralName().section('.',0,-2))
            {
                mainFile = pathDir.absolutePath()
                        + "/"
                        +  wDockManager->getCentralName();
            }
            else
            {
                    mainFile = pathDir.absolutePath()
                        + "/"
                        +  wDockManager->getCentralName().section('.',0,-2);
            }

            //qDebug() << "MainForm: tempPath" << GuiCfg::getInstance().getTempPath();
            //qDebug() << "MainForm: mainfile" << mainFile;
            //qDebug() << "MainForm: section" << wDockManager->getCentralPath().section('/',-1);
            //qDebug() << "MainForm: section2" << wDockManager->getCentralPath().section('/',-1).section('.',0,0);


            if (projectMan->getUntracked()->compileOpt.at(0))
            {
                //options->m_symbolTable = (mainFile + ".stbl").toStdString();
                options->m_symbolTable = ( mainFile
                                         + ".stbl"
                                         ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(1))
            {
                options->m_macroTable = ( mainFile
                                        + ".mtbl"
                                        ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(2))
            {
                options->m_mdsDebugFile = ( mainFile
                                        + ".dbg"
                                        ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(3))
            {
                options->m_codeTree = ( mainFile
                                      + ".ctr"
                                      ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(4))
            {
                options->m_lstFile = ( mainFile
                                     + ".lst"
                                     ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(5))
            {
                options->m_hexFile = ( mainFile
                                     + ".ihex"
                                     ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(6))
            {
                options->m_binFile = ( mainFile
                                     + ".bin"
                                     ).toLocal8Bit().constData();
            }
            if (projectMan->getUntracked()->compileOpt.at(7))
            {
                options->m_srecFile = ( mainFile
                                      + ".srec"
                                      ).toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(8))
            {
                options->m_memFile = ( mainFile
                                    + ".mem"
                                    ).toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(9))
            {
                options->m_rawHexDumpFile = ( mainFile
                                            + ".rawhex"
                                            ).toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(10))
            {
                options->m_verilogFile = ( mainFile
                                        + ".v"
                                        ).toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(11))
            {
                options->m_vhdlFile = ( mainFile
                                    + ".vhd"
                                    ).toLocal8Bit().constData();
            }
            if (projectMan->getActive()->compileOpt.at(12))
            {
                options->m_stringTable = ( mainFile + ".strtbl").toLocal8Bit().constData();
            }
        }
        //return;
    }
    //else compile main file of actual project
    else
    {
        //qDebug() << "MainForm: actual project, main file";
        if (projectMan->getActive()->mainFileName == "" && projectMan->getActive()->mainFilePath == "")
        {
            error(ERR_NO_MAINFILE);
            return;
        }

        this->saveProject();

        CompileInfo *compileInfo = ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()));
        compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Compilation settings:",
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Project:      " + projectMan->getActive()->prjName,
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("File:         " + projectMan->getActive()->mainFileName,
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Architecture: Picoblaze",
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Family:       " + projectMan->getActive()->family + "\n\n",
                                   CompilerBase::MessageType::MT_REMARK);

        options->m_sourceFiles.push_back(  (projectMan->getActive()->prjPath.section('/',0, -2)
                                          + "/"
                                          +  this->projectMan->getActive()->mainFilePath
                                           ).toLocal8Bit().constData()
                                        );
        QDir prjDir(projectMan->getActive()->prjPath.section('/',0, -2));
        QDir fileDir(QString::fromStdString(options->m_sourceFiles.at(0)).section('/',0, -2));
        if (fileDir.relativeFilePath(prjDir.absolutePath()) == "")
        {
            if ("" == this->projectMan->getActive()->mainFileName.section('.',0,-2))
            {
                mainFile = "./" +  this->projectMan->getActive()->mainFileName.section('.',0,-2);
            }
            else
            {
                mainFile = "./" +  this->projectMan->getActive()->mainFileName;
            }
        }
        else
        {
            if ("" == this->projectMan->getActive()->mainFilePath.section('.',0,-2))
            {
                mainFile = fileDir.relativeFilePath(prjDir.absolutePath())
                        + "/"
                        +  this->projectMan->getActive()->mainFilePath;
            }
            else
            {
                mainFile = fileDir.relativeFilePath(prjDir.absolutePath())
                        + "/"
                        +  this->projectMan->getActive()->mainFilePath.section('.',0,-2);
            }
        }

        options->m_device = this->projectMan->getActive()->family.toStdString();


        QDir templateDir(GuiCfg::getInstance().getTemplatePath());
        if (true == this->projectMan->getActive()->defaultVerilog)
        {
            options->m_verilogTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + this->projectMan->getActive()->family
                                        + ".v"
                                        ).toLocal8Bit().constData();
        }
        else
        {
            options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
        }

        if (true == this->projectMan->getActive()->defaultVHDL)
        {
            options->m_vhdlTemplate = ( templateDir.absolutePath()
                                    + "/"
                                    + this->projectMan->getActive()->family
                                    + ".vhd"
                                    ).toLocal8Bit().constData();
        }
        else
        {
            options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
        }


        if (projectMan->getActive()->compileOpt.at(0))
        {
            options->m_symbolTable = (mainFile + ".stbl").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(1))
        {
            options->m_macroTable = (mainFile + ".mtbl").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(2))
        {
            options->m_mdsDebugFile = (mainFile + ".dbg").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(3))
        {
            options->m_codeTree = (mainFile + ".ctr").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(4))
        {
            options->m_lstFile = (mainFile + ".lst").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(5))
        {
            options->m_hexFile = (mainFile + ".ihex").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(6))
        {
            options->m_binFile = (mainFile + ".bin").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(7))
        {
            options->m_srecFile = (mainFile + ".srec").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(8))
        {
            options->m_memFile = ( mainFile + ".mem").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(9))
        {
            options->m_rawHexDumpFile = ( mainFile + ".rawhex" ).toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(10))
        {
            options->m_verilogFile = ( mainFile  + ".v").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(11))
        {
            options->m_vhdlFile = ( mainFile + ".vhd").toLocal8Bit().constData();
        }
        if (projectMan->getActive()->compileOpt.at(12))
        {
            options->m_stringTable = ( mainFile + ".strtbl").toLocal8Bit().constData();
        }
    }


    //QString mainFile =  this->projectMan->getActive()->mainFileName.section('.',0,-2);

    //qDebug() << QString::fromStdString(options->m_sourceFile);
    //qDebug() << mainFile;


    CompilerThread *compiler = new CompilerThread(GuiCfg::getInstance().getCompilerPath().toStdString());
    //    CompilerThread *compiler = new CompilerThread("../compiler/include/");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<CompilerBase::MessageType>("CompilerBase::MessageType");
    connect(compiler,
            SIGNAL(compilationMessage(const std::string&, CompilerBase::MessageType)),
            this,
            SLOT(reloadCompileInfo(const std::string&, CompilerBase::MessageType))
           );
    connect(compiler,
            SIGNAL(compilationFinished(bool)),
            this,
            SLOT(compilationFinished(bool))
           );
    compiler->compile(CompilerBase::LI_ASM, CompilerBase::TA_PICOBLAZE, options);
}


/**
 * @brief Slot. Writes compilation exit status to CompileInfo
 * @param success Compiper exit status
 *
 */
void MainForm::compilationFinished(bool success)
{
    wDockManager->setBottomAreaToCompilerInfo();
    if ( true == success )
    {
        ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->setFinished(true);
        if (true == this->simulationRequest)
        {
            this->simulationFlowHandle();
            this->simulationRequest = false;
        }
    }
    else
    {
        ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->setFinished(false);
        if (true == this->simulationRequest)
        {
            error(ErrorCode::ERR_SIM_RECOMPILE_FAILED);
            this->simulationRequest = false;
        }
    }
    delete this->options;
}


/**
 * @brief Slot. Writes compilation message to CompileInfo
 * @param text Compilation message
 * @param type Type of message
 */
void MainForm::reloadCompileInfo(const std::string &text, CompilerBase::MessageType type)
{
    //qDebug() << QString::fromStdString(text);
    ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->appendMessage(QString::fromStdString(text), type);
}


/*
 * @brief Write text to wCompileInfo dock widget, used in compilation
 * @param text String value that will be written to widget
 */
//void MainForm::writeToWCompileInfo(QString text, , CompilerBase::MessageType type)
//{
    //((QPlainTextEdit*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->appendPlainText(text);
//    (CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->appendMessage(text);
//}


/**
 * @brief Returns pointer to dock widgets manager.
 * @return Returns pointer to dock widgets manager
 */
WDockManager* MainForm::getWDockManager()
{
    return wDockManager;
}




/**
 * @brief Slot. Makes step in active simulation.
 */
void MainForm::simulationStep()
{
    if (true == simulationStatus)
    {
        projectMan->getSimulated()->step();
    }
}


/**
 * @brief Slot. Run simulation, does not show changes.
 */
void MainForm::simulationRunHandle()
{
    if (true == simulationStatus)
    {
        delete this->icon_simRun;
        if (true == simulationRunStatus)
        {
            this->icon_simRun = new QIcon(*pm_simRun);
            simulationRunAct->setIcon(*icon_simRun);
            simulationRunAct->setText(tr("Run"));
            this->simulationAnimateAct->setEnabled(true);
            this->simulationStepAct->setEnabled(true);
            this->simulationResetAct->setEnabled(true);
            this->simulationBreakpointAct->setEnabled(true);
            this->simulationDisableBreakpointsAct->setEnabled(true);
            this->simulationRunStatus = false;
        }
        else
        {
            this->icon_simRun = new QIcon(*pm_cross);
            simulationRunAct->setIcon(*icon_simRun);
            simulationRunAct->setText(tr("Stop Run"));
            this->simulationAnimateAct->setDisabled(true);
            this->simulationStepAct->setDisabled(true);
            this->simulationResetAct->setDisabled(true);
            this->simulationBreakpointAct->setDisabled(true);
            this->simulationDisableBreakpointsAct->setDisabled(true);
            this->simulationRunStatus = true;
        }
        projectMan->getSimulated()->run();
    }
}


/**
 * @brief Slot. Run simulation, does show changes.
 */
void MainForm::simulationAnimateHandle()
{
    if (true == simulationStatus)
    {
        delete this->icon_simAnimate;
        if (true == simulationAnimateStatus)
        {
            this->icon_simAnimate = new QIcon(*pm_simAnimate);
            simulationAnimateAct->setIcon(*icon_simAnimate);
            simulationAnimateAct->setText(tr("Animate"));
            this->simulationRunAct->setEnabled(true);
            this->simulationStepAct->setEnabled(true);
            this->simulationResetAct->setEnabled(true);
            this->simulationBreakpointAct->setEnabled(true);
            this->simulationDisableBreakpointsAct->setEnabled(true);
            this->simulationAnimateStatus = false;
        }
        else
        {
            this->icon_simAnimate = new QIcon(*pm_cross);
            simulationAnimateAct->setIcon(*icon_simAnimate);
            simulationAnimateAct->setText(tr("Stop Animate"));
            this->simulationRunAct->setDisabled(true);
            this->simulationStepAct->setDisabled(true);
            this->simulationResetAct->setDisabled(true);
            this->simulationBreakpointAct->setDisabled(true);
            this->simulationDisableBreakpointsAct->setDisabled(true);
            this->simulationAnimateStatus = true;
        }
        projectMan->getSimulated()->animate();
    }
}



/**
 * @brief Slot. Resets active simulation.
 */
void MainForm::simulationReset()
{
    if (true == simulationStatus)
    {
        projectMan->getSimulated()->reset();
        //this->wDockManager->getCentralTextEdit()->clearHighlight();
    }
}


/**
 * @brief Slot. Starts/stops simulation.
 */
void MainForm::simulationFlowHandle()
{
    if (false == simulationStatus)
    {
        QString file = "";
        QString dumpFiles = "";
        if (projectMan->getActive() == NULL)
        {
            error(ERR_NO_PROJECT);
        }
        if (projectMan->getActive()->prjPath == "untracked")
        {
            this->saveFile();
            file = wDockManager->getCentralPath();
            dumpFiles = GuiCfg::getInstance().getTempPath() + "/" + wDockManager->getCentralName();
        }
        else
        {
            if ( false == projectMan->getActive()->useMainFile )
            {
                //check if enabled, if it isnt, simulate current file
                this->saveFile();

                QDir prjDir(projectMan->getActive()->prjPath.section('/',0, -2));
                QDir fileDir;
                QString filePath;

                bool found = false;

                for (int i = 0; i < projectMan->getActive()->filePaths.count(); i++)
                {
                    filePath = QDir::cleanPath(prjDir.absolutePath() + "/" + projectMan->getActive()->filePaths.at(i));
                    /*fileDir.setPath(prjDir.absolutePath()
                                    + "/"
                                    + projectMan->getActive()->filePaths.at(i).section('/',0, -2)
                                );*/
                    //qDebug() << "MainForm: central path:" << wDockManager->getCentralPath();
                    //qDebug() << "MainForm: file path" << QDir::cleanPath(fileDir.absolutePath() + "/" + projectMan->getActive()->fileNames.at(i));
                    //qDebug() << filePath;
                    //qDebug() << wDockManager->getCentralPath();
                    if (filePath == wDockManager->getCentralPath())
                    {
                        found = true;
                        break;
                    }
                }
                if (true == found)
                {
                    file = filePath;
                    dumpFiles = prjDir.absolutePath() + "/" + wDockManager->getCentralName();
                }
                else
                {
                    if (projectMan->getUntracked() != NULL)
                    {
                        projectMan->addUntrackedProject();
                        projectMan->setActive(projectMan->getUntracked());
                    }
                    file = filePath;
                    dumpFiles = GuiCfg::getInstance().getTempPath() + "/" + wDockManager->getCentralName();
                }
            }
            else
            {
                this->saveProject();
                //simulate main file
                //file = "";
            }
        }
        //qDebug() << "MainForm: sim file" << file;
        //qDebug() << "MainForm: sim dump file" << dumpFiles;
        projectMan->setSimulated(projectMan->getActive());
        int start = projectMan->getActive()->start(file, dumpFiles);
        if ( 0 == start )
        {
            delete this->icon_simFlow;
            this->icon_simFlow = new QIcon(*pm_simFlowStop);
            simulationFlowAct->setIcon(*icon_simFlow);
            simulationFlowAct->setText(tr("Stop simulation"));
            simulationStatus = true;
            simulationStepAct->setEnabled(true);
            simulationRunAct->setEnabled(true);
            simulationAnimateAct->setEnabled(true);
            simulationDisableBreakpointsAct->setEnabled(true);
            simulationResetAct->setEnabled(true);
            simulationUnhighlightAct->setEnabled(true);
            #ifdef MDS_FEATURE_SIM_LED_PANEL
                toolSimLedsAct->setEnabled(true);
            #endif
            #ifdef MDS_FEATURE_SIM_7_SEGMENT
                toolSim7SegAct->setEnabled(true);
            #endif
            #ifdef MDS_FEATURE_SIM_PORT_LOGGER
                //toolSimLoggerAct->setEnabled(true);
            #endif
            #ifdef MDS_FEATURE_SIM_SWITCH
                toolSimSwitchAct->setEnabled(true);
            #endif
            projectConfigAct->setDisabled(true);
            projectCompileAct->setDisabled(true);
            if (true == simulationBreakpointsEnabled)
            {
                //qDebug() << "MainForm: simulationBreakpointsEnabled true";
                projectMan->getSimulated()->setBreakpoints(true);
            }
            else
            {
                //qDebug() << "MainForm: simulationBreakpointsEnabled false";
                projectMan->getSimulated()->setBreakpoints(false);
            }
            wDockManager->setBottomAreaToSimulationInfo();
        }
        else
        {
            projectMan->setSimulated(NULL);
            switch (start)
            {
                case 1:
                {
                    error(ErrorCode::ERR_SIM_NOSTART);
                    break;
                }
                case 2:
                {
                    error(ErrorCode::ERR_SIM_NOSTART);
                    break;
                }
                case 3:
                {
                    //error(ErrorCode::ERR_SIM_NOT_COMPILED);
                    QMessageBox msgBox;
                    msgBox.setText("The source files have not been compiled.");
                    msgBox.setInformativeText("Do you want to compile?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    msgBox.setIcon(QMessageBox::Question);
                    int ret = msgBox.exec();
                    switch (ret)
                    {
                        case QMessageBox::Yes:
                        {
                            this->simulationRequest = true;
                            this->compileProject();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case 4:
                {
                    //error(ERR_SIM_NOT_COMPILED_MODIFIED);
                    QMessageBox msgBox;
                    msgBox.setText("The source files have been modified.");
                    msgBox.setInformativeText("Do you want to recompile?");
                    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                    msgBox.setDefaultButton(QMessageBox::Yes);
                    msgBox.setIcon(QMessageBox::Question);
                    int ret = msgBox.exec();
                    switch (ret)
                    {
                        case QMessageBox::Yes:
                        {
                            this->simulationRequest = true;
                            this->compileProject();
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
            }

        }
    }
    else
    {
        delete this->icon_simFlow;
        this->icon_simFlow = new QIcon(*pm_simFlowStart);
        simulationFlowAct->setIcon(*icon_simFlow);
        simulationFlowAct->setText(tr("Start simulation"));
        if (true == simulationAnimateStatus)
        {
            this->simulationAnimateHandle();
        }
        else if (true == simulationRunStatus)
        {
            this->simulationRunHandle();
        }
        simulationStatus = false;
        simulationStepAct->setDisabled(true);
        simulationRunAct->setDisabled(true);
        simulationAnimateAct->setDisabled(true);
        simulationDisableBreakpointsAct->setDisabled(true);
        simulationResetAct->setDisabled(true);
        simulationUnhighlightAct->setDisabled(true);
        #ifdef MDS_FEATURE_SIM_LED_PANEL
            toolSimLedsAct->setDisabled(true);
        #endif
        #ifdef MDS_FEATURE_SIM_7_SEGMENT
            toolSim7SegAct->setDisabled(true);
        #endif
        #ifdef MDS_FEATURE_SIM_PORT_LOGGER
        //    toolSimLoggerAct->setDisabled(true);
        #endif
        #ifdef MDS_FEATURE_SIM_SWITCH
            toolSimSwitchAct->setDisabled(true);
        #endif
        projectConfigAct->setEnabled(true);
        projectCompileAct->setEnabled(true);
        projectMan->getSimulated()->stop();
        //this->unhighlight();
        //this->wDockManager->getCentralTextEdit()->clearHighlight();
    }
}





/**
* @brief Slot. Shows tool - convertor.
*/
void MainForm::toolConvertor()
{
    #ifdef MDS_FEATURE_CONVERTER_TOOL
        /*ConvertorTool *a = */new ConvertorTool(0);
    #endif
}



/**
* @brief Slot. Shown tool - 8 Segment Editor.
*/
void MainForm::toolDisplay()
{
    #ifdef MDS_FEATURE_8_SEGMENT_EDITOR
        /*DisplayTool *a = */new DisplayTool(0);
    #endif
}


/**
 * @brief Returns pointer to project manager.
 * @return Returns pointer to project manager.
 */
ProjectMan* MainForm::getProjectMan()
{
    return this->projectMan;
}


/**
 * @brief Slot. Opens example project.
 */
void MainForm::exampleOpen()
{
    //qDebug() << "MainForm: exampleOpen";
    if (false == this->openProject(GuiCfg::getInstance().getExamplePath() + "/MDSExample.mds-project"))
    {
        return;
    }
    int count = this->projectMan->getActive()->filePaths.count();
    QDir projectDir = QFileInfo(this->projectMan->getActive()->prjPath).dir();
    QString absolutePath = projectDir.path();
    for (int i = 0; i < count; i++)
    {
        //qDebug() << "MainForm: loading";
        this->openFilePath(QDir(absolutePath + "/" + this->projectMan->getActive()->filePaths.at(i)).canonicalPath());
    }
    if (wDockManager->getTabCount() > 0)
    {
        wDockManager->setCentralByIndex(0);
    }
    //qDebug() << "MainForm: return exampleOpen";
}


/* *
 * @brief Slot. Show plugin manager gui.
 */
/*void MainForm::showPlugins()
{
    //PluginMan_GUI *a = new PluginMan_GUI(0);
}*/


/**
 * @brief Slot. Access sim project data to wdockmanager.
 */
void MainForm::simProjectData()
{
    wDockManager->addSimDockWidgetP2(projectMan->getActive()->prjPath, projectMan->getActive()->getSimControl());
    wDockManager->openSimWidgets.at(wDockManager->openSimWidgets.count()-1)->setClock(projectMan->getActive()->clock,
                                                                                      projectMan->getActive()->clockMult
                                                                                     );
}

/**
 * @brief Slot. Tabify two dock widgets.
 */
void MainForm::tabifyDockWidgetSlot(QDockWidget *widget1, QDockWidget *widget2)
{
    this->tabifyDockWidget(widget1, widget2);
}


/**
 * @brief Slot. Adds dockwidget to dockwidgetarea.
 */
void MainForm::addDockWidgetSlot(Qt::DockWidgetArea area, QDockWidget *widget)
{
    this->addDockWidget(area, widget);
}


/**
 * @brief Slot. Connects project signals to mainform slots.
 */
void MainForm::connectProjectSlot(Project *project)
{
    //connect(project, SIGNAL(highlightLine(QString, int, QColor*)), this, SLOT(highlightLine(QString, int, QColor*)));
    connect(project,
            SIGNAL(simHighlightLines(std::vector<std::pair<const std::string *, unsigned int>>,
                                     std::vector<std::pair<const std::string *, unsigned int>>,
                                     std::vector<std::pair<const std::string *, unsigned int>>,
                                     QList<QColor*>)),
            this,
            SLOT(simHighlightLines(std::vector<std::pair<const std::string *, unsigned int>>,
                                   std::vector<std::pair<const std::string *, unsigned int>>,
                                   std::vector<std::pair<const std::string *, unsigned int>>,
                                   QList<QColor*>)));

    connect(project, SIGNAL(setCentralByName(QString)), this, SLOT(setCentralByName(QString)));
    //connect(project, SIGNAL(scrollToLine(int)), this, SLOT(scrollCentralToLine(int)));
    connect(project, SIGNAL(addUntrackedFile(QString, QString)), this, SLOT(addUntrackedFile(QString, QString)));
    connect(project, SIGNAL(openFilePath(QString)), this, SLOT(openFilePath(QString)));
    connect(project, SIGNAL(setEditorReadOnly(bool)), this, SLOT(setEditorReadOnly(bool)));
    connect(project, SIGNAL(startConfig(Project*)), this, SLOT(startProjectConfig(Project*)));
    connect(project, SIGNAL(changeFamily(QString)), this, SLOT(changeProjectFamily(QString)));
    connect(project, SIGNAL(closeProject()), this, SLOT(closeProject()));
    connect(project, SIGNAL(breakpointReached()), this, SLOT(pauseSimulation()));
}



/**
 * @brief
 * @param
 * @param
 * @param
 * @param
 */
void MainForm::highlightLine(QString file, int line, QColor *color)
{
    //qDebug() << "MainForm: highlightLine";
    if (file != "")
    {
        this->getWDockManager()->setCentralByName(file.section('/', -1));
        this->getWDockManager()->getCentralTextEdit()->highlightLine(line, color);
        //this->getWDockManager()->getCentralWidget()->setSaved();
        //if (this->getWDockManager()->getCentralTextEdit()->highlightLine(line, color) == true)
        //{
        //    this->getWDockManager()->getCentralTextEdit()->scrollToLine(line, false);
        //}
        /*else
        {
            qDebug() << "MainForm: highlightLine failed";
        }*/
    }
    //qDebug() << "MainForm: return highlightLine";
}


/**
 * @brief
 * @param
 */
void MainForm::setCentralByName(QString file)
{
    this->getWDockManager()->setCentralByName(file);
}


/**
 * @brief
 * @param
 * @details currently unused
 */
void MainForm::scrollCentralToLine(int line)
{
    this->getWDockManager()->getCentralTextEdit()->scrollToLine(line);
}


/**
 * @brief
 * @param
 */
void MainForm::setEditorReadOnly(bool readOnly)
{
    //qDebug() << "MainForm: setEditorReadOnly";
    this->getWDockManager()->setEditorsReadOnly(readOnly);
    undoAct->setEnabled(!readOnly);
    redoAct->setEnabled(!readOnly);
    copyAct->setEnabled(!readOnly);
    cutAct->setEnabled(!readOnly);
    pasteAct->setEnabled(!readOnly);
    selectAllAct->setEnabled(!readOnly);
    deselectAct->setEnabled(!readOnly);
    findAct->setEnabled(!readOnly);
    findNextAct->setEnabled(!readOnly);
    findPreviousAct->setEnabled(!readOnly);
    replaceAct->setEnabled(!readOnly);
    jmpToLineAct->setEnabled(!readOnly);
    commentAct->setEnabled(!readOnly);
    deleteCommentAct->setEnabled(!readOnly);
    jmpToBookmarkNextAct->setEnabled(!readOnly);
    jmpToBookmarkPrevAct->setEnabled(!readOnly);
    //qDebug() << "MainForm: return setEditorReadOnly";
}


/**
 * @brief
 * @param
 * @param
 */
void MainForm::addUntrackedFile(QString name, QString path)
{
    //qDebug() << "MainForm: addUntrackedFile";
    if (name != NULL && path != NULL)
    {
        wDockManager->addUntrackedCentralWidget(name, path);
        wDockManager->getCentralWidget()->setParentProject(projectMan->getUntracked());
        wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getUntracked());
        //getWDockManager()->getCentralWidget()->setChanged();
        //getWDockManager()->getCentralWidget()->connectAct();
    }
    //qDebug() << "MainForm: return addUntrackedFile";
}

/**
 * @brief
 * @param
 */
void MainForm::startProjectConfig(Project *project)
{
    ProjectConfigDialog_Core *cfgdlg = new ProjectConfigDialog_Core(this, project);
    connect(cfgdlg, SIGNAL(reloadTree()), project, SLOT(reloadProjectTree()));
    cfgdlg->exec();
}


/**
 * @brief
 */
void MainForm::help()
{
    /*HelpWidget *helpWidget = */new HelpWidget(0, this->width(), this->height());
}


/**
* @brief
*/
void MainForm::toolDisassemble()
{
    #ifdef MDS_FEATURE_DISASSEMBLER
        DisAsmDialog *dlg = new DisAsmDialog(this);
        connect(dlg, SIGNAL(output(std::vector<std::string>)), this, SLOT(disassembleOutput(std::vector<std::string>)));
    #endif
}


/**
* @brief
*/
void MainForm::disassembleOutput(std::vector<std::string> text)
{
    #ifdef MDS_FEATURE_DISASSEMBLER
        QStringList qText;
        for (unsigned int i = 0; i < text.size(); i++)
        {
            qText.append(QString::fromStdString(text.at(i)));
        }

        //QString name = this->projectMan->addUntrackedFile(NULL, "disasm");
        this->wDockManager->addUntrackedCentralWidget("disasm","untracked",qText);
        getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
        //getWDockManager()->getCentralWidget()->connectAct();
    #endif
}


/**
* @brief
*/
void MainForm::toolTranslate()
{
    #ifdef MDS_FEATURE_TRANSLATOR
        TranslatorDlg *dlg = new TranslatorDlg(this);
        connect(dlg,
                SIGNAL(output(std::vector<std::string> &)),
                this,
                SLOT(translatorOutput(std::vector<std::string> &)));
        connect(dlg,
                SIGNAL(outputError(const std::vector<std::pair<unsigned int, std::string>> &)),
                this,
                SLOT(translatorOutput(const std::vector<std::pair<unsigned int, std::string>> &)));
    #endif
}


/**
* @brief
*/
void MainForm::translatorOutput(std::vector<std::string> & text)
{
    #ifdef MDS_FEATURE_TRANSLATOR
        QStringList qText;
        for (unsigned int i = 0; i < text.size(); i++)
        {
            qText.append(QString::fromStdString(text.at(i)));
        }
        //QString name = this->projectMan->addUntrackedFile(NULL, "disasm");
        this->wDockManager->addUntrackedCentralWidget("Assembler Translator","untracked",qText);
        //qDebug() << getWDockManager()->getCentralTextEdit()->toPlainText();
        //getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
        //getWDockManager()->getCentralWidget()->connectAct();
    #endif
}


/**
* @brief
*/
void MainForm::translatorOutput(const std::vector<std::pair<unsigned int, std::string>> & text)
{
    #ifdef MDS_FEATURE_TRANSLATOR
        QStringList qText;
        for (unsigned int i = 0; i < text.size(); i++)
        {
            qText.append(QString::fromStdString(std::get<1>(text.at(i))));
        }
        //QString name = this->projectMan->addUntrackedFile(NULL, "disasm");
        this->wDockManager->addUntrackedCentralWidget("Assembler Translator error","untracked",qText);
        //qDebug() << getWDockManager()->getCentralTextEdit()->toPlainText();
        //this->getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
        //getWDockManager()->getCentralWidget()->connectAct();
    #endif
}


/**
* @brief
*/
void MainForm::toolFileConvert()
{
    #ifdef MDS_FEATURE_FILECONVERTER
        /*FileConvertDlg *dlg = */new FileConvertDlg(this);
    #endif
}

/**
 *
 */
void MainForm::unhighlight()
{
    emit unhighlightSim();
}


void MainForm::projectConfig()
{
    if (false == simulationStatus)
    {
        this->startProjectConfig(this->projectMan->getActive());
    }
}


void MainForm::interfaceConfig()
{
    InterfaceCfgDlg_Core *cfgdlg = new InterfaceCfgDlg_Core(this);
    cfgdlg->exec();
}


void MainForm::stopSimSlot()
{
    //qDebug() << "MainForm: stopSimSlot";
    if (true == simulationRunStatus)
    {
        this->simulationRunHandle();
    }
    if (true == simulationAnimateStatus)
    {
        this->simulationAnimateHandle();
    }
}


void MainForm::activeProjectChanged(int index)
{
    //qDebug() << "MainForm: activeProjectChanged" << index;
    //if (false == this->simulationStatus)
    if (index >= 0)
    {
        projectMan->setActiveByIndex(index);
        wDockManager->changeSimWidget(index);
        wDockManager->showProjectEditors(projectMan->getActive());
        if (wDockManager->getBreakpointList() != NULL)
        {
            wDockManager->getBreakpointList()->reload(projectMan->getActive()->getBreakpointsListRef());
        }
        if (wDockManager->getBookmarkList() != NULL)
        {
            wDockManager->getBookmarkList()->reload(projectMan->getActive()->getBookmarksListRef());
        }
    }
    //qDebug() << "MainForm: return activeProjectChanged";
}


void MainForm::enableSimActs()
{
    if (false == projectCompileAct->isEnabled())
    {
        projectCompileAct->setEnabled(true);
    }
    if (false == simulationFlowAct->isEnabled())
    {
        simulationFlowAct->setEnabled(true);
    }
    if (false == simulationBreakpointAct->isEnabled())
    {
        simulationBreakpointAct->setEnabled(true);
    }
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    saveAllAct->setEnabled(true);
    saveProjAct->setEnabled(true);
    closeFileAct->setEnabled(true);
    undoAct->setEnabled(true);
    redoAct->setEnabled(true);
    copyAct->setEnabled(true);
    cutAct->setEnabled(true);
    pasteAct->setEnabled(true);
    selectAllAct->setEnabled(true);
    deselectAct->setEnabled(true);
    findAct->setEnabled(true);
    findNextAct->setEnabled(true);
    findPreviousAct->setEnabled(true);
    replaceAct->setEnabled(true);
    jmpToLineAct->setEnabled(true);
    commentAct->setEnabled(true);
    deleteCommentAct->setEnabled(true);
    jmpToBookmarkNextAct->setEnabled(true);
    jmpToBookmarkPrevAct->setEnabled(true);
}


void MainForm::disableSimActs()
{
    if (true == projectCompileAct->isEnabled())
    {
        projectCompileAct->setDisabled(true);
    }
    if (true == simulationFlowAct->isEnabled())
    {
        simulationFlowAct->setDisabled(true);
    }
    if (true == simulationBreakpointAct->isEnabled())
    {
        simulationBreakpointAct->setDisabled(true);
    }
    saveAct->setEnabled(false);
    saveAsAct->setEnabled(false);
    saveAllAct->setEnabled(false);
    saveProjAct->setEnabled(false);
    closeFileAct->setEnabled(false);
    undoAct->setEnabled(false);
    redoAct->setEnabled(false);
    copyAct->setEnabled(false);
    cutAct->setEnabled(false);
    pasteAct->setEnabled(false);
    selectAllAct->setEnabled(false);
    deselectAct->setEnabled(false);
    findAct->setEnabled(false);
    findNextAct->setEnabled(false);
    findPreviousAct->setEnabled(false);
    replaceAct->setEnabled(false);
    jmpToLineAct->setEnabled(false);
    commentAct->setEnabled(false);
    deleteCommentAct->setEnabled(false);
    jmpToBookmarkNextAct->setEnabled(false);
    jmpToBookmarkPrevAct->setEnabled(false);
}


void MainForm::changeProjectFamily(QString family)
{
    //qDebug() << "MainForm: changeFamily";
    projectMan->getActive()->setupSim(family);
}


void MainForm::closeProject()
{
    if (false == this->simulationStatus || projectMan->getActive() != projectMan->getSimulated())
    {
        Project *project = projectMan->getActive();
        QDir path;
        for (int i = 0; i < project->filePaths.count(); i++)
        {
            path.setPath(project->prjPath.section('/',0,-2));
            wDockManager->closeFile(QDir::cleanPath(path.absoluteFilePath(project->filePaths.at(i))));
        }
        //qDebug() << "MainForm: delete active sim widget";
        wDockManager->deleteActiveSimWidget();
        //qDebug() << "MainForm: remove dock widget";
        this->removeDockWidget(project->prjDockWidget);
        //qDebug() << "MainForm: close project";
        projectMan->closeProject(project);
        if (wDockManager->getBreakpointList() != NULL)
        {
            wDockManager->getBreakpointList()->reload(projectMan->getActive()->getBreakpointsListRef());
            /*if (wDockManager->getCentralWidget() != NULL)
            {
                wDockManager->getCentralWidget()->setBreakpointsLines(projectMan->getActive()->getBreakpointsForFileAbsolute(wDockManager->getCentralWidget()->getPath()));
            }*/
        }
        if (wDockManager->getBookmarkList() != NULL)
        {
            wDockManager->getBookmarkList()->reload(projectMan->getActive()->getBookmarksListRef());
            /*if (wDockManager->getCentralWidget() != NULL)
            {
                wDockManager->getCentralWidget()->setBookmarksLines(projectMan->getActive()->getBookmarksForFileAbsolute(wDockManager->getCentralWidget()->getPath()));
            }*/
        }
        if (projectMan->getOpenProjects().count() == 0)
        {
            wDockManager->setCentralWelcome();
            closeProjectAct->setEnabled(false);
            newAddAct->setEnabled(false);
            saveProjConfigAct->setEnabled(false);
            saveProjAct->setEnabled(false);
            projectConfigAct->setEnabled(false);
        }
    }
}


void MainForm::manageBreakpointEmit(QString file, int line)
{
    //qDebug() << "MainForm: breakpoint:" << file << ":" << line + 1;
    int result = projectMan->getActive()->handleBreakpoint(file, line + 1);
    //add
    if (0 == result)
    {
        wDockManager->getBreakpointList()->breakpointListAdd(file, line + 1);
        wDockManager->getCentralWidget()->addBreakpointLine(line + 1);
    }
    //remove
    else if (1 == result)
    {
        wDockManager->getBreakpointList()->breakpointListRemove(file, line + 1);
        wDockManager->getCentralWidget()->removeBreakpointLine(line + 1);
    }
    //else project doesnt contain current file - result == -1
    /*QList<Project*> projects = projectMan->getOpenProjects();
    for (int i = 0; i < projects.count(); i++)
    {
        projects.at(i)->handleBreakpoint(file, line + 1);
    }*/
}

void MainForm::breakpointsAddLines(QString file, int line, int linesAdded)
{
    projectMan->getActive()->moveBreakpointsAdd(file, line + 1, linesAdded);
    wDockManager->getBreakpointList()->breakpointsAddLines(file, line + 1, linesAdded);
    wDockManager->getCentralWidget()->moveBreakpointsLines(line + 1, linesAdded, true);
}


void MainForm::breakpointsRemoveLines(QString file, int line, int linesRemoved)
{
    projectMan->getActive()->moveBreakpointsRemove(file, line + 1, linesRemoved);
    wDockManager->getBreakpointList()->breakpointsRemoveLines(file, line + 1, linesRemoved);
    wDockManager->getCentralWidget()->moveBreakpointsLines(line + 1, linesRemoved, false);
}


void MainForm::manageBookmarkEmit(QString file, int line)
{
    //qDebug() << "MainForm: bookmark:" << file << ":" << line + 1;
    int result = projectMan->getActive()->handleBookmark(file, line + 1);
    //add
    if (0 == result)
    {
        wDockManager->getBookmarkList()->bookmarkListAdd(file, line + 1);
        wDockManager->getCentralWidget()->addBookmarkLine(line + 1);
    }
    //remove
    else if (1 == result)
    {
        wDockManager->getBookmarkList()->bookmarkListRemove(file, line + 1);
        wDockManager->getCentralWidget()->removeBookmarkLine(line + 1);
    }
    //else project doesnt contain current file - result == -1
    /*QList<Project*> projects = projectMan->getOpenProjects();
    for (int i = 0; i < projects.count(); i++)
    {
        projects.at(i)->handleBreakpoint(file, line + 1);
    }*/
}


void MainForm::bookmarksAddLines(QString file, int line, int linesAdded)
{
    //qDebug() << "MainForm: bookmarksAddLines";
    projectMan->getActive()->moveBookmarksAdd(file, line + 1, linesAdded);
    wDockManager->getBookmarkList()->bookmarksAddLines(file, line + 1, linesAdded);
    wDockManager->getCentralWidget()->moveBookmarksLines(line + 1, linesAdded, true);
}


void MainForm::bookmarksRemoveLines(QString file, int line, int linesRemoved)
{
    projectMan->getActive()->moveBookmarksRemove(file, line + 1, linesRemoved);
    wDockManager->getBookmarkList()->bookmarksRemoveLines(file, line + 1, linesRemoved);
    wDockManager->getCentralWidget()->moveBookmarksLines(line + 1, linesRemoved, false);
}


/*void MainForm::manageBreakpointRemove(QString file, int line)
{
    qDebug() << "MainForm: breakpoint remove:" << file << ":" << line;
    QList<Project*> projects = projectMan->getOpenProjects();
    for (int i = 0; i < projects.count(); i++)
    {
        projects.at(i)->handleBreakpoint(file, line, false);
    }
}*/


/*void MainForm::emitSessionRestorationSignal()
{
    emit sessionRestorationSignal();
}*/


void MainForm::sessionRestorationSlot()
{
    //qDebug() << "MainForm: session restoration";
    QApplication::processEvents();
    //qDebug() << "MainForm: height" << this->height();
    //open projects and files
    /*if (this->height() == this->startHeight)
    {
        qDebug() << "Mainform not visible";
        qDebug() << "MainForm: session not loaded, Mainform not visible";
        QTimer::singleShot(50, this, SLOT(sessionRestorationSlot()));
        return;
    }*/
    QList<QString> projectPaths = GuiCfg::getInstance().getSessionProjectPaths();
    QList<QString> filePaths = GuiCfg::getInstance().getSessionFilePaths();
    QList<QString> fileParentProjects = GuiCfg::getInstance().getSessionFileParentProjects();
    for (int i = 0; i < projectPaths.count(); i++)
    {
        //qDebug() << "MainForm: session open project";
        if (projectPaths.at(i) == "untracked")
        {
            this->projectMan->addUntrackedProject();
        }
        else
        {
            this->openProject(projectPaths.at(i));
        }
    }
    for (int i = 0; i < filePaths.count(); i++)
    {
        //qDebug() << "MainForm: session open file" << filePaths.at(i);
        //qDebug() << "MainForm: session file parent" << fileParentProjects.at(i);
        this->openFilePath(filePaths.at(i), fileParentProjects.at(i));
    }
    GuiCfg::getInstance().sessionClear();
    //hack for fixing the linecount height (bigger at start)
    QTimer::singleShot(50, this->wDockManager->getCentralWidget(), SLOT(changeHeight()));
    //qDebug() << "MainForm: height" << this->height();
    //qDebug() << "MainForm: session loaded";
}


void MainForm::pauseSimulation()
{
    if (true == simulationStatus)
    {
        if (true == simulationAnimateStatus)
        {
            delete this->icon_simAnimate;
            this->icon_simAnimate = new QIcon(*pm_simAnimate);
            simulationAnimateAct->setIcon(*icon_simAnimate);
            simulationAnimateAct->setText(tr("Animate"));
            this->simulationRunAct->setEnabled(true);
            this->simulationStepAct->setEnabled(true);
            this->simulationResetAct->setEnabled(true);
            this->simulationBreakpointAct->setEnabled(true);
            this->simulationDisableBreakpointsAct->setEnabled(true);
            this->simulationAnimateStatus = false;
            return;
        }
        if (true == simulationRunStatus)
        {
            delete this->icon_simRun;
            this->icon_simRun = new QIcon(*pm_simRun);
            simulationRunAct->setIcon(*icon_simRun);
            simulationRunAct->setText(tr("Run"));
            this->simulationAnimateAct->setEnabled(true);
            this->simulationStepAct->setEnabled(true);
            this->simulationResetAct->setEnabled(true);
            this->simulationBreakpointAct->setEnabled(true);
            this->simulationDisableBreakpointsAct->setEnabled(true);
            this->simulationRunStatus = false;
            return;
        }
    }
}



void MainForm::closeEvent(QCloseEvent *event)
{
    qDebug() << "MainForm: closeEvent";
    QStringList lst;
    for (int i = 0; i < wDockManager->getTabCount(); i++)
    {
        //qDebug() << wDockManager->getTabWidget(i)->isChanged();
        if (wDockManager->getTabStatusChanged(i) == true && wDockManager->getTabWidget(i)->getPath() != "untracked")
        {
            lst.append(wDockManager->getTabWidget(i)->getPath());
        }
    }
    if (lst.count() > 0)
    {
        SaveDialog *dlg = new SaveDialog(this, lst);
        connect(dlg, SIGNAL(save(QString)), this, SLOT(saveOnClose(QString)));
        if (QDialog::Rejected == dlg->exec())
        {
            event->ignore();
        }
        else
        {
            event->accept();
        }
    }
    else
    {
        event->accept();
    }
    qDebug() << "MainForm: closeEvent done";
}


void MainForm::undoSlot()
{
    this->getWDockManager()->getCentralTextEdit()->undo();
}


void MainForm::redoSlot()
{
    this->getWDockManager()->getCentralTextEdit()->redo();
}


void MainForm::cutSlot()
{
    this->getWDockManager()->getCentralTextEdit()->cut();
}


void MainForm::copySlot()
{
    this->getWDockManager()->getCentralTextEdit()->copy();
}


void MainForm::pasteSlot()
{
    this->getWDockManager()->getCentralTextEdit()->paste();
}


void MainForm::selectAllSlot()
{
    this->getWDockManager()->getCentralTextEdit()->selectAll();
}


void MainForm::deselectSlot()
{
    this->getWDockManager()->getCentralTextEdit()->deselect();
}


void MainForm::scrollToFileLine(QString file, int line)
{
    wDockManager->setCentralByPath(file);
    wDockManager->getCentralTextEdit()->scrollToLine(line);
}


void MainForm::clockChangedSlot(double clock, int clockMult)
{
    projectMan->getActive()->setClock(clock, clockMult);
}


void MainForm::requestMacrosCodeEdits()
{
    QList<CodeEdit*> list;
    for (int i = 0; i < wDockManager->getTabCount(); i++)
    {
        if (true == wDockManager->getTabWidget(i)->isChild(projectMan->getActive()))
        {
            list.append(wDockManager->getTabWidget(i));
        }
    }
    emit provideMacroCodeEdits(list);
}


void MainForm::breakpointActHandle()
{
    this->manageBreakpointEmit(wDockManager->getCentralPath(), wDockManager->getCentralTextEdit()->textCursor().blockNumber());
}


void MainForm::disableBreakpointsHandle()
{
    if (true == this->simulationBreakpointsEnabled)
    {
        this->simulationBreakpointsEnabled = false;
        this->simulationDisableBreakpointsAct->setText(tr("Enable Breakpoints"));
        if (true == this->simulationStatus)
        {
            this->projectMan->getSimulated()->setBreakpoints(false);
        }
    }
    else
    {
        this->simulationBreakpointsEnabled = true;
        this->simulationDisableBreakpointsAct->setText(tr("Disable Breakpoints"));
        if (true == this->simulationStatus)
        {
            this->projectMan->getSimulated()->setBreakpoints(true);
        }
    }
}


void MainForm::simHighlightLines(std::vector<std::pair<const std::string *, unsigned int>> curr,
                                 std::vector<std::pair<const std::string *, unsigned int>> prev,
                                 std::vector<std::pair<const std::string *, unsigned int>> prev2,
                                 QList<QColor*> colors)
{
    QSet<QString> files;
    if (prev.size() > 0)
    {
        if (prev.size() > 1)
        {
            for (unsigned int i = 0; i < prev.size(); i++)
            {
                files << QString::fromStdString(*(std::get<0>(prev.at(i)))).replace('\\', '/');
            }
        }
        else
        {
            files << QString::fromStdString(*(std::get<0>(prev.at(0)))).replace('\\', '/');
        }
    }
    if (prev2.size() > 0)
    {
        if (prev2.size() > 1)
        {
            for (unsigned int i = 0; i < prev2.size(); i++)
            {
                files << QString::fromStdString(*(std::get<0>(prev2.at(i)))).replace('\\', '/');
            }
        }
        else
        {
            files << QString::fromStdString(*(std::get<0>(prev2.at(0)))).replace('\\', '/');
        }
    }
    if (curr.size() > 0)
    {
        if (curr.size() > 1)
        {
            for (unsigned int i = 0; i < curr.size(); i++)
            {
                files << QString::fromStdString(*(std::get<0>(curr.at(i)))).replace('\\', '/');
            }
        }
        else
        {
            files << QString::fromStdString(*(std::get<0>(curr.at(0)))).replace('\\', '/');
        }
    }

    //const QString prjDir = QDir::cleanPath(QDir(projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath());
    QString simulatedFilePath;
    //qDebug() << "MainForm: simulatedFile" << projectMan->getSimulated()->simulatedFile;
    QString simulatedFileChop = QDir(projectMan->getSimulated()->simulatedFile.section('/',0, -2)).absolutePath() + '/';
    foreach (const QString &value, files)
    {
        simulatedFilePath = QDir::cleanPath(simulatedFileChop + value);
        //qDebug() << "MainForm: simulated file path" << simulatedFilePath;
        if (false == this->getWDockManager()->setCentralByPath(simulatedFilePath))
        {
            //qDebug() << "MainForm: simHighlightLines value" << value;
            this->openFilePath(simulatedFilePath);
            this->getWDockManager()->setCentralByPath(simulatedFilePath);
        }
        this->getWDockManager()->getCentralTextEdit()->clearHighlight();

        /*if (NULL == this->getWDockManager()->getCentralByPath(value))
        {
            this->openFilePath(value);
        }
        this->getWDockManager()->getCentralByPath(value)->getTextEdit()->clearHighlight();*/
    }

    if (prev.size() > 0)
    {
        if (prev.size() > 1)
        {
            for (unsigned int i = 0; i < prev.size(); i++)
            {
                if (QString::fromStdString(*(std::get<0>(prev.at(i)))) != "")
                {
                    simulatedFilePath = simulatedFileChop;
                    simulatedFilePath += QString::fromStdString(*(std::get<0>(prev.at(i))));
                    simulatedFilePath = QDir::cleanPath(simulatedFilePath);
                    this->getWDockManager()->setCentralByPath(simulatedFilePath);
                    this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev.at(i)) - 1, colors.at(1));
                    //this->getWDockManager()->getCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(i)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev.at(i)), colors.at(1));
                }
            }
        }
        else
        {
            simulatedFilePath = simulatedFileChop;
            simulatedFilePath += QString::fromStdString(*(std::get<0>(prev.at(0))));
            simulatedFilePath = QDir::cleanPath(simulatedFilePath);
            this->getWDockManager()->setCentralByPath(simulatedFilePath);
            this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev.at(0)) - 1, colors.at(1));
            //this->getWDockManager()->getCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev.at(0)), colors.at(1));
        }
    }

    if (prev2.size() > 0)
    {
        if (prev2.size() > 1)
        {
            for (unsigned int i = 0; i < prev2.size(); i++)
            {
                if (QString::fromStdString(*(std::get<0>(prev2.at(i)))) != "")
                {
                    simulatedFilePath = simulatedFileChop;
                    simulatedFilePath += QString::fromStdString(*(std::get<0>(prev2.at(i))));
                    simulatedFilePath = QDir::cleanPath(simulatedFilePath);
                    this->getWDockManager()->setCentralByPath(simulatedFilePath);
                    this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev2.at(i)) - 1, colors.at(2));
                    //this->getWDockManager()->getCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)), colors.at(2));
                }
            }
        }
        else
        {
            simulatedFilePath = simulatedFileChop;
            simulatedFilePath += QString::fromStdString(*(std::get<0>(prev2.at(0))));
            simulatedFilePath = QDir::cleanPath(simulatedFilePath);
            this->getWDockManager()->setCentralByPath(simulatedFilePath);
            this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)) - 1, colors.at(2));
            //this->getWDockManager()->getCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)), colors.at(2));
        }
    }

    if (curr.size() > 0)
    {
        if (curr.size() > 1)
        {
            for (unsigned int i = 0; i < curr.size(); i++)
            {
                if (QString::fromStdString(*(std::get<0>(curr.at(i)))) != "")
                {
                    simulatedFilePath = simulatedFileChop;
                    simulatedFilePath += QString::fromStdString(*(std::get<0>(curr.at(i))));
                    simulatedFilePath = QDir::cleanPath(simulatedFilePath);
                    this->getWDockManager()->setCentralByPath(simulatedFilePath);
                    this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(curr.at(i)) - 1, colors.at(0));
                }
            }
        }
        else
        {
            simulatedFilePath = simulatedFileChop;
            simulatedFilePath += QString::fromStdString(*(std::get<0>(curr.at(0))));
            simulatedFilePath = QDir::cleanPath(simulatedFilePath);
            this->getWDockManager()->setCentralByPath(simulatedFilePath);
            this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(curr.at(0)) - 1, colors.at(0));
        }
    }
}


void MainForm::manageLicense()
{
    #ifdef MDS_FEATURE_LICENCE_CERTIFICATE
        LicenseWidget *widget = new LicenseWidget(this);
        widget->tryLoad();
        widget->exec();
    #endif
}

void MainForm::loopGen()
{
    #ifdef MDS_FEATURE_LOOP_GENERATOR
        loop_gen *widget = new loop_gen(0);
        widget->show();
    #endif
}

void MainForm::about()
{
    AboutWidget *widget = new AboutWidget(this);
    widget->exec();
}


void MainForm::refreshProjectTree()
{
        //qDebug() << "count: " << projectTabs->count();
        projectTabs->setCurrentIndex(projectTabs->count()-1);
        //qDebug() << "MainForm: activeProject" << projectMan->getActive()->prjName;
}


/*void MainForm::refreshProjectTree(int index)
{
        qDebug() << "count: " << projectTabs->count();
        if (projectTabs->count <= index)
        {
            qDebug() << "MainForm: refreshProjectTree error";
            projectTabs->setCurrentIndex(projectTabs->count()-1);
        }
        else
        {
            projectTabs->setCurrentIndex(index);
        }
        qDebug() << "MainForm: activeProject" << projectMan->getActive()->prjName;
}*/


void MainForm::showWebSite(QAction */*action*/)
{
    QDesktopServices::openUrl(QUrl("http://www.moravia-microsystems.com"));
}


void MainForm::simLeds()
{
    #ifdef MDS_FEATURE_SIM_LED_PANEL
        if (true == this->simulationStatus)
        {
            Leds_sim *widget = new Leds_sim(0, this->projectMan->getSimulated()->getSimControl());
            widget->show();
        }
    #endif
}


void MainForm::sim7Seg()
{
    #ifdef MDS_FEATURE_SIM_7_SEGMENT
        if (true == this->simulationStatus)
        {
            Sim7Seg *widget = new Sim7Seg(0, this->projectMan->getSimulated()->getSimControl());
            widget->show();
        }
    #endif
}


void MainForm::simSwitch()
{
    #ifdef MDS_FEATURE_SIM_SWITCH
        if (true == this->simulationStatus)
        {
            SimSwitch *widget = new SimSwitch(0, this->projectMan->getSimulated()->getSimControl());
            widget->show();
        }
    #endif
}


void MainForm::simPortLogger()
{
    #ifdef MDS_FEATURE_SIM_PORT_LOGGER
        //if (true == this->simulationStatus)
        //{
        //    SimPortLogger *widget = new SimPortLogger(0, this->projectMan->getSimulated()->getSimControl());
        //    widget->show();
        //}
    #endif
}


void MainForm::userGuide()
{
    QDir dir(GuiCfg::getInstance().getUserGuidePath() + "/QuickUserGuideAssembler.pdf");
    QDesktopServices::openUrl(QUrl("file:///" + dir.absolutePath()));
    //qDebug() << "user guide:" << dir.absolutePath();
}


void MainForm::openRecentFileSlot(QAction *action)
{
    this->openFilePath(action->text(), "untracked");
}


void MainForm::openRecentProjectSlot(QAction *action)
{
    this->openProject(action->text());
}


void MainForm::welcomeDialog()
{
    WelcomeWidget *widget = new WelcomeWidget();
    widget->show();
    connect(widget, SIGNAL(tutorial()), this, SLOT(exampleOpen()));
    connect(widget, SIGNAL(manual()), this, SLOT(help()));
}


void MainForm::saveOnClose(QString path)
{
    wDockManager->setCentralByPath(path);
    this->saveFile();
}


void MainForm::shortcutCloseTab()
{
    wDockManager->closeCurrentTab();
}


void MainForm::shortcutChangeTabLeft()
{
    wDockManager->changeTab(false);
}


void MainForm::shortcutChangeTabRight()
{
    wDockManager->changeTab(true);
}


void MainForm::reloadTabIcons()
{
    for (int i = 0; i < wDockManager->rightAreaTabs->count(); i++)
    {
        QString text = wDockManager->rightAreaTabs->tabText(i);
        if ("Breakpoints" == text)
        {
            QPixmap pixmap(":resources/icons/breakpoint.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
        else if ("Bookmarks" == text)
        {
            QPixmap pixmap(":resources/icons/bullet_star.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
        else if ("Macros" == text)
        {
            QPixmap pixmap(":resources/icons/brick.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
        else if ("Hide" == text)
        {
            QPixmap pixmap(":resources/icons/bullet_arrow_right.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
    }
    for (int i = 0; i < wDockManager->bottomAreaTabs->count(); i++)
    {
        QString text = wDockManager->bottomAreaTabs->tabText(i);
        if ("Compiler Messages" == text)
        {
            wDockManager->bottomAreaTabs->setTabIcon(i, QIcon(":resources/icons/messages.png"));
        }
        else if ("Simulator" == text)
        {
            wDockManager->bottomAreaTabs->setTabIcon(i, QIcon(":resources/icons/cog.png"));
        }
        else if ("Hide" == text)
        {
            wDockManager->bottomAreaTabs->setTabIcon(i, QIcon(":resources/icons/bullet_arrow_down.png"));
        }
    }
}



void MainForm::findSlot()
{
    wDockManager->getCentralTextEdit()->shortcutFind();
}


void MainForm::findNextSlot()
{
    wDockManager->getCentralTextEdit()->shortcutFindNext();
}


void MainForm::findPrevSlot()
{
    wDockManager->getCentralTextEdit()->shortcutFindPrevious();
}


void MainForm::replaceSlot()
{
    wDockManager->getCentralTextEdit()->shortcutReplace();
}


void MainForm::jmpToLineSlot()
{
    wDockManager->getCentralTextEdit()->shortcutJmpToLine();
}


void MainForm::commentSlot()
{
    wDockManager->getCentralTextEdit()->shortcutComment();
}


void MainForm::deleteCommentSlot()
{
    wDockManager->getCentralTextEdit()->shortcutDeleteComment();
}


void MainForm::jmpToBookmarkNextSlot()
{
    wDockManager->getCentralTextEdit()->shortcutJmpToBookmarkNext();
}


void MainForm::jmpToBookmarkPrevSlot()
{
    wDockManager->getCentralTextEdit()->shortcutJmpToBookmarkPrev();
}