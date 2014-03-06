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


#include <QtGui>
//#include <QtHelp/QHelpEngineCore>
//pozdeji zamenit QtGui za mensi celky
#include "mainform.h"
#include "../dialogs/projectdlg/projectdlg.h"
#include "../dialogs/disasmdlg/disasmdlg.h"
#include "../dialogs/translatordlg/translatordlg.h"
#include "../dialogs/fileconvertdlg/fileconvertdlg.h"
#include "../errordialog/errordlg.h"
#include "pluginman_gui.h"
#include "../dialogs/projectcfg/projectcfgdlg_core.h"
#include "../dialogs/interfacecfg/interfacecfgdlg_core.h"
//#include "../widgets/CompileWidget/compilewidget.h"
#include "../widgets/HelpWidget/helpwidget.h"
#include "../guicfg/guicfg.h"



    Q_DECLARE_METATYPE (std::string);
    Q_DECLARE_METATYPE (CompilerBase::MessageType);
/**
 * @brief Constructor, inits project and dock widget manager and create menus/toolbars
 */
MainForm::MainForm()
{
    //qDebug() << "MainForm: MainForm()";
    this->projectTabConnected = false;
    this->simulationStatus = false;
    this->simulationRunStatus = false;
    this->simulationAnimateStatus = false;
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
    //connect(this, SIGNAL(dockWidgetsCreated()), wDockManager, SLOT(dockWidgetsCreated()));
    connect(wDockManager,
            SIGNAL(createDockWidgets()),
            this,
            SLOT(createDockWidgets())
           );
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
            SIGNAL(centralCreated()),
            this,
            SLOT(enableSimActs())
           );
    connect(wDockManager,
            SIGNAL(centralClosed()),
            this,
            SLOT(disableSimActs())
           );
    //this->dockWidgets = false;
    createActions();
    createMenu();
    //createShortcuts();
    createToolbar();
    //CreateDockWidgets();
    //CreateWelcome();
    //qDebug() << "MainForm: return MainForm()";
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
    fileMenu->addAction(openAct);
    //fileMenu->addAction(addAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAllAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    interfaceMenu = menuBar()->addMenu(tr("&Interface"));
    interfaceMenu->addAction(interfaceConfigAct);
    //interfaceMenu->addAction(pluginAct);

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newProjAct);
    projectMenu->addAction(openProjAct);
    projectMenu->addAction(saveProjAct);
    projectMenu->addAction(projectCompileAct);
    projectMenu->addAction(projectConfigAct);

    simulationMenu = menuBar()->addMenu(tr("&Simulation"));
    simulationMenu->addAction(simulationFlowAct);
    simulationMenu->addAction(simulationRunAct);
    simulationMenu->addAction(simulationAnimateAct);
    simulationMenu->addAction(simulationStepAct);
    simulationMenu->addAction(simulationResetAct);
    simulationMenu->addAction(simulationUnhighlightAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(toolDisassemblerAct);
    toolsMenu->addAction(toolTranslatorAct);
    toolsMenu->addAction(toolFileConvertAct);
    toolsMenu->addAction(toolConvertorAct);
    toolsMenu->addAction(toolDisplayAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQTAct);
    helpMenu->addAction(helpActionAct);
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
    newAct = new QAction(tr("New File"), this);
    newAct->setStatusTip("Create a new file");
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    newAct->setShortcut(QKeySequence("Ctrl+N"));

    openAct = new QAction(tr("Open File"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    openAct->setShortcut(QKeySequence("Ctrl+O"));



    /*QPixmap *pm_projAdd = new QPixmap(":/resources//icons//projAdd.png");
    QIcon *icon_projAdd = new QIcon(*pm_projAdd);
    addAct = new QAction(*icon_projAdd, tr("Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));
    addAct->setDisabled(true);*/


    this->pm_projNewAdd = new QPixmap(":/resources//icons//projNewAdd.png");
    this->icon_projNewAdd = new QIcon(*pm_projNewAdd);
    newAddAct = new QAction(*icon_projNewAdd, tr("New project file"), this);
    connect(newAddAct, SIGNAL(triggered()), this, SLOT(newAddFile()));
    newAddAct->setDisabled(true);


    /*QPixmap *pm_projRemoveFile = new QPixmap("src//resources//icons//projDelete.png");
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




    //INTERFACE
    interfaceConfigAct = new QAction(tr("Config"), this);
    connect(interfaceConfigAct, SIGNAL(triggered()), this, SLOT(interfaceConfig()));
    //pluginAct = new QAction(tr("Plugins"), this);
    //connect(pluginAct, SIGNAL(triggered()), this, SLOT(showPlugins()));



    
    this->pm_projNew = new QPixmap(":/resources//icons//projNew.png");
    this->icon_projNew = new QIcon(*pm_projNew);
    newProjAct = new QAction(*icon_projNew, tr("New Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    this->pm_projOpen = new QPixmap(":/resources//icons//projOpen.png");
    this->icon_projOpen = new QIcon(*pm_projOpen);
    openProjAct = new QAction(*icon_projOpen, tr("Open Project"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    this->pm_projSave = new QPixmap(":/resources//icons//projSave.png");
    this->icon_projSave = new QIcon(*pm_projSave);
    saveProjAct = new QAction(*icon_projSave, tr("Save Project"), this);
    connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));
    saveProjAct->setDisabled(true);

    exitAct = new QAction(tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    this->pm_projComp = new QPixmap(":/resources//icons//compile.png");
    this->icon_projComp = new QIcon(*pm_projComp);
    projectCompileAct = new QAction(*icon_projComp, tr("Compile"), this);
    connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));
    projectCompileAct->setDisabled(true);
    projectCompileAct->setShortcut(Qt::Key_F5);

    projectConfigAct = new QAction(tr("Config"), this);
    connect(projectConfigAct, SIGNAL(triggered()), this, SLOT(projectConfig()));
    projectConfigAct->setDisabled(true);
    //connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));


    this->pm_simFlowStart = new QPixmap(":/resources//icons//simulationStart.png");
    this->pm_simFlowStop = new QPixmap(":/resources//icons//simulationStop.png");
    this->icon_simFlow = new QIcon(*pm_simFlowStart);
    simulationFlowAct = new QAction(*icon_simFlow, tr("Start simulation"), this);
    connect(simulationFlowAct, SIGNAL(triggered()), this, SLOT(simulationFlowHandle()));
    simulationStatus = false;
    simulationRunStatus = false;
    simulationAnimateStatus = false;
    simulationFlowAct->setDisabled(true);
    simulationFlowAct->setShortcut(Qt::Key_F6);

    this->pm_simRun = new QPixmap(":/resources//icons//simulationRun.png");
    this->icon_simRun = new QIcon(*pm_simRun);
    simulationRunAct = new QAction(*icon_simRun, tr("Run"), this);
    connect(simulationRunAct, SIGNAL(triggered()), this, SLOT(simulationRunHandle()));
    simulationRunAct->setDisabled(true);
    simulationRunAct->setShortcut(Qt::Key_F7);

    this->pm_simAnimate = new QPixmap(":/resources//icons//simulationAnimate.png");
    this->icon_simAnimate = new QIcon(*pm_simAnimate);
    simulationAnimateAct = new QAction(*icon_simAnimate, tr("Animate"), this);
    connect(simulationAnimateAct, SIGNAL(triggered()), this, SLOT(simulationAnimateHandle()));
    simulationAnimateAct->setDisabled(true);
    simulationAnimateAct->setShortcut(Qt::Key_F8);

    this->pm_simStep = new QPixmap(":/resources//icons//simulationStep.png");
    this->icon_simStep = new QIcon(*pm_simStep);
    simulationStepAct = new QAction(*icon_simStep, tr("Do step"), this);
    connect(simulationStepAct, SIGNAL(triggered()), this, SLOT(simulationStep()));
    simulationStepAct->setDisabled(true);
    simulationStepAct->setShortcut(Qt::Key_F9);

    this->pm_simReset = new QPixmap(":/resources//icons//simulationReset.png");
    this->icon_simReset = new QIcon(*pm_simReset);
    simulationResetAct = new QAction(*icon_simReset, tr("Reset"), this);
    connect(simulationResetAct, SIGNAL(triggered()), this, SLOT(simulationReset()));
    simulationResetAct->setDisabled(true);
    simulationResetAct->setShortcut(Qt::Key_F10);

    this->pm_simUnhighlight = new QPixmap(":/resources//icons//unhighlight.png");
    this->icon_simUnhighlight = new QIcon(*pm_simUnhighlight);
    simulationUnhighlightAct = new QAction(*icon_simUnhighlight, tr("Unhighlight"), this);
    connect(simulationUnhighlightAct, SIGNAL(triggered()), this, SLOT(unhighlight()));
    simulationUnhighlightAct->setDisabled(true);

    this->pm_toolDis = new QPixmap(":/resources//icons//disassemble.png");
    this->icon_toolDis = new QIcon(*pm_toolDis);
    toolDisassemblerAct = new QAction(*icon_toolDis, tr("Disassemble"), this);
    connect(toolDisassemblerAct, SIGNAL(triggered()), this, SLOT(toolDisassemble()));
    toolTranslatorAct = new QAction(tr("ASM Translator"), this);
    connect(toolTranslatorAct, SIGNAL(triggered()), this, SLOT(toolTranslate()));
    toolFileConvertAct = new QAction(tr("DataFile Convertor"), this);
    connect(toolFileConvertAct, SIGNAL(triggered()), this, SLOT(toolFileConvert()));
    toolConvertorAct = new QAction(tr("Convertor"), this);
    connect(toolConvertorAct, SIGNAL(triggered()), this, SLOT(toolConvertor()));
    toolDisplayAct = new QAction(tr("Segment Display"), this);
    connect(toolDisplayAct, SIGNAL(triggered()), this, SLOT(toolDisplay()));

    aboutAct = new QAction(tr("About"), this);
    aboutQTAct = new QAction(tr("About QT"), this);
    connect(aboutQTAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    helpActionAct = new QAction(tr("Help"), this);
    connect(helpActionAct, SIGNAL(triggered()), this, SLOT(help()));
    example1Act = new QAction(tr("Example 1"), this);
    connect(example1Act, SIGNAL(triggered()), this, SLOT(exampleOpen()));

    this->pm_cross = new QPixmap(":/resources//icons//cross.png");
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

    projectToolBar->setAllowedAreas(Qt::TopToolBarArea);
    simulationToolBar->setAllowedAreas(Qt::TopToolBarArea);
    //fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, projectToolBar);
    addToolBar(Qt::TopToolBarArea, simulationToolBar);
    //addToolBar(Qt::TopToolBarArea, fileToolBar);
    //qDebug() << "MainForm: return CreateToolbar()";
}





//nacteni defaultnich widgetu podle ulozeneho nastaveni
//v tuto chvili jen informacni defaultni layout
//s kazdym novym widgetem se zavola prislusna funkce s telem podobnym teto funkci
//pojmenovani widgetu zacinat s "wid", popr. "w"
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

        //mozno stejne jako u WDockManager - ulozit si ptr na okno
        //projectMan->addProject(NULL, NULL, NULL);

        //wDockManager->addDockWidget(wListCode);
        //wDockManager->addDockWidget(wListCode2);
        wDockManager->addDockWidget(wCompileInfo);
        wDockManager->addDockWidget(wSimulationInfo);
        wDockManager->addDockWidget(wBottomHide);
        //wDockManager->addDockWidget(wBookmarkList);
        wDockManager->addDockWidget(wBreakpointList);
        //wDockManager->addDockWidget(wAnalysVar);
        //wDockManager->addDockWidget(wAnalysFunc);
        //addAct->setEnabled(true);
        newAddAct->setEnabled(true);
        saveProjAct->setEnabled(true);
        //projectCompileAct->setEnabled(true);
        //simulationFlowAct->setEnabled(true);
        saveAct->setEnabled(true);
        saveAsAct->setEnabled(true);
        saveAllAct->setEnabled(true);
        wDockManager->dockWidgets = true;
        QList<QTabBar*> tabList = this->findChildren<QTabBar*>();
        wDockManager->bottomAreaTabs = tabList.at(tabList.size()-1);
        connect(tabList.at(tabList.size()-1),
                SIGNAL(currentChanged(int)),
                wDockManager,
                SLOT(handleShowHideBottom(int))
               );

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
    wDockManager->addCentralWidget("Welcome", "Tips from developers.");
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
        }
    }
    //qDebug() << "MainForm: return openFile()";
}


/**
 * @brief Opens file selected by programmer.
 * @param path Path to the file.
 */
void MainForm::openFilePath(QString path)
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
            //qDebug() << "MainForm: addCentralWidget";
            wDockManager->addCentralWidget(path.section('/', -1), path);
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            file.close();
            //qDebug() << "MainForm: connect";
            wDockManager->getCentralWidget()->connectAct();
            //qDebug() << "MainForm: set parent";
            wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
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
                //wDockManager->getCentralWidget()->setSaved();
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
    //qDebug() << "MainForm: openProject()";
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
        else {
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
            error(ERR_OPENFILE);
            return false;
        }
        else
        {
            //nacteni obsahu do widgetu
            projectMan->openProject(&file);
            file.close();
            return true;
        }
    }
    return false;
    //qDebug() << "MainForm: return openProject()";
}


void MainForm::projectOpened()
{
    //qDebug() << "MainForm: projectOpened";
    projectConfigAct->setEnabled(true);
    QList<QTabBar*> tabList = this->findChildren<QTabBar*>();
    if (false == projectTabConnected && tabList.size() > 1)
    {
        projectTabConnected = true;
        connect(tabList.at(tabList.size()-1), SIGNAL(currentChanged(int)), this, SLOT(activeProjectChanged(int)));
    }
    this->createDockWidgets();
}


/**
 * @brief Slot. Saves all changed files in active project.
 */
void MainForm::saveProject()
{
    //qDebug() << "MainForm: saveProject()";
    for (int i = 0; i < wDockManager->getTabCount(); i++)
    {
        if (wDockManager->getTabWidget(i)->isChild(projectMan->getActive()) == true)
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
        bool found = false;
        
        for (int i = 0; i < projectMan->getActive()->filePaths.count(); i++)
        {
            fileDir.setPath(prjDir.absolutePath()
                            + "/"
                            + projectMan->getActive()->filePaths.at(i).section('/',0, -2)
                           );
            //qDebug() << "MainForm: central path:" << wDockManager->getCentralPath();
            //qDebug() << "MainForm: file path" << QDir::cleanPath(fileDir.absolutePath() + "/" + projectMan->getActive()->fileNames.at(i));
            if (QDir::cleanPath(fileDir.absolutePath() + "/" + projectMan->getActive()->fileNames.at(i)) == wDockManager->getCentralPath())
            {
                found = true;
                break;
            }
        }
        if (found == true)
        {
            qDebug() << "MainForm: compiled actual project, actual file";
            options->m_sourceFiles.push_back(  (projectMan->getActive()->prjPath.section('/',0, -2)
                                                + "/"
                                                +  wDockManager->getCentralName()
                                                ).toStdString()
                                            );


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
            

            
            mainFile = prjDir.absolutePath()
                     + "/"
                     +  wDockManager->getCentralName().section('.',0,-2);

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

            mainFile = pathDir.absolutePath() + "/" + wDockManager->getCentralName();


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
                     +  this->projectMan->getActive()->mainFileName.section('.',0,-2);
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
    }
    else
    {
        ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->setFinished(false);
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
        projectMan->getActive()->step();
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
            this->simulationRunStatus = false;
        }
        else
        {
            this->icon_simRun = new QIcon(*pm_cross);
            simulationRunAct->setIcon(*icon_simRun);
            simulationRunAct->setText(tr("Stop run"));
            this->simulationAnimateAct->setDisabled(true);
            this->simulationStepAct->setDisabled(true);
            this->simulationResetAct->setDisabled(true);
            this->simulationRunStatus = true;
        }
        projectMan->getActive()->run();
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
            this->simulationAnimateStatus = false;
        }
        else
        {
            this->icon_simAnimate = new QIcon(*pm_cross);
            simulationAnimateAct->setIcon(*icon_simAnimate);
            simulationAnimateAct->setText(tr("Stop animate"));
            this->simulationRunAct->setDisabled(true);
            this->simulationStepAct->setDisabled(true);
            this->simulationResetAct->setDisabled(true);
            this->simulationAnimateStatus = true;
        }
        projectMan->getActive()->animate();
    }
}


