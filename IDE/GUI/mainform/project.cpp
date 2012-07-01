#include <QtGui>
#include "project.h"
#include "McuSimCfgMgr.h"
#include "MCUSimControl.h"
#include "../widgets/projecttree.h"

ProjectMan::ProjectMan(MainForm *qMainWindow)
{
    //openProjects = new QList<Project*>;
    projectCount = 0;
    mainWindow = qMainWindow;
    activeProject = NULL;
    
}


void ProjectMan::openProject(QFile *file)
{
    Project *newProject = new Project(file, mainWindow, this);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //if (projectCount == 1 && openProjects.at(0)->prjName == NULL)
    //{
    //    delete openProjects.at(0);
    //    openProjects.removeAt(0);
    //    projectCount--;
    //}
    //else 
    openProjects.append(newProject);
    activeProject = newProject;
    if (projectCount > 0) 
        mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    else
        mainWindow->CreateDockWidgets();

    projectCount++;
}


void ProjectMan::addFile(QFile *file, QString path, QString name)
{
    getActive()->addFile(file, path, name);
}

void ProjectMan::addProject(QString name, QString path, QFile *file)
{
    Project *newProject = new Project(name, path, mainWindow, file, this);
    //mainWindow->getWDockManager()->hideDockWidgetArea(0);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //mainWindow->getWDockManager()->showDockWidgetArea(0);
    
    //if (projectCount == 1 && openProjects.at(0)->prjName == NULL) {
    //    delete openProjects.at(0);
    //    openProjects.removeAt(0);
    //    projectCount--;
    //}
    //else
    if (projectCount > 0) 
        mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    else
        mainWindow->CreateDockWidgets();

    openProjects.append(newProject);
    activeProject = newProject;
    projectCount++;
}


void ProjectMan::setActive(Project *activePrj)
{
    activeProject = activePrj;
}


bool ProjectMan::isActiveProject(Project *project)
{
    return (project==activeProject);
}


Project* ProjectMan::getActive()
{
    return activeProject;
}



void ProjectMan::createActiveMakefile()
{
    QFileInfo projectInfo(activeProject->prjPath);
    QDir projectDir = projectInfo.dir();
    projectDir.mkdir("make");
    QDir makefileDir(projectDir.path() + "/make");
    QFile makefile(makefileDir.path() + "/Makefile");
    if (!makefile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
         error(ERR_OPENFILE);
    }
    else
    {
        QTextStream makeOut(&makefile);
        makeOut << "NAME=" << activeProject->prjName << endl << endl;
        makeOut << "CC=sdcc" << endl << endl;
        makeOut << "CFLAGS=" << endl << endl;
        makeOut << "FILES=";
        QString relativePath;
        for (int i = 0; i < activeProject->fileCount; i++)
        {
            relativePath = makefileDir.relativeFilePath(projectDir.path() + "/" + activeProject->filePaths.at(i));
            makeOut << " " << relativePath;
        }
        makeOut << endl << endl;

        makeOut << "$(NAME):" << endl;
        makeOut << "\t$(CC) $(CFLAGS) $(FILES)" << endl;
    }
    
}





Project::~Project()
{
    delete this->prjDockWidget;
    
}

