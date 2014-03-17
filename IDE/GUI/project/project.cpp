/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup ProjectManager
 * @file project.cpp
 */


#include <QtGui>
#include "project.h"
#include "McuSimCfgMgr.h"
#include "MCUSimControl.h"
#include "../widgets/ProjectTree/projecttree.h"
#include "../guicfg/guicfg.h"




/**
 * @brief Constructor of project manager. Inits basic variables.
 * @param qMainWindow Pointer to MainForm instance.
 */
ProjectMan::ProjectMan(QWidget *parent)
    : QObject(parent)
{
    //qDebug() << "ProjectMan: ProjectMan()";
    //openProjects = new QList<Project*>;
    this->untrackedProject = NULL;
    projectCount = 0;
    activeProject = NULL;
    //qDebug() << "ProjectMan: return ProjectMan()";
}


/**
 * @brief Opens and loads project according to opened xml (.mds-project) file.
 * @param file Opened xml (.mds-project) file.
 */
void ProjectMan::openProject(QFile *file)
{
    //qDebug() << "ProjectMan: openProject()";
    Project *newProject = new Project(file, this);
    emit addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
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
    {
        emit tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
        //mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    }
    //else
    //    mainWindow->CreateDockWidgets();

    projectCount++;
    emit connectProject(newProject);
    emit projectOpened();
    //qDebug() << "ProjectMan: return openProject()";
}


/**
 * @brief Adds file to active project.
 * @param path Path to the file
 * @param name Name of the file
 */
void ProjectMan::addFile(QString path, QString name)
{
    //qDebug() << "ProjectMan: addFile()";
    getActive()->addFile(path, name);
    //qDebug() << "ProjectMan: return addFile()";
}


/**
 * @brief Adds file to untracked.
 * @param path Path to the file
 * @param name Name of the file
 */
QString ProjectMan::addUntrackedFile(QString path, QString name)
{
    //qDebug() << "ProjectMan: addUntrackedFile()";
    if (this->untrackedProject == NULL)
    {
        this->addUntrackedProject();
    }
    //qDebug() << "ProjectMan: return addUntrackedFile() before project->addFile()";
    return this->untrackedProject->addFile(path, name);
}


/**
 * @brief Creates untracked project.
 */
void ProjectMan::addUntrackedProject()
{
    //qDebug() << "ProjectMan: addUntrackedProject()";
    Project *newProject = new Project(this);

    //mainWindow->getWDockManager()->hideDockWidgetArea(0);
    emit addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //mainWindow->getWDockManager()->showDockWidgetArea(0);

    //if (projectCount == 1 && openProjects.at(0)->prjName == NULL) {
    //    delete openProjects.at(0);
    //    openProjects.removeAt(0);
    //    projectCount--;
    //}
    //else
    if (projectCount > 0)
    {
        emit tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
        //mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    }

    openProjects.append(newProject);
    activeProject = newProject;
    untrackedProject = newProject;
    projectCount++;
    emit connectProject(newProject);
    emit projectOpened();
    //qDebug() << "ProjectMan: return addUntrackedProject()";
}


/**
 * @brief Creates new project.
 * @param name name of the new project
 * @param path path to the new project xml (.mds-project) file
 * @param architecture target architecture of the new project
 * @param file xml (.mds-project) file of the new project
 */
void ProjectMan::addProject(QString name, QString path, QString architecture, LangType langType, QFile *file)
{
    //qDebug() << "ProjectMan: addProject()";
    Project *newProject = new Project(name, path, architecture, langType, file, this);

    //mainWindow->getWDockManager()->hideDockWidgetArea(0);
    emit addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    //mainWindow->getWDockManager()->showDockWidgetArea(0);
    
    //if (projectCount == 1 && openProjects.at(0)->prjName == NULL) {
    //    delete openProjects.at(0);
    //    openProjects.removeAt(0);
    //    projectCount--;
    //}
    //else
    if (projectCount > 0)
    {
        emit tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
        //mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    }
    //else
    //{
    //    mainWindow->CreateDockWidgets();
    //}

    activeProject = newProject;
    openProjects.append(newProject);
    projectCount++;
    emit connectProject(newProject);
    emit projectOpened();
    //qDebug() << "ProjectMan: return addProject()";
}


/**
 * @brief Sets project selected by programmer as active
 * @param activePrj Pointer to the soon-to-be active project
 */
void ProjectMan::setActive(Project *activePrj)
{
    //qDebug() << "ProjectMan: setActive()";
    activeProject = activePrj;
    //qDebug() << "ProjectMan: return setActive()";
}


void ProjectMan::setActiveByIndex(int index)
{
    //qDebug() << "ProjectMan: setActiveByIndex" << index;
    activeProject = openProjects.at(index);
    //qDebug() << "ProjectMan: setActiveByIndex name" << activeProject->prjName;
    //qDebug() << "ProjectMan: return setActiveByIndex";
}


/**
 * @brief Checks if project selected by programmer is active.
 * @param project Pointer to the checked project
 * @return True if project is active, otherwise false.
 */
bool ProjectMan::isActiveProject(Project *project)
{
    //qDebug() << "ProjectMan: isActiveProject()";
    //qDebug() << "ProjectMan: return isActiveProject()";
    return (project==activeProject);
}


/**
 * @brief Returns pointer to the active project.
 * @return Pointer to the active project.
 */
Project* ProjectMan::getActive()
{
    //qDebug() << "ProjectMan: getActive()";
    //qDebug() << "ProjectMan: return getActive()";
    return activeProject;
}


Project* ProjectMan::getUntracked()
{
    return untrackedProject;
}


Project* ProjectMan::getSimulated()
{
    return this->simulatedProject;
}


void ProjectMan::setSimulated(Project* project)
{
    this->simulatedProject = project;
}


bool ProjectMan::isOpened(QString path)
{
    for (int i = 0; i < openProjects.size(); i++)
    {
        if (path == openProjects.at(i)->prjPath)
        {
            return true;
        }
    }
    return false;
}


/**
 * @brief Creates makefile for active project. Used for compilation.
 * @details Not used at the moment.
 */
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
        makeOut << "CFLAGS= -mavr" << endl << endl;
        makeOut << "FILES=";
        QString relativePath;
        //for (int i = 0; i < activeProject->fileCount; i++)
        //{
            //relativePath = makefileDir.relativeFilePath(projectDir.path() + "/" + activeProject->filePaths.at(i));
        relativePath = makefileDir.relativeFilePath(projectDir.path() + "/" + activeProject->mainFilePath);
            makeOut << " " << relativePath;
        //}
        makeOut << endl << endl;

        makeOut << "$(NAME):" << endl;
        makeOut << "\t$(CC) $(CFLAGS) $(FILES)" << endl;
    }
}



void ProjectMan::closeProject(Project *project)
{
    int index = openProjects.indexOf(project);
    openProjects.removeAll(project);
    if (project == untrackedProject)
    {
        untrackedProject = NULL;
    }
    if (openProjects.count() > 0)
    {
        if (index == openProjects.count())
        {
            activeProject = openProjects.at(index - 1);
        }
        else
        {
            activeProject = openProjects.at(index);
        }
    }
    else
    {
        activeProject = NULL;
    }
    delete project;
    this->projectCount--;
}





//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


/**
 * @brief Project destructor. Deletes project's tree widget
 */
/*Project::~Project()
{
    delete this->prjDockWidget;
    
}*/

//otevreni projektu
/**
 * @brief Opens project from opened xml (.mds-project) file.
 * @param file Opened xml (.mds-project) file.
 * @param parent Project parent project manager.
 */
Project::Project(QFile *file, ProjectMan *parent)
    : QObject(parent)
{
    //qDebug() << "Project: Project()";
    mainFileName = "";
    mainFilePath = "";
    for (int i = 0; i < 12; i++)
    {
        compileOpt.append(false);
    }
    
    fileCount = 0;
    parentManager = parent;
    this->m_simControlUnit = NULL;
    prjPath = QFileInfo(*file).filePath();
    currLineColor = GuiCfg::getInstance().getCurrLineColor();
    prevLineColor = GuiCfg::getInstance().getPrevLineColor();
    prevLine2Color = GuiCfg::getInstance().getPrevLine2Color();
    //nacteni ze souboru
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(file))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                            else if (xmlGeneralElement.tagName() == "Family")
                            {
                                family = xmlGeneralElement.attribute("family", "");
                            }
                            else if (xmlGeneralElement.tagName() == "Language")
                            {
                                langType = (LangType)(xmlGeneralElement.attribute("language", "").toInt(NULL));
                            }
                            else if (xmlGeneralElement.tagName() == "IntVector")
                            {
                                intVector = xmlGeneralElement.attribute("value", "").toInt(NULL);
                            }
                            else if (xmlGeneralElement.tagName() == "HWBuild")
                            {
                                hwBuild = xmlGeneralElement.attribute("value", "").toInt(NULL);
                            }
                            else if (xmlGeneralElement.tagName() == "ScratchpadSize")
                            {
                                scratchpadSize = xmlGeneralElement.attribute("value", "").toInt(NULL);
                            }
                            else if (xmlGeneralElement.tagName() == "ProgMemSize")
                            {
                                progMemSize = xmlGeneralElement.attribute("value", "").toInt(NULL);
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
                                fileCount++;
                            }
                            xmlFilesNode = xmlFilesNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "Mainfile")
                    {
                        if (xmlElement.attribute("name", "") != ""
                            && xmlElement.attribute("path", "") != "")
                        {
                            mainFileName = xmlElement.attribute("name", "");
                            mainFilePath = xmlElement.attribute("path", "");
                            if ("true" == xmlElement.attribute("enabled", ""))
                            {
                                useMainFile = true;
                            }
                            else
                            {
                                useMainFile = false;
                            }
                        }
                    }
                    else if (xmlElement.tagName() == "Compiler")
                    {
                        QDomNode xmlCompilerNode = xmlElement.firstChild();
                        QDomElement xmlCompilerElement;
                        while (!xmlCompilerNode.isNull())
                        {
                            //qDebug() << "node";
                            xmlCompilerElement = xmlCompilerNode.toElement();
                            if (xmlCompilerElement.tagName() == "Options")
                            {
                                QDomNode xmlCompileOptNode = xmlCompilerElement.firstChild();
                                QDomElement xmlCompileOptElem;
                                while (!xmlCompileOptNode.isNull())
                                {
                                    xmlCompileOptElem = xmlCompileOptNode.toElement();
                                    if (xmlCompileOptElem.tagName() == "SymbolTable")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[0] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "MacroTable")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[1] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "DebugFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[2] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "CodeTree")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[3] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "ListFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[4] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "HexFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[5] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "BinFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[6] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "SRecFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[7] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "MemFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[8] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "RawHexFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[9] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "VerilogFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[10] = true;
                                        }
                                    }
                                    else if (xmlCompileOptElem.tagName() == "VHDLFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[11] = true;
                                        }
                                    }
                                    xmlCompileOptNode = xmlCompileOptNode.nextSibling();
                                    //qDebug() << "Opt";
                                }
                            }
                            else if (xmlCompilerElement.tagName() == "Templates")
                            {
                                QDomNode xmlTemplatesNode = xmlCompilerElement.firstChild();
                                QDomElement xmlTemplatesElement;
                                while (!xmlTemplatesNode.isNull())
                                {
                                    xmlTemplatesElement = xmlTemplatesNode.toElement();
                                    if (xmlTemplatesElement.tagName() == "VHDL")
                                    {
                                        if (xmlTemplatesElement.attribute("default") == "true")
                                        {
                                            defaultVHDL = true;
                                        }
                                        else
                                        {
                                            defaultVHDL = false;
                                        }
                                        templateVHDL = xmlTemplatesElement.attribute("path");
                                    }
                                    if (xmlTemplatesElement.tagName() == "Verilog")
                                    {
                                        if (xmlTemplatesElement.attribute("default") == "true")
                                        {
                                            defaultVerilog = true;
                                        }
                                        else
                                        {
                                            defaultVerilog = false;
                                        }
                                        templateVerilog = xmlTemplatesElement.attribute("path");
                                    }
                                    xmlTemplatesNode = xmlTemplatesNode.nextSibling();
                                }
                            }
                            else if (xmlCompilerElement.tagName() == "IncludePaths")
                            {
                                //qDebug() << "Include paths";
                                QDomNode xmlIncludeNode = xmlCompilerElement.firstChild();
                                QDomElement xmlIncludeElement;
                                while (!xmlIncludeNode.isNull())
                                {
                                    xmlIncludeElement = xmlIncludeNode.toElement();
                                    if (xmlIncludeElement.tagName() == "Path")
                                    {
                                        compileIncPaths << xmlIncludeElement.attribute("path");
                                        //qDebug() << "Include";
                                    }
                                    xmlIncludeNode = xmlIncludeNode.nextSibling();
                                }
                            }
                            xmlCompilerNode = xmlCompilerNode.nextSibling();
                        }
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
            prjDockWidget = new QDockWidget(prjName, (QWidget *)(parent->parent()));
            prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
            prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            prjTreeWidget = new ProjectTree(prjDockWidget, true);
            /*connect signals!!!*/
            prjDockWidget->setWidget(prjTreeWidget);

            this->treeProjName = new QTreeWidgetItem(prjTreeWidget);
            treeProjName->setText(0, prjName);
            treeProjName->setData(0, Qt::ToolTipRole, prjPath);

            this->treeProjSource = new QTreeWidgetItem(treeProjName);
            treeProjSource->setText(0, "Source");
            
            this->treeProjInclude = new QTreeWidgetItem(treeProjName);
            treeProjInclude->setText(0, "Include");

            this->treeProjCompiled = new QTreeWidgetItem(treeProjName);
            treeProjCompiled->setText(0, "Compiled");

            this->treeProjOther = new QTreeWidgetItem(treeProjName);
            treeProjOther->setText(0, "Other");


            QDir projectDir = QFileInfo(prjPath).dir();
            QString absolutePath = projectDir.path();
            for (int i=0; i<fileCount; i++)
            {
                QTreeWidgetItem *treeProjFile;
                int index = fileNames.at(i).lastIndexOf(".");
                if (index > 0)
                {
                    QString text(fileNames.at(i).right(fileNames.at(i).size() - index));
                    if (text == ".inc")
                    {
                        treeProjFile = new QTreeWidgetItem(treeProjInclude);
                    }
                    else if (text == ".asm" || text == ".psm")
                    {
                        treeProjFile = new QTreeWidgetItem(treeProjSource);
                    }
                    else
                    {
                        treeProjFile = new QTreeWidgetItem(treeProjOther);
                    }
                }
                else
                {
                    treeProjFile = new QTreeWidgetItem(treeProjOther);
                }
                treeProjFile->setText(0, fileNames.at(i));
                if ( NULL == QDir(absolutePath + "/" + filePaths.at(i)).canonicalPath())
                {
                    treeProjFile->setData(0, Qt::ToolTipRole, absolutePath + "/" + filePaths.at(i));
                    treeProjFile->setBackground(0, Qt::red);
                }
                else
                {
                    treeProjFile->setData(0, Qt::ToolTipRole, QDir(absolutePath + "/" + filePaths.at(i)).canonicalPath());
                }
            }
            if (mainFileName != "" && mainFilePath != "")
            {
                prjTreeWidget->setMainFileManual(mainFileName, mainFilePath);
            }

            connect(prjDockWidget, SIGNAL(visibilityChanged(bool)),this,SLOT(setActive()));  
            connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),this,SLOT(openItem()));
            connect(prjTreeWidget, SIGNAL(requestFileCount()), this, SLOT(emitFileCount()));
            connect(prjTreeWidget, SIGNAL(startProjectCfgDlgCore()), this, SLOT(startCfgDlgCore()));
            connect(prjTreeWidget, SIGNAL(setMainFile(QString, QString)), this, SLOT(setMainFile(QString, QString)));
            connect(prjTreeWidget, SIGNAL(removeFile(QString, QString)), this, SLOT(removeFile(QString, QString)));
            connect(prjTreeWidget, SIGNAL(addFile(QString, QString)), this, SLOT(addFile(QString, QString)));
            connect(prjTreeWidget, SIGNAL(closeProject()), this, SLOT(closeProjectSlot()));
            connect(this, SIGNAL(fileCountSignal(int)), prjTreeWidget, SLOT(contextP2(int)));
            setupSim();
        }
    }
    /*for (int i = 0; i < fileCount; i++)
    {
        qDebug() << filePaths.at(i);
    }*/
    //qDebug() << "Project: return Project()";
}