/**
 * @brief Slot. Resets active simulation.
 */
void MainForm::simulationReset()
{
    if (true == simulationStatus)
    {
        projectMan->getActive()->reset();
    }
}


/**
 * @brief Slot. Starts/stops simulation.
 */
void MainForm::simulationFlowHandle()
{
    QString file = "";
    if (projectMan->getActive() == NULL)
    {
        error(ERR_NO_PROJECT);
    }
    if (projectMan->getActive()->prjPath == "untracked")
    {
        file = GuiCfg::getInstance().getTempPath() + "/" + wDockManager->getCentralName();
    }
    else
    {
        if ( false == projectMan->getActive()->useMainFile )
        {
            //check if enabled, if it isnt, simulate
            QDir prjDir(projectMan->getActive()->prjPath.section('/',0, -2));
            QDir fileDir;
            bool found = false;

            for (int i = 0; i < projectMan->getActive()->filePaths.count(); i++)
            {
                fileDir.setPath(prjDir.absolutePath()
                                + "/"
                                + projectMan->getActive()->filePaths.at(i).section('/',0, -2)
                            );
                //qDebug() << "MainForm: central path:" << wDockManager->getCentralPath();
                //qDebug() << "MainForm: file path" << QDir::cleanPath(fileDir.absolutePath() + "/" + projectMan->getActive()->fileNames.at(i));
                if (QDir::cleanPath(fileDir.absolutePath() + "/" + projectMan->getActive()->fileNames.at(i)) == wDockManager->getCentralPath())
                {
                    found = true;
                    break;
                }
            }
            if (true == found)
            {
                file = wDockManager->getCentralPath();
            }
            else
            {
                projectMan->setActive(projectMan->getUntracked());
                file = GuiCfg::getInstance().getTempPath() + "/" + wDockManager->getCentralName();
            }
        }
        //else
        //{
            //simulate main file
            //file = "";
        //}
    }
    if (false == simulationStatus)
    {
        if ( true == projectMan->getActive()->start(file) )
        {
            delete this->icon_simFlow;
            this->icon_simFlow = new QIcon(*pm_simFlowStop);
            simulationFlowAct->setIcon(*icon_simFlow);
            simulationFlowAct->setText(tr("Stop simulation"));
            simulationStatus = true;
            simulationStepAct->setEnabled(true);
            simulationRunAct->setEnabled(true);
            simulationAnimateAct->setEnabled(true);
            simulationResetAct->setEnabled(true);
            simulationUnhighlightAct->setEnabled(true);
            projectMan->setSimulated(projectMan->getActive());
        }
        else
        {
            error(ERR_SIM_NOSTART);
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
        simulationResetAct->setDisabled(true);
        simulationUnhighlightAct->setDisabled(true);
        projectMan->getSimulated()->stop();
        this->unhighlight();
    }
}


/**
 * @brief Slot. Shows tool - convertor.
 */
void MainForm::toolConvertor()
{
    ConvertorTool *a = new ConvertorTool(0);
}


/**
 * @brief Slot. Shown tool - 8 Segment Editor.
 */
void MainForm::toolDisplay()
{
    DisplayTool *a = new DisplayTool(0);
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
    if (false == this->openProject(GuiCfg::getInstance().getExamplePath() + "/Example.mds-project"))
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
    wDockManager->addSimDockWidgetP2(this->getProjectMan()->getActive()->prjPath, this->getProjectMan()->getActive()->getSimControl());
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
    connect(project, SIGNAL(highlightLine(QString, int, QColor*)), this, SLOT(highlightLine(QString, int, QColor*)));
    connect(project, SIGNAL(setCentralByName(QString)), this, SLOT(setCentralByName(QString)));
    //connect(project, SIGNAL(scrollToLine(int)), this, SLOT(scrollCentralToLine(int)));
    connect(project, SIGNAL(addUntrackedFile(QString, QString)), this, SLOT(addUntrackedFile(QString, QString)));
    connect(project, SIGNAL(openFilePath(QString)), this, SLOT(openFilePath(QString)));
    connect(project, SIGNAL(setEditorReadOnly(bool)), this, SLOT(setEditorReadOnly(bool)));
    connect(project, SIGNAL(startConfig(Project*)), this, SLOT(startProjectConfig(Project*)));
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
        getWDockManager()->addUntrackedCentralWidget(name, path);
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
    cfgdlg->exec();
}


/**
 * @brief
 */
void MainForm::help()
{
    HelpWidget *helpWidget = new HelpWidget(0, this->width(), this->height());
}


/**
 * @brief
 */
void MainForm::toolDisassemble()
{
    DisAsmDialog *dlg = new DisAsmDialog(this);
    connect(dlg, SIGNAL(output(std::vector<std::string>)), this, SLOT(disassembleOutput(std::vector<std::string>)));
}


/**
 * @brief
 */
void MainForm::disassembleOutput(std::vector<std::string> text)
{
    QStringList qText;
    for (unsigned int i = 0; i < text.size(); i++)
    {
        qText.append(QString::fromStdString(text.at(i)));
    }
    //QString name = this->projectMan->addUntrackedFile(NULL, "disasm");
    this->wDockManager->addUntrackedCentralWidget("disasm","untracked",qText);
    getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
    //getWDockManager()->getCentralWidget()->connectAct();
}


/**
 * @brief
 */
void MainForm::toolTranslate()
{
    TranslatorDlg *dlg = new TranslatorDlg(this);
    connect(dlg,
            SIGNAL(output(std::vector<std::string> &)),
            this,
            SLOT(translatorOutput(std::vector<std::string> &)));
    connect(dlg,
            SIGNAL(outputError(const std::vector<std::pair<unsigned int, std::string>> &)),
            this,
            SLOT(translatorOutput(const std::vector<std::pair<unsigned int, std::string>> &)));
}


/**
 * @brief
 */
void MainForm::translatorOutput(std::vector<std::string> & text)
{
    QStringList qText;
    for (unsigned int i = 0; i < text.size(); i++)
    {
        qText.append(QString::fromStdString(text.at(i)));
    }
    //QString name = this->projectMan->addUntrackedFile(NULL, "disasm");
    this->wDockManager->addUntrackedCentralWidget("ASM Translator","untracked",qText);
    getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
    //getWDockManager()->getCentralWidget()->connectAct();
}


/**
 * @brief
 */
void MainForm::translatorOutput(const std::vector<std::pair<unsigned int, std::string>> & text)
{
    QStringList qText;
    for (unsigned int i = 0; i < text.size(); i++)
    {
        qText.append(QString::fromStdString(std::get<1>(text.at(i))));
    }
    //QString name = this->projectMan->addUntrackedFile(NULL, "disasm");
    this->wDockManager->addUntrackedCentralWidget("ASM Translator error","untracked",qText);
    this->getWDockManager()->getCentralTextEdit()->reloadHighlighter(PICOBLAZEASM);
    //getWDockManager()->getCentralWidget()->connectAct();
}


/**
 * @brief
 */
void MainForm::toolFileConvert()
{
    FileConvertDlg *dlg = new FileConvertDlg(this);
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
    this->startProjectConfig(this->projectMan->getActive());
}


void MainForm::interfaceConfig()
{
    InterfaceCfgDlg_Core *cfgdlg = new InterfaceCfgDlg_Core(this);
    cfgdlg->exec();
}


void MainForm::stopSimSlot()
{
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
    if (false == this->simulationStatus)
    {
        wDockManager->changeSimWidget(index);
        projectMan->setActiveByIndex(index);
    }
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
}