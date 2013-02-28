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
//pozdeji zamenit QtGui za mensi celky
#include "mainform.h"
#include "../dialogs/projectdlg.h"
#include "../dialogs/errordlg.h"



/**
 * @brief Constructor, inits project and dock widget manager and create menus/toolbars
 */
MainForm::MainForm()
{
    projectMan = new ProjectMan(this);
    wDockManager = new WDockManager(this);
    this->dockWidgets = false;
    CreateActions();
    CreateMenu();
    CreateToolbar();
    //CreateDockWidgets();
    //CreateWelcome();
}



/**
 * @brief Creates menus in main menu
 */
void MainForm::CreateMenu()
{
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

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newProjAct);
    projectMenu->addAction(openProjAct);
    projectMenu->addAction(saveProjAct);
    projectMenu->addAction(projectCompileAct);
    projectMenu->addAction(projectConfigAct);

    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(toolConvertorAct);
    toolsMenu->addAction(toolDisplayAct);
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQTAct);
    helpMenu->addAction(helpActionAct);
    helpMenu->addAction(example1Act);
}



/**
 * @brief Creates actions used in menus and toolbars and connects them to appropriate slots
 *
 */
void MainForm::CreateActions()
{
    newAct = new QAction(tr("&New File"), this);
    newAct->setStatusTip("Create a new file");
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open File"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));



    QPixmap *pm_projAdd = new QPixmap("resources//icons//projAdd.png");
    QIcon *icon_projAdd = new QIcon(*pm_projAdd);
    addAct = new QAction(*icon_projAdd, tr("Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));


    QPixmap *pm_projNewAdd = new QPixmap("resources//icons//projNewAdd.png");
    QIcon *icon_projNewAdd = new QIcon(*pm_projNewAdd);
    newAddAct = new QAction(*icon_projNewAdd, tr("New project file"), this);
    connect(newAddAct, SIGNAL(triggered()), this, SLOT(newAddFile()));


    /*QPixmap *pm_projRemoveFile = new QPixmap("src//resources//icons//projDelete.png");
    QIcon *icon_projRemoveFile = new QIcon(*pm_projRemoveFile);
    removeFileAct = new QAction(*icon_projRemoveFile, tr("&Remove file"), this);
    connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeProjFile()));*/


    saveAct = new QAction(tr("&Save File"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(tr("Save As"), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    saveAllAct = new QAction(tr("Save All"), this);
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));



    QPixmap *pm_projNew = new QPixmap("resources//icons//projNew.png");
    QIcon *icon_projNew = new QIcon(*pm_projNew);
    newProjAct = new QAction(*icon_projNew, tr("New Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    QPixmap *pm_projOpen = new QPixmap("resources//icons//projOpen.png");
    QIcon *icon_projOpen = new QIcon(*pm_projOpen);
    openProjAct = new QAction(*icon_projOpen, tr("Open Project"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    QPixmap *pm_projSave = new QPixmap("resources//icons//projSave.png");
    QIcon *icon_projSave = new QIcon(*pm_projSave);
    saveProjAct = new QAction(*icon_projSave, tr("Save Project"), this);
    connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));

    exitAct = new QAction(tr("Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    QPixmap *pm_projComp = new QPixmap("resources//icons//compile.png");
    QIcon *icon_projComp = new QIcon(*pm_projComp);
    projectCompileAct = new QAction(*icon_projComp, tr("Compile"), this);
    connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));

    projectConfigAct = new QAction(tr("Config"), this);
    //connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));


    QPixmap *pm_simFlow = new QPixmap("resources//icons//simulationStart.png");
    QIcon *icon_simFlow = new QIcon(*pm_simFlow);
    simulationFlowAct = new QAction(*icon_simFlow, tr("Start simulation"), this);
    connect(simulationFlowAct, SIGNAL(triggered()), this, SLOT(simulationFlowHandle()));
    simulationStatus = false;

    simulationRunAct = new QAction(tr("Run"), this);
    connect(simulationRunAct, SIGNAL(triggered()), this, SLOT(simulationRunHandle()));

    QPixmap *pm_simStep = new QPixmap("resources//icons//simulationStep.png");
    QIcon *icon_simStep = new QIcon(*pm_simStep);
    simulationStepAct = new QAction(*icon_simStep, tr("Do step"), this);
    connect(simulationStepAct, SIGNAL(triggered()), this, SLOT(simulationStep()));

    simulationResetAct = new QAction(tr("Reset"), this);
    connect(simulationResetAct, SIGNAL(triggered()), this, SLOT(simulationReset()));


    toolConvertorAct = new QAction(tr("Convertor"), this);
    connect(toolConvertorAct, SIGNAL(triggered()), this, SLOT(toolConvertor()));

    toolDisplayAct = new QAction(tr("Segment Display"), this);
    connect(toolDisplayAct, SIGNAL(triggered()), this, SLOT(toolDisplay()));

    aboutAct = new QAction(tr("About"), this);
    aboutQTAct = new QAction(tr("About QT"), this);
    helpActionAct = new QAction(tr("Help"), this);
    example1Act = new QAction(tr("Example 1"), this);
    connect(example1Act, SIGNAL(triggered()), this, SLOT(exampleOpen()));
}




/**
 * @brief Creates toolbars used in main window
 */
void MainForm::CreateToolbar()
{
    //fileToolBar = addToolBar(tr("File Toolbar"));
    projectToolBar = addToolBar(tr("Project Toolbar"));
    simulationToolBar = addToolBar(tr("Simulation Toolbar"));


    projectToolBar->addAction(newProjAct);
    projectToolBar->addAction(openProjAct);
    projectToolBar->addAction(saveProjAct);
    projectToolBar->addAction(newAddAct);
    //projectToolBar->addAction(removeFileAct);
    projectToolBar->addAction(addAct);
    projectToolBar->addAction(projectCompileAct);

    //fileToolBar->addAction(saveAct);


    simulationToolBar->addAction(simulationFlowAct);
    simulationToolBar->addAction(simulationRunAct);
    simulationToolBar->addAction(simulationStepAct);
    simulationToolBar->addAction(simulationResetAct);

    projectToolBar->setAllowedAreas(Qt::TopToolBarArea);
    simulationToolBar->setAllowedAreas(Qt::TopToolBarArea);
    //fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, projectToolBar);
    addToolBar(Qt::TopToolBarArea, simulationToolBar);
    //addToolBar(Qt::TopToolBarArea, fileToolBar);
}





//nacteni defaultnich widgetu podle ulozeneho nastaveni
//v tuto chvili jen informacni defaultni layout
//s kazdym novym widgetem se zavola prislusna funkce s telem podobnym teto funkci
//pojmenovani widgetu zacinat s "wid", popr. "w"
/**
 * @brief Inits default or user-saved layout of basic dock widgets
 * @bug Only default layout
 */
void MainForm::CreateDockWidgets()
{
    //setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomLeftCorner, Qt::BottomDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //mozno stejne jako u WDockManager - ulozit si ptr na okno
    //projectMan->addProject(NULL, NULL, NULL);

    //wDockManager->addDockWidget(wListCode);
    //wDockManager->addDockWidget(wListCode2);
    wDockManager->addDockWidget(wCompileInfo);
    wDockManager->addDockWidget(wSimulationInfo);
    wDockManager->addDockWidget(wBookmarkList);
    wDockManager->addDockWidget(wBreakpointList);
    wDockManager->addDockWidget(wAnalysVar);
    wDockManager->addDockWidget(wAnalysFunc);
    this->dockWidgets = true;
}


/**
 * @brief Creates welcome screen with tips and basic functions (create project etc.)
 * @details Still not implemented
 */
void MainForm::CreateWelcome()
{
    wDockManager->addCentralWidget("Welcome", "Free tips from developers!");
}



/**
 * @brief Slot. Opens new blank file.
 */
void MainForm::newFile()
{
    //jen se vytvori novy tab na code editoru
    //prepta se dialogem, zda pridat do aktivniho projektu
    wDockManager->addCentralWidget(NULL, NULL);
    wDockManager->getCentralWidget()->setChanged();
    wDockManager->getCentralWidget()->connectAct();
}


/**
 * @brief Slot. Opens new blank file and adds it to the active project.
 */
void MainForm::newAddFile()
{
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
}


/**
 * @brief Slot. Opens file selected by user in dialog.
 */
void MainForm::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (path != NULL)
    {
        QFile file(path);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            error(ERR_OPENFILE);
        }
        else {
            wDockManager->addCentralWidget(path.section('/', -1), path);
            //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
            file.close();
            wDockManager->getCentralWidget()->connectAct();
        }
    }
}


/**
 * @brief Opens file selected by programmer.
 * @param path Path to the file.
 */
void MainForm::openFilePath(QString path)
{
    //QDir thisDir(".");
    //QDir projectDir(QFileInfo(projectMan->activeProject->prjPath).dir());
    //QString absoluteFilePath = QFileInfo(projectMan->getActive()->prjPath).dir().path() + "/" + path;
    //qDebug() << absoluteFilePath;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        error(ERR_OPENFILE);
    }
    else {
        wDockManager->addCentralWidget(path.section('/', -1), path);
        //wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
        file.close();
        wDockManager->getCentralWidget()->connectAct();
        wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
    }
}


/**
 * @brief Slot. Adds opened file to the active project.
 */
void MainForm::addFile()
{
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
}

/**
 * @brief Slot. Saves the active file.
 */
void MainForm::saveFile()
{
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
            qDebug() << "mainform: file saved";
        }
    }
}


