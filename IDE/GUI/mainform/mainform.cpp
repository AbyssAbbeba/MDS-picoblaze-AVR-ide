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
#ifdef MDS_FEATURE_DISASSEMBLER
    #include "../dialogs/disasmdlg/disasmdlg.h"
#endif
#ifdef MDS_FEATURE_TRANSLATOR
    #include "../dialogs/translatordlg/translatordlg.h"
#endif
#ifdef MDS_FEATURE_FILECONVERTER
    #include "../dialogs/fileconvertdlg/fileconvertdlg.h"
#endif
#include "../errordialog/errordlg.h"
//#include "pluginman_gui.h"
#include "../dialogs/projectcfg/projectcfgdlg_core.h"
#include "../dialogs/interfacecfg/interfacecfgdlg_core.h"
//#include "../widgets/CompileWidget/compilewidget.h"
#include "../widgets/HelpWidget/helpwidget.h"
#include "../widgets/AsmMacroAnalyser/asmmacroanalyser.h"

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
    #include "../widgets/Tools/SimLed/simled.h"
#endif
#ifdef MDS_FEATURE_SIM_7_SEGMENT
    #include "../widgets/Tools/Sim7Seg/sim7seg.h"
#endif

#include "../widgets/Tools/SimSwitch/simswitch.h"
#include "../widgets/Tools/SimPortLogger/simportlogger.h"

#include "../guicfg/guicfg.h"





    Q_DECLARE_METATYPE (std::string);
    Q_DECLARE_METATYPE (CompilerBase::MessageType);
/**
 * @brief Constructor, inits project and dock widget manager and create menus/toolbars
 */
