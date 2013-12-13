/**
 * @brief C++ file for MainForm class
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
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



    Q_DECLARE_METATYPE (std::string);
    Q_DECLARE_METATYPE (CompilerBase::MessageType);
/**
 * @brief Constructor, inits project and dock widget manager and create menus/toolbars
 */
MainForm::MainForm()
{
    //qDebug() << "MainForm: MainForm()";
    projectMan = new ProjectMan(this);
    connect(projectMan, SIGNAL(addDockWidget(Qt::DockWidgetArea, QDockWidget*)), this, SLOT(addDockWidgetSlot(Qt::DockWidgetArea, QDockWidget*)));
    connect(projectMan, SIGNAL(tabifyDockWidget(QDockWidget*, QDockWidget*)), this, SLOT(tabifyDockWidgetSlot(QDockWidget*, QDockWidget*)));
    connect(projectMan, SIGNAL(connectProject(Project*)), this, SLOT(connectProjectSlot(Project*)));
    QWidget *centralWidget = new QWidget(this);
    wDockManager = new WDockManager(this, centralWidget);
    this->setCentralWidget(centralWidget);
    //connect(this, SIGNAL(dockWidgetsCreated()), wDockManager, SLOT(dockWidgetsCreated()));
    connect(wDockManager, SIGNAL(createDockWidgets()), this, SLOT(createDockWidgets()));
    connect(wDockManager, SIGNAL(tabifyDockWidget(QDockWidget*, QDockWidget*)), this, SLOT(tabifyDockWidgetSlot(QDockWidget*, QDockWidget*)));
    connect(wDockManager, SIGNAL(addDockWidget(Qt::DockWidgetArea, QDockWidget*)), this, SLOT(addDockWidgetSlot(Qt::DockWidgetArea, QDockWidget*)));
    connect(wDockManager, SIGNAL(getSimProjectData()), this, SLOT(simProjectData()));
    connect(this, SIGNAL(unhighlightSim()), wDockManager, SLOT(unhighlightSimWidget()));
    //this->dockWidgets = false;
    createActions();
    createMenu();
    createShortcuts();
    createToolbar();
    //CreateDockWidgets();
    //CreateWelcome();
    //qDebug() << "MainForm: return MainForm()";
}



/**
 * @brief Creates menus in main menu
 */