/**
 * @brief Slot. Saves the active file by the name selected by user.
 */
void MainForm::saveFileAs()
{
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
}


/**
 * @brief Saves file from selected CodeEditor.
 */
void MainForm::saveFile(CodeEdit *editor)
{
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
            qDebug() << "mainform: editor saved";
        }
    }
}



/**
 * @brief Slot. Saves all opened files which are changed (or unnamed).
 */
void MainForm::saveAll()
{
    //ulozi vsechny zmenene a nepojmenovane
    for (int i = 0; i < wDockManager->getTabCount(); i++)
        if (wDockManager->getTabWidget(i)->isChanged() == true)
        {
            saveFile(wDockManager->getTabWidget(i));
        }
        
}

/**
 * @brief Slot. Creates a new project.
 */
void MainForm::newProject()
{ 
    ProjectDialog *projectEdit = new ProjectDialog(this, projectMan);
    projectEdit->show();
}


/**
 * @brief Slot. Opens project selected by user in dialog.
 */
void MainForm::openProject()
{
    //nalezeni projektu
    QFileDialog dialog;
    QString path = QFileDialog::getOpenFileName (this, tr("Project Directory"), tr("Project (*.mmp)"));
    
    if (path.isEmpty()==false) {
    //nacteni projektu
        qDebug() << path;
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
}


/**
 * @brief Opens project selected by programmer.
 * @param path Path to the project.
 */
void MainForm::openProject(QString path)
{
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


/**
 * @brief Slot. Saves all changed files in active project.
 */
void MainForm::saveProject()
{
    for (int i = 0; i < wDockManager->getTabCount(); i++)
        if (wDockManager->getTabWidget(i)->isChild(projectMan->getActive()) == true)
           saveFile(wDockManager->getTabWidget(i));
}


/**
 * @brief Slot. Compiles active project.
 * @bug Segfault if no project is active (opened).
 * @details Still not finished. Add functionality for PIC/ASM...
 */
void MainForm::compileProject()
{
    QPlainTextEdit *compileWidget = (QPlainTextEdit*)(wDockManager->getDockWidget(wCompileInfo)->widget());
    QString mainFileName = projectMan->getActive()->mainFileName.section('.',0,-2);
    compileWidget->clear();
    
    //QStringList args;
    //args << " -g -Os -mmcu=atmega8 -c ../" + projectMan->getActive()->mainFileName;
    QProcess compiler(this);
    compiler.setProcessChannelMode(QProcess::MergedChannels);
    
    compiler.setWorkingDirectory(projectMan->getActive()->prjPath.section('/',0, -2));
    compiler.start("mkdir build");
    compiler.waitForFinished();
    
    compiler.setWorkingDirectory(projectMan->getActive()->prjPath.section('/',0, -2) + "/build/");
    qDebug() << compiler.workingDirectory();

    compiler.start("cp ../" + projectMan->getActive()->mainFileName + " " + projectMan->getActive()->mainFileName);
    compiler.waitForFinished();

    if (projectMan->getActive()->langType == LangType::LANG_ASM)
    {
        compiler.start("avra -I /usr/share/avra -l " + projectMan->getActive()->mainFileName.section('.',0,-2) + ".lst " + projectMan->getActive()->mainFileName);
        if (!compiler.waitForFinished())
        {
            compileWidget->appendPlainText(compiler.errorString());
        }
        else
        {
            compileWidget->appendPlainText(compiler.readAll() + "\n\n");
        }
    }
    else if (projectMan->getActive()->langType == LangType::LANG_C)
    {
        compiler.start("avr-gcc -g -Os -mmcu=atmega8 -c " + projectMan->getActive()->mainFileName);
        if (!compiler.waitForFinished())
        {
            compileWidget->appendPlainText(compiler.errorString());
            return;
        }
        else
        {
            compileWidget->appendPlainText(compiler.readAll() + "\n\n");
        }


        compiler.setProcessChannelMode(QProcess::MergedChannels);
        compiler.start("avr-gcc -g -mmcu=atmega8 -o " + mainFileName + ".elf " + mainFileName + ".o");
        if (!compiler.waitForFinished())
        {
            compileWidget->appendPlainText(compiler.errorString());
            return;
        }
        else
        {
            compileWidget->appendPlainText(compiler.readAll() + "\n\n");
        }


        compiler.setProcessChannelMode(QProcess::MergedChannels);
        compiler.start("avr-objcopy -j .text -j .data -O ihex " + mainFileName + ".elf " + mainFileName + ".hex");
        if (!compiler.waitForFinished())
        {
            compileWidget->appendPlainText(compiler.errorString());
        }
        else
        {
            compileWidget->appendPlainText(compiler.readAll() + "\n\n");
        }
    }
    
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
            QPixmap *pm_simFlow = new QPixmap("resources//icons//simulationStop.png");
            QIcon *icon_simFlow = new QIcon(*pm_simFlow);
            simulationFlowAct->setIcon(*icon_simFlow);
            simulationFlowAct->setText(tr("Stop simulation"));
            simulationStatus = true;
            projectMan->getActive()->start();
        }
        else
        {
            QPixmap *pm_simFlow = new QPixmap("resources//icons//simulationStart.png");
            QIcon *icon_simFlow = new QIcon(*pm_simFlow);
            simulationFlowAct->setIcon(*icon_simFlow);
            simulationFlowAct->setText(tr("Start simulation"));
            simulationStatus = false;
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
    this->openProject("./demoprojekt/Example.mmp");
}