Project::Project(ProjectMan *parent)
    : QObject(parent)
{
    //qDebug() << "Project: Project()";
    parentManager = parent;
    this->m_simControlUnit = NULL;
    this->prjName = "untracked";
    this->prjPath = "untracked";

    currLineColor = GuiCfg::getInstance().getCurrLineColor();
    prevLineColor = GuiCfg::getInstance().getPrevLineColor();
    prevLine2Color = GuiCfg::getInstance().getPrevLine2Color();

    this->family = GuiCfg::getInstance().getProjectFamily();
    this->intVector = GuiCfg::getInstance().getProjectIntVector();
    this->hwBuild = GuiCfg::getInstance().getProjectHWBuild();
    this->scratchpadSize = GuiCfg::getInstance().getProjectScratchpadSize();
    this->progMemSize = GuiCfg::getInstance().getProjectProgMemSize();
    this->useMainFile = false;
    this->mainFileName = "";
    this->mainFilePath = "";
    this->compileOpt = GuiCfg::getInstance().getProjectCompOpt();
    this->defaultVHDL = GuiCfg::getInstance().getProjectDefVHDL();
    this->defaultVerilog = GuiCfg::getInstance().getProjectDefVerilog();
    this->templateVHDL =  GuiCfg::getInstance().getProjectPathVHDL();
    this->templateVerilog = GuiCfg::getInstance().getProjectPathVerilog();

    prjDockWidget = new QDockWidget(prjName, (QWidget *)(parent->parent()));
    prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    prjTreeWidget = new ProjectTree(prjDockWidget, false);
            /*connect signals!!! ...done*/

    prjDockWidget->setWidget(prjTreeWidget);


    this->treeProjName = new QTreeWidgetItem(prjTreeWidget);
    treeProjName->setText(0, prjName);
    treeProjName->setData(0, Qt::ToolTipRole, prjPath);
    fileCount=0;

    this->treeProjSource = new QTreeWidgetItem(treeProjName);
    treeProjSource->setText(0, "Source");

    this->treeProjInclude = new QTreeWidgetItem(treeProjName);
    treeProjInclude->setText(0, "Include");

    this->treeProjCompiled = new QTreeWidgetItem(treeProjName);
    treeProjCompiled->setText(0, "Compiled");

    this->treeProjOther = new QTreeWidgetItem(treeProjName);
    treeProjOther->setText(0, "Other");
    
    connect(prjDockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(setActive()));
    connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)), this, SLOT(openUntrackedItem()));
    connect(prjTreeWidget, SIGNAL(requestFileCount()), this, SLOT(emitFileCount()));
    connect(prjTreeWidget, SIGNAL(startProjectCfgDlgCore()), this, SLOT(startCfgDlgCore()));
    //connect(prjTreeWidget, SIGNAL(setMainFile(QString, QString)), this, SLOT(setMainFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(removeFile(QString, QString)), this, SLOT(removeFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(addFile(QString, QString)), this, SLOT(addFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(closeProject()), this, SLOT(closeProjectSlot()));
    connect(this, SIGNAL(fileCountSignal(int)), prjTreeWidget, SLOT(contextP2(int)));
    setupSim();
    //qDebug() << "Project: return Project()";
}



