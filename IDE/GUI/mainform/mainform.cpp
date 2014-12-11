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
#include <fstream>
#include "../../utilities/AsmTranslator/AsmTranslator.h"
#include "../../utilities/AsmTranslator/AsmTranslatorConfig.h"
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
    this->m_reloadDlg = NULL;
    this->reloadDlgChange = false;

    m_projectMan = new ProjectMan(this);
    connect(m_projectMan,
            SIGNAL(addDockWidget(Qt::DockWidgetArea, QDockWidget*)),
            this,
            SLOT(addDockWidgetSlot(Qt::DockWidgetArea, QDockWidget*))
           );
    connect(m_projectMan,
            SIGNAL(tabifyDockWidget(QDockWidget*, QDockWidget*)),
            this,
            SLOT(tabifyDockWidgetSlot(QDockWidget*, QDockWidget*))
           );
    connect(m_projectMan,
            SIGNAL(connectProject(Project*)),
            this,
            SLOT(connectProjectSlot(Project*))
           );
    connect(m_projectMan,
            SIGNAL(projectOpened()),
            this,
            SLOT(projectOpened())
           );
    QWidget *centralWidget = new QWidget(this);
    m_wDockManager = new WDockManager(this, centralWidget);
    this->setCentralWidget(centralWidget);
    centralWidget->show();
    //connect(this, SIGNAL(dockWidgetsCreated()), m_wDockManager, SLOT(dockWidgetsCreated()));
    /*connect(m_wDockManager,
            SIGNAL(createDockWidgets()),
            this,
            SLOT(createDockWidgets())
           );*/
    connect(m_wDockManager,
            SIGNAL(tabifyDockWidget(QDockWidget*, QDockWidget*)),
            this,
            SLOT(tabifyDockWidgetSlot(QDockWidget*, QDockWidget*))
           );
    connect(m_wDockManager,
            SIGNAL(addDockWidget(Qt::DockWidgetArea, QDockWidget*)),
            this,
            SLOT(addDockWidgetSlot(Qt::DockWidgetArea, QDockWidget*))
           );
    connect(m_wDockManager,
            SIGNAL(getSimProjectData()),
            this,
            SLOT(simProjectData())
           );
    connect(this,
            SIGNAL(unhighlightSim()),
            m_wDockManager,
            SLOT(unhighlightSimWidget())
           );
    connect(m_wDockManager,
            SIGNAL(stopSimSig()),
            this,
            SLOT(stopSimSlot())
           );
    connect(m_wDockManager,
            SIGNAL(clockChangedSig(double, int)),
            this,
            SLOT(clockChangedSlot(double, int))
           );
    connect(m_wDockManager,
            SIGNAL(centralCreated()),
            this,
            SLOT(enableSimActs())
           );
    connect(m_wDockManager,
            SIGNAL(centralClosed()),
            this,
            SLOT(disableSimActs())
           );
    connect(m_wDockManager,
            SIGNAL(breakpointEmit(QString, int)),
            this,
            SLOT(manageBreakpointEmit(QString, int))
           );
    connect(m_wDockManager,
            SIGNAL(breakpointsAddLines(QString, int, int)),
            this,
            SLOT(breakpointsAddLines(QString, int, int))
           );
    connect(m_wDockManager,
            SIGNAL(breakpointsRemoveLines(QString, int, int)),
            this,
            SLOT(breakpointsRemoveLines(QString, int, int))
           );
    connect(m_wDockManager,
            SIGNAL(bookmarkEmit(QString, int)),
            this,
            SLOT(manageBookmarkEmit(QString, int))
           );
    connect(m_wDockManager,
            SIGNAL(bookmarksAddLines(QString, int, int)),
            this,
            SLOT(bookmarksAddLines(QString, int, int))
           );
    connect(m_wDockManager,
            SIGNAL(bookmarksRemoveLines(QString, int, int)),
            this,
            SLOT(bookmarksRemoveLines(QString, int, int))
           );
    connect(m_wDockManager,
            SIGNAL(welcomeScrExampleSig()),
            this,
            SLOT(exampleOpen())
           );
    connect(m_wDockManager,
            SIGNAL(welcomeScrOpenPrjSig()),
            this,
            SLOT(openProject())
           );
    connect(m_wDockManager,
            SIGNAL(welcomeScrNewPrjSig()),
            this,
            SLOT(newProject())
           );
    connect(m_wDockManager,
            SIGNAL(welcomeScrManualSig()),
            this,
            SLOT(help())
           );
    connect(m_wDockManager,
            SIGNAL(welcomeScrGuideSig()),
            this,
            SLOT(userGuide())
           );
    connect(m_wDockManager,
            SIGNAL(welcomeScrRecentSig(QString)),
            this,
            SLOT(openProject(QString))
           );
    connect(m_wDockManager,
            SIGNAL(tabClosed(QString)),
            this,
            SLOT(fileClosed(QString))
           );
    /*connect(m_wDockManager,
            SIGNAL(breakpointListRemove(QString, int)),
            this,
            SLOT(manageBreakpointRemove(QString, int))
           );*/


    /*connect(this,
            SIGNAL(sessionRestorationSignal()),
            this,
            SLOT(sessionRestorationSlot())
           );*/

    connect(&m_fileWatcher,
            SIGNAL(fileChanged(QString)),
            this,
            SLOT(fileChanged(QString))
           );
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
        m_projectMan->getActive()->stop();
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
        if (m_projectMan->getOpenProjects().count() > 0)
        {
            bool fileFound;
            QList<Project*> projects = m_projectMan->getOpenProjects();
            //qDebug() << "Mainform: prepare to project session restoration";
            for (int i = 0; i < projects.count(); i++)
            {
                fileFound = false;
                //qDebug() << "Mainform: saving project" << projects.at(i)->prjName;
                if (m_wDockManager->getTabCount() > 0)
                {
                    //qDebug() << "Mainform: prepare to files session restoration";
                    for (int j = 0; j < m_wDockManager->getTabCount(); j++)
                    {
                        if (true == m_wDockManager->getTabWidget(j)->isChild(projects.at(i)))
                        {
                            if ("untracked" == m_wDockManager->getTabWidget(j)->getPath())
                            {
                                continue;
                            }
                            fileFound = true;
                            //qDebug() << "Mainform: saving file" << m_wDockManager->getTabWidget(j)->getName();
                            //qDebug() << "MainForm: with parent" << projects.at(i)->prjPath;
                            GuiCfg::getInstance().sessionAppendFile(m_wDockManager->getTabWidget(j)->getPath());
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

    while (m_projectMan->getOpenProjects().count() > 0)
    {
        m_projectMan->closeProject(m_projectMan->getActive());
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
    jmpToBookmarkNextAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_PageDown));
    jmpToBookmarkNextAct->setDisabled(true);
    jmpToBookmarkNextAct->setShortcutContext(Qt::ApplicationShortcut);
    connect(jmpToBookmarkNextAct, SIGNAL(triggered()), this, SLOT(jmpToBookmarkNextSlot()));
    jmpToBookmarkPrevAct = new QAction(tr("Jump to Previous Bookmark"), this);
    jmpToBookmarkPrevAct->setShortcut(QKeySequence(Qt::ALT + Qt::Key_PageUp));
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
    if (false == m_wDockManager->dockWidgets)
    {
        setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
        setCorner(Qt::BottomRightCorner, Qt::BottomDockWidgetArea);
        setTabPosition(Qt::RightDockWidgetArea, QTabWidget::East);

        //mozno stejne jako u WDockManager - ulozit si ptr na okno
        //m_projectMan->addProject(NULL, NULL, NULL);

        QList<QTabBar*> tabList;
        //m_wDockManager->addDockWidget(wListCode);
        //m_wDockManager->addDockWidget(wListCode2);
        /*
        tabList= this->findChildren<QTabBar*>();
        m_wDockManager->leftAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                m_wDockManager,
                SLOT(handleShowHideLeft(int))
               );
         */
        m_wDockManager->addDockWidget(wCompileInfo);
        m_wDockManager->addDockWidget(wSimulationInfo);
        m_wDockManager->addDockWidget(wBottomHide);
        /*#ifdef Q_OS_WIN
            Sleep(50);
        #else
            usleep(50000);
        #endif*/
        tabList= this->findChildren<QTabBar*>();
        m_wDockManager->bottomAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                m_wDockManager,
                SLOT(handleShowHideBottom(int))
               );

        m_wDockManager->addDockWidget(wBreakpointList);
        m_wDockManager->addDockWidget(wBookmarkList);
        m_wDockManager->addDockWidget(wAsmMacroAnalyser);
        AsmMacroAnalyser *analys = (AsmMacroAnalyser*)(m_wDockManager->getDockWidget(wAsmMacroAnalyser)->widget());
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
        m_wDockManager->addDockWidget(wRightHide);
        /*#ifdef Q_OS_WIN
            Sleep(50);
        #else
            usleep(50000);
        #endif*/
        tabList= this->findChildren<QTabBar*>();
        m_wDockManager->rightAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                m_wDockManager,
                SLOT(handleShowHideRight(int))
               );
        //m_wDockManager->addDockWidget(wAnalysVar);
        //m_wDockManager->addDockWidget(wAnalysFunc);
        //addAct->setEnabled(true);
        //projectCompileAct->setEnabled(true);
        //simulationFlowAct->setEnabled(true);
        m_wDockManager->dockWidgets = true;
        if (m_wDockManager->getBreakpointList() != NULL)
        {
            connect(m_wDockManager->getBreakpointList(),
                    SIGNAL(breakpointClicked(QString, int)),
                    this,
                    SLOT(scrollToFileLine(QString, int))
                   );
        }
        if (m_wDockManager->getBookmarkList() != NULL)
        {
            connect(m_wDockManager->getBookmarkList(),
                    SIGNAL(bookmarkClicked(QString, int)),
                    this,
                    SLOT(scrollToFileLine(QString, int))
                   );
        }

        m_wDockManager->hideDockWidgetArea(1);
        m_wDockManager->hideDockWidgetArea(2);
        QApplication::processEvents();
        reloadTabIcons();
    }
    else
    {
        m_wDockManager->addDockWidget(wSimulationInfo);
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
    m_wDockManager->setCentralWelcome();
}



/**
 * @brief Slot. Opens new blank file.
 */
void MainForm::newFile()
{
    //qDebug() << "MainForm: newFile()";
    //jen se vytvori novy tab na code editoru
    //prepta se dialogem, zda pridat do aktivniho projektu
    m_projectMan->addUntrackedFile(NULL, NULL);
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
        if (m_projectMan->getActive() != NULL && m_projectMan->getActive()->prjPath != "untracked")
        {
            path = QFileDialog::getSaveFileName(this, tr("New File"), QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
        m_wDockManager->addCentralWidget(path.section('/', -1), path);
        m_wDockManager->getCentralWidget()->setChanged();
        //m_wDockManager->getCentralWidget()->connectAct();

        //je sice prehlednejsi zavolat saveFile(), ale
        //vlozeni kodu pro ulozeni je rychlejsi a efektivnejsi
        //...ale necham volani saveFile()...
        saveFile();
        //pridani do projektu
        m_projectMan->addFile(path, path.section('/', -1));
        m_wDockManager->getCentralWidget()->setParentProject(m_projectMan->getActive());
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
    if (m_projectMan->getActive() != NULL && m_projectMan->getActive()->prjPath != "untracked")
    {
        path = QFileDialog::getOpenFileName(this, tr("Open File"), QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
            //m_wDockManager->addCentralWidget(path.section('/', -1), path);
            //m_wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            file.close();
            if (m_projectMan->getOpenProjects().count() > 0)
            {
                m_projectMan->getActive()->addFile(path, path.section('/', -1));
                m_wDockManager->addCentralWidget(path.section('/', -1), path);
                m_wDockManager->getCentralWidget()->setParentProject(m_projectMan->getActive());
                m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->setParentProject(m_projectMan->getActive());
            }
            else
            {
                //m_wDockManager->getCentralWidget()->connectAct();
                m_projectMan->addUntrackedFile(path, path.section('/', -1));
                m_wDockManager->addUntrackedCentralWidget(path.section('/', -1), path);
                m_wDockManager->getCentralWidget()->setParentProject(m_projectMan->getUntracked());
                m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->setParentProject(m_projectMan->getUntracked());
            }
            GuiCfg::getInstance().fileOpened(path);
            m_fileWatcher.addPath(path);
            QTimer::singleShot(100, this->m_wDockManager->getCentralWidget(), SLOT(changeHeight()));
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
    //QDir projectDir(QFileInfo(m_projectMan->activeProject->prjPath).dir());
    //QString absoluteFilePath = QFileInfo(m_projectMan->getActive()->prjPath).dir().path() + "/" + path;
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
            //m_wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            //qDebug() << "MainForm: connect";
            //qDebug() << "MainForm: set parent";
            if (parentProjectPath != "")
            {
                for (int i = 0; i < m_projectMan->getOpenProjects().count(); i++)
                {
                    if (m_projectMan->getOpenProjects().at(i)->prjPath == parentProjectPath)
                    {
                        m_wDockManager->addCentralWidget(path.section('/', -1), path);
                        CodeEdit *centralCodeEdit = m_wDockManager->getCentralWidget();
                        //centralCodeEdit->connectAct();
                        centralCodeEdit->setParentProject(m_projectMan->getOpenProjects().at(i));
                        m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->setParentProject(m_projectMan->getOpenProjects().at(i));
                        centralCodeEdit->setBreakpointsLines(m_projectMan->getActive()->getBreakpointsForFileAbsolute(centralCodeEdit->getPath()));
                        centralCodeEdit->setBookmarksLines(m_projectMan->getActive()->getBookmarksForFileAbsolute(centralCodeEdit->getPath()));
                        //m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->updateLineCounter();
                        break;
                    }
                }
                if (parentProjectPath == "untracked")
                {
                    m_projectMan->addUntrackedFile(path, path.section('/', -1));
                    m_wDockManager->addUntrackedCentralWidget(path.section('/', -1), path);
                    //m_wDockManager->getCentralWidget()->connectAct();
                    m_wDockManager->getCentralWidget()->setParentProject(m_projectMan->getUntracked());
                    m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->setParentProject(m_projectMan->getUntracked());
                    GuiCfg::getInstance().fileOpened(path);
                }
            }
            else
            {
                m_wDockManager->addCentralWidget(path.section('/', -1), path);
                CodeEdit *centralCodeEdit = m_wDockManager->getCentralWidget();
                //centralCodeEdit->connectAct();
                centralCodeEdit->setParentProject(m_projectMan->getActive());
                m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->setParentProject(m_projectMan->getActive());
                centralCodeEdit->setBreakpointsLines(m_projectMan->getActive()->getBreakpointsForFileAbsolute(centralCodeEdit->getPath()));
                centralCodeEdit->setBookmarksLines(m_projectMan->getActive()->getBookmarksForFileAbsolute(centralCodeEdit->getPath()));
                //m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->updateLineCounter();

            }
            //m_wDockManager->getCentralWidget()->setSaved();
            /*if (true == centralCodeEdit->isChanged())
            {
                qDebug() << "MainForm: openfilepath - some error here";
            }*/
            m_fileWatcher.addPath(path);
            QTimer::singleShot(100, this->m_wDockManager->getCentralWidget(), SLOT(changeHeight()));
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
    if (m_wDockManager->isEmpty() == false)
    {
        QString path;
        if (m_wDockManager->getCentralPath() == NULL)
        {
            if (m_projectMan->getActive() != NULL && m_projectMan->getActive()->prjPath != "untracked")
            {
                path = QFileDialog::getSaveFileName(this, tr("Save File"), QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
            }
            else
            {
                path = QFileDialog::getSaveFileName(this, tr("Save File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
            }
            m_wDockManager->setCentralPath(path);
            m_wDockManager->setCentralName(path.section('/', -1));
        }
        else
        {
            path = m_wDockManager->getCentralPath();
        }

        //je sice prehlednejsi zavolat saveFile(), ale
        //vlozeni kodu pro ulozeni je rychlejsi a efektivnejsi
        //...ale necham volani saveFile()...
        saveFile();
        //pridani do projektu

        m_projectMan->addFile(path, path.section('/', -1));
        m_wDockManager->getCentralWidget()->setParentProject(m_projectMan->getActive());
    }
    //qDebug() << "MainForm: return addFile()";
}

/**
 * @brief Slot. Saves the active file.
 */
void MainForm::saveFile()
{
    //qDebug() << "MainForm: saveFile()";
    if (m_wDockManager->getCentralWidget()->isChanged() == true || m_wDockManager->getCentralPath() == "untracked")
    {
        /*if (m_wDockManager->getCentralWidget()->isChanged() == true )
        {
            qDebug() << "MainForm: central is changed";
        }
        qDebug() << "Mainform: saving file";*/
        QString path;
        if (m_wDockManager->getCentralPath() == NULL || m_wDockManager->getCentralPath() == "untracked")
        {
            //path = QFileDialog::getSaveFileName(this, tr("Source File");
            bool done = false;
            while (false == done)
            {
                if (m_projectMan->getActive() != NULL && m_projectMan->getActive()->prjPath != "untracked")
                {
                    path = QFileDialog::getSaveFileName (this, tr("Save File"), QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
                m_wDockManager->setCentralPath(path);
                m_wDockManager->setCentralName(path.section('/', -1));
            }
        }
        else
        {
            path = m_wDockManager->getCentralPath();
        }

        if (path != NULL)
        {
            //qDebug() << "mainform: block signal true";
            //QApplication::processEvents();
            m_fileWatcher.blockSignals(true);
            QFile file(path);
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                error(ERR_OPENFILE, path);
            }
            else
            {
                QTextStream fout(&file);
                fout << m_wDockManager->getCentralTextEdit()->toPlainText();
                file.close();
                m_wDockManager->setTabSaved();
                m_wDockManager->getCentralWidget()->setSaved();
            }
            QApplication::processEvents();
            m_fileWatcher.blockSignals(false);
            //qDebug() << "mainform: block signal false";
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
        if (m_projectMan->getActive() != NULL && m_projectMan->getActive()->prjPath != "untracked")
        {
            path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
        m_fileWatcher.blockSignals(true);
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE, path);
        }
        else
        {
            QTextStream fout(&file);
            fout << m_wDockManager->getCentralTextEdit()->toPlainText();
            file.close();
            m_wDockManager->getCentralWidget()->getParentProject()->renameFile(m_wDockManager->getCentralPath(), path);
            m_wDockManager->setCentralPath(path);
            m_wDockManager->setCentralName(path.section('/', -1));
            m_wDockManager->getCentralWidget()->setSaved();
        }
        m_fileWatcher.blockSignals(false);
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
                if (m_projectMan->getActive() != NULL && m_projectMan->getActive()->prjPath != "untracked")
                {
                    path = QFileDialog::getSaveFileName(this, tr("Save File"), QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
            m_fileWatcher.blockSignals(true);
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
            m_fileWatcher.blockSignals(false);
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
    for (int i = 0; i < m_wDockManager->getTabCount(); i++)
    {
        if (m_wDockManager->getTabStatusChanged(i) == true
            || m_wDockManager->getTabWidget(i)->getPath() == "untracked")
        {
            //qDebug() << "MainForm: saveFile codeedit";
            saveFile(m_wDockManager->getTabWidget(i));
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
    ProjectDialog *projectEdit = new ProjectDialog(this, m_projectMan);
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

    if (path.isEmpty() == false && m_projectMan->isOpened(path) == false)
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
            m_projectMan->openProject(&file);
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
    if (false == m_projectMan->isOpened(path))
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
            m_projectMan->openProject(&file);
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
    m_wDockManager->deleteCentralWelcome();
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
    if (m_wDockManager->getBreakpointList() != NULL)
    {
        m_wDockManager->getBreakpointList()->reload(m_projectMan->getActive()->getBreakpointsListRef());
        /*if (m_wDockManager->getCentralWidget() != NULL)
        {
            m_wDockManager->getCentralWidget()->setBreakpointsLines(m_projectMan->getActive()->getBreakpointsForFileAbsolute(m_wDockManager->getCentralWidget()->getPath()));
        }*/
    }
    if (m_wDockManager->getBookmarkList() != NULL)
    {
        m_wDockManager->getBookmarkList()->reload(m_projectMan->getActive()->getBookmarksListRef());
        /*if (m_wDockManager->getCentralWidget() != NULL)
        {
            m_wDockManager->getCentralWidget()->setBookmarksLines(m_projectMan->getActive()->getBookmarksForFileAbsolute(m_wDockManager->getCentralWidget()->getPath()));
        }*/
    }
    if (projectTabs != NULL )
    {
        //qDebug() << "count: " << projectTabs->count();
        //projectTabs->setCurrentIndex(projectTabs->count()-1);
        QTimer::singleShot(50, this, SLOT(refreshProjectTree()));
    }

    if (m_projectMan->getActive()->prjPath != "untracked")
    {
        GuiCfg::getInstance().projectOpened(m_projectMan->getActive()->prjPath);
    }
    //qDebug() << "MainForm: projectOpened done";
}


/**
 * @brief Slot. Saves all changed files in active project.
 */
void MainForm::saveProject()
{
    //qDebug() << "MainForm: saveProject()";
    for (int i = 0; i < m_wDockManager->getTabCount(); i++)
    {
        if (m_wDockManager->getTabWidget(i)->isChild(m_projectMan->getActive()) == true
            && true == m_wDockManager->getTabWidget(i)->isChanged())
        {
            saveFile(m_wDockManager->getTabWidget(i));
        }
    }
    //qDebug() << "MainForm: return saveProject()";
}



QString MainForm::translateBeforeCompilation(QString path)
{
    //qDebug() << "MainForm: translating";
    m_compatibilityMode.clearFileMaps();
    QList<QString> filesToTranslate;
    filesToTranslate.append(path);
    
    QString newPath = path.section('.', 0, -2) + ".asm";
    QString sourceDir = path.section('/', 0, -2) + "/";
    
    AsmTranslator translator;
    translator.m_config.m_letterCase[0] = (AsmTranslatorConfig::LetterCase) 0;
    translator.m_config.m_letterCase[1] = (AsmTranslatorConfig::LetterCase) 0;
    translator.m_config.m_letterCase[2] = (AsmTranslatorConfig::LetterCase) 0;
    translator.m_config.m_eol = (AsmTranslatorConfig::EndOfLine) 0;
    translator.m_config.m_indentation = (AsmTranslatorConfig::Indentation) 0;
    translator.m_config.m_tabSize = 4;
    translator.m_config.m_shortInstructions = true;

    for (int i = 0; i < filesToTranslate.count(); i++)
    {
        //qDebug() << "MainForm: translating file" << filesToTranslate.at(i);
        if ("psm" != filesToTranslate.at(i).section('.',-1).toLower())
        {
            return "";
        }
        
        std::ifstream inputStream(filesToTranslate.at(i).toStdString());
        if (false == inputStream.is_open())
        {
            return "";
        }
        std::ofstream outputStream((filesToTranslate.at(i).section('.',0, -2) + ".asm").toStdString(), std::ofstream::out);
        if (false == outputStream.is_open())
        {
            inputStream.close();
            return "";
        }

        bool finalResult = translator.translate(AsmTranslator::Variant::V_KCPSM_XILINX,
                                                outputStream,
                                                inputStream);
        if ( true == finalResult )
        {
            m_compatibilityMode.appendFileMap(filesToTranslate.at(i).section('.',0, -2) + ".asm", translator.getLineMap());
            std::vector<std::string> includedFiles = translator.getIncludedFiles();
            for (unsigned int j = 0; j < includedFiles.size(); j++)
            {
                if (false == filesToTranslate.contains(QString::fromStdString(includedFiles.at(j))))
                {
                    filesToTranslate.append(sourceDir + QString::fromStdString(includedFiles.at(j)));
                }
            }
        }
        else
        {
            inputStream.close();
            outputStream.close();
            return "";
        }

        inputStream.close();
        outputStream.close();
    }

    return newPath;
}


/**
 * @brief Slot. Compiles active project.
 */
void MainForm::compileProject()
{

    if (m_projectMan->getActive() == NULL)
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

    ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()))->clear();
    this->options = new CompilerOptions();
    QString mainFile;

    if (m_projectMan->getActive()->prjPath == "untracked" && m_projectMan->getActive()->prjName == "untracked")
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

        if (m_wDockManager->getCentralWidget() == NULL)
        {
            //qDebug() << "MainForm: returned";
            return;
        }

        this->saveFile();
        m_projectMan->getUntracked()->addFile(m_wDockManager->getCentralPath(),m_wDockManager->getCentralName());

        if (0 != m_projectMan->getActive()->getAsmType())
        {
            QString newPath = translateBeforeCompilation(m_wDockManager->getCentralPath());
            if ("" == newPath)
            {
                return;
            }
            options->m_sourceFiles.push_back(newPath.toLocal8Bit().constData());
        }
        else
        {
            options->m_sourceFiles.push_back(m_wDockManager->getCentralPath().toLocal8Bit().constData());
        }
        //options->m_sourceFiles.push_back(m_wDockManager->getCentralPath().toStdString());

        options->m_device = m_projectMan->getActive()->family.toStdString();


        QDir templateDir(GuiCfg::getInstance().getTemplatePath());
        if (true == m_projectMan->getActive()->defaultVerilog)
        {
            options->m_verilogTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + m_projectMan->getActive()->family
                                        + ".v"
                                        ).toLocal8Bit().constData();
        }
        else
        {
            options->m_verilogTemplate = m_projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
        }

        if (true == m_projectMan->getActive()->defaultVHDL)
        {
            options->m_vhdlTemplate = ( templateDir.absolutePath()
                                    + "/"
                                    + m_projectMan->getActive()->family
                                    + ".vhd"
                                    ).toLocal8Bit().constData();
        }
        else
        {
            options->m_vhdlTemplate = m_projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
        }


        QDir pathDir(GuiCfg::getInstance().getTempPath());
        if (false == pathDir.exists())
        {
            pathDir.mkpath(".");
        }

        if ("" == m_wDockManager->getCentralName().section('.',0,-2))
        {
            mainFile = pathDir.absolutePath()
                    + "/"
                    +  m_wDockManager->getCentralName();
        }
        else
        {
            mainFile = pathDir.absolutePath()
                    + "/"
                    +  m_wDockManager->getCentralName().section('.',0,-2);
        }
        //qDebug() << "MainForm: file" << mainFile;


        CompileInfo *compileInfo = ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()));
        compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Compilation settings:",
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Project:      untracked",
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("File:         " + m_wDockManager->getCentralName(),
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Architecture: Picoblaze",
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Family:       " + m_projectMan->getUntracked()->family,
                                    CompilerBase::MessageType::MT_REMARK);
        if (0 == m_projectMan->getActive()->getAsmType())
        {
            compileInfo->appendMessage("Asm syntax:   MDS assembler" ,
                                        CompilerBase::MessageType::MT_REMARK);
        }
        else
        {
            compileInfo->appendMessage("Asm syntax:   Xilinx assembler" ,
                                        CompilerBase::MessageType::MT_REMARK);
        }


        if (m_projectMan->getActive()->compileOpt.at(0))
        {
            //options->m_symbolTable = (mainFile + ".stbl").toStdString();
            options->m_symbolTable = ( mainFile
                                     + ".stbl"
                                     ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(1))
        {
            options->m_macroTable = ( mainFile
                                    + ".mtbl"
                                    ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(2))
        {
            options->m_mdsDebugFile = ( mainFile
                                      + ".dbg"
                                      ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(3))
        {
            options->m_codeTree = ( mainFile
                                  + ".ctr"
                                  ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(4))
        {
            options->m_lstFile = ( mainFile
                                  + ".lst"
                                 ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(5))
        {
            options->m_hexFile = ( mainFile
                                 + ".ihex"
                                 ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(6))
        {
            options->m_binFile = ( mainFile
                                 + ".bin"
                                 ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(7))
        {
            options->m_srecFile = ( mainFile
                                  + ".srec"
                                  ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(8))
        {
            options->m_memFile = ( mainFile
                                 + ".mem"
                                 ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(9))
        {
            options->m_rawHexDumpFile = ( mainFile
                                        + ".rawhex"
                                        ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(10))
        {
            options->m_verilogFile = ( mainFile
                                     + ".v"
                                     ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(11))
        {
            options->m_vhdlFile = ( mainFile
                                  + ".vhd"
                                  ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(12))
        {
            options->m_stringTable = ( mainFile
                                  + ".strtbl"
                                  ).toLocal8Bit().constData();
        }
        //return;
    }
    else if ( false == m_projectMan->getActive()->useMainFile )
    {
        //not error
        //check if enabled, if it isnt, compile actual file
        //(but find whether it is in act project for compile settings)
        //error(ERR_NO_MAINFILE);
        if (m_wDockManager->getCentralWidget() == NULL)
        {
            return;
        }

        this->saveFile();

        QDir prjDir(m_projectMan->getActive()->prjPath.section('/',0, -2));
        QDir fileDir;
        QString filePath;
        bool found = false;

        for (int i = 0; i < m_projectMan->getActive()->filePaths.count(); i++)
        {
            //fileDir.setPath(QDir::cleanPath(prjDir.absolutePath() + "/" + m_projectMan->getActive()->filePaths.at(i)));
            filePath = QDir::cleanPath(prjDir.absolutePath() + "/" + m_projectMan->getActive()->filePaths.at(i));
            //qDebug() << prjDir.relativeFilePath(m_projectMan->getActive()->filePaths.at(i));
            //qDebug() << m_projectMan->getActive()->filePaths.at(i);
            //qDebug() << "fixed?" << prjDir.absolutePath() + "/" + m_projectMan->getActive()->filePaths.at(i);
            //qDebug() << "MainForm: file dir" << filePath;
            if (filePath == m_wDockManager->getCentralPath())
            {
                found = true;
                break;
            }
        }
        if (found == true)
        {
            //qDebug() << "MainForm: compiled actual project, actual file" << filePath;
            if (0 != m_projectMan->getActive()->getAsmType())
            {
                QString newPath = translateBeforeCompilation(filePath);
                if ("" == newPath)
                {
                    return;
                }
                options->m_sourceFiles.push_back(newPath.toLocal8Bit().constData());
            }
            else
            {
                options->m_sourceFiles.push_back(filePath.toLocal8Bit().constData());
            }


            CompileInfo *compileInfo = ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()));
            compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                        CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Compilation settings:",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Project:      " + m_projectMan->getActive()->prjName,
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("File:         " + m_wDockManager->getCentralName(),
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Architecture: Picoblaze",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Family:       " + m_projectMan->getActive()->family + "\n\n",
                                    CompilerBase::MessageType::MT_REMARK);
            if (0 == m_projectMan->getActive()->getAsmType())
            {
                compileInfo->appendMessage("Asm syntax:   MDS assembler" ,
                                            CompilerBase::MessageType::MT_REMARK);
            }
            else
            {
                compileInfo->appendMessage("Asm syntax:   Xilinx assembler" ,
                                            CompilerBase::MessageType::MT_REMARK);
            }


        if ("" == m_wDockManager->getCentralName().section('.',0,-2))
        {
            mainFile = prjDir.absolutePath() + "/" + m_wDockManager->getCentralName();
        }
        else
        {
            mainFile = prjDir.absolutePath() + "/" + m_wDockManager->getCentralName().section('.',0,-2);
        }
            //qDebug() << mainFile;

            options->m_device = m_projectMan->getActive()->family.toStdString();


            QDir templateDir(GuiCfg::getInstance().getTemplatePath());
            if (true == m_projectMan->getActive()->defaultVerilog)
            {
                options->m_verilogTemplate = ( templateDir.absolutePath()
                                            + "/"
                                            + m_projectMan->getActive()->family
                                            + ".v"
                                            ).toLocal8Bit().constData();
            }
            else
            {
                options->m_verilogTemplate = m_projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
            }

            if (true == m_projectMan->getActive()->defaultVHDL)
            {
                options->m_vhdlTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + m_projectMan->getActive()->family
                                        + ".vhd"
                                        ).toLocal8Bit().constData();
            }
            else
            {
                options->m_vhdlTemplate = m_projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
            }

            if (m_projectMan->getActive()->compileOpt.at(0))
            {
                options->m_symbolTable = (mainFile + ".stbl").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(1))
            {
                options->m_macroTable = (mainFile + ".mtbl").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(2))
            {
                options->m_mdsDebugFile = (mainFile + ".dbg").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(3))
            {
                options->m_codeTree = (mainFile + ".ctr").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(4))
            {
                options->m_lstFile = (mainFile + ".lst").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(5))
            {
                options->m_hexFile = (mainFile + ".ihex").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(6))
            {
                options->m_binFile = (mainFile + ".bin").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(7))
            {
                options->m_srecFile = (mainFile + ".srec").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(8))
            {
                options->m_memFile = ( mainFile + ".mem").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(9))
            {
                options->m_rawHexDumpFile = ( mainFile + ".rawhex" ).toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(10))
            {
                options->m_verilogFile = ( mainFile  + ".v").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(11))
            {
                options->m_vhdlFile = ( mainFile + ".vhd").toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(12))
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
            if (m_projectMan->getUntracked() != NULL)
            {
                m_projectMan->getUntracked()->addFile(m_wDockManager->getCentralPath(),m_wDockManager->getCentralName());
                m_projectMan->setActive(m_projectMan->getUntracked());
                if (projectTabs != NULL )
                {
                    QList<Project*> projects = m_projectMan->getOpenProjects();
                    int index = 0;
                    for (;index < projects.count(); index++)
                    {
                        if (projects.at(index) == m_projectMan->getUntracked())
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
                m_projectMan->addUntrackedProject();
                m_projectMan->getUntracked()->addFile(m_wDockManager->getCentralPath(),m_wDockManager->getCentralName());
                m_projectMan->setActive(m_projectMan->getUntracked());
                QTimer::singleShot(50, this, SLOT(refreshProjectTree()));
            }
            if (0 != m_projectMan->getActive()->getAsmType())
            {
                QString newPath = translateBeforeCompilation(m_wDockManager->getCentralPath());
                if ("" == newPath)
                {
                    return;
                }
                options->m_sourceFiles.push_back(newPath.toLocal8Bit().constData());
            }
            else
            {
                options->m_sourceFiles.push_back(m_wDockManager->getCentralPath().toLocal8Bit().constData());
            }

            options->m_device = m_projectMan->getActive()->family.toStdString();


            QDir templateDir(GuiCfg::getInstance().getTemplatePath());
            if (true == m_projectMan->getActive()->defaultVerilog)
            {
                options->m_verilogTemplate = ( templateDir.absolutePath()
                                             + "/"
                                             + m_projectMan->getActive()->family
                                             + ".v"
                                             ).toLocal8Bit().constData();
            }
            else
            {
                options->m_verilogTemplate = m_projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
            }

            if (true == m_projectMan->getActive()->defaultVHDL)
            {
                options->m_vhdlTemplate = ( templateDir.absolutePath()
                                          + "/"
                                          + m_projectMan->getActive()->family
                                          + ".vhd"
                                          ).toLocal8Bit().constData();
            }
            else
            {
                options->m_vhdlTemplate = m_projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
            }


            CompileInfo *compileInfo = ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()));
            compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                        CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Compilation settings:",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Project:      untracked",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("File:         " + m_wDockManager->getCentralName(),
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Architecture: Picoblaze",
                                    CompilerBase::MessageType::MT_REMARK);
            compileInfo->appendMessage("Family:       " + m_projectMan->getUntracked()->family + "\n\n",
                                    CompilerBase::MessageType::MT_REMARK);
            if (0 == m_projectMan->getActive()->getAsmType())
            {
                compileInfo->appendMessage("Asm syntax:   MDS assembler" ,
                                            CompilerBase::MessageType::MT_REMARK);
            }
            else
            {
                compileInfo->appendMessage("Asm syntax:   Xilinx assembler" ,
                                            CompilerBase::MessageType::MT_REMARK);
            }

            QDir pathDir(GuiCfg::getInstance().getTempPath());
            if (false == pathDir.exists())
            {
                pathDir.mkpath(".");
            }

            QDir prjDir;
            if ("" == m_wDockManager->getCentralName().section('.',0,-2))
            {
                mainFile = pathDir.absolutePath()
                        + "/"
                        +  m_wDockManager->getCentralName();
            }
            else
            {
                    mainFile = pathDir.absolutePath()
                        + "/"
                        +  m_wDockManager->getCentralName().section('.',0,-2);
            }

            //qDebug() << "MainForm: tempPath" << GuiCfg::getInstance().getTempPath();
            //qDebug() << "MainForm: mainfile" << mainFile;
            //qDebug() << "MainForm: section" << m_wDockManager->getCentralPath().section('/',-1);
            //qDebug() << "MainForm: section2" << m_wDockManager->getCentralPath().section('/',-1).section('.',0,0);


            if (m_projectMan->getUntracked()->compileOpt.at(0))
            {
                //options->m_symbolTable = (mainFile + ".stbl").toStdString();
                options->m_symbolTable = ( mainFile
                                         + ".stbl"
                                         ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(1))
            {
                options->m_macroTable = ( mainFile
                                        + ".mtbl"
                                        ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(2))
            {
                options->m_mdsDebugFile = ( mainFile
                                        + ".dbg"
                                        ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(3))
            {
                options->m_codeTree = ( mainFile
                                      + ".ctr"
                                      ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(4))
            {
                options->m_lstFile = ( mainFile
                                     + ".lst"
                                     ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(5))
            {
                options->m_hexFile = ( mainFile
                                     + ".ihex"
                                     ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(6))
            {
                options->m_binFile = ( mainFile
                                     + ".bin"
                                     ).toLocal8Bit().constData();
            }
            if (m_projectMan->getUntracked()->compileOpt.at(7))
            {
                options->m_srecFile = ( mainFile
                                      + ".srec"
                                      ).toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(8))
            {
                options->m_memFile = ( mainFile
                                    + ".mem"
                                    ).toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(9))
            {
                options->m_rawHexDumpFile = ( mainFile
                                            + ".rawhex"
                                            ).toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(10))
            {
                options->m_verilogFile = ( mainFile
                                        + ".v"
                                        ).toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(11))
            {
                options->m_vhdlFile = ( mainFile
                                    + ".vhd"
                                    ).toLocal8Bit().constData();
            }
            if (m_projectMan->getActive()->compileOpt.at(12))
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
        if (m_projectMan->getActive()->mainFileName == "" && m_projectMan->getActive()->mainFilePath == "")
        {
            error(ERR_NO_MAINFILE);
            return;
        }

        this->saveProject();

        CompileInfo *compileInfo = ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()));
        compileInfo->appendMessage("Compilation started at: " + QDateTime::currentDateTime().toString(),
                                    CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Compilation settings:",
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Project:      " + m_projectMan->getActive()->prjName,
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("File:         " + m_projectMan->getActive()->mainFileName,
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Architecture: Picoblaze",
                                   CompilerBase::MessageType::MT_REMARK);
        compileInfo->appendMessage("Family:       " + m_projectMan->getActive()->family + "\n\n",
                                   CompilerBase::MessageType::MT_REMARK);
        if (0 == m_projectMan->getActive()->getAsmType())
        {
            compileInfo->appendMessage("Asm syntax:   MDS assembler" ,
                                        CompilerBase::MessageType::MT_REMARK);
        }
        else
        {
            compileInfo->appendMessage("Asm syntax:   Xilinx assembler" ,
                                        CompilerBase::MessageType::MT_REMARK);
        }

        if (0 != m_projectMan->getActive()->getAsmType())
        {
            QString newPath = translateBeforeCompilation(m_projectMan->getActive()->prjPath.section('/',0, -2)
                                                         + "/"  +  m_projectMan->getActive()->mainFilePath);
            if ("" == newPath)
            {
                return;
            }
            options->m_sourceFiles.push_back(newPath.toLocal8Bit().constData());
        }
        else
        {
            options->m_sourceFiles.push_back((m_projectMan->getActive()->prjPath.section('/',0, -2)
                                             + "/"
                                             +  m_projectMan->getActive()->mainFilePath
                                             ).toLocal8Bit().constData()
                                            );
        }
        QDir prjDir(m_projectMan->getActive()->prjPath.section('/',0, -2));
        QDir fileDir(QString::fromStdString(options->m_sourceFiles.at(0)).section('/',0, -2));
        if (fileDir.relativeFilePath(prjDir.absolutePath()) == "")
        {
            if ("" == m_projectMan->getActive()->mainFileName.section('.',0,-2))
            {
                mainFile = "./" +  m_projectMan->getActive()->mainFileName.section('.',0,-2);
            }
            else
            {
                mainFile = "./" +  m_projectMan->getActive()->mainFileName;
            }
        }
        else
        {
            if ("" == m_projectMan->getActive()->mainFilePath.section('.',0,-2))
            {
                mainFile = fileDir.relativeFilePath(prjDir.absolutePath())
                        + "/"
                        +  m_projectMan->getActive()->mainFilePath;
            }
            else
            {
                mainFile = fileDir.relativeFilePath(prjDir.absolutePath())
                        + "/"
                        +  m_projectMan->getActive()->mainFilePath.section('.',0,-2);
            }
        }

        options->m_device = m_projectMan->getActive()->family.toStdString();


        QDir templateDir(GuiCfg::getInstance().getTemplatePath());
        if (true == m_projectMan->getActive()->defaultVerilog)
        {
            options->m_verilogTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + m_projectMan->getActive()->family
                                        + ".v"
                                        ).toLocal8Bit().constData();
        }
        else
        {
            options->m_verilogTemplate = m_projectMan->getActive()->templateVerilog.toLocal8Bit().constData();
        }

        if (true == m_projectMan->getActive()->defaultVHDL)
        {
            options->m_vhdlTemplate = ( templateDir.absolutePath()
                                    + "/"
                                    + m_projectMan->getActive()->family
                                    + ".vhd"
                                    ).toLocal8Bit().constData();
        }
        else
        {
            options->m_vhdlTemplate = m_projectMan->getActive()->templateVHDL.toLocal8Bit().constData();
        }


        if (m_projectMan->getActive()->compileOpt.at(0))
        {
            options->m_symbolTable = (mainFile + ".stbl").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(1))
        {
            options->m_macroTable = (mainFile + ".mtbl").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(2))
        {
            options->m_mdsDebugFile = (mainFile + ".dbg").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(3))
        {
            options->m_codeTree = (mainFile + ".ctr").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(4))
        {
            options->m_lstFile = (mainFile + ".lst").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(5))
        {
            options->m_hexFile = (mainFile + ".ihex").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(6))
        {
            options->m_binFile = (mainFile + ".bin").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(7))
        {
            options->m_srecFile = (mainFile + ".srec").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(8))
        {
            options->m_memFile = ( mainFile + ".mem").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(9))
        {
            options->m_rawHexDumpFile = ( mainFile + ".rawhex" ).toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(10))
        {
            options->m_verilogFile = ( mainFile  + ".v").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(11))
        {
            options->m_vhdlFile = ( mainFile + ".vhd").toLocal8Bit().constData();
        }
        if (m_projectMan->getActive()->compileOpt.at(12))
        {
            options->m_stringTable = ( mainFile + ".strtbl").toLocal8Bit().constData();
        }
    }


    //QString mainFile =  m_projectMan->getActive()->mainFileName.section('.',0,-2);

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
    m_wDockManager->setBottomAreaToCompilerInfo();
    if ( true == success )
    {
        ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()))->setFinished(true);
        if (true == this->simulationRequest)
        {
            this->simulationFlowHandle();
            this->simulationRequest = false;
        }
    }
    else
    {
        ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()))->setFinished(false);
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
    ((CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()))->appendMessage(QString::fromStdString(text), type);
}


/*
 * @brief Write text to wCompileInfo dock widget, used in compilation
 * @param text String value that will be written to widget
 */
//void MainForm::writeToWCompileInfo(QString text, , CompilerBase::MessageType type)
//{
    //((QPlainTextEdit*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()))->appendPlainText(text);
//    (CompileInfo*)(m_wDockManager->getDockWidget(wCompileInfo)->widget()))->appendMessage(text);
//}


/**
 * @brief Returns pointer to dock widgets manager.
 * @return Returns pointer to dock widgets manager
 */
WDockManager* MainForm::getWDockManager()
{
    return m_wDockManager;
}




/**
 * @brief Slot. Makes step in active simulation.
 */
void MainForm::simulationStep()
{
    if (true == simulationStatus)
    {
        m_projectMan->getSimulated()->step();
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
        m_projectMan->getSimulated()->run();
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
        m_projectMan->getSimulated()->animate();
    }
}



/**
 * @brief Slot. Resets active simulation.
 */
void MainForm::simulationReset()
{
    if (true == simulationStatus)
    {
        m_projectMan->getSimulated()->reset();
        //this->m_wDockManager->getCentralTextEdit()->clearHighlight();
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
        if (m_projectMan->getActive() == NULL)
        {
            error(ERR_NO_PROJECT);
        }
        if (0 != m_projectMan->getActive()->getAsmType() && false == m_compatibilityMode.loaded())
        {
            simulationRequest = true;
            compileProject();
            return;
        }
        if (m_projectMan->getActive()->prjPath == "untracked")
        {
            this->saveFile();
            file = m_wDockManager->getCentralPath();
            dumpFiles = GuiCfg::getInstance().getTempPath() + "/" + m_wDockManager->getCentralName();
        }
        else
        {
            if ( false == m_projectMan->getActive()->useMainFile )
            {
                //check if enabled, if it isnt, simulate current file
                this->saveFile();

                QDir prjDir(m_projectMan->getActive()->prjPath.section('/',0, -2));
                QDir fileDir;
                QString filePath;

                bool found = false;

                for (int i = 0; i < m_projectMan->getActive()->filePaths.count(); i++)
                {
                    filePath = QDir::cleanPath(prjDir.absolutePath() + "/" + m_projectMan->getActive()->filePaths.at(i));
                    /*fileDir.setPath(prjDir.absolutePath()
                                    + "/"
                                    + m_projectMan->getActive()->filePaths.at(i).section('/',0, -2)
                                );*/
                    //qDebug() << "MainForm: central path:" << m_wDockManager->getCentralPath();
                    //qDebug() << "MainForm: file path" << QDir::cleanPath(fileDir.absolutePath() + "/" + m_projectMan->getActive()->fileNames.at(i));
                    //qDebug() << filePath;
                    //qDebug() << m_wDockManager->getCentralPath();
                    if (filePath == m_wDockManager->getCentralPath())
                    {
                        found = true;
                        break;
                    }
                }
                if (true == found)
                {
                    file = filePath;
                    dumpFiles = prjDir.absolutePath() + "/" + m_wDockManager->getCentralName();
                }
                else
                {
                    if (m_projectMan->getUntracked() != NULL)
                    {
                        m_projectMan->addUntrackedProject();
                        m_projectMan->setActive(m_projectMan->getUntracked());
                    }
                    file = filePath;
                    dumpFiles = GuiCfg::getInstance().getTempPath() + "/" + m_wDockManager->getCentralName();
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
        m_projectMan->setSimulated(m_projectMan->getActive());
        int start = m_projectMan->getActive()->start(file, dumpFiles);
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
                m_projectMan->getSimulated()->setBreakpoints(true);
            }
            else
            {
                //qDebug() << "MainForm: simulationBreakpointsEnabled false";
                m_projectMan->getSimulated()->setBreakpoints(false);
            }
            m_wDockManager->setBottomAreaToSimulationInfo();
        }
        else
        {
            m_projectMan->setSimulated(NULL);
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
        m_projectMan->getSimulated()->stop();
        //this->unhighlight();
        //this->m_wDockManager->getCentralTextEdit()->clearHighlight();
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
    return m_projectMan;
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
    int count = m_projectMan->getActive()->filePaths.count();
    QDir projectDir = QFileInfo(m_projectMan->getActive()->prjPath).dir();
    QString absolutePath = projectDir.path();
    for (int i = 0; i < count; i++)
    {
        //qDebug() << "MainForm: loading";
        this->openFilePath(QDir(absolutePath + "/" + m_projectMan->getActive()->filePaths.at(i)).canonicalPath());
    }
    if (m_wDockManager->getTabCount() > 0)
    {
        m_wDockManager->setCentralByIndex(0);
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
    m_wDockManager->addSimDockWidgetP2(m_projectMan->getActive()->prjPath, m_projectMan->getActive()->getSimControl());
    m_wDockManager->openSimWidgets.at(m_wDockManager->openSimWidgets.count()-1)->setClock(m_projectMan->getActive()->clock,
                                                                                      m_projectMan->getActive()->clockMult
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
        m_wDockManager->setCentralByName(file.section('/', -1));
        m_wDockManager->getCentralTextEdit()->highlightLine(line, color);
        //m_wDockManager->getCentralWidget()->setSaved();
        //if (m_wDockManager->getCentralTextEdit()->highlightLine(line, color) == true)
        //{
        //    m_wDockManager->getCentralTextEdit()->scrollToLine(line, false);
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
    m_wDockManager->setCentralByName(file);
}


/**
 * @brief
 * @param
 * @details currently unused
 */
void MainForm::scrollCentralToLine(int line)
{
    m_wDockManager->getCentralTextEdit()->scrollToLine(line);
}


/**
 * @brief
 * @param
 */
void MainForm::setEditorReadOnly(bool readOnly)
{
    //qDebug() << "MainForm: setEditorReadOnly";
    m_wDockManager->setEditorsReadOnly(readOnly);
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
        m_wDockManager->addUntrackedCentralWidget(name, path);
        m_wDockManager->getCentralWidget()->setParentProject(m_projectMan->getUntracked());
        m_wDockManager->getTabWidget(m_wDockManager->getTabCount() - 1)->setParentProject(m_projectMan->getUntracked());
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

        //QString name = m_projectMan->addUntrackedFile(NULL, "disasm");
        this->m_wDockManager->addUntrackedCentralWidget("disasm","untracked",qText);
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
        //QString name = m_projectMan->addUntrackedFile(NULL, "disasm");
        this->m_wDockManager->addUntrackedCentralWidget("Assembler Translator","untracked",qText);
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
        //QString name = m_projectMan->addUntrackedFile(NULL, "disasm");
        this->m_wDockManager->addUntrackedCentralWidget("Assembler Translator error","untracked",qText);
        //qDebug() << getWDockManager()->getCentralTextEdit()->toPlainText();
        //m_wDockManager->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
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
        this->startProjectConfig(m_projectMan->getActive());
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
        m_projectMan->setActiveByIndex(index);
        m_wDockManager->changeSimWidget(index);
        m_wDockManager->showProjectEditors(m_projectMan->getActive());
        if (m_wDockManager->getBreakpointList() != NULL)
        {
            m_wDockManager->getBreakpointList()->reload(m_projectMan->getActive()->getBreakpointsListRef());
        }
        if (m_wDockManager->getBookmarkList() != NULL)
        {
            m_wDockManager->getBookmarkList()->reload(m_projectMan->getActive()->getBookmarksListRef());
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
    m_projectMan->getActive()->setupSim(family);
}


void MainForm::closeProject()
{
    if (false == this->simulationStatus || m_projectMan->getActive() != m_projectMan->getSimulated())
    {
        Project *project = m_projectMan->getActive();
        QDir path;
        for (int i = 0; i < project->filePaths.count(); i++)
        {
            path.setPath(project->prjPath.section('/',0,-2));
            m_wDockManager->closeFile(QDir::cleanPath(path.absoluteFilePath(project->filePaths.at(i))));
        }
        //qDebug() << "MainForm: delete active sim widget";
        m_wDockManager->deleteActiveSimWidget();
        //qDebug() << "MainForm: remove dock widget";
        this->removeDockWidget(project->prjDockWidget);
        //qDebug() << "MainForm: close project";
        m_projectMan->closeProject(project);
        if (m_wDockManager->getBreakpointList() != NULL)
        {
            m_wDockManager->getBreakpointList()->reload(m_projectMan->getActive()->getBreakpointsListRef());
            /*if (m_wDockManager->getCentralWidget() != NULL)
            {
                m_wDockManager->getCentralWidget()->setBreakpointsLines(m_projectMan->getActive()->getBreakpointsForFileAbsolute(m_wDockManager->getCentralWidget()->getPath()));
            }*/
        }
        if (m_wDockManager->getBookmarkList() != NULL)
        {
            m_wDockManager->getBookmarkList()->reload(m_projectMan->getActive()->getBookmarksListRef());
            /*if (m_wDockManager->getCentralWidget() != NULL)
            {
                m_wDockManager->getCentralWidget()->setBookmarksLines(m_projectMan->getActive()->getBookmarksForFileAbsolute(m_wDockManager->getCentralWidget()->getPath()));
            }*/
        }
        if (m_projectMan->getOpenProjects().count() == 0)
        {
            m_wDockManager->setCentralWelcome();
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
    int result = m_projectMan->getActive()->handleBreakpoint(file, line + 1);
    //add
    if (0 == result)
    {
        m_wDockManager->getBreakpointList()->breakpointListAdd(file, line + 1);
        m_wDockManager->getCentralWidget()->addBreakpointLine(line + 1);
    }
    //remove
    else if (1 == result)
    {
        m_wDockManager->getBreakpointList()->breakpointListRemove(file, line + 1);
        m_wDockManager->getCentralWidget()->removeBreakpointLine(line + 1);
    }
    //else project doesnt contain current file - result == -1
    /*QList<Project*> projects = m_projectMan->getOpenProjects();
    for (int i = 0; i < projects.count(); i++)
    {
        projects.at(i)->handleBreakpoint(file, line + 1);
    }*/
}

void MainForm::breakpointsAddLines(QString file, int line, int linesAdded)
{
    m_projectMan->getActive()->moveBreakpointsAdd(file, line + 1, linesAdded);
    m_wDockManager->getBreakpointList()->breakpointsAddLines(file, line + 1, linesAdded);
    m_wDockManager->getCentralWidget()->moveBreakpointsLines(line + 1, linesAdded, true);
}


void MainForm::breakpointsRemoveLines(QString file, int line, int linesRemoved)
{
    m_projectMan->getActive()->moveBreakpointsRemove(file, line + 1, linesRemoved);
    m_wDockManager->getBreakpointList()->breakpointsRemoveLines(file, line + 1, linesRemoved);
    m_wDockManager->getCentralWidget()->moveBreakpointsLines(line + 1, linesRemoved, false);
}


void MainForm::manageBookmarkEmit(QString file, int line)
{
    //qDebug() << "MainForm: bookmark:" << file << ":" << line + 1;
    int result = m_projectMan->getActive()->handleBookmark(file, line + 1);
    //add
    if (0 == result)
    {
        m_wDockManager->getBookmarkList()->bookmarkListAdd(file, line + 1);
        m_wDockManager->getCentralWidget()->addBookmarkLine(line + 1);
    }
    //remove
    else if (1 == result)
    {
        m_wDockManager->getBookmarkList()->bookmarkListRemove(file, line + 1);
        m_wDockManager->getCentralWidget()->removeBookmarkLine(line + 1);
    }
    //else project doesnt contain current file - result == -1
    /*QList<Project*> projects = m_projectMan->getOpenProjects();
    for (int i = 0; i < projects.count(); i++)
    {
        projects.at(i)->handleBreakpoint(file, line + 1);
    }*/
}


void MainForm::bookmarksAddLines(QString file, int line, int linesAdded)
{
    //qDebug() << "MainForm: bookmarksAddLines";
    m_projectMan->getActive()->moveBookmarksAdd(file, line + 1, linesAdded);
    m_wDockManager->getBookmarkList()->bookmarksAddLines(file, line + 1, linesAdded);
    m_wDockManager->getCentralWidget()->moveBookmarksLines(line + 1, linesAdded, true);
}


void MainForm::bookmarksRemoveLines(QString file, int line, int linesRemoved)
{
    m_projectMan->getActive()->moveBookmarksRemove(file, line + 1, linesRemoved);
    m_wDockManager->getBookmarkList()->bookmarksRemoveLines(file, line + 1, linesRemoved);
    m_wDockManager->getCentralWidget()->moveBookmarksLines(line + 1, linesRemoved, false);
}


/*void MainForm::manageBreakpointRemove(QString file, int line)
{
    qDebug() << "MainForm: breakpoint remove:" << file << ":" << line;
    QList<Project*> projects = m_projectMan->getOpenProjects();
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
            m_projectMan->addUntrackedProject();
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
    QTimer::singleShot(50, this->m_wDockManager->getCentralWidget(), SLOT(changeHeight()));
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
    for (int i = 0; i < m_wDockManager->getTabCount(); i++)
    {
        //qDebug() << m_wDockManager->getTabWidget(i)->isChanged();
        if (m_wDockManager->getTabStatusChanged(i) == true && m_wDockManager->getTabWidget(i)->getPath() != "untracked")
        {
            lst.append(m_wDockManager->getTabWidget(i)->getPath());
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
    m_wDockManager->getCentralTextEdit()->undo();
}


void MainForm::redoSlot()
{
    m_wDockManager->getCentralTextEdit()->redo();
}


void MainForm::cutSlot()
{
    m_wDockManager->getCentralTextEdit()->cut();
}


void MainForm::copySlot()
{
    m_wDockManager->getCentralTextEdit()->copy();
}


void MainForm::pasteSlot()
{
    m_wDockManager->getCentralTextEdit()->paste();
}


void MainForm::selectAllSlot()
{
    m_wDockManager->getCentralTextEdit()->selectAll();
}


void MainForm::deselectSlot()
{
    m_wDockManager->getCentralTextEdit()->deselect();
}


void MainForm::scrollToFileLine(QString file, int line)
{
    m_wDockManager->setCentralByPath(file);
    m_wDockManager->getCentralTextEdit()->scrollToLine(line);
}


void MainForm::clockChangedSlot(double clock, int clockMult)
{
    m_projectMan->getActive()->setClock(clock, clockMult);
}


void MainForm::requestMacrosCodeEdits()
{
    QList<CodeEdit*> list;
    for (int i = 0; i < m_wDockManager->getTabCount(); i++)
    {
        if (true == m_wDockManager->getTabWidget(i)->isChild(m_projectMan->getActive()))
        {
            list.append(m_wDockManager->getTabWidget(i));
        }
    }
    emit provideMacroCodeEdits(list);
}


void MainForm::breakpointActHandle()
{
    this->manageBreakpointEmit(m_wDockManager->getCentralPath(), m_wDockManager->getCentralTextEdit()->textCursor().blockNumber());
}


void MainForm::disableBreakpointsHandle()
{
    if (true == this->simulationBreakpointsEnabled)
    {
        this->simulationBreakpointsEnabled = false;
        this->simulationDisableBreakpointsAct->setText(tr("Enable Breakpoints"));
        if (true == this->simulationStatus)
        {
            m_projectMan->getSimulated()->setBreakpoints(false);
        }
    }
    else
    {
        this->simulationBreakpointsEnabled = true;
        this->simulationDisableBreakpointsAct->setText(tr("Disable Breakpoints"));
        if (true == this->simulationStatus)
        {
            m_projectMan->getSimulated()->setBreakpoints(true);
        }
    }
}


void MainForm::simHighlightLines(std::vector<std::pair<const std::string *, unsigned int>> curr,
                                 std::vector<std::pair<const std::string *, unsigned int>> prev,
                                 std::vector<std::pair<const std::string *, unsigned int>> prev2,
                                 QList<QColor*> colors)
{
    QString tmpFile;
    QSet<QString> files;
    int compatibility = m_projectMan->getSimulated()->getAsmType();
    if (prev.size() > 0)
    {
        if (prev.size() > 1)
        {
            for (unsigned int i = 0; i < prev.size(); i++)
            {
                if (0 != compatibility)
                {
                    tmpFile = QString::fromStdString(*(std::get<0>(prev.at(i)))).replace('\\', '/').section('.', 0, -2) + ".psm";
                    files << tmpFile;
                }
                else
                {
                    files << QString::fromStdString(*(std::get<0>(prev.at(i)))).replace('\\', '/');
                }
            }
        }
        else
        {
            if (0 != compatibility)
            {
                tmpFile = QString::fromStdString(*(std::get<0>(prev.at(0)))).replace('\\', '/').section('.', 0, -2) + ".psm";
                files << tmpFile;
            }
            else
            {
                files << QString::fromStdString(*(std::get<0>(prev.at(0)))).replace('\\', '/');
            }
        }
    }
    if (prev2.size() > 0)
    {
        if (prev2.size() > 1)
        {
            for (unsigned int i = 0; i < prev2.size(); i++)
            {
                if (0 != compatibility)
                {
                    tmpFile = QString::fromStdString(*(std::get<0>(prev2.at(i)))).replace('\\', '/').section('.', 0, -2) + ".psm";
                    files << tmpFile;
                }
                else
                {
                    files << QString::fromStdString(*(std::get<0>(prev2.at(i)))).replace('\\', '/');
                }
            }
        }
        else
        {
            if (0 != compatibility)
            {
                tmpFile = QString::fromStdString(*(std::get<0>(prev2.at(0)))).replace('\\', '/').section('.', 0, -2) + ".psm";
                files << tmpFile;
            }
            else
            {
                files << QString::fromStdString(*(std::get<0>(prev2.at(0)))).replace('\\', '/');
            }
        }
    }
    if (curr.size() > 0)
    {
        if (curr.size() > 1)
        {
            for (unsigned int i = 0; i < curr.size(); i++)
            {
                if (0 != compatibility)
                {
                    tmpFile = QString::fromStdString(*(std::get<0>(curr.at(i)))).replace('\\', '/').section('.', 0, -2) + ".psm";
                    files << tmpFile;
                }
                else
                {
                    files << QString::fromStdString(*(std::get<0>(curr.at(i)))).replace('\\', '/');
                }
            }
        }
        else
        {
            if (0 != compatibility)
            {
                tmpFile = QString::fromStdString(*(std::get<0>(curr.at(0)))).replace('\\', '/').section('.', 0, -2) + ".psm";
                files << tmpFile;
            }
            else
            {
                files << QString::fromStdString(*(std::get<0>(curr.at(0)))).replace('\\', '/');
            }
        }
    }

    //const QString prjDir = QDir::cleanPath(QDir(m_projectMan->getActive()->prjPath.section('/',0, -2)).absolutePath());
    QString simulatedFilePath;
    //qDebug() << "MainForm: simulatedFile" << m_projectMan->getSimulated()->simulatedFile;
    QString simulatedFileChop = QDir(m_projectMan->getSimulated()->simulatedFile.section('/', 0, -2)).absolutePath() + '/';
    foreach (const QString &value, files)
    {
        simulatedFilePath = QDir::cleanPath(simulatedFileChop + value);
        //qDebug() << "MainForm: simulated file path" << simulatedFilePath;
        if (NULL == m_wDockManager->getCentralByPath(simulatedFilePath))
        {
            //qDebug() << "MainForm: simHighlightLines value" << value;
            this->openFilePath(simulatedFilePath);
            //m_wDockManager->setCentralByPath(simulatedFilePath);
        }

        /*if (NULL == m_wDockManager->getCentralByPath(value))
        {
            this->openFilePath(value);
        }
        m_wDockManager->getCentralByPath(value)->getTextEdit()->clearHighlight();*/
    }
    for (int i = 0; i < m_wDockManager->getTabCount(); i++)
    {
        m_wDockManager->getTabTextEdit(i)->clearHighlight();
    }
    m_wDockManager->getCentralTextEdit()->clearHighlight();

    if (prev.size() > 0)
    {
        if (prev.size() > 1)
        {
            for (unsigned int i = 0; i < prev.size(); i++)
            {
                if (QString::fromStdString(*(std::get<0>(prev.at(i)))) != "")
                {
                    simulatedFilePath = simulatedFileChop;
                    if (0 != compatibility)
                    {
                        simulatedFilePath += QString::fromStdString(*(std::get<0>(prev.at(i)))).section('.', 0, -2) + ".psm";
                    }
                    else
                    {
                        simulatedFilePath += QString::fromStdString(*(std::get<0>(prev.at(i))));
                    }
                    simulatedFilePath = QDir::cleanPath(simulatedFilePath);
                    m_wDockManager->setCentralByPath(simulatedFilePath);
                    if (0 != compatibility)
                    {
                        int line = m_compatibilityMode.getOrigLine(simulatedFileChop
                                                                + QString::fromStdString(*(std::get<0>(prev.at(i)))),
                                                                  std::get<1>(prev.at(i))
                                                                 );
                        m_wDockManager->getCentralTextEdit()->highlightLineAppend(line - 1, colors.at(1));
                    }
                    else
                    {
                        m_wDockManager->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev.at(i)) - 1, colors.at(1));
                    }
                    //m_wDockManager->getCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(i)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev.at(i)), colors.at(1));
                }
            }
        }
        else
        {
            simulatedFilePath = simulatedFileChop;
            if (0 != compatibility)
            {
                simulatedFilePath += QString::fromStdString(*(std::get<0>(prev.at(0)))).section('.', 0, -2) + ".psm";
            }
            else
            {
                simulatedFilePath += QString::fromStdString(*(std::get<0>(prev.at(0))));
            }
            simulatedFilePath = QDir::cleanPath(simulatedFilePath);
            m_wDockManager->setCentralByPath(simulatedFilePath);
            if (0 != compatibility)
            {
                int line = m_compatibilityMode.getOrigLine(simulatedFileChop
                                                        +  QString::fromStdString(*(std::get<0>(prev.at(0)))),
                                                           std::get<1>(prev.at(0))
                                                          );
                m_wDockManager->getCentralTextEdit()->highlightLineAppend(line - 1, colors.at(1));
            }
            else
            {
                m_wDockManager->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev.at(0)) - 1, colors.at(1));
            }
            //m_wDockManager->getCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev.at(0)), colors.at(1));
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
                    if (0 != compatibility)
                    {
                        simulatedFilePath += QString::fromStdString(*(std::get<0>(prev2.at(i)))).section('.', 0, -2) + ".psm";
                    }
                    else
                    {
                        simulatedFilePath += QString::fromStdString(*(std::get<0>(prev2.at(i))));
                    }
                    simulatedFilePath = QDir::cleanPath(simulatedFilePath);
                    m_wDockManager->setCentralByPath(simulatedFilePath);
                    if (0 != compatibility)
                    {
                        int line = m_compatibilityMode.getOrigLine(simulatedFileChop
                                                                + QString::fromStdString(*(std::get<0>(prev2.at(i)))),
                                                                  std::get<1>(prev2.at(i))
                                                                 );
                        m_wDockManager->getCentralTextEdit()->highlightLineAppend(line - 1, colors.at(2));
                    }
                    else
                    {
                        m_wDockManager->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev2.at(i)) - 1, colors.at(2));
                    }
                    //m_wDockManager->getCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)), colors.at(2));
                }
            }
        }
        else
        {
            simulatedFilePath = simulatedFileChop;
            if (0 != compatibility)
            {
                simulatedFilePath += QString::fromStdString(*(std::get<0>(prev2.at(0)))).section('.', 0, -2) + ".psm";
            }
            else
            {
                simulatedFilePath += QString::fromStdString(*(std::get<0>(prev2.at(0))));
            }
            simulatedFilePath = QDir::cleanPath(simulatedFilePath);
            m_wDockManager->setCentralByPath(simulatedFilePath);
            if (0 != compatibility)
            {
                int line = m_compatibilityMode.getOrigLine(simulatedFileChop
                                                        +  QString::fromStdString(*(std::get<0>(prev2.at(0)))),
                                                           std::get<1>(prev2.at(0))
                                                          );
                m_wDockManager->getCentralTextEdit()->highlightLineAppend(line - 1, colors.at(2));
            }
            else
            {
                m_wDockManager->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)) - 1, colors.at(2));
            }
            //m_wDockManager->getCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)), colors.at(2));
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
                    if (0 != compatibility)
                    {
                        simulatedFilePath += QString::fromStdString(*(std::get<0>(curr.at(i)))).section('.', 0, -2) + ".psm";
                    }
                    else
                    {
                        simulatedFilePath += QString::fromStdString(*(std::get<0>(curr.at(i))));
                    }
                    simulatedFilePath = QDir::cleanPath(simulatedFilePath);
                    m_wDockManager->setCentralByPath(simulatedFilePath);
                    if (0 != compatibility)
                    {
                        int line = m_compatibilityMode.getOrigLine(simulatedFileChop
                                                                + QString::fromStdString(*(std::get<0>(curr.at(i)))),
                                                                  std::get<1>(curr.at(i))
                                                                 );
                        m_wDockManager->getCentralTextEdit()->highlightLineAppend(line - 1, colors.at(0));
                    }
                    else
                    {
                        m_wDockManager->getCentralTextEdit()->highlightLineAppend(std::get<1>(curr.at(i)) - 1, colors.at(0));
                    }
                }
            }
        }
        else
        {
            simulatedFilePath = simulatedFileChop;
            if (0 != compatibility)
            {
                simulatedFilePath += QString::fromStdString(*(std::get<0>(curr.at(0)))).section('.', 0, -2) + ".psm";
            }
            else
            {
                simulatedFilePath += QString::fromStdString(*(std::get<0>(curr.at(0))));
            }
            simulatedFilePath = QDir::cleanPath(simulatedFilePath);
            m_wDockManager->setCentralByPath(simulatedFilePath);
            if (0 != compatibility)
            {
                int line = m_compatibilityMode.getOrigLine(simulatedFileChop
                                                        +  QString::fromStdString(*(std::get<0>(curr.at(0)))),
                                                           std::get<1>(curr.at(0))
                                                          );
                m_wDockManager->getCentralTextEdit()->highlightLineAppend(line - 1, colors.at(0));
            }
            else
            {
                m_wDockManager->getCentralTextEdit()->highlightLineAppend(std::get<1>(curr.at(0)) - 1, colors.at(0));
            }
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
        //qDebug() << "MainForm: activeProject" << m_projectMan->getActive()->prjName;
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
        qDebug() << "MainForm: activeProject" << m_projectMan->getActive()->prjName;
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
            Leds_sim *widget = new Leds_sim(0, m_projectMan->getSimulated()->getSimControl());
            widget->show();
        }
    #endif
}


void MainForm::sim7Seg()
{
    #ifdef MDS_FEATURE_SIM_7_SEGMENT
        if (true == this->simulationStatus)
        {
            Sim7Seg *widget = new Sim7Seg(0, m_projectMan->getSimulated()->getSimControl());
            widget->show();
        }
    #endif
}


void MainForm::simSwitch()
{
    #ifdef MDS_FEATURE_SIM_SWITCH
        if (true == this->simulationStatus)
        {
            SimSwitch *widget = new SimSwitch(0, m_projectMan->getSimulated()->getSimControl());
            widget->show();
        }
    #endif
}


void MainForm::simPortLogger()
{
    #ifdef MDS_FEATURE_SIM_PORT_LOGGER
        //if (true == this->simulationStatus)
        //{
        //    SimPortLogger *widget = new SimPortLogger(0, m_projectMan->getSimulated()->getSimControl());
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
    m_wDockManager->setCentralByPath(path);
    this->saveFile();
}


void MainForm::shortcutCloseTab()
{
    m_wDockManager->closeCurrentTab();
}


void MainForm::shortcutChangeTabLeft()
{
    m_wDockManager->changeTab(false);
}


void MainForm::shortcutChangeTabRight()
{
    m_wDockManager->changeTab(true);
}


void MainForm::reloadTabIcons()
{
    for (int i = 0; i < m_wDockManager->rightAreaTabs->count(); i++)
    {
        QString text = m_wDockManager->rightAreaTabs->tabText(i);
        if ("Breakpoints" == text)
        {
            QPixmap pixmap(":resources/icons/breakpoint.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            m_wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
        else if ("Bookmarks" == text)
        {
            QPixmap pixmap(":resources/icons/bullet_star.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            m_wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
        else if ("Macros" == text)
        {
            QPixmap pixmap(":resources/icons/brick.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            m_wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
        else if ("Hide" == text)
        {
            QPixmap pixmap(":resources/icons/bullet_arrow_right.png");
            QMatrix rm;
            rm.rotate(-90);
            pixmap = pixmap.transformed(rm);
            m_wDockManager->rightAreaTabs->setTabIcon(i, QIcon(pixmap));
        }
    }
    for (int i = 0; i < m_wDockManager->bottomAreaTabs->count(); i++)
    {
        QString text = m_wDockManager->bottomAreaTabs->tabText(i);
        if ("Compiler Messages" == text)
        {
            m_wDockManager->bottomAreaTabs->setTabIcon(i, QIcon(":resources/icons/messages.png"));
        }
        else if ("Simulator" == text)
        {
            m_wDockManager->bottomAreaTabs->setTabIcon(i, QIcon(":resources/icons/cog.png"));
        }
        else if ("Hide" == text)
        {
            m_wDockManager->bottomAreaTabs->setTabIcon(i, QIcon(":resources/icons/bullet_arrow_down.png"));
        }
    }
}



void MainForm::findSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutFind();
}


void MainForm::findNextSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutFindNext();
}


void MainForm::findPrevSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutFindPrevious();
}


void MainForm::replaceSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutReplace();
}


void MainForm::jmpToLineSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutJmpToLine();
}


void MainForm::commentSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutComment();
}


void MainForm::deleteCommentSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutDeleteComment();
}


void MainForm::jmpToBookmarkNextSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutJmpToBookmarkNext();
}


void MainForm::jmpToBookmarkPrevSlot()
{
    m_wDockManager->getCentralTextEdit()->shortcutJmpToBookmarkPrev();
}


void MainForm::fileClosed(QString path)
{
    m_fileWatcher.removePath(path);
}


void MainForm::fileChanged(QString path)
{
    //qDebug() << "file changed" << path;
    //QApplication::processEvents();
    if (NULL == m_reloadDlg)
    {
        m_reloadDlg = new SaveDialog(this, QStringList(), true);
        connect(m_reloadDlg, SIGNAL(reload(QString)), this, SLOT(reloadFile(QString)));
        m_reloadDlg->exec();
        delete m_reloadDlg;
        m_reloadDlg = NULL;
    }
    if (NULL != m_reloadDlg)
    {
        m_reloadDlg->appendFile(path);
    }
    //m_fileWatcher.blockSignals(true);
    //m_fileWatcher.blockSignals(false);
}


void MainForm::reloadFile(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        error(ERR_OPENFILE, path);
    }
    else
    {
        m_wDockManager->setCentralByPath(path);
        m_wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
        file.close();
        QTimer::singleShot(100, this->m_wDockManager->getCentralWidget(), SLOT(changeHeight()));
    }
}