//otevreni projektu
Project::Project(QFile *file, MainForm* mainWindow, ProjectMan *parent)
{
    errorFlag = ERR_OK;
    fileCount = 0;
    parentManager = parent;
    prjPath = QFileInfo(*file).filePath();
    //nacteni ze souboru
    QDomDocument domDoc("MMProject");
    if (!domDoc.setContent(file))
    {
        errorFlag = ERR_ASSIGN;
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MMProject")
        {
            errorFlag = ERR_CONTENT;
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (!xmlGeneralNode.isNull())
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (xmlGeneralElement.tagName() == "Name")
                            {
                                prjName = xmlGeneralElement.attribute("name", "");
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "Files")
                    {
                        QDomNode xmlFilesNode = xmlElement.firstChild();
                        QDomElement xmlFilesElement;
                        while (!xmlFilesNode.isNull())
                        {
                            xmlFilesElement = xmlFilesNode.toElement();
                            if (xmlFilesElement.tagName() == "File")
                            {
                                fileNames.append(xmlFilesElement.attribute("name", ""));
                                filePaths.append(xmlFilesElement.attribute("path", ""));
                                if (xmlFilesElement.attribute("main", "") == "yes")
                                {
                                    mainFileName = fileNames.at(fileCount);
                                    mainFilePath = filePaths.at(fileCount);
                                }
                                fileCount++;
                            }
                            xmlFilesNode = xmlFilesNode.nextSibling();
                        }
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
            prjDockWidget = new QDockWidget(prjName, mainWindow);
            prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
            prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            prjTreeWidget = new ProjectTree(prjDockWidget, this);
            //QAction *setMainAct = new QAction("Set as main file", prjTreeWidget);
            //QAction *removeFileAct = new QAction("Remove file from project", prjTreeWidget);
            //prjTreeWidget->addAction(setMainAct);
            //prjTreeWidget->addAction(removeFileAct);
            prjDockWidget->setWidget(prjTreeWidget);

            QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
            treeProjName->setText(0, prjName);
            treeProjName->setData(0, Qt::ToolTipRole, prjPath);

            QDir projectDir = QFileInfo(prjPath).dir();
            QString absolutePath = projectDir.path();
            for (int i=0; i<fileCount; ++i)
            {
                 QTreeWidgetItem *treeProjFile = new QTreeWidgetItem(treeProjName);
                 treeProjFile->setText(0, fileNames.at(i));
                 
                 treeProjFile->setData(0, Qt::ToolTipRole, QDir(absolutePath + "/" + filePaths.at(i)).canonicalPath());
            }

            //connect(setMainAct, SIGNAL(triggered()), this, SLOT(setMainFile()));
            //connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeFile()));
            connect(prjDockWidget, SIGNAL(visibilityChanged(bool)),this,SLOT(setActive()));  
            connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),this,SLOT(openItem()));  
            setupSim();
        }
    }
}

//vytvoreni prazdneho projektu
Project::Project(QString name, QString path, MainForm* mainWindow, QFile *file, ProjectMan *parent)
{
    errorFlag = ERR_OK;
    parentManager = parent;

    if (name != NULL)
        prjDockWidget = new QDockWidget(name, mainWindow);
    else
        prjDockWidget = new QDockWidget("Projects", mainWindow);
    prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    prjTreeWidget = new ProjectTree(prjDockWidget, this);
    //QAction *setMainAct = new QAction("Set as main file", prjTreeWidget);
    //QAction *removeFileAct = new QAction("Remove file from project", prjTreeWidget);
    //prjTreeWidget->addAction(setMainAct);
    //prjTreeWidget->addAction(removeFileAct);

    prjDockWidget->setWidget(prjTreeWidget);
    //prjDockWidget->setMinimumWidth(150);
    prjName=name;
    //pri nacteni programu neni nacteny projekt
    //if (name != NULL) {
        prjPath=path;
        QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
    	treeProjName->setText(0, name);
        treeProjName->setData(0, Qt::ToolTipRole, path);
    	fileCount=0;
        
        //a zapsani do souboru
        QDomDocument domDoc("MMProject");
        QDomElement xmlRoot = domDoc.createElement("MMProject");
        domDoc.appendChild(xmlRoot);

        QDomElement xmlGeneral = domDoc.createElement("General");
        QDomElement xmlName = domDoc.createElement("Name");
        xmlName.setAttribute("name", name);
        xmlGeneral.appendChild(xmlName);
        //QDomElement xmlPath = domDoc.createElement("Path");
        //xmlPath.setAttribute("path", path);
        //xmlGeneral.appendChild(xmlPath);
        xmlRoot.appendChild(xmlGeneral);

        QDomElement xmlFiles = domDoc.createElement("Files");
        xmlRoot.appendChild(xmlFiles);

        QDomElement xmlSimulator = domDoc.createElement("Simulator");
        xmlRoot.appendChild(xmlSimulator);

        QDomElement xmlCompiler = domDoc.createElement("Compiler");
        xmlRoot.appendChild(xmlCompiler);

        QTextStream xmlStream(file);
        xmlStream << domDoc.toString();

        //connect(setMainAct, SIGNAL(triggered()), this, SLOT(setMainFile()));
        //connect(removeFileAct, SIGNAL(triggered()), this, SLOT(removeFile()));
        connect(prjDockWidget, SIGNAL(visibilityChanged(bool)),this,SLOT(setActive()));
        connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),this,SLOT(openItem()));  
        setupSim();
    //}

}