MainForm::MainForm()
{
    //this->statusBar()->showMessage("Loading", 0);
    //qDebug() << "MainForm: MainForm()";
    this->setMinimumWidth(800);
    this->setMinimumHeight(600);
    this->setWindowTitle("MDS");
    #ifdef MDS_VARIANT_NONCOMMERCIAL
        this->setWindowTitle("MDS NON-COMMERCIAL");
    #endif
    #ifdef MDS_VARIANT_TRIAL
        QFileInfo mdsInfo(QCoreApplication::applicationFilePath());
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
    //createShortcuts();
    createToolbar();
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


    delete this->pm_projNewAdd;
    delete this->pm_projNew;
    delete this->pm_projOpen;
    delete this->pm_projSave;
    delete this->pm_projComp;
    delete this->pm_simFlowStart;
    delete this->pm_simFlowStop;
    delete this->pm_simRun;
    delete this->pm_simAnimate;
    delete this->pm_simStep;
    delete this->pm_simReset;
    delete this->pm_simUnhighlight;
    delete this->pm_toolDis;
    delete this->pm_cross;

    delete this->icon_projNewAdd;
    delete this->icon_projNew;
    delete this->icon_projOpen;
    delete this->icon_projSave;
    delete this->icon_projComp;
    delete this->icon_simFlow;
    delete this->icon_simRun;
    delete this->icon_simAnimate;
    delete this->icon_simStep;
    delete this->icon_simReset;
    delete this->icon_simUnhighlight;
    delete this->icon_toolDis;

    if (true == GuiCfg::getInstance().getSessionRestoration())
    {
        if (projectMan->getOpenProjects().count() > 0)
        {
            QList<Project*> projects = projectMan->getOpenProjects();
            qDebug() << "Mainform: prepare to project session restoration";
            for (int i = 0; i < projects.count(); i++)
            {
                qDebug() << "Mainform: saving project" << projects.at(i)->prjName;
                GuiCfg::getInstance().sessionAppendProject(projects.at(i)->prjPath);
                if (wDockManager->getTabCount() > 0)
                {
                    qDebug() << "Mainform: prepare to files session restoration";
                    for (int j = 0; j < wDockManager->getTabCount(); j++)
                    {
                        if (true == wDockManager->getTabWidget(j)->isChild(projects.at(i)))
                        {
                            qDebug() << "Mainform: saving file" << wDockManager->getTabWidget(j)->getName();
                            GuiCfg::getInstance().sessionAppendFile(wDockManager->getTabWidget(j)->getPath());
                            GuiCfg::getInstance().sessionAppendFileParentProject(projects.at(i)->prjPath);
                        }
                    }
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
}



/**
 * @brief Creates menus in main menu
 */
void MainForm::createMenu()
{
    //qDebug() << "MainForm: createMenu()";
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(newAddAct);
    fileMenu->addSeparator();
    fileMenu->addAction(openAct);
    recentFilesMenu = fileMenu->addMenu(tr("Open Recent"));
    QList<QString> recentFiles = GuiCfg::getInstance().getRecentFiles();
    for (int i = recentFiles.count() -1; i >= 0 ; i--)
    {
        QAction *recent = new QAction(recentFiles.at(i), recentFilesMenu);
        recentFilesMenu->addAction(recent);
    }
    fileMenu->addSeparator();
    //fileMenu->addAction(addAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAllAct);
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

    #if defined(MDS_FEATURE_SIM_LED_PANEL) || defined(MDS_FEATURE_SIM_7_SEGMENT)
        simToolsMenu = menuBar()->addMenu(tr("Simulation tools"));
        #ifdef MDS_FEATURE_SIM_LED_PANEL
            simToolsMenu->addAction(toolSimLedsAct);
        #endif
        #ifdef MDS_FEATURE_SIM_7_SEGMENT
            simToolsMenu->addAction(toolSim7SegAct);
        #endif
        simToolsMenu->addAction(toolSimSwitchAct);
        simToolsMenu->addAction(toolSimLoggerAct);
    #endif

    helpMenu = menuBar()->addMenu(tr("&Help"));
    #ifdef MDS_FEATURE_LICENCE_CERTIFICATE
        helpMenu->addAction(licenseAct);
    #endif
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQTAct);
    helpMenu->addAction(helpActionAct);
    helpMenu->addSeparator();
    helpMenu->addAction(example1Act);
    //qDebug() << "MainForm: return createMenu()";
}



/**
 * @brief Creates actions used in menus and toolbars and connects them to appropriate slots
 *
 */
void MainForm::createActions()
{
    //qDebug() << "MainForm: CreateActions()";
    newAct = new QAction(tr("New Untracked File"), this);
    newAct->setStatusTip("Create a new file");
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    newAct->setShortcut(QKeySequence("Ctrl+N"));

    openAct = new QAction(tr("Open File"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    openAct->setShortcut(QKeySequence("Ctrl+O"));



    /*QPixmap *pm_projAdd = new QPixmap(":resources/icons/projAdd.png");
    QIcon *icon_projAdd = new QIcon(*pm_projAdd);
    addAct = new QAction(*icon_projAdd, tr("Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));
    addAct->setDisabled(true);*/


    this->pm_projNewAdd = new QPixmap(":resources/icons/projNewAdd.png");
    this->icon_projNewAdd = new QIcon(*pm_projNewAdd);
    newAddAct = new QAction(*icon_projNewAdd, tr("New Project File"), this);
    connect(newAddAct, SIGNAL(triggered()), this, SLOT(newAddFile()));
    newAddAct->setDisabled(true);


    /*QPixmap *pm_projRemoveFile = new QPixmap("src//resources/icons/projDelete.png");
    QIcon *icon_projRemoveFile = new QIcon(*pm_projRemoveFile);
    removeFileAct = new QAction(*icon_projRemoveFile, tr("&Remove file"), this);
    connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeProjFile()));*/


    saveAct = new QAction(tr("Save File"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));
    saveAct->setDisabled(true);
    saveAct->setShortcut(QKeySequence("Ctrl+S"));

    saveAsAct = new QAction(tr("Save As"), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));
    saveAsAct->setDisabled(true);

    saveAllAct = new QAction(tr("Save All"), this);
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));
    saveAllAct->setDisabled(true);


    //EDIT
    undoAct = new QAction(tr("Undo"), this);
    connect(undoAct, SIGNAL(triggered()), this, SLOT(undoSlot()));
    redoAct = new QAction(tr("Redo"), this);
    connect(redoAct, SIGNAL(triggered()), this, SLOT(redoSlot()));
    cutAct = new QAction(tr("Cut"), this);
    connect(cutAct, SIGNAL(triggered()), this, SLOT(cutSlot()));
    copyAct = new QAction(tr("Copy"), this);
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copySlot()));
    pasteAct = new QAction(tr("Paste"), this);
    connect(pasteAct, SIGNAL(triggered()), this, SLOT(pasteSlot()));
    selectAllAct = new QAction(tr("Select All"), this);
    connect(selectAllAct, SIGNAL(triggered()), this, SLOT(selectAllSlot()));
    deselectAct = new QAction(tr("Deselect"), this);
    connect(deselectAct, SIGNAL(triggered()), this, SLOT(deselectSlot()));



    //INTERFACE
    interfaceConfigAct = new QAction(tr("Config"), this);
    connect(interfaceConfigAct, SIGNAL(triggered()), this, SLOT(interfaceConfig()));
    //pluginAct = new QAction(tr("Plugins"), this);
    //connect(pluginAct, SIGNAL(triggered()), this, SLOT(showPlugins()));




    this->pm_projNew = new QPixmap(":resources/icons/projNew.png");
    this->icon_projNew = new QIcon(*pm_projNew);
    newProjAct = new QAction(*icon_projNew, tr("New Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    this->pm_projOpen = new QPixmap(":resources/icons/projOpen.png");
    this->icon_projOpen = new QIcon(*pm_projOpen);
    openProjAct = new QAction(*icon_projOpen, tr("Open Project"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    this->pm_projSave = new QPixmap(":resources/icons/projSave.png");
    this->icon_projSave = new QIcon(*pm_projSave);
    saveProjAct = new QAction(*icon_projSave, tr("Save Project"), this);
    connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));
    saveProjAct->setDisabled(true);

    closeProjectAct = new QAction(tr("Close Project"), this);
    connect(closeProjectAct, SIGNAL(triggered()), this, SLOT(closeProject()));
    closeProjectAct->setDisabled(true);

    saveProjConfigAct = new QAction(tr("Save Project Config"), this);
    saveProjConfigAct->setDisabled(true);

    exitAct = new QAction(tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    this->pm_projComp = new QPixmap(":resources/icons/compile.png");
    this->icon_projComp = new QIcon(*pm_projComp);
    projectCompileAct = new QAction(*icon_projComp, tr("Compile"), this);
    connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));
    projectCompileAct->setDisabled(true);
    projectCompileAct->setShortcut(Qt::Key_F5);

    projectConfigAct = new QAction(tr("Config"), this);
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

    this->pm_simStep = new QPixmap(":resources/icons/simulationStep.png");
    this->icon_simStep = new QIcon(*pm_simStep);
    simulationStepAct = new QAction(*icon_simStep, tr("Do step"), this);
    connect(simulationStepAct, SIGNAL(triggered()), this, SLOT(simulationStep()));
    simulationStepAct->setDisabled(true);
    simulationStepAct->setShortcut(Qt::Key_F9);

    this->pm_simReset = new QPixmap(":resources/icons/simulationReset.png");
    this->icon_simReset = new QIcon(*pm_simReset);
    simulationResetAct = new QAction(*icon_simReset, tr("Reset"), this);
    connect(simulationResetAct, SIGNAL(triggered()), this, SLOT(simulationReset()));
    simulationResetAct->setDisabled(true);
    simulationResetAct->setShortcut(Qt::Key_F10);

    this->pm_simUnhighlight = new QPixmap(":resources/icons/unhighlight.png");
    this->icon_simUnhighlight = new QIcon(*pm_simUnhighlight);
    simulationUnhighlightAct = new QAction(*icon_simUnhighlight, tr("Unhighlight"), this);
    connect(simulationUnhighlightAct, SIGNAL(triggered()), this, SLOT(unhighlight()));
    simulationUnhighlightAct->setDisabled(true);

    this->pm_breakpoint = new QPixmap(":resources/icons/breakpoint.png");
    this->icon_breakpoint = new QIcon(*pm_breakpoint);
    simulationBreakpointAct = new QAction(*icon_breakpoint, tr("Breakpoint"), this);
    simulationBreakpointAct->setDisabled(true);
    connect(simulationBreakpointAct, SIGNAL(triggered()), this, SLOT(breakpointActHandle()));

    simulationDisableBreakpointsAct = new QAction(tr("Disable breakpoints"), this);
    simulationDisableBreakpointsAct->setDisabled(true);
    connect(simulationDisableBreakpointsAct, SIGNAL(triggered()), this, SLOT(disableBreakpointsHandle()));

    #ifdef MDS_FEATURE_DISASSEMBLER
        this->pm_toolDis = new QPixmap(":resources/icons/disassemble.png");
        this->icon_toolDis = new QIcon(*pm_toolDis);
        toolDisassemblerAct = new QAction(*icon_toolDis, tr("Disassembler"), this);
        connect(toolDisassemblerAct, SIGNAL(triggered()), this, SLOT(toolDisassemble()));
    #endif
    #ifdef MDS_FEATURE_TRANSLATOR
        toolTranslatorAct = new QAction(tr("ASM Translator"), this);
        connect(toolTranslatorAct, SIGNAL(triggered()), this, SLOT(toolTranslate()));
    #endif
    #ifdef MDS_FEATURE_FILECONVERTER
        toolFileConvertAct = new QAction(tr("DataFile Converter"), this);
        connect(toolFileConvertAct, SIGNAL(triggered()), this, SLOT(toolFileConvert()));
    #endif
    #ifdef MDS_FEATURE_CONVERTER_TOOL
        toolConvertorAct = new QAction(tr("Converter"), this);
        connect(toolConvertorAct, SIGNAL(triggered()), this, SLOT(toolConvertor()));
    #endif
    #ifdef MDS_FEATURE_8_SEGMENT_EDITOR
        toolDisplayAct = new QAction(tr("Segment Display"), this);
        connect(toolDisplayAct, SIGNAL(triggered()), this, SLOT(toolDisplay()));
    #endif
    #ifdef MDS_FEATURE_LOOP_GENERATOR
        toolLoopGenAct = new QAction(tr("Loop Generator"), this);
        connect(toolLoopGenAct, SIGNAL(triggered()), this, SLOT(loopGen()));
    #endif
    #ifdef MDS_FEATURE_SIM_LED_PANEL
        toolSimLedsAct = new QAction(tr("LED panel"), this);
        toolSimLedsAct->setDisabled(true);
        connect(toolSimLedsAct, SIGNAL(triggered()), this, SLOT(simLeds()));
    #endif
    #ifdef MDS_FEATURE_SIM_7_SEGMENT
        toolSim7SegAct = new QAction(tr("7 Segment"), this);
        toolSim7SegAct->setDisabled(true);
        connect(toolSim7SegAct, SIGNAL(triggered()), this, SLOT(sim7Seg()));
    #endif
    toolSimSwitchAct = new QAction(tr("Switch"), this);
    toolSimSwitchAct->setDisabled(true);
    connect(toolSimSwitchAct, SIGNAL(triggered()), this, SLOT(simSwitch()));
    toolSimLoggerAct = new QAction(tr("Port Logger"), this);
    toolSimLoggerAct->setDisabled(true);
    connect(toolSimLoggerAct, SIGNAL(triggered()), this, SLOT(simPortLogger()));

    #ifdef MDS_FEATURE_LICENCE_CERTIFICATE
        licenseAct = new QAction(tr("License"), this);
        connect(licenseAct, SIGNAL(triggered()), this, SLOT(manageLicense()));
    #endif
   
    aboutAct = new QAction(tr("About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    aboutQTAct = new QAction(tr("About QT"), this);
    connect(aboutQTAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    helpActionAct = new QAction(tr("Help"), this);
    connect(helpActionAct, SIGNAL(triggered()), this, SLOT(help()));
    example1Act = new QAction(tr("See Example Project"), this);
    connect(example1Act, SIGNAL(triggered()), this, SLOT(exampleOpen()));

    this->pm_cross = new QPixmap(":resources/icons/pause.png");
    //qDebug() << "MainForm: return CreateActions()";
}


void MainForm::createShortcuts()
{
    //QShortcut *srtSave = new QShortcut(QKeySequence("Ctrl+S"), this);
    //connect(srtSave, SIGNAL(activated()), this, SLOT(saveFile()));

    /*QShortcut *srtCompile = new QShortcut(QKeySequence("F5"), this);
    connect(srtCompile, SIGNAL(activated()), this, SLOT(compileProject()));

    QShortcut *srtSimFlow = new QShortcut(QKeySequence("F6"), this);
    connect(srtSimFlow, SIGNAL(activated()), this, SLOT(simulationFlowHandle()));

    QShortcut *srtSimRun = new QShortcut(QKeySequence("F7"), this);
    connect(srtSimRun, SIGNAL(activated()), this, SLOT(simulationRunHandle()));

    QShortcut *srtSimAnimate = new QShortcut(QKeySequence("F8"), this);
    connect(srtSimAnimate, SIGNAL(activated()), this, SLOT(simulationAnimateHandle()));

    QShortcut *srtSimStep = new QShortcut(QKeySequence("F9"), this);
    connect(srtSimStep, SIGNAL(activated()), this, SLOT(simulationStep()));

    QShortcut *srtSimReset = new QShortcut(QKeySequence("F10"), this);
    connect(srtSimReset, SIGNAL(activated()), this, SLOT(simulationReset()));*/
}



/**
 * @brief Creates toolbars used in main window
 */
void MainForm::createToolbar()
{
    //qDebug() << "MainForm: CreateToolbar()";
    //fileToolBar = addToolBar(tr("File Toolbar"));
    projectToolBar = addToolBar(tr("Project Toolbar"));
    simulationToolBar = addToolBar(tr("Simulation Toolbar"));


    projectToolBar->addAction(newProjAct);
    projectToolBar->addAction(openProjAct);
    projectToolBar->addAction(saveProjAct);
    projectToolBar->addAction(newAddAct);
    //projectToolBar->addAction(removeFileAct);
    //projectToolBar->addAction(addAct);

    //fileToolBar->addAction(saveAct);


    simulationToolBar->addAction(projectCompileAct);
    simulationToolBar->addAction(simulationFlowAct);
    simulationToolBar->addAction(simulationRunAct);
    simulationToolBar->addAction(simulationAnimateAct);
    simulationToolBar->addAction(simulationStepAct);
    simulationToolBar->addAction(simulationResetAct);
    simulationToolBar->addAction(simulationUnhighlightAct);
    simulationToolBar->addAction(simulationBreakpointAct);
    simulationToolBar->addAction(simulationDisableBreakpointsAct);



    projectToolBar->setAllowedAreas(Qt::TopToolBarArea);
    simulationToolBar->setAllowedAreas(Qt::TopToolBarArea);
    //fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, projectToolBar);
    addToolBar(Qt::TopToolBarArea, simulationToolBar);
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
        QFileInfo mdsInfo(QCoreApplication::applicationFilePath());
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
        newAddAct->setEnabled(true);
        saveProjAct->setEnabled(true);
        saveProjConfigAct->setEnabled(true);
        //projectCompileAct->setEnabled(true);
        //simulationFlowAct->setEnabled(true);
        saveAct->setEnabled(true);
        saveAsAct->setEnabled(true);
        saveAllAct->setEnabled(true);
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
    }
    else
    {
        wDockManager->addDockWidget(wSimulationInfo);
    }
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
    //qDebug() << "MainForm: newAddFile()";
    //jen se vytvori novy tab na code editoru
    //a soubor se prida k projektu
    QString path = QFileDialog::getSaveFileName(this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
    if (path != NULL)
    {
        wDockManager->addCentralWidget(path.section('/', -1), path);
        wDockManager->getCentralWidget()->setChanged();
        wDockManager->getCentralWidget()->connectAct();

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
    //qDebug() << "MainForm: openFile()";
    QString path = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (path != NULL)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
        }
        else
        {
            //wDockManager->addCentralWidget(path.section('/', -1), path);
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            file.close();
            //wDockManager->getCentralWidget()->connectAct();
            projectMan->addUntrackedFile(path, path.section('/', -1));
            wDockManager->addUntrackedCentralWidget(path.section('/', -1), path);
            wDockManager->getCentralWidget()->setParentProject(projectMan->getUntracked());
            wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getUntracked());
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
    //qDebug() << absoluteFilePath;
    if (NULL != path)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
        }
        else
        {
            file.close();
            //qDebug() << "MainForm: addCentralWidget";
            wDockManager->addCentralWidget(path.section('/', -1), path);
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            //qDebug() << "MainForm: connect";
            CodeEdit *centralCodeEdit = wDockManager->getCentralWidget();
            centralCodeEdit->connectAct();
            //qDebug() << "MainForm: set parent";
            if (parentProjectPath != "")
            {
                for (int i = 0; i < projectMan->getOpenProjects().count(); i++)
                {
                    if (projectMan->getOpenProjects().at(i)->prjPath == parentProjectPath)
                    {
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
                    //TODO: something
                    //WHAT DOES IT MEAN? Session restoration?
                }
            }
            else
            {
                centralCodeEdit->setParentProject(projectMan->getActive());
                wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->setParentProject(projectMan->getActive());
                centralCodeEdit->setBreakpointsLines(projectMan->getActive()->getBreakpointsForFileAbsolute(centralCodeEdit->getPath()));
                centralCodeEdit->setBookmarksLines(projectMan->getActive()->getBookmarksForFileAbsolute(centralCodeEdit->getPath()));
                //wDockManager->getTabWidget(wDockManager->getTabCount() - 1)->updateLineCounter();

            }
            //wDockManager->getCentralWidget()->setSaved();
            if (true == centralCodeEdit->isChanged())
            {
                qDebug() << "MainForm: openfilepath - some error here";
            }
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
            path = QFileDialog::getSaveFileName(this, tr("Source File"));
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
        if (wDockManager->getCentralWidget()->isChanged() == true )
        {
            qDebug() << "MainForm: central is changed";
        }
        qDebug() << "Mainform: saving file";
        QString path;
        if (wDockManager->getCentralPath() == NULL || wDockManager->getCentralPath() == "untracked")
        {
            //path = QFileDialog::getSaveFileName(this, tr("Source File");
            path = QFileDialog::getSaveFileName (this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
                error(ERR_OPENFILE);
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
    QString path = QFileDialog::getSaveFileName(this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
    if (path != NULL)
    {
        QFile file(path);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
        }
        else
        {
            QTextStream fout(&file);
            fout << wDockManager->getCentralTextEdit()->toPlainText();
            file.close();
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
            path = QFileDialog::getSaveFileName(this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
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
                error(ERR_OPENFILE);
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
        if (wDockManager->getTabWidget(i)->isChanged() == true
            || wDockManager->getTabWidget(i)->getPath() == "untracked")
        {
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
    qDebug() << "MainForm: openProject()";
    //nalezeni projektu
    QFileDialog dialog;
    QString path = QFileDialog::getOpenFileName (this, tr("Project Directory"), "", tr("Project (*.mds-project)"));

    if (path.isEmpty() == false && projectMan->isOpened(path) == false)
    {
    //nacteni projektu
        //qDebug() << path;
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
        }
        else
        {
            //nacteni obsahu do widgetu
            projectMan->openProject(&file);
            file.close();
        }
    }
    qDebug() << "MainForm: return openProject()";
}


/**
 * @brief Opens selected project.
 * @param path Path to the project.
 */
bool MainForm::openProject(QString path)
{
    qDebug() << "MainForm: openProject()";
    if (false == projectMan->isOpened(path))
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
            return false;
        }
        else
        {
            //nacteni obsahu do widgetu
            projectMan->openProject(&file);
            file.close();
            qDebug() << "MainForm: return openProject()";
            return true;
        }
    }
    qDebug() << "MainForm: return openProject() false";
    return false;
}


void MainForm::projectOpened()
{
    qDebug() << "MainForm: projectOpened";
    wDockManager->deleteCentralWelcome();
    if (false == projectConfigAct->isEnabled())
    {
        projectConfigAct->setEnabled(true);
    }
    if (false == projectTabConnected)
    {
        QList<QTabBar*> tabList = this->findChildren<QTabBar*>();
        if (tabList.size() > 1)
        {
            qDebug() << "MainForm: tab connected";
            projectTabConnected = true;
            connect(tabList.at(tabList.size()-1),
                    SIGNAL(currentChanged(int)),
                    this,
                    SLOT(activeProjectChanged(int))
                   );
            qDebug() << "projectTabs = ";
            projectTabs = tabList.at(tabList.size()-1);
            qDebug() << "projectTabs = done";
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
    qDebug() << "MainForm: projectOpened done";
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

    ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->clear();
    CompilerOptions *options = new CompilerOptions();
    QString mainFile;

    if (projectMan->getActive()->prjPath == "untracked" && projectMan->getActive()->prjName == "untracked")
    {
        //compile actual file with global settings, not error
        //error(ERR_UNTRACKED_PROJECT);
        qDebug() << "MainForm: compiled untracked project, actual file";

        if (wDockManager->getCentralWidget() == NULL)
        {
            qDebug() << "MainForm: returned";
            return;
        }

        this->saveFile();

        options->m_sourceFiles.push_back(wDockManager->getCentralPath().toStdString());

        options->m_device = this->projectMan->getActive()->family.toStdString();


        QDir templateDir(GuiCfg::getInstance().getTemplatePath());
        if (true == this->projectMan->getActive()->defaultVerilog)
        {
            options->m_verilogTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + this->projectMan->getActive()->family
                                        + ".v"
                                        ).toStdString();
        }
        else
        {
            options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toStdString();
        }

        if (true == this->projectMan->getActive()->defaultVHDL)
        {
            options->m_vhdlTemplate = ( templateDir.absolutePath()
                                    + "/"
                                    + this->projectMan->getActive()->family
                                    + ".vhd"
                                    ).toStdString();
        }
        else
        {
            options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toStdString();
        }



        QDir pathDir(GuiCfg::getInstance().getTempPath());
        if (false == pathDir.exists())
        {
            pathDir.mkpath(".");
        }

        QDir prjDir;
        mainFile = pathDir.absolutePath()
                 + "/"
                 +  wDockManager->getCentralName().section('.',0,-2);

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
                                     ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(1))
        {
            options->m_macroTable = ( mainFile
                                    + ".mtbl"
                                    ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(2))
        {
            options->m_mdsDebugFile = ( mainFile
                                      + ".dbg"
                                      ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(3))
        {
            options->m_codeTree = ( mainFile
                                  + ".ctr"
                                  ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(4))
        {
            options->m_lstFile = ( mainFile
                                  + ".lst"
                                 ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(5))
        {
            options->m_hexFile = ( mainFile
                                 + ".ihex"
                                 ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(6))
        {
            options->m_binFile = ( mainFile
                                 + ".bin"
                                 ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(7))
        {
            options->m_srecFile = ( mainFile
                                  + ".srec"
                                  ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(8))
        {
            options->m_memFile = ( mainFile
                                 + ".mem"
                                 ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(9))
        {
            options->m_rawHexDumpFile = ( mainFile
                                        + ".rawhex"
                                        ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(10))
        {
            options->m_verilogFile = ( mainFile
                                     + ".v"
                                     ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(11))
        {
            options->m_vhdlFile = ( mainFile
                                  + ".vhd"
                                  ).toStdString();
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
            qDebug() << prjDir.relativeFilePath(projectMan->getActive()->filePaths.at(i));
            qDebug() << projectMan->getActive()->filePaths.at(i);
            qDebug() << "fixed?" << prjDir.absolutePath() + "/" + projectMan->getActive()->filePaths.at(i);
            qDebug() << "MainForm: file dir" << filePath;
            if (filePath == wDockManager->getCentralPath())
            {
                found = true;
                break;
            }
        }
        if (found == true)
        {
            qDebug() << "MainForm: compiled actual project, actual file";
            options->m_sourceFiles.push_back(filePath.toStdString());


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



            mainFile = prjDir.absolutePath() + "/" + wDockManager->getCentralName().section('.',0,-2);
            qDebug() << mainFile;

            options->m_device = this->projectMan->getActive()->family.toStdString();


            QDir templateDir(GuiCfg::getInstance().getTemplatePath());
            if (true == this->projectMan->getActive()->defaultVerilog)
            {
                options->m_verilogTemplate = ( templateDir.absolutePath()
                                            + "/"
                                            + this->projectMan->getActive()->family
                                            + ".v"
                                            ).toStdString();
            }
            else
            {
                options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toStdString();
            }

            if (true == this->projectMan->getActive()->defaultVHDL)
            {
                options->m_vhdlTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + this->projectMan->getActive()->family
                                        + ".vhd"
                                        ).toStdString();
            }
            else
            {
                options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toStdString();
            }

            if (projectMan->getActive()->compileOpt.at(0))
            {
                options->m_symbolTable = (mainFile + ".stbl").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(1))
            {
                options->m_macroTable = (mainFile + ".mtbl").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(2))
            {
                options->m_mdsDebugFile = (mainFile + ".dbg").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(3))
            {
                options->m_codeTree = (mainFile + ".ctr").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(4))
            {
                options->m_lstFile = (mainFile + ".lst").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(5))
            {
                options->m_hexFile = (mainFile + ".ihex").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(6))
            {
                options->m_binFile = (mainFile + ".bin").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(7))
            {
                options->m_srecFile = (mainFile + ".srec").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(8))
            {
                options->m_memFile = ( mainFile + ".mem").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(9))
            {
                options->m_rawHexDumpFile = ( mainFile + ".rawhex" ).toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(10))
            {
                options->m_verilogFile = ( mainFile  + ".v").toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(11))
            {
                options->m_vhdlFile = ( mainFile + ".vhd").toStdString();
            }
        }
        else
        {
            qDebug() << "MainForm: compiled untracked project, actual file";
            if (this->projectMan->getUntracked() != NULL)
            {
                this->projectMan->getUntracked()->addFile(wDockManager->getCentralPath(),wDockManager->getCentralName());
                this->projectMan->setActive(this->projectMan->getUntracked());
            }
            else
            {
                this->projectMan->addUntrackedProject();
                this->projectMan->getUntracked()->addFile(wDockManager->getCentralPath(),wDockManager->getCentralName());
                this->projectMan->setActive(this->projectMan->getUntracked());
            }
            options->m_sourceFiles.push_back(wDockManager->getCentralPath().toStdString());

            options->m_device = this->projectMan->getActive()->family.toStdString();


            QDir templateDir(GuiCfg::getInstance().getTemplatePath());
            if (true == this->projectMan->getActive()->defaultVerilog)
            {
                options->m_verilogTemplate = ( templateDir.absolutePath()
                                             + "/"
                                             + this->projectMan->getActive()->family
                                             + ".v"
                                             ).toStdString();
            }
            else
            {
                options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toStdString();
            }

            if (true == this->projectMan->getActive()->defaultVHDL)
            {
                options->m_vhdlTemplate = ( templateDir.absolutePath()
                                          + "/"
                                          + this->projectMan->getActive()->family
                                          + ".vhd"
                                          ).toStdString();
            }
            else
            {
                options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toStdString();
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

            mainFile = QDir::cleanPath( GuiCfg::getInstance().getTempPath()
                                      + "/"
                                      +  wDockManager->getCentralPath().section('.',0,-2)
                                      );


            if (projectMan->getUntracked()->compileOpt.at(0))
            {
                //options->m_symbolTable = (mainFile + ".stbl").toStdString();
                options->m_symbolTable = ( mainFile
                                         + ".stbl"
                                         ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(1))
            {
                options->m_macroTable = ( mainFile
                                        + ".mtbl"
                                        ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(2))
            {
                options->m_mdsDebugFile = ( mainFile
                                        + ".dbg"
                                        ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(3))
            {
                options->m_codeTree = ( mainFile
                                      + ".ctr"
                                      ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(4))
            {
                options->m_lstFile = ( mainFile
                                     + ".lst"
                                     ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(5))
            {
                options->m_hexFile = ( mainFile
                                     + ".ihex"
                                     ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(6))
            {
                options->m_binFile = ( mainFile
                                     + ".bin"
                                     ).toStdString();
            }
            if (projectMan->getUntracked()->compileOpt.at(7))
            {
                options->m_srecFile = ( mainFile
                                      + ".srec"
                                      ).toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(8))
            {
                options->m_memFile = ( mainFile
                                    + ".mem"
                                    ).toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(9))
            {
                options->m_rawHexDumpFile = ( mainFile
                                            + ".rawhex"
                                            ).toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(10))
            {
                options->m_verilogFile = ( mainFile
                                        + ".v"
                                        ).toStdString();
            }
            if (projectMan->getActive()->compileOpt.at(11))
            {
                options->m_vhdlFile = ( mainFile
                                    + ".vhd"
                                    ).toStdString();
            }
        }
        //return;
    }
    //else compile main file of actual project
    else
    {
        qDebug() << "MainForm: actual project, main file";
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
                                           ).toStdString()
                                        );
        QDir prjDir(projectMan->getActive()->prjPath.section('/',0, -2));
        QDir fileDir(QString::fromStdString(options->m_sourceFiles.at(0)).section('/',0, -2));
        if (fileDir.relativeFilePath(prjDir.absolutePath()) == "")
        {
            mainFile = "./" +  this->projectMan->getActive()->mainFileName.section('.',0,-2);
        }
        else
        {
            mainFile = fileDir.relativeFilePath(prjDir.absolutePath())
                     + "/"
                     +  this->projectMan->getActive()->mainFilePath.section('.',0,-2);
        }

        options->m_device = this->projectMan->getActive()->family.toStdString();


        QDir templateDir(GuiCfg::getInstance().getTemplatePath());
        if (true == this->projectMan->getActive()->defaultVerilog)
        {
            options->m_verilogTemplate = ( templateDir.absolutePath()
                                        + "/"
                                        + this->projectMan->getActive()->family
                                        + ".v"
                                        ).toStdString();
        }
        else
        {
            options->m_verilogTemplate = this->projectMan->getActive()->templateVerilog.toStdString();
        }

        if (true == this->projectMan->getActive()->defaultVHDL)
        {
            options->m_vhdlTemplate = ( templateDir.absolutePath()
                                    + "/"
                                    + this->projectMan->getActive()->family
                                    + ".vhd"
                                    ).toStdString();
        }
        else
        {
            options->m_vhdlTemplate = this->projectMan->getActive()->templateVHDL.toStdString();
        }


        if (projectMan->getActive()->compileOpt.at(0))
        {
            options->m_symbolTable = (mainFile + ".stbl").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(1))
        {
            options->m_macroTable = (mainFile + ".mtbl").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(2))
        {
            options->m_mdsDebugFile = (mainFile + ".dbg").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(3))
        {
            options->m_codeTree = (mainFile + ".ctr").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(4))
        {
            options->m_lstFile = (mainFile + ".lst").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(5))
        {
            options->m_hexFile = (mainFile + ".ihex").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(6))
        {
            options->m_binFile = (mainFile + ".bin").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(7))
        {
            options->m_srecFile = (mainFile + ".srec").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(8))
        {
            options->m_memFile = ( mainFile + ".mem").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(9))
        {
            options->m_rawHexDumpFile = ( mainFile + ".rawhex" ).toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(10))
        {
            options->m_verilogFile = ( mainFile  + ".v").toStdString();
        }
        if (projectMan->getActive()->compileOpt.at(11))
        {
            options->m_vhdlFile = ( mainFile + ".vhd").toStdString();
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
    //delete options;
    /*QThread *thread = new QThread;
    CompileWidget *compiler = new CompileWidget(projectMan->getActive()->mainFileName, projectMan->getActive()->prjPath, projectMan->getActive()->langType);
    compiler->moveToThread(thread);
    connect(thread, SIGNAL(started()), compiler, SLOT(compile()));
    connect(compiler, SIGNAL(finished()), thread, SLOT(quit()));
    connect(compiler, SIGNAL(finished()), compiler, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(compiler, SIGNAL(write(QString)), this, SLOT(writeToWCompileInfo(QString)));
    thread->start();*/


    /*projectMan->createActiveMakefile();

    ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->clear();

    QStringList args;
    args << "-C" + projectMan->getActive()->prjPath.section('/',0, -2)+"/make/";
    QProcess compiler(this);

    compiler.setProcessChannelMode(QProcess::MergedChannels);
    compiler.start("make", args);
    if (!compiler.waitForFinished())
        ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->appendPlainText("Make failed:\n" + compiler.errorString());
    else
    {
        ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->appendPlainText("Make successful:\n" + compiler.readAll() + "\n\n");

        QProcess packihx(this);
        QStringList args2;
        QString projectMainName = projectMan->getActive()->mainFileName.section('.',0,-2);
        QString ihxPath = projectMan->getActive()->prjPath.section('/',0, -2)+"/make/" + projectMainName + ".ihx";
        QString hexPath = projectMan->getActive()->prjPath.section('/',0, -2)+"/make/" + projectMainName + ".hex";

        args2 << ihxPath;
        packihx.setProcessChannelMode(QProcess::MergedChannels);
        packihx.setStandardOutputFile(hexPath);
        packihx.start("packihx", args2);

        //vyresit chybu - oba pripady vypisuji packihx successful
        if (!packihx.waitForFinished())
            ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->appendPlainText("packihx failed: " + packihx.errorString());
        else
            ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->appendPlainText("packihx successful: " + packihx.readAll());
    }*/
}


/**
 * @brief Slot. Writes compilation exit status to CompileInfo
 * @param success Compiper exit status
 *
 */
void MainForm::compilationFinished(bool success)
{
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
                    qDebug() << filePath;
                    qDebug() << wDockManager->getCentralPath();
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
        qDebug() << "MainForm: sim file" << file;
        qDebug() << "MainForm: sim dump file" << dumpFiles;
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
                //TODO:ifdef
                toolSimLoggerAct->setEnabled(true);
                toolSimSwitchAct->setEnabled(true);
            projectConfigAct->setDisabled(true);
            projectMan->setSimulated(projectMan->getActive());
            if (true == simulationBreakpointsEnabled)
            {
                qDebug() << "MainForm: simulationBreakpointsEnabled true";
                projectMan->getSimulated()->setBreakpoints(true);
            }
            else
            {
                qDebug() << "MainForm: simulationBreakpointsEnabled false";
                projectMan->getSimulated()->setBreakpoints(false);
            }
        }
        else
        {
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
            //TODO:ifdef
            toolSimLoggerAct->setDisabled(true);
            toolSimSwitchAct->setDisabled(true);
        projectConfigAct->setEnabled(true);
        projectMan->getSimulated()->stop();
        this->unhighlight();
        this->wDockManager->getCentralTextEdit()->clearHighlight();
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
    qDebug() << "MainForm: exampleOpen";
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
    qDebug() << "MainForm: return exampleOpen";
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
        saveAct->setEnabled(true);
        saveAsAct->setEnabled(true);
        saveAllAct->setEnabled(true);
        saveProjAct->setEnabled(true);
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
        this->wDockManager->addUntrackedCentralWidget("ASM Translator","untracked",qText);
        //qDebug() << getWDockManager()->getCentralTextEdit()->toPlainText();
        getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
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
        this->wDockManager->addUntrackedCentralWidget("ASM Translator error","untracked",qText);
        //qDebug() << getWDockManager()->getCentralTextEdit()->toPlainText();
        this->getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
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
    else if (true == simulationAnimateStatus)
    {
        this->simulationAnimateHandle();
    }
}


void MainForm::activeProjectChanged(int index)
{
    qDebug() << "MainForm: activeProjectChanged";
    //if (false == this->simulationStatus)
    if (index >= 0)
    {
        projectMan->setActiveByIndex(index);
    qDebug() << "MainForm: activeProjectChanged2";
        wDockManager->changeSimWidget(index);
    qDebug() << "MainForm: activeProjectChanged3";
        if (wDockManager->getBreakpointList() != NULL)
        {
            wDockManager->getBreakpointList()->reload(projectMan->getActive()->getBreakpointsListRef());
        }
        if (wDockManager->getBookmarkList() != NULL)
        {
            wDockManager->getBookmarkList()->reload(projectMan->getActive()->getBookmarksListRef());
        }
    }
    qDebug() << "MainForm: return activeProjectChanged";
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
        qDebug() << "MainForm: delete active sim widget";
        wDockManager->deleteActiveSimWidget();
        qDebug() << "MainForm: remove dock widget";
        this->removeDockWidget(project->prjDockWidget);
        qDebug() << "MainForm: close project";
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
        }
    }
}


void MainForm::manageBreakpointEmit(QString file, int line)
{
    qDebug() << "MainForm: breakpoint:" << file << ":" << line + 1;
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
    qDebug() << "MainForm: bookmark:" << file << ":" << line + 1;
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
    qDebug() << "MainForm: bookmarksAddLines";
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
    qDebug() << "MainForm: session restoration";
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



void MainForm::closeEvent(QCloseEvent */*event*/)
{
    //TODO: save prompt
    QApplication::closeAllWindows();
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

    foreach (const QString &value, files)
    {
        if (false == this->getWDockManager()->setCentralByPath(value))
        {
            this->openFilePath(value);
            this->getWDockManager()->setCentralByPath(value);
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
                    this->getWDockManager()->setCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(i)))));
                    this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev.at(i)) - 1, colors.at(1));
                    //this->getWDockManager()->getCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(i)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev.at(i)), colors.at(1));
                }
            }
        }
        else
        {
            this->getWDockManager()->setCentralByPath(QString::fromStdString(*(std::get<0>(prev.at(0)))));
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
                    this->getWDockManager()->setCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))));
                    this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(prev2.at(i)) - 1, colors.at(2));
                    //this->getWDockManager()->getCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))))->getTextEdit()->highlightLineAppend(std::get<1>(prev2.at(0)), colors.at(2));
                }
            }
        }
        else
        {
            this->getWDockManager()->setCentralByPath(QString::fromStdString(*(std::get<0>(prev2.at(0)))));
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
                    this->getWDockManager()->setCentralByPath(QString::fromStdString(*(std::get<0>(curr.at(0)))));
                    this->getWDockManager()->getCentralTextEdit()->highlightLineAppend(std::get<1>(curr.at(i)) - 1, colors.at(0));
                }
            }
        }
        else
        {
            this->getWDockManager()->setCentralByPath(QString::fromStdString(*(std::get<0>(curr.at(0)))));
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
        qDebug() << "count: " << projectTabs->count();
        projectTabs->setCurrentIndex(projectTabs->count()-1);
        qDebug() << "MainForm: activeProject" << projectMan->getActive()->prjName;
}


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
    //TODO: ifdef
    if (true == this->simulationStatus)
    {
        SimSwitch *widget = new SimSwitch(0, this->projectMan->getSimulated()->getSimControl());
        widget->show();
    }
}


void MainForm::simPortLogger()
{
    //TODO: ifdef
    if (true == this->simulationStatus)
    {
        SimPortLogger *widget = new SimPortLogger(0, this->projectMan->getSimulated()->getSimControl());
        widget->show();
    }
}


void MainForm::userGuide()
{
    QDir dir(GuiCfg::getInstance().getUserGuidePath() + "/QuickUserGuideAssembler.pdf");
    QDesktopServices::openUrl(QUrl("file:///" + dir.absolutePath()));
    qDebug() << "user guide:" << dir.absolutePath();
}