//vytvoreni prazdneho projektu
/**
 * @brief Constructor. Creates a blank new project
 * @param name The name of the project
 * @param path The path to xml (.mds-project) file
 * @param arch Compiler option, architecture
 * @param langType Compiler option, language
 * @param file Opened file for xml (.mds-project) stream
 * @param parent Pointer to parent (project manager)
 */
Project::Project(QString name, QString path, QString arch, LangType lang, QFile *file, ProjectMan *parent)
{
    //qDebug() << "Project: Project() blank";
    parentManager = parent;
    this->m_simControlUnit = NULL;
    currLineColor = GuiCfg::getInstance().getCurrLineColor();
    prevLineColor = GuiCfg::getInstance().getPrevLineColor();
    prevLine2Color = GuiCfg::getInstance().getPrevLine2Color();
    //currLineColor = new QColor(102,204,255,255);
    //prevLineColor = new QColor(102,204,255,125);
    //prevLine2Color = new QColor(102,204,255,50);

    if (name != NULL)
    {
        prjDockWidget = new QDockWidget(name, (QWidget *)(parent->parent()));
    }
    else
    {
        prjDockWidget = new QDockWidget("Projects", (QWidget *)(parent->parent()));
    }
    prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    prjTreeWidget = new ProjectTree(prjDockWidget, true);
            /*connect signals!!! - connected elsewhere*/

    prjDockWidget->setWidget(prjTreeWidget);
    //prjDockWidget->setMinimumWidth(150);
    prjName=name;
    prjPath=path;
    this->treeProjName = new QTreeWidgetItem(prjTreeWidget);
    treeProjName->setText(0, name);
    treeProjName->setData(0, Qt::ToolTipRole, path);
    fileCount=0;

    this->treeProjSource = new QTreeWidgetItem(treeProjName);
    this->treeProjSource->setText(0, "Source");

    this->treeProjInclude = new QTreeWidgetItem(treeProjName);
    this->treeProjInclude->setText(0, "Include");

    this->treeProjCompiled = new QTreeWidgetItem(treeProjName);
    this->treeProjCompiled->setText(0, "Compiled");

    this->treeProjOther = new QTreeWidgetItem(treeProjName);
    this->treeProjOther->setText(0, "Other");

    for (int i = 0; i < 12; i++)
    {
        compileOpt.append(false);
    }

    this->family = arch;
    this->langType = lang;
    
    //a zapsani do souboru
    QDomDocument domDoc("MDSProject");
    QDomElement xmlRoot = domDoc.createElement("MDSProject");
    domDoc.appendChild(xmlRoot);

    QDomElement xmlGeneral = domDoc.createElement("General");
    QDomElement xmlName = domDoc.createElement("Name");
    xmlName.setAttribute("name", name);
    xmlGeneral.appendChild(xmlName);
    QDomElement xmlArch = domDoc.createElement("Family");
    xmlArch.setAttribute("family", arch);
    xmlGeneral.appendChild(xmlArch);
    QDomElement xmlLang = domDoc.createElement("Language");
    xmlLang.setAttribute("language", lang);
    xmlGeneral.appendChild(xmlLang);
    QDomElement xmlScratchpad = domDoc.createElement("ScratchpadSize");
    xmlGeneral.appendChild(xmlScratchpad);
    QDomElement xmlProgMem = domDoc.createElement("ProgMemSize");
    xmlGeneral.appendChild(xmlProgMem);
    QDomElement xmlIntVector = domDoc.createElement("IntVector");
    xmlGeneral.appendChild(xmlIntVector);
    QDomElement xmlHWBuild = domDoc.createElement("HWBuild");
    xmlGeneral.appendChild(xmlHWBuild);
    xmlRoot.appendChild(xmlGeneral);

    QDomElement xmlFiles = domDoc.createElement("Files");
    xmlRoot.appendChild(xmlFiles);

    QDomElement xmlMainFile = domDoc.createElement("Mainfile");
    xmlMainFile.setAttribute("name", "");
    xmlMainFile.setAttribute("path", "");
    xmlMainFile.setAttribute("enabled", "false");
    xmlRoot.appendChild(xmlMainFile);

    QDomElement xmlSimulator = domDoc.createElement("Simulator");
    xmlRoot.appendChild(xmlSimulator);

    QDomElement xmlCompiler = domDoc.createElement("Compiler");
    
    QDomElement xmlCompilerOpt = domDoc.createElement("Options");
    QDomElement xmlSymbolTbl = domDoc.createElement("SymbolTable");
    xmlSymbolTbl.setAttribute("enable", "false");
    xmlCompilerOpt.appendChild(xmlSymbolTbl);
    QDomElement xmlMacroTbl = domDoc.createElement("MacroTable");
    xmlMacroTbl.setAttribute("enable", "false");
    xmlCompilerOpt.appendChild(xmlMacroTbl);
    QDomElement xmlDbgFile = domDoc.createElement("DebugFile");
    xmlDbgFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlDbgFile);
    QDomElement xmlCodeTree = domDoc.createElement("CodeTree");
    xmlCodeTree.setAttribute("enable", "false");
    xmlCompilerOpt.appendChild(xmlCodeTree);
    QDomElement xmlLstFile = domDoc.createElement("ListFile");
    xmlLstFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlLstFile);
    QDomElement xmlHexFile = domDoc.createElement("HexFile");
    xmlHexFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlHexFile);
    QDomElement xmlBinFile = domDoc.createElement("BinFile");
    xmlBinFile.setAttribute("enable", "false");
    xmlCompilerOpt.appendChild(xmlBinFile);
    QDomElement xmlSRecFile = domDoc.createElement("SRecFile");
    xmlSRecFile.setAttribute("enable", "false");
    xmlCompilerOpt.appendChild(xmlSRecFile);
    QDomElement xmlMemFile = domDoc.createElement("MemFile");
    xmlMemFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlMemFile);
    QDomElement xmlRawHexFile = domDoc.createElement("RawHexFile");
    xmlRawHexFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlRawHexFile);
    QDomElement xmlVerilogFile = domDoc.createElement("VerilogFile");
    xmlVerilogFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlVerilogFile);
    QDomElement xmlVHDLFile = domDoc.createElement("VHDLFile");
    xmlVHDLFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlVHDLFile);
    xmlCompiler.appendChild(xmlCompilerOpt);
    
    QDomElement xmlCompilerTemplates = domDoc.createElement("Templates");
    QDomElement xmlVHDLTemplate = domDoc.createElement("VHDL");
    xmlVHDLTemplate.setAttribute("default", "true");
    xmlVHDLTemplate.setAttribute("path", "");
    xmlCompilerTemplates.appendChild(xmlVHDLTemplate);
    QDomElement xmlVerilogTemplate = domDoc.createElement("Verilog");
    xmlVerilogTemplate.setAttribute("default", "true");
    xmlVerilogTemplate.setAttribute("path", "");
    xmlCompilerTemplates.appendChild(xmlVerilogTemplate);
    xmlCompiler.appendChild(xmlCompilerTemplates);
    
    QDomElement xmlCompilerInclude = domDoc.createElement("IncludePaths");
    xmlCompiler.appendChild(xmlCompilerInclude);
    xmlRoot.appendChild(xmlCompiler);

    QTextStream xmlStream(file);
    xmlStream << domDoc.toString();

    connect(prjDockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(setActive()));
    connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem*,int)), this, SLOT(openItem()));
    connect(prjTreeWidget, SIGNAL(requestFileCount()), this, SLOT(emitFileCount()));
    connect(prjTreeWidget, SIGNAL(startProjectCfgDlgCore()), this, SLOT(startCfgDlgCore()));
    connect(prjTreeWidget, SIGNAL(setMainFile(QString, QString)), this, SLOT(setMainFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(removeFile(QString, QString)), this, SLOT(removeFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(addFile(QString, QString)), this, SLOT(addFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(closeProject()), this, SLOT(closeProjectSlot()));
    connect(this, SIGNAL(fileCountSignal(int)), prjTreeWidget, SLOT(contextP2(int)));
    setupSim();
    //qDebug() << "Project: return Project() blank";
}



/**
 * @brief Adds file to the active project
 * @param path The path to the file
 * @param name The name of the file
 */
QString Project::addFile(QString path, QString name)
{
    //qDebug() << "Project: addFile()";
    QString fileName = name;
    if (this->prjName != "untracked" && this->prjPath != "untracked")
    {
        for (int i = 0; i < this->fileCount; i++)
        {
            if (path == prjPath.section('/', 0, -2) + "/" + filePaths.at(i))
            {
                return "";
            }
        }
        QString relativePath;
        QDomDocument domDoc("MDSProject");
        QFile *file = new QFile(prjPath);
        if(!file->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            error(ERR_OPENFILE);
            return "";
        }
        if (!domDoc.setContent(file))
        {
            error(ERR_XML_ASSIGN);
        }
        else
        {
            QDir project(QFileInfo(this->prjPath).dir());
            relativePath = project.relativeFilePath(path);
            QDomElement xmlRoot = domDoc.documentElement();
            if (xmlRoot.tagName() != "MDSProject")
            {
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
                        }
                        else if (xmlElement.tagName() == "Mainfile")
                        {
                            if (mainFileName == ""
                                && mainFilePath == "")
                            {
                                mainFileName = name;
                                mainFilePath = relativePath;
                                xmlElement.setAttribute("name", mainFileName);
                                xmlElement.setAttribute("path", mainFilePath);
                                xmlElement.setAttribute("enabled", "false");
                            }
                        }
                    }
                    xmlNode = xmlNode.nextSibling();
                }
                file->close();
                file->open(QIODevice::WriteOnly);

                QTextStream xmlStream(file);
                xmlStream << domDoc.toString();

                QTreeWidgetItem *treeProjFile;
                int index = name.lastIndexOf(".");
                if (index > 0)
                {
                    QString text(name.right(name.size() - index));
                    if (text == ".inc")
                    {
                        treeProjFile = new QTreeWidgetItem(treeProjInclude);
                    }
                    else if (text == ".asm" || text == ".psm")
                    {
                        treeProjFile = new QTreeWidgetItem(treeProjSource);
                    }
                    else
                    {
                        treeProjFile = new QTreeWidgetItem(treeProjOther);
                    }
                }
                else
                {
                    treeProjFile = new QTreeWidgetItem(treeProjOther);
                }
                treeProjFile->setText(0, name);
                treeProjFile->setData(0, Qt::ToolTipRole, path);

            }
        }
        file->close();
    }
    else
    {

        QTreeWidgetItem *treeProjFile;
        int index = name.lastIndexOf(".");
        if (index > 0)
        {
            QString text(name.right(name.size() - index));
            if (text == ".inc")
            {
                treeProjFile = new QTreeWidgetItem(treeProjInclude);
            }
            else if (text == ".asm" || text == ".psm")
            {
                treeProjFile = new QTreeWidgetItem(treeProjSource);
            }
            else
            {
                treeProjFile = new QTreeWidgetItem(treeProjOther);
            }
        }
        else
        {
            treeProjFile = new QTreeWidgetItem(treeProjOther);
        }
        if (path == "" || name == "")
        {
            if (name == "disasm")
            {
                treeProjFile->setText(0, "disasm"+QString::number(fileCount));
                fileName = "disasm"+QString::number(fileCount);
            }
            else
            {
                treeProjFile->setText(0, "untracked"+QString::number(fileCount));
                fileName = "untracked"+QString::number(fileCount);
            }
            treeProjFile->setData(0, Qt::ToolTipRole, "untracked");
            filePaths.append("untracked");
        }
        else
        {
            treeProjFile->setText(0, name);
            treeProjFile->setData(0, Qt::ToolTipRole, path);
            filePaths.append(path);
        }
        fileCount++;
    }
    return fileName;
    //qDebug() << "Project: return addFile()";
}