void Project::addFile(QFile *file, QString path, QString name)
{
    QString relativePath;
    QDomDocument domDoc("MMProject");
    if (!domDoc.setContent(file))
    {
        errorFlag = ERR_ASSIGN;
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MMProject")
        {
            errorFlag = ERR_CONTENT;
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomElement xmlFile = domDoc.createElement("File");


            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "Files")
                    {
                        QDir project(QFileInfo(this->prjPath).dir());
                        relativePath = project.relativeFilePath(path);
                        xmlFile.setAttribute("name", name);
                        xmlFile.setAttribute("path", relativePath);
                        /*if (name == mainFileName && relativePath == mainFilePath)
                            xmlFile.setAttribute("main", "yes");
                        else
                            xmlFile.setAttribute("main", "no");*/
                        xmlElement.appendChild(xmlFile);

                        fileNames.append(name);
                        fileCount++;
                        filePaths.append(relativePath);
                        break;
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
            file->close();
            file->open(QIODevice::WriteOnly);

            QTextStream xmlStream(file);
            xmlStream << domDoc.toString();

            QTreeWidgetItem *treeProjFile = new QTreeWidgetItem(prjTreeWidget->topLevelItem(0));
            treeProjFile->setText(0, name);
            treeProjFile->setData(0, Qt::ToolTipRole, path);
            
        }
    }
}



void Project::setActive()
{
    if (false == parentManager->isActiveProject(this))
        parentManager->setActive(this);
}




void Project::openItem()
{
    if (prjTreeWidget->currentItem() != NULL)
        parentManager->mainWindow->openFilePath(prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
}



void Project::setMainFile(QString path, QString name)
{
    
}


void Project::removeFile(QString path, QString name)
{
    /*QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    if (prjTreeWidget->currentItem() != NULL)
    {
        QDomDocument domDoc("MMProject");
        if (!domDoc.setContent(&prjFile))
        {
            errorFlag = ERR_ASSIGN;
            error(ERR_XML_ASSIGN);
        }
        else
        {
            //otevrit xml, upravit a ulozit
            QDomElement xmlRoot = domDoc.documentElement();
            if (xmlRoot.tagName() != "MMProject")
            {
                errorFlag = ERR_CONTENT;
                error(ERR_XML_CONTENT);
            }
            else
            {
                QDomNode xmlNode = xmlRoot.firstChild();
                QDomElement xmlElement;
                bool done = false;
                while (!xmlNode.isNull() && done == false)
                {
                    xmlElement = xmlNode.toElement();
                    if (!xmlElement.isNull())
                    {
                        if (xmlElement.tagName() == "Files")
                        {
                            QDomNode xmlFilesNode = xmlElement.firstChild();
                            QDomElement xmlFilesElement;
                            while (!xmlFilesNode.isNull())
                            {
                                xmlFilesElement = xmlFilesNode.toElement();
                                if (xmlFilesElement.tagName() == "File"
                                    && xmlFilesElement.attribute("name") == fileNames.at(prjTreeWidget->currentColumn())
                                    && xmlFilesElement.attribute("path") == filePaths.at(prjTreeWidget->currentColumn()))
                                {
                        
                                    xmlFilesNode.parentNode().removeChild(xmlFilesNode);
                                    done = true;
                                    break;
                                }
                                xmlFilesNode = xmlFilesNode.nextSibling();
                            }
                        }
                    }
                    xmlNode = xmlNode.nextSibling();
                }
                prjFile.close();
                prjFile.open(QIODevice::WriteOnly);
                QTextStream xmlStream(&prjFile);
                xmlStream << domDoc.toString();
            }
        }
        //
        fileNames.removeAt(prjTreeWidget->currentColumn());
        filePaths.removeAt(prjTreeWidget->currentColumn());
        fileCount--;

        //znovu nacist treewidget
        prjTreeWidget->clear();
        QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
        treeProjName->setText(0, prjName);
        treeProjName->setData(0, Qt::ToolTipRole, prjPath);

        
        
    }*/
}


void Project::setupSim()
{
    McuSimCfgMgr::getInstance()->openConfigFile("../simulators/MCUSim/McuSimCfgMgr/mcuspecfile.xml");
    m_simControlUnit = new MCUSimControl("ATmega8A");
}

void Project::start()
{
    QString path = mainFilePath + "/make/" + mainFileName + ".hex";
    m_simControlUnit->start("avr8_test_code.hex");
}

void Project::stop()
{
    m_simControlUnit->stop();
}

void Project::reset()
{
    m_simControlUnit->reset();
}

void Project::step()
{
    m_simControlUnit->step();
}


MCUSimControl* Project::getSimControl()
{
    return this->m_simControlUnit;
}
