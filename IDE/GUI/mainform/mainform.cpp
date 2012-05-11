#include <QtGui>
//pozdeji zamenit QtGui za mensi celky
#include "mainform.h"
#include "../dialogs/projectdlg.h"
#include "../dialogs/errordlg.h"




MainForm::MainForm()
{
    projectMan = new ProjectMan(this);
    wDockManager = new WDockManager(this);
    CreateActions();
    CreateMenu();
    CreateToolbar();
    //CreateDockWidgets();
    //CreateWelcome();
}




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
    helpMenu = menuBar()->addMenu(tr("&Help"));

    //MenuMan menuMan(this);
}




void MainForm::CreateActions()
{
    newAct = new QAction(tr("&New File"), this);
    newAct->setStatusTip("Create a new file");
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("&Open File"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));



    QPixmap *pm_projAdd = new QPixmap("src//resources//icons//projAdd.png");
    QIcon *icon_projAdd = new QIcon(*pm_projAdd);
    addAct = new QAction(*icon_projAdd, tr("&Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));


    QPixmap *pm_projNewAdd = new QPixmap("src//resources//icons//projNewAdd.png");
    QIcon *icon_projNewAdd = new QIcon(*pm_projNewAdd);
    newAddAct = new QAction(*icon_projNewAdd, tr("&New project file"), this);
    connect(newAddAct, SIGNAL(triggered()), this, SLOT(newAddFile()));



    saveAct = new QAction(tr("&Save File"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAsAct = new QAction(tr("Sa&ve As"), this);
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveFileAs()));

    saveAllAct = new QAction(tr("Save &All"), this);
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));



    QPixmap *pm_projNew = new QPixmap("src//resources//icons//projNew.png");
    QIcon *icon_projNew = new QIcon(*pm_projNew);
    newProjAct = new QAction(*icon_projNew, tr("New &Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    QPixmap *pm_projOpen = new QPixmap("src//resources//icons//projOpen.png");
    QIcon *icon_projOpen = new QIcon(*pm_projOpen);
    openProjAct = new QAction(*icon_projOpen, tr("Open P&roject"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    QPixmap *pm_projSave = new QPixmap("src//resources//icons//projSave.png");
    QIcon *icon_projSave = new QIcon(*pm_projSave);
    saveProjAct = new QAction(*icon_projSave, tr("&Save Project"), this);
    connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));

    exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));


    QPixmap *pm_projComp = new QPixmap("src//resources//icons//compile.png");
    QIcon *icon_projComp = new QIcon(*pm_projComp);
    projectCompileAct = new QAction(*icon_projComp, tr("&Compile"), this);
    connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));

    projectConfigAct = new QAction(tr("&Config"), this);
    //connect(projectCompileAct, SIGNAL(triggered()), this, SLOT(compileProject()));
}





void MainForm::CreateToolbar()
{
    //fileToolBar = addToolBar(tr("File Toolbar"));
    projectToolBar = addToolBar(tr("Project Toolbar"));


    projectToolBar->addAction(newProjAct);
    projectToolBar->addAction(openProjAct);
    projectToolBar->addAction(saveProjAct);
    projectToolBar->addAction(newAddAct);
    projectToolBar->addAction(addAct);
    projectToolBar->addAction(projectCompileAct);

    //fileToolBar->addAction(saveAct);

    projectToolBar->setAllowedAreas(Qt::TopToolBarArea);
    //fileToolBar->setAllowedAreas(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, projectToolBar);
    //addToolBar(Qt::TopToolBarArea, fileToolBar);
}





//nacteni defaultnich widgetu podle ulozeneho nastaveni
//v tuto chvili jen informacni defaultni layout
//s kazdym novym widgetem se zavola prislusna funkce s telem podobnym teto funkci
//pojmenovani widgetu zacinat s "wid", popr. "w"
void MainForm::CreateDockWidgets()
{
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    //mozno stejne jako u WDockManager - ulozit si ptr na okno
    //projectMan->addProject(NULL, NULL, NULL);

    //wDockManager->addDockWidget(wListCode);
    //wDockManager->addDockWidget(wListCode2);
    wDockManager->addDockWidget(wCompileInfo);
    wDockManager->addDockWidget(wSimulationInfo);

}