/**
 * @brief Slot. Sets current project as active
 */
void Project::setActive()
{
    //qDebug() << "Project: setActive()";
    if (false == parentManager->isActiveProject(this))
    {
        parentManager->setActive(this);
    }
    //qDebug() << "Project: return setActive()";
}


/**
 * @brief Slot. Opens selected item from untracked project's tree widget
 */
void Project::openUntrackedItem()
{
    //qDebug() << "Project: openUntrackedItem()";
    if (prjTreeWidget->currentItem() != NULL)
    {
        emit addUntrackedFile(prjTreeWidget->currentItem()->text(0), prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
        //parentWindow->getWDockManager()->addUntrackedCentralWidget(prjTreeWidget->currentItem()->text(0), prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
        //parentWindow->getWDockManager()->getCentralWidget()->setChanged();
        //wDockManager->getCentralWidget()->connectAct();
    }
    //qDebug() << "Project: return openUntrackedItem()";
}


/**
 * @brief Slot. Opens selected item from project's tree widget
 */
void Project::openItem()
{
    //qDebug() << "Project: openFile()";
    if (prjTreeWidget->currentItem() != NULL)
    {
        //qDebug() << "Project: emit OpenFilePath";
        emit openFilePath(prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
        //parentManager->mainWindow->openFilePath(prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
    }
    //qDebug() << "Project: return openFile()";
}



/**
 * @brief Sets selected name and path as main file
 * @param path The path to the selected file
 * @param name The name of the selected file
 */
void Project::setMainFile(QString path, QString name)
{
    //qDebug() << "Project: setMainFile()";
    QDir project(QFileInfo(prjPath).dir());
    QString relativePath = project.relativeFilePath(path);

    mainFileName = name;
    mainFilePath = relativePath;

    //QDir project(QFileInfo(prjPath).dir());
    //QString relativePath = project.relativeFilePath(path);

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "Mainfile")
                    {
                        xmlElement.setAttribute("name", mainFileName);
                        xmlElement.setAttribute("path", mainFilePath);
                        done = true;
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
    //qDebug() << "Project: return setMainFile()";
}

/**
 * @brief Sets usage of main file
 * @param enabled indicator
 */ 
void Project::setUseMainFile(bool enabled)
{
    //qDebug() << "Project: setMainFile()";
    //QDir project(QFileInfo(prjPath).dir());

    //QDir project(QFileInfo(prjPath).dir());
    //QString relativePath = project.relativeFilePath(path);
    useMainFile = enabled;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "Mainfile")
                    {
                        if (enabled == true)
                        {
                            xmlElement.setAttribute("enabled", "true");
                        }
                        else
                        {
                            xmlElement.setAttribute("enabled", "false");
                        }
                        done = true;
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
    //qDebug() << "Project: return setMainFile()";
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setIntVector(int value)
{
    intVector = value;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            bool done = false;
            while (xmlNode.isNull() == false && done == false)
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (xmlGeneralNode.isNull() == false && done == false)
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (!xmlGeneralElement.isNull())
                            {
                                if (xmlGeneralElement.tagName() == "IntVector")
                                {
                                    xmlGeneralElement.setAttribute("value", QString::number(value));
                                    done = true;
                                }
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
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
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setHWBuild(int value)
{
    hwBuild = value;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (!xmlGeneralNode.isNull() && done == false)
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (!xmlGeneralElement.isNull())
                            {
                                if (xmlGeneralElement.tagName() == "HWBuild")
                                {
                                    xmlGeneralElement.setAttribute("value", QString::number(value));
                                    done = true;
                                }
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
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
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setScratchpad(int value)
{
    scratchpadSize = value;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (!xmlGeneralNode.isNull() && done == false)
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (!xmlGeneralElement.isNull())
                            {
                                if (xmlGeneralElement.tagName() == "ScratchpadSize")
                                {
                                    xmlGeneralElement.setAttribute("value", QString::number(value));
                                    done = true;
                                }
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
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
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setProgMem(int value)
{
    progMemSize = value;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (!xmlGeneralNode.isNull() && done == false)
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (!xmlGeneralElement.isNull())
                            {
                                if (xmlGeneralElement.tagName() == "ProgMemSize")
                                {
                                    xmlGeneralElement.setAttribute("value", QString::number(value));
                                    done = true;
                                }
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
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
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setName(QString name)
{
    prjName = name;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (!xmlGeneralNode.isNull() && done == false)
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (!xmlGeneralElement.isNull())
                            {
                                if (xmlGeneralElement.tagName() == "Name")
                                {
                                    xmlGeneralElement.setAttribute("name", name);
                                    done = true;
                                }
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
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
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setFamily(QString family)
{
    this->family = family;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "General")
                    {
                        QDomNode xmlGeneralNode = xmlElement.firstChild();
                        QDomElement xmlGeneralElement;
                        while (!xmlGeneralNode.isNull() && done == false)
                        {
                            xmlGeneralElement = xmlGeneralNode.toElement();
                            if (!xmlGeneralElement.isNull())
                            {
                                if (xmlGeneralElement.tagName() == "Family")
                                {
                                    xmlGeneralElement.setAttribute("family", family);
                                    done = true;
                                }
                            }
                            xmlGeneralNode = xmlGeneralNode.nextSibling();
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
    emit changeFamily(family);
}


void Project::setTemplates(bool verilog, QString verilogTemplate, bool VHDL, QString VHDLTemplate)
{
    this->defaultVerilog = verilog;
    this->defaultVHDL = VHDL;
    this->templateVerilog = verilogTemplate;
    this->templateVHDL = VHDLTemplate;

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "Compiler")
                    {
                        QDomNode xmlCompilerNode = xmlElement.firstChild();
                        QDomElement xmlCompilerElement;
                        while (!xmlCompilerNode.isNull())
                        {
                            xmlCompilerElement = xmlCompilerNode.toElement();
                            if (xmlCompilerElement.tagName() == "Templates")
                            {
                                QDomNode xmlTemplatesNode = xmlElement.firstChild();
                                QDomElement xmlTemplatesElement;
                                while (!xmlTemplatesNode.isNull())
                                {
                                    xmlTemplatesElement = xmlTemplatesNode.toElement();
                                    if (!xmlTemplatesElement.isNull())
                                    {
                                        if (xmlTemplatesElement.tagName() == "VHDLTemplate")
                                        {
                                            if (true == this->defaultVHDL)
                                            {
                                                xmlTemplatesElement.setAttribute("default", "true");
                                            }
                                            else
                                            {
                                                xmlTemplatesElement.setAttribute("default", "false");
                                            }
                                            xmlTemplatesElement.setAttribute("path", this->templateVHDL);
                                        }
                                        else if (xmlTemplatesElement.tagName() == "VerilogTemplate")
                                        {
                                            if (true == this->defaultVerilog)
                                            {
                                                xmlTemplatesElement.setAttribute("default", "true");
                                            }
                                            else
                                            {
                                                xmlTemplatesElement.setAttribute("default", "false");
                                            }
                                            xmlTemplatesElement.setAttribute("path", this->templateVerilog);
                                        }
                                    }
                                    xmlTemplatesNode = xmlTemplatesNode.nextSibling();
                                }
                            }
                            xmlCompilerNode = xmlCompilerNode.nextSibling();
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
}


/**
 * @brief Removes selected file from project
 * @param path The path to the file
 * @param name The name of the file
 */
void Project::removeFile(QString path, QString name)
{
    //qDebug() << "Project: removeFile()";
    QDir project(QFileInfo(prjPath).dir());
    QString relativePath = project.relativeFilePath(path);
    if (relativePath == this->mainFilePath && name == mainFileName)
    {
        this->setMainFile("", "");
    }

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                                && xmlFilesElement.attribute("name") == name
                                && xmlFilesElement.attribute("path") == relativePath)
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
            prjFile.close();
        }
    }
    //pozor na stejna jmena, musi se to smazat zaroven (item at index check pres iterator)
    fileNames.removeOne(name);
    filePaths.removeOne(path);
    fileCount--;
    //qDebug() << "Project: return removeFile()";
}



/**
 * @brief Inits simulation control unit
 */
void Project::setupSim()
{
    //qDebug() << "Project: setupSim()";
    //McuSimCfgMgr::getInstance()->openConfigFile(":/resources//xml//mcuspecfile.xml");
    //"kcpsm3"
    if (this->m_simControlUnit == NULL)
    {
        this->m_simControlUnit = new MCUSimControl(family.toUtf8().constData());
        connect(m_simControlUnit, SIGNAL(updateRequest(int)), this, SLOT(handleUpdateRequest(int)));
    }
    //qDebug() << architecture;
    //qDebug() << "Project: return setupSim()";
}



/**
 * @brief Inits simulation control unit
 */
void Project::setupSim(QString family)
{
    //qDebug() << "Project: setupSim()";
    //McuSimCfgMgr::getInstance()->openConfigFile(":/resources//xml//mcuspecfile.xml");
    //"kcpsm3"
    this->m_simControlUnit->changeDevice(family.toUtf8().constData());
    //qDebug() << architecture;
    //qDebug() << "Project: return setupSim()";
}


/**
 * @brief Starts simulation
 * @return 0 - started; >0 - not started;
 * @details  1 - sim not started; 2 - getLineNumber error; 3 - files do not exist; 4 - files modified;
 */
int Project::start(QString file)
{
    //qDebug() << "Project: start()";
    //parentWindow->getWDockManager()->setEditorsReadOnly(true);
    if (langType == LANG_ASM)
    {
        //QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        QString hexPath;
        if (file != "")
        {
            hexPath = file.section('.',0,-2);
        }
        else
        {
            QDir dir(prjPath.section('/',0, -2));
            hexPath = dir.absoluteFilePath(mainFileName.section('.',0,-2));
        }
        //QString hexPath = prjPath.section('/',0, -2) + "/" + mainFileName.section('.',0,-2);
        QFileInfo infoAsm(file);
        QFileInfo infoHex(hexPath + ".ihex");
        QFileInfo infoDbg(hexPath + ".dbg");
        if ( false == infoHex.exists() || false == infoDbg.exists())
        {
            qDebug() << "Project: files do not exist";
            return 3;
        }
        if (infoAsm.lastModified() > infoHex.lastModified() || infoAsm.lastModified() > infoDbg.lastModified())
        {
            qDebug() << "Project: files modified";
            return 4;
        }
        qDebug() << "ASM:" << hexPath;
        std::string stdPath = hexPath.toUtf8().constData();
        if ( false == m_simControlUnit->startSimulation(stdPath,
                                                        m_simControlUnit->COMPILER_NATIVE,
                                                        m_simControlUnit->DBGFILEID_HEX)
           )
        {
            qDebug() << "Project: m_simControlUnit->startSimulation() returned false";
            std::vector<std::string> messages = m_simControlUnit->getMessages();
            for (unsigned int i = 0; i < messages.size(); i++)
            {
                qDebug() << QString::fromStdString(messages.at(i));
            }
            return 1;
        }
        //else
        //{
        //    qDebug() << "Project: m_simControlUnit->startSimulation() returned true";
        //}
    }
    else if (langType == LANG_C)
    {
        QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        //qDebug() << "C:" << hexPath;
        std::string stdPath = hexPath.toUtf8().constData();
        if ( false == m_simControlUnit->startSimulation(stdPath,
                                                        m_simControlUnit->COMPILER_GCC,
                                                        m_simControlUnit->DBGFILEID_HEX)
           )
        {
            qDebug() << "Project: return false start()";
            return 1;
        }
        //else
        //{
        //    qDebug() << "Project: m_simControlUnit->start() returned true";
        //}
    }
    //qDebug() << "Project: getLineNumber";
    m_simControlUnit->getLineNumber(currLine);
    //qDebug() << "Project: getLineNumber check";
    if (currLine.empty() == true)
    {
        return 2;
    }
    //qDebug() << "Project: getLineNumber done";
    emit setEditorReadOnly(true);
    //qDebug() << "Project: currFile";
    this->currFile = QString::fromStdString(*(std::get<0>(this->currLine.at(0))));
    //qDebug() << "Project: current line number:" << line << "in file" << this->currFile;
    //qDebug() << "Project: program counter value:" << dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter();
    //qDebug() << "Project: highlightLine";
    emit highlightLine(this->currFile, std::get<1>(this->currLine.at(0))-1, &(this->currLineColor));
    //parentWindow->getWDockManager()->setCentralByName(fileNameQStr);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line, currLineColor, origCurrLineCol);
    this->prevLine = std::get<1>(this->currLine.at(0))-1;
    this->prevLine2 = -1;
    this->prevLine3 = -1;

    this->prevFile = this->currFile;
    this->prevFile2 = this->currFile;
    this->prevFile3 = this->currFile;
    //qDebug() << "Project: return start()";
    return 0;
}



/**
 * @brief Stops simulation
 */
void Project::stop()
{
    //qDebug() << "Project: stop()";
    std::string fileName; //= new std::string;
    m_simControlUnit->getLineNumber(currLine);
    if (currLine.empty() == true)
    {
        return;
    }
    this->currFile = QString::fromStdString(*(std::get<0>(this->currLine.at(0))));
    m_simControlUnit->stopSimulation();
    emit highlightLine(this->prevFile, this->prevLine, NULL);
    emit highlightLine(this->prevFile2, this->prevLine2, NULL);
    emit highlightLine(this->prevFile3, this->prevLine3, NULL);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine, NULL, NULL);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine2, NULL, NULL);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine3, NULL, NULL);
    emit setEditorReadOnly(false);
    //parentWindow->getWDockManager()->setEditorsReadOnly(false);
    //qDebug() << "Project: return stop()";
}


/**
 * @brief Resets simulation
 */
void Project::reset()
{
    m_simControlUnit->resetProgram();
}


void Project::handleUpdateRequest(int mask)
{
    if (4 & mask)
    {
        //std::string fileName; //= new std::string();
        m_simControlUnit->getLineNumber(currLine);
        if (currLine.empty() == true)
        {
            qDebug() << "Project: currline empty, should never happen";
            return;
        }
        this->currFile = QString::fromStdString(*(std::get<0>(this->currLine.at(0))));
        //qDebug() << "Project: current line number:" << line << "in file" << this->currFile;
        //qDebug() << "Project: program counter value:" << dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter();
        //parentWindow->getWDockManager()->setCentralByName(fileNameQStr);
        emit highlightLine(this->prevFile3, this->prevLine3, NULL);
        emit highlightLine(this->prevFile2, this->prevLine2, NULL);
        emit highlightLine(this->prevFile, this->prevLine, NULL);
        emit highlightLine(this->currFile, std::get<1>(this->currLine.at(0))-1, &(this->currLineColor));

        //emit setCentralByName(this->currFile);
        //emit scrollToLine(this->line);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line, currLineColor, origCurrLineCol);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine, prevLineColor, origPrevLineCol);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine2, prevLine2Color, origPrevLine2Col);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine3, NULL, NULL);
        this->prevLine3 = -1;
        this->prevLine2 = -1;
        this->prevLine = std::get<1>(this->currLine.at(0))-1;

        this->prevFile3 = this->currFile;
        this->prevFile2 = this->currFile;
        this->prevFile = this->currFile;
    }
    else if (2 & mask)
    {
        //std::string fileName; //= new std::string();
        m_simControlUnit->getLineNumber(currLine);
        if (currLine.empty() == true)
        {
            return;
        }
        this->currFile = QString::fromStdString(*(std::get<0>(this->currLine.at(0))));
        //qDebug() << "Project: current line number:" << line << "in file" << this->currFile;
        //qDebug() << "Project: program counter value:" << dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter();
        //parentWindow->getWDockManager()->setCentralByName(fileNameQStr);
        emit highlightLine(this->prevFile3, this->prevLine3, NULL);
        emit highlightLine(this->prevFile2, this->prevLine2,&(this->prevLine2Color));
        emit highlightLine(this->prevFile, this->prevLine, &(this->prevLineColor));
        emit highlightLine(this->currFile, std::get<1>(this->currLine.at(0))-1, &(this->currLineColor));

        //emit setCentralByName(this->currFile);
        //emit scrollToLine(this->line);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line, currLineColor, origCurrLineCol);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine, prevLineColor, origPrevLineCol);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine2, prevLine2Color, origPrevLine2Col);
        //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine3, NULL, NULL);
        this->prevLine3 = this->prevLine2;
        this->prevLine2 = this->prevLine;
        this->prevLine = std::get<1>(this->currLine.at(0))-1;

        this->prevFile3 = this->prevFile2;
        this->prevFile2 = this->prevFile;
        this->prevFile = this->currFile;
    }
}


/**
 * @brief Makes step in simulation
 */
void Project::step()
{
    m_simControlUnit->stepProgram();
    /*std::string fileName; //= new std::string();
    m_simControlUnit->getLineNumber(currLine);
    if (currLine.empty() == true)
    {
        return;
    }
    this->currFile = QString::fromStdString(*(std::get<0>(this->currLine.at(0))));
    //qDebug() << "Project: current line number:" << line << "in file" << this->currFile;
    //qDebug() << "Project: program counter value:" << dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter();
    //parentWindow->getWDockManager()->setCentralByName(fileNameQStr);
    emit highlightLine(this->prevFile3, this->prevLine3, NULL);
    emit highlightLine(this->prevFile2, this->prevLine2,this-> prevLine2Color);
    emit highlightLine(this->prevFile, this->prevLine, this->prevLineColor);
    emit highlightLine(this->currFile, std::get<1>(this->currLine.at(0))-1, this->currLineColor);

    //emit setCentralByName(this->currFile);
    //emit scrollToLine(this->line);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line, currLineColor, origCurrLineCol);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine, prevLineColor, origPrevLineCol);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine2, prevLine2Color, origPrevLine2Col);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine3, NULL, NULL);
    this->prevLine3 = this->prevLine2;
    this->prevLine2 = this->prevLine;
    this->prevLine = std::get<1>(this->currLine.at(0))-1;

    this->prevFile3 = this->prevFile2;
    this->prevFile2 = this->prevFile;
    this->prevFile = this->currFile;*/
    /*origPrevLine3Col = origPrevLine2Col;
    origPrevLine2Col = origPrevLineCol;
    origPrevLineCol = origCurrLineCol;*/
}


/**
 * @brief Runs simulation
 */
void Project::run()
{
    m_simControlUnit->runProgram();
}


/**
 * @brief Runs simulation with animate
 */
void Project::animate()
{
    m_simControlUnit->animateProgram();
}


/**
 * @brief Returns simulation control unit
 * @return Returns simulation control unit
 */
MCUSimControl* Project::getSimControl()
{
    //qDebug() << "Project: getSimControl()";
    if (this->m_simControlUnit == NULL)
    {
        qDebug() << "Project: simcontrolunit is NULL";
    }
    //qDebug() << "Project: return getSimControl()";
    return this->m_simControlUnit;
}


void Project::emitFileCount()
{
    //qDebug() << "Project: emitFileCount()";
    emit fileCountSignal(fileCount);
    //qDebug() << "Project: return emitFileCount()";
}


void Project::startCfgDlgCore()
{
    emit startConfig(this);
}


void Project::setCompileOpt(QList<bool> opt)
{

    for (int i = 0; i < opt.size(); i++)
    {
        this->compileOpt[i] = opt.at(i);
    }
    
    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "Compiler")
                    {
                        QDomNode xmlCompilerNode = xmlElement.firstChild();
                        QDomElement xmlCompilerElement;
                        while (!xmlCompilerNode.isNull())
                        {
                            xmlCompilerElement = xmlCompilerNode.toElement();
                            if (xmlCompilerElement.tagName() == "Options")
                            {
                                QDomNode xmlCompOptNode = xmlCompilerNode.firstChild();
                                while (!xmlCompOptNode.isNull())
                                {
                                    QDomElement xmlCompOptElem = xmlCompOptNode.toElement();
                                    //prochazeni elementu
                                    if (xmlCompOptElem.tagName() == "SymbolTable")
                                    {
                                        if (true == this->compileOpt.at(0))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "MacroTable")
                                    {
                                        if (true == this->compileOpt.at(1))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "DebugFile")
                                    {
                                        if (true == this->compileOpt.at(2))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "CodeTree")
                                    {
                                        if (true == this->compileOpt.at(3))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "ListFile")
                                    {
                                        if (true == this->compileOpt.at(4))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "HexFile")
                                    {
                                        if (true == this->compileOpt.at(5))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "BinFile")
                                    {
                                        if (true == this->compileOpt.at(6))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "SRecFile")
                                    {
                                        if (true == this->compileOpt.at(7))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "MemFile")
                                    {
                                        if (true == this->compileOpt.at(8))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "RawHexFile")
                                    {
                                        if (true == this->compileOpt.at(9))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "VerilogFile")
                                    {
                                        if (true == this->compileOpt.at(10))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    else if (xmlCompOptElem.tagName() == "VHDLFile")
                                    {
                                        if (true == this->compileOpt.at(11))
                                        {
                                            xmlCompOptElem.setAttribute("enable", "true");
                                        }
                                        else
                                        {
                                            xmlCompOptElem.setAttribute("enable", "false");
                                        }
                                    }
                                    
                                    xmlCompOptNode = xmlCompOptNode.nextSibling();
                                }
                            }
                            xmlCompilerNode = xmlCompilerNode.nextSibling();
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
}


void Project::setCompileIncPaths(QList<QString> paths)
{
    this->compileIncPaths.clear();
    for (int i = 0; i < paths.size(); i++)
    {
        this->compileIncPaths.append(paths.at(i));
    }

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
    QDomDocument domDoc("MDSProject");
    if (!domDoc.setContent(&prjFile))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        //otevrit xml, upravit a ulozit
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "MDSProject")
        {
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
                    if (xmlElement.tagName() == "Compiler")
                    {
                        QDomNode xmlCompilerNode = xmlElement.firstChild();
                        QDomElement xmlCompilerElement;
                        while (!xmlCompilerNode.isNull())
                        {
                            xmlCompilerElement = xmlCompilerNode.toElement();
                            if (xmlCompilerElement.tagName() == "IncludePaths")
                            {
                                //smazat tag
                                xmlNode.removeChild(xmlCompilerNode);
                                //znovu pridat tag
                                QDomElement xmlNewCompilerElement = domDoc.createElement("IncludePaths");
                                for (int i = 0; i < this->compileIncPaths.size(); i++)
                                {
                                    QDomElement xmlNewPathElem = domDoc.createElement("Path");
                                    xmlNewPathElem.setAttribute("path", this->compileIncPaths.at(i));
                                    xmlNewCompilerElement.appendChild(xmlNewPathElem);
                                }
                                xmlElement.appendChild(xmlNewCompilerElement);
                                break;
                            }
                            xmlCompilerNode = xmlCompilerNode.nextSibling();
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
}


void Project::closeProjectSlot()
{
    emit closeProject();
}