void MainForm::createMenu()
{
    //qDebug() << "MainForm: createMenu()";
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(addAct);
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
    simulationMenu->addAction(simulationStepAct);
    simulationMenu->addAction(simulationRunAct);
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

    openAct = new QAction(tr("Open File"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));



    QPixmap *pm_projAdd = new QPixmap(":/resources//icons//projAdd.png");
    QIcon *icon_projAdd = new QIcon(*pm_projAdd);
    addAct = new QAction(*icon_projAdd, tr("Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));
    addAct->setDisabled(true);


    QPixmap *pm_projNewAdd = new QPixmap(":/resources//icons//projNewAdd.png");
    QIcon *icon_projNewAdd = new QIcon(*pm_projNewAdd);
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



    
    QPixmap *pm_projNew = new QPixmap(":/resources//icons//projNew.png");
    QIcon *icon_projNew = new QIcon(*pm_projNew);
    newProjAct = new QAction(*icon_projNew, tr("New Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    QPixmap *pm_projOpen = new QPixmap(":/resources//icons//projOpen.png");
    QIcon *icon_projOpen = new QIcon(*pm_projOpen);
    openProjAct = new QAction(*icon_projOpen, tr("Open Project"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    QPixmap *pm_projSave = new QPixmap(":/resources//icons//projSave.png");
    QIcon *icon_projSave = new QIcon(*pm_projSave);
    saveProjAct = new QAction(*icon_projSave, tr("Save Project"), this);
    connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));
    saveProjAct->setDisabled(true);

    exitAct = new QAction(tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    QPixmap *pm_projComp = new QPixmap(":/resources//icons//compile.png");
    QIcon *icon_projComp = new QIcon(*pm_projComp);
    projectCompileAct = new QAction(*icon_projComp, tr("Compile"), this);
    connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));
    projectCompileAct->setDisabled(true);

    projectConfigAct = new QAction(tr("Config"), this);
    connect(projectConfigAct, SIGNAL(triggered()), this, SLOT(projectConfig()));
    projectConfigAct->setDisabled(true);
    //connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));


    QPixmap *pm_simFlow = new QPixmap(":/resources//icons//simulationStart.png");
    QIcon *icon_simFlow = new QIcon(*pm_simFlow);
    simulationFlowAct = new QAction(*icon_simFlow, tr("Start simulation"), this);
    connect(simulationFlowAct, SIGNAL(triggered()), this, SLOT(simulationFlowHandle()));
    simulationStatus = false;
    simulationFlowAct->setDisabled(true);

    QPixmap *pm_simRun = new QPixmap(":/resources//icons//simulationRun.png");
    QIcon *icon_simRun = new QIcon(*pm_simRun);
    simulationRunAct = new QAction(*icon_simRun, tr("Run"), this);
    connect(simulationRunAct, SIGNAL(triggered()), this, SLOT(simulationRunHandle()));
    simulationRunAct->setDisabled(true);

    QPixmap *pm_simStep = new QPixmap(":/resources//icons//simulationStep.png");
    QIcon *icon_simStep = new QIcon(*pm_simStep);
    simulationStepAct = new QAction(*icon_simStep, tr("Do step"), this);
    connect(simulationStepAct, SIGNAL(triggered()), this, SLOT(simulationStep()));
    simulationStepAct->setDisabled(true);

    QPixmap *pm_simReset = new QPixmap(":/resources//icons//simulationReset.png");
    QIcon *icon_simReset = new QIcon(*pm_simReset);
    simulationResetAct = new QAction(*icon_simReset, tr("Reset"), this);
    connect(simulationResetAct, SIGNAL(triggered()), this, SLOT(simulationReset()));
    simulationResetAct->setDisabled(true);

    QPixmap *pm_simUnhighlight = new QPixmap(":/resources//icons//unhighlight.png");
    QIcon *icon_simUnhighlight = new QIcon(*pm_simUnhighlight);
    simulationUnhighlightAct = new QAction(*icon_simUnhighlight, tr("Unhighlight"), this);
    connect(simulationUnhighlightAct, SIGNAL(triggered()), this, SLOT(unhighlight()));
    simulationUnhighlightAct->setDisabled(true);

    QPixmap *pm_toolDis = new QPixmap(":/resources//icons//disassemble.png");
    QIcon *icon_toolDis = new QIcon(*pm_toolDis);
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
    //qDebug() << "MainForm: return CreateActions()";
}


void MainForm::createShortcuts()
{
    QShortcut *srtSave = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(srtSave, SIGNAL(activated()), this, SLOT(saveFile()));
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
    projectToolBar->addAction(addAct);

    //fileToolBar->addAction(saveAct);


    simulationToolBar->addAction(projectCompileAct);
    simulationToolBar->addAction(simulationFlowAct);
    simulationToolBar->addAction(simulationRunAct);
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
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //mozno stejne jako u WDockManager - ulozit si ptr na okno
    //projectMan->addProject(NULL, NULL, NULL);

    //wDockManager->addDockWidget(wListCode);
    //wDockManager->addDockWidget(wListCode2);
    wDockManager->addDockWidget(wCompileInfo);
    wDockManager->addDockWidget(wSimulationInfo);
    //wDockManager->addDockWidget(wBookmarkList);
    //wDockManager->addDockWidget(wBreakpointList);
    //wDockManager->addDockWidget(wAnalysVar);
    //wDockManager->addDockWidget(wAnalysFunc);
    addAct->setEnabled(true);
    newAddAct->setEnabled(true);
    saveProjAct->setEnabled(true);
    projectCompileAct->setEnabled(true);
    simulationFlowAct->setEnabled(true);
    projectConfigAct->setEnabled(true);
    saveAct->setEnabled(true);
    wDockManager->dockWidgets = true;
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
        else {
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
            wDockManager->addCentralWidget(path.section('/', -1), path);
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            file.close();
            wDockManager->getCentralWidget()->connectAct();
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
        if (wDockManager->getCentralPath() == NULL) {
            path = QFileDialog::getSaveFileName(this, tr("Source File"));
            wDockManager->setCentralPath(path);
            wDockManager->setCentralName(path.section('/', -1));
        }
        else
            path = wDockManager->getCentralPath();

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
    if (wDockManager->getCentralWidget()->isChanged() == true)
    {
        QString path;
        if (wDockManager->getCentralPath() == NULL) {
            //path = QFileDialog::getSaveFileName(this, tr("Source File");
            path = QFileDialog::getSaveFileName (this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
            wDockManager->setCentralPath(path);
            wDockManager->setCentralName(path.section('/', -1));
        }
        else
            path = wDockManager->getCentralPath();

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
            //qDebug() << "mainform: file saved";
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
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        error(ERR_OPENFILE);
    }
    else {
        QTextStream fout(&file);
        fout << wDockManager->getCentralTextEdit()->toPlainText();
        file.close();
        wDockManager->setCentralPath(path);
        wDockManager->setCentralName(path.section('/', -1));
        wDockManager->getCentralWidget()->setSaved();
    }
    //qDebug() << "MainForm: return saveFileAs()";
}


/**
 * @brief Saves file from selected CodeEditor.
 */
void MainForm::saveFile(CodeEdit *editor)
{
    //qDebug() << "MainForm: saveFile()";
    if (editor->isChanged() == true)
    {
        QString path;
        if (editor->getPath() == NULL) {
            //path = QFileDialog::getSaveFileName(this, tr("Source File");
            path = QFileDialog::getSaveFileName(this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
            editor->setPath(path);
            editor->setName(path.section('/', -1));
        }
        else
        {
            path = editor->getPath();
        }
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
        if (wDockManager->getTabWidget(i)->isChanged() == true)
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
    QString path = QFileDialog::getOpenFileName (this, tr("Project Directory"), tr("Project (*.mmp)"));
    
    if (path.isEmpty() == false)
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
void MainForm::openProject(QString path)
{
    //qDebug() << "MainForm: openProject()";
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
    //qDebug() << "MainForm: return openProject()";
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
 * @bug Segfault if no project is active (opened).
 * @details Still not finished. Add functionality for PIC/ASM...
 */
void MainForm::compileProject()
{
    if (projectMan->getActive() == NULL)
    {
        error(ERR_NO_PROJECT);
        return;
    }

    
    ((CompileInfo*)(wDockManager->getDockWidget(wCompileInfo)->widget()))->clear();

    CompilerOptions *options = new CompilerOptions();
    //QString mainFile =  this->projectMan->getActive()->mainFileName.section('.',0,-2);
    options->m_sourceFile = (projectMan->getActive()->prjPath.section('/',0, -2) + "/" +  this->projectMan->getActive()->mainFilePath).toStdString();
    QDir prjDir(projectMan->getActive()->prjPath.section('/',0, -2));
    QDir fileDir(QString::fromStdString(options->m_sourceFile).section('/',0, -2));
    QString mainFile;
    
    if (fileDir.relativeFilePath(prjDir.absolutePath()) == "")
    {
        mainFile = "./" +  this->projectMan->getActive()->mainFileName.section('.',0,-2);
    }
    else
    {
        mainFile = fileDir.relativeFilePath(prjDir.absolutePath()) + "/" +  this->projectMan->getActive()->mainFileName.section('.',0,-2);
    }
    
    //qDebug() << QString::fromStdString(options->m_sourceFile);
    //qDebug() << mainFile;

    options->m_device = this->projectMan->getActive()->architecture.toStdString();
    
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
        options->m_hexFile = (mainFile + ".hex").toStdString();
    }
    if (projectMan->getActive()->compileOpt.at(6))
    {
        options->m_binFile = (mainFile + ".bin").toStdString();
    }
    if (projectMan->getActive()->compileOpt.at(7))
    {
        options->m_srecFile = (mainFile + ".srec").toStdString();
    }
    
    CompilerThread *compiler = new CompilerThread("../compiler/include/");
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<CompilerBase::MessageType>("CompilerBase::MessageType");
    connect(compiler, SIGNAL(compilationMessage(const std::string&, CompilerBase::MessageType)), this, SLOT(reloadCompileInfo(const std::string&, CompilerBase::MessageType)));
    connect(compiler, SIGNAL(compilationFinished (bool)), this, SLOT(compilationFinished(bool)));
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
 * @brief Slot. Run simulation with shown changes.
 */
void MainForm::simulationRunHandle()
{
    if (true == simulationStatus)
    {
        projectMan->getActive()->run();
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
    if (projectMan->getActive() != NULL)
    {
        if (false == simulationStatus)
        {
            if ( true == projectMan->getActive()->start() )
            {
                QPixmap *pm_simFlow = new QPixmap("resources//icons//simulationStop.png");
                QIcon *icon_simFlow = new QIcon(*pm_simFlow);
                simulationFlowAct->setIcon(*icon_simFlow);
                simulationFlowAct->setText(tr("Stop simulation"));
                simulationStatus = true;
                simulationStepAct->setEnabled(true);
                simulationRunAct->setEnabled(true);
                simulationResetAct->setEnabled(true);
                simulationUnhighlightAct->setEnabled(true);
            }
        }
        else
        {
            QPixmap *pm_simFlow = new QPixmap("resources//icons//simulationStart.png");
            QIcon *icon_simFlow = new QIcon(*pm_simFlow);
            simulationFlowAct->setIcon(*icon_simFlow);
            simulationFlowAct->setText(tr("Start simulation"));
            simulationStatus = false;
            simulationStepAct->setDisabled(true);
            simulationRunAct->setDisabled(true);
            simulationResetAct->setDisabled(true);
            simulationUnhighlightAct->setDisabled(true);
            projectMan->getActive()->stop();
        }
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
    this->openProject("./demoprojekt/Example/Example.mmp");
    int count = this->projectMan->getActive()->filePaths.count();
    QDir projectDir = QFileInfo(this->projectMan->getActive()->prjPath).dir();
    QString absolutePath = projectDir.path();
    for (int i = 0; i < count; i++)
    {
        this->openFilePath(QDir(absolutePath + "/" + this->projectMan->getActive()->filePaths.at(i)).canonicalPath());
    }
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
    connect(project, SIGNAL(scrollToLine(int)), this, SLOT(scrollCentralToLine(int)));
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
        if (this->getWDockManager()->getCentralTextEdit()->highlightLine(line, color) == true)
        {
            this->getWDockManager()->getCentralTextEdit()->scrollToLine(line);
        }
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
    getWDockManager()->addUntrackedCentralWidget(name, path);
    //getWDockManager()->getCentralWidget()->setChanged();
    //getWDockManager()->getCentralWidget()->connectAct();
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
    connect(dlg, SIGNAL(output(std::vector<std::string>)), this, SLOT(translatorOutput(std::vector<std::string>)));
}


/**
 * @brief
 */
void MainForm::translatorOutput(std::vector<std::string> text)
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