void MainForm::CreateWelcome()
{
    wDockManager->addCentralWidget("Welcome", "Free tips from developers!");
}




void MainForm::newFile()
{
    //jen se vytvori novy tab na code editoru
    //prepta se dialogem, zda pridat do aktivniho projektu
    wDockManager->addCentralWidget(NULL, NULL);
    wDockManager->getCentralWidget()->setChanged();
    wDockManager->getCentralWidget()->connectAct();
}


void MainForm::newAddFile()
{
    //jen se vytvori novy tab na code editoru
    //a soubor se prida k projektu
    QString path = QFileDialog::getSaveFileName(this, tr("Source File"), QString(), QString(), 0, QFileDialog::DontUseNativeDialog);
    wDockManager->addCentralWidget(path.section('/', -1), path);
    wDockManager->getCentralWidget()->setChanged();
    wDockManager->getCentralWidget()->connectAct();

    //je sice prehlednejsi zavolat saveFile(), ale
    //vlozeni kodu pro ulozeni je rychlejsi a efektivnejsi
    //...ale necham volani saveFile()...
    saveFile();
    //pridani do projektu
    QFile prjFile(projectMan->getActive()->prjPath);
    if(!prjFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        error(ERR_OPENFILE);
    }
    else 
    {
        projectMan->addFile(&prjFile, path, path.section('/', -1));
        prjFile.close();      
        wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
    }
}


void MainForm::openFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    //if (!file.open(QIODevice::ReadOnly))
    {
        error(ERR_OPENFILE);
    }
    else {
        wDockManager->addCentralWidget(path.section('/', -1), path);
        wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
        file.close();
        wDockManager->getCentralWidget()->connectAct();
    }
}



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
        wDockManager->getCentralTextEdit()->setPlainText(file.readAll());
        file.close();
        wDockManager->getCentralWidget()->connectAct();
        wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
    }
}




void MainForm::addFile()
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
    QFile prjFile(projectMan->getActive()->prjPath);
    if(!prjFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        error(ERR_OPENFILE);
    }
    else 
    {
        projectMan->addFile(&prjFile, path, path.section('/', -1));
        prjFile.close();      
        wDockManager->getCentralWidget()->setParentProject(projectMan->getActive());
    }
}

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
        }
    }
}



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
            path = editor->getPath();  
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
        }
    }
}






void MainForm::saveAll()
{
    //ulozi vsechny zmenene a nepojmenovane
    for (int i = 0; i < wDockManager->getTabCount(); i++)
        if (wDockManager->getTabWidget(i)->isChanged() == true)
        {
            saveFile(wDockManager->getTabWidget(i));
        }
        
}

void MainForm::newProject()
{ 
    ProjectDialog *projectEdit = new ProjectDialog(this, projectMan);
    projectEdit->show();
}

void MainForm::openProject()
{
    //nalezeni projektu
    QFileDialog dialog;
    QString path = QFileDialog::getOpenFileName (this, tr("Project Directory"), tr("Project (*.mmp)"));
    
    if (path.isEmpty()==false) {
    //nacteni projektu
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

void MainForm::saveProject()
{
    for (int i = 0; i < wDockManager->getTabCount(); i++)
        if (wDockManager->getTabWidget(i)->isChild(projectMan->getActive()) == true)
           saveFile(wDockManager->getTabWidget(i));
}


void MainForm::compileProject()
{
    projectMan->createActiveMakefile();

    ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->clear();

    QStringList args;
    args << "-C" + projectMan->getActive()->prjPath.section('/',0, -2)+"/make/";
    QProcess compiler(this);

    compiler.setProcessChannelMode(QProcess::MergedChannels);
    compiler.start("make", args);
    if (!compiler.waitForFinished())
        ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->appendPlainText("Make failed:\n" + compiler.errorString());
    else
        ((QPlainTextEdit*)wDockManager->getDockWidget(wCompileInfo)->widget())->appendPlainText("Make succesfull:\n" + compiler.readAll());
}


WDockManager* MainForm::getWDockManager()
{
    return wDockManager;
}
