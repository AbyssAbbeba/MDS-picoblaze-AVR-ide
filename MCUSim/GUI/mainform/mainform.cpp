#include <QtGui>
//pozdeji zamenit QtGui za mensi celky
#include "mainform.h"
#include "../dialogs/projectdlg.h"


MainForm::MainForm()
{
    //nacteni nastaveni prostredi
    //Editor = new CodeEdit(this);
    projectMan = new ProjectMan(this);
    wDockManager = new WDockManager(this);
    CreateActions();
    CreateMenu();
    CreateToolbar();
    CreateDockWidgets();
    
    //setCentralWidget(Editor);

    //QVBoxLayout *layout = new QVBoxLayout;
    //layout->setMargin(5);

    //Editor->setLayout(layout);


    //setWindowTitle(tr("Menus"));
    //resize(480, 320);
}


void MainForm::CreateMenu()
{ 
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(addAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAllAct);
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    viewMenu = menuBar()->addMenu(tr("&View"));

    projectMenu = menuBar()->addMenu(tr("&Project"));
    projectMenu->addAction(newProjAct);
    projectMenu->addAction(openProjAct);

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

    addAct = new QAction(tr("&Add to Project"), this);
    connect(addAct, SIGNAL(triggered()), this, SLOT(addFile()));

    saveAct = new QAction(tr("&Save File"), this);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(saveFile()));

    saveAllAct = new QAction(tr("Save &All"), this);
    connect(saveAllAct, SIGNAL(triggered()), this, SLOT(saveAll()));

    newProjAct = new QAction(tr("New &Project"), this);
    connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));

    openProjAct = new QAction(tr("Open P&roject"), this);
    connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

    //saveProjAct = new QAction(tr("&Save Project"), this);
    //connect(saveProjAct, SIGNAL(triggered()), this, SLOT(saveProject()));

    exitAct = new QAction(tr("&Exit"), this);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

}





void MainForm::CreateToolbar()
{
    toolBar = addToolBar(tr("Toolbar"));
    //QPixmap *pm_newProj = new QPixmap("icons//newProj.bmp");
    //navazani pomoci dalsich argumentu addAction
    toolBar->addAction(newProjAct);
    toolBar->addAction(openProjAct);
    toolBar->addAction(addAct);
    toolBar->addAction(saveAllAct);

    toolBar->setAllowedAreas(Qt::TopToolBarArea);
    addToolBar(Qt::TopToolBarArea, toolBar);
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
    projectMan->addProject(NULL, NULL, NULL);

    wDockManager->addCentralWidget("");
    wDockManager->addDockWidget(wListCode);
    wDockManager->addDockWidget(wListCode2);
    wDockManager->addDockWidget(wListCode3);
    //this->tabifyDockWidget(wBottomDock2, wBottomDock);


}





void MainForm::newFile()
{
    //jen se vytvori novy tab na code editoru
    wDockManager->addCentralWidget("");
}

void MainForm::openFile()
{
    QString path = QFileDialog::getOpenFileName (this, tr("Source File"), "");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Error opening file");
        msgBox.exec();
    }
    else {
        QFileInfo filePath(path);
        wDockManager->addCentralWidget(filePath.fileName());
        wDockManager->getCentralWidget()->setPlainText(file.readAll());
        file.close();
    }
}

void MainForm::addFile()
{
    QString path = QFileDialog::getSaveFileName (this, tr("Source File"), "");
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Error opening file");
        msgBox.exec();
    }
    else {
        QTextStream fout(&file);
        fout << wDockManager->getCentralWidget()->toPlainText();
        file.close();
        //pridani do projektu
        QFile prjFile(projectMan->getActive()->prjPath);
        if(!prjFile.open(QIODevice::ReadWrite | QIODevice::Text))
        {
            QMessageBox msgBox;
            msgBox.setText("Error opening file");
            msgBox.exec();
        }
        else {
            QFileInfo filePath(path);
            projectMan->addFile(&prjFile, path, filePath.fileName());
            prjFile.close();
        }
    }
}

void MainForm::saveFile()
{
    
}

void MainForm::saveAll()
{

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
            QMessageBox msgBox;
            msgBox.setText("Error opening file");
            msgBox.exec();
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

}
