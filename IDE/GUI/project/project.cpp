/**
 * @brief C++ file for ProjectMan class and Project class
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
#include "project.h"
#include "McuSimCfgMgr.h"
#include "MCUSimControl.h"
#include "../widgets/ProjectTree/projecttree.h"




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
 * @brief Opens and loads project according to opened xml (.mmp) file.
 * @param file Opened xml (.mmp) file.
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
    //qDebug() << "ProjectMan: return addUntrackedProject()";
}


/**
 * @brief Creates new project.
 * @param name name of the new project
 * @param path path to the new project xml (.mmp) file
 * @param architecture target architecture of the new project
 * @param file xml (.mmp) file of the new project
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
 * @brief Opens project from opened xml (.mmp) file.
 * @param file Opened xml (.mmp) file.
 * @param parent Project parent project manager.
 */
Project::Project(QFile *file, ProjectMan *parent)
    : QObject(parent)
{
    //qDebug() << "Project: Project()";
    mainFileName = "";
    mainFilePath = "";
    for (int i = 0; i < 8; i++)
    {
        compileOpt.append(false);
    }
    
    errorFlag = ERR_OK;
    fileCount = 0;
    parentManager = parent;
    this->m_simControlUnit = NULL;
    prjPath = QFileInfo(*file).filePath();
    currLineColor = new QColor(102,204,255,255);
    prevLineColor = new QColor(102,204,255,125);
    prevLine2Color = new QColor(102,204,255,50);
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
                            else if (xmlGeneralElement.tagName() == "Architecture")
                            {
                                architecture = xmlGeneralElement.attribute("architecture", "");
                            }
                            else if (xmlGeneralElement.tagName() == "Language")
                            {
                                langType = (LangType)(xmlGeneralElement.attribute("language", "").toInt(NULL));
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
                                    xmlCompileOptNode = xmlCompileOptNode.nextSibling();
                                    //qDebug() << "Opt";
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
            prjTreeWidget = new ProjectTree(prjDockWidget);
            /*connect signals!!!*/
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
    prjName = "untracked";
    prjPath = "untracked";

    prjDockWidget = new QDockWidget(prjName, (QWidget *)(parent->parent()));
    prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    prjTreeWidget = new ProjectTree(prjDockWidget);
            /*connect signals!!! ...done*/

    prjDockWidget->setWidget(prjTreeWidget);


    QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
    treeProjName->setText(0, prjName);
    treeProjName->setData(0, Qt::ToolTipRole, prjPath);
    fileCount=0;
    connect(prjDockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(setActive()));
    connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)), this, SLOT(openUntrackedItem()));
    connect(prjTreeWidget, SIGNAL(requestFileCount()), this, SLOT(emitFileCount()));
    connect(prjTreeWidget, SIGNAL(startProjectCfgDlgCore()), this, SLOT(startCfgDlgCore()));
    connect(prjTreeWidget, SIGNAL(setMainFile(QString, QString)), this, SLOT(setMainFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(removeFile(QString, QString)), this, SLOT(removeFile(QString, QString)));
    connect(prjTreeWidget, SIGNAL(addFile(QString, QString)), this, SLOT(addFile(QString, QString)));
    connect(this, SIGNAL(fileCountSignal(int)), prjTreeWidget, SLOT(contextP2(int)));
    //qDebug() << "Project: return Project()";
}



//vytvoreni prazdneho projektu
/**
 * @brief Constructor. Creates a blank new project
 * @param name The name of the project
 * @param path The path to xml (.mmp) file
 * @param arch Compiler option, architecture
 * @param langType Compiler option, language
 * @param file Opened file for xml (.mmp) stream
 * @param parent Pointer to parent (project manager)
 */
Project::Project(QString name, QString path, QString arch, LangType lang, QFile *file, ProjectMan *parent)
{
    //qDebug() << "Project: Project() blank";
    errorFlag = ERR_OK;
    parentManager = parent;
    this->m_simControlUnit = NULL;
    currLineColor = new QColor(102,204,255,255);
    prevLineColor = new QColor(102,204,255,125);
    prevLine2Color = new QColor(102,204,255,50);

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
    prjTreeWidget = new ProjectTree(prjDockWidget);
            /*connect signals!!!*/

    prjDockWidget->setWidget(prjTreeWidget);
    //prjDockWidget->setMinimumWidth(150);
    prjName=name;
    prjPath=path;
    QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
    treeProjName->setText(0, name);
    treeProjName->setData(0, Qt::ToolTipRole, path);
    fileCount=0;

    for (int i = 0; i < 8; i++)
    {
        compileOpt.append(false);
    }

    this->architecture = arch;
    this->langType = lang;
    
    //a zapsani do souboru
    QDomDocument domDoc("MMProject");
    QDomElement xmlRoot = domDoc.createElement("MMProject");
    domDoc.appendChild(xmlRoot);

    QDomElement xmlGeneral = domDoc.createElement("General");
    QDomElement xmlName = domDoc.createElement("Name");
    xmlName.setAttribute("name", name);
    xmlGeneral.appendChild(xmlName);
    QDomElement xmlArch = domDoc.createElement("Architecture");
    xmlArch.setAttribute("architecture", arch);
    xmlGeneral.appendChild(xmlArch);
    QDomElement xmlLang = domDoc.createElement("Language");
    xmlLang.setAttribute("language", lang);
    xmlGeneral.appendChild(xmlLang);
    xmlRoot.appendChild(xmlGeneral);

    QDomElement xmlFiles = domDoc.createElement("Files");
    xmlRoot.appendChild(xmlFiles);

    QDomElement xmlMainFile = domDoc.createElement("Mainfile");
    xmlMainFile.setAttribute("name", "");
    xmlMainFile.setAttribute("path", "");
    xmlRoot.appendChild(xmlMainFile);

    QDomElement xmlSimulator = domDoc.createElement("Simulator");
    xmlRoot.appendChild(xmlSimulator);

    QDomElement xmlCompiler = domDoc.createElement("Compiler");
    QDomElement xmlCompilerOpt = domDoc.createElement("Options");
    QDomElement xmlSymbolTbl = domDoc.createElement("SymbolTable");
    xmlSymbolTbl.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlSymbolTbl);
    QDomElement xmlMacroTbl = domDoc.createElement("MacroTable");
    xmlMacroTbl.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlMacroTbl);
    QDomElement xmlDbgFile = domDoc.createElement("DebugFile");
    xmlDbgFile.setAttribute("enable", "true");
    xmlCompilerOpt.appendChild(xmlDbgFile);
    QDomElement xmlCodeTree = domDoc.createElement("CodeTree");
    xmlCodeTree.setAttribute("enable", "true");
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
    xmlCompiler.appendChild(xmlCompilerOpt);
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
        QDomDocument domDoc("MMProject");
        QFile *file = new QFile(prjPath);
        if(!file->open(QIODevice::ReadWrite | QIODevice::Text))
        {
            error(ERR_OPENFILE);
            return "";
        }
        if (!domDoc.setContent(file))
        {
            errorFlag = ERR_ASSIGN;
            error(ERR_XML_ASSIGN);
        }
        else
        {
            QDir project(QFileInfo(this->prjPath).dir());
            relativePath = project.relativeFilePath(path);
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
                            }
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
        file->close();
    }
    else
    {
        QTreeWidgetItem *treeProjFile = new QTreeWidgetItem(prjTreeWidget->topLevelItem(0));
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
        }
        else
        {
            treeProjFile->setText(0, name);
            treeProjFile->setData(0, Qt::ToolTipRole, path);
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
 * @brief Removes selected file from project
 * @param path The path to the file
 * @param name The name of the file
 */
void Project::removeFile(QString path, QString name)
{
    //qDebug() << "Project: removeFile()";
    QDir project(QFileInfo(prjPath).dir());
    QString relativePath = project.relativeFilePath(path);

    QFile prjFile(prjPath);
    prjFile.open(QIODevice::ReadOnly);
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
    McuSimCfgMgr::getInstance()->openConfigFile(":/resources//xml//mcuspecfile.xml");
    //"kcpsm3"
    this->m_simControlUnit = new MCUSimControl(architecture.toUtf8().constData());
    //qDebug() << architecture;
    //qDebug() << "Project: return setupSim()";
}


/**
 * @brief Starts simulation
 */
bool Project::start()
{
    //qDebug() << "Project: start()";
    emit setEditorReadOnly(true);
    //parentWindow->getWDockManager()->setEditorsReadOnly(true);
    if (langType == LANG_ASM)
    {
        //QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        QDir dir(prjPath.section('/',0, -2));
        QString hexPath = dir.absoluteFilePath(mainFileName.section('.',0,-2));
        //QString hexPath = prjPath.section('/',0, -2) + "/" + mainFileName.section('.',0,-2);
        //qDebug() << "ASM:" << hexPath;
        std::string stdPath = hexPath.toUtf8().constData();
        if ( false == m_simControlUnit->start(stdPath, m_simControlUnit->COMPILER_NATIVE, m_simControlUnit->DBGFILEID_HEX) )
        {
            qDebug() << "Project: return false start()";
            std::vector<std::string> messages = m_simControlUnit->getMessages();
            for (unsigned int i = 0; i < messages.size(); i++)
            {
                qDebug() << QString::fromStdString(messages.at(i));
            }
            return false;
        }
        else
        {
            qDebug() << "Project: m_simControlUnit->start() returned true";
        }
    }
    else if (langType == LANG_C)
    {
        QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        //qDebug() << "C:" << hexPath;
        std::string stdPath = hexPath.toUtf8().constData();
        if ( false == m_simControlUnit->start(stdPath, m_simControlUnit->COMPILER_GCC, m_simControlUnit->DBGFILEID_HEX) )
        {
            qDebug() << "Project: return false start()";
            return false;
        }
        else
        {
            qDebug() << "Project: m_simControlUnit->start() returned true";
        }
    }
    //qDebug() << "Project: getLineNumber";
    std::string fileName; //= new std::string;
    this->line = m_simControlUnit->getLineNumber(&fileName) - 1;
    this->currFile = QString::fromStdString(fileName);
    //qDebug() << "Project: current line number:" << line << "in file" << this->currFile;
    //qDebug() << "Project: program counter value:" << dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter();
    emit highlightLine(this->currFile, this->line, this->currLineColor);
    //parentWindow->getWDockManager()->setCentralByName(fileNameQStr);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line, currLineColor, origCurrLineCol);
    this->prevLine = line;
    this->prevLine2 = -1;
    this->prevLine3 = -1;

    this->prevFile = this->currFile;
    this->prevFile2 = this->currFile;
    this->prevFile3 = this->currFile;
    //qDebug() << "Project: return start()";
    return true;
}



/**
 * @brief Stops simulation
 */
void Project::stop()
{
    //qDebug() << "Project: stop()";
    std::string fileName; //= new std::string;
    this->line = m_simControlUnit->getLineNumber(&fileName) - 1;
    this->currFile = QString::fromStdString(fileName);
    m_simControlUnit->stop();
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
    m_simControlUnit->reset();
}


/**
 * @brief Makes step in simulation
 */
void Project::step()
{
    m_simControlUnit->step();
    std::string fileName; //= new std::string();
    this->line = m_simControlUnit->getLineNumber(&fileName) - 1;
    this->currFile = QString::fromStdString(fileName);
    //qDebug() << "Project: current line number:" << line << "in file" << this->currFile;
    //qDebug() << "Project: program counter value:" << dynamic_cast<MCUSimCPU*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU))->getProgramCounter();
    //parentWindow->getWDockManager()->setCentralByName(fileNameQStr);
    emit highlightLine(this->prevFile3, this->prevLine3, NULL);
    emit highlightLine(this->prevFile2, this->prevLine2,this-> prevLine2Color);
    emit highlightLine(this->prevFile, this->prevLine, this->prevLineColor);
    emit highlightLine(this->currFile, this->line, this->currLineColor);

    emit setCentralByName(this->currFile);
    emit scrollToLine(this->line);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line, currLineColor, origCurrLineCol);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine, prevLineColor, origPrevLineCol);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine2, prevLine2Color, origPrevLine2Col);
    //parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(prevLine3, NULL, NULL);
    this->prevLine3 = this->prevLine2;
    this->prevLine2 = this->prevLine;
    this->prevLine = this->line;

    this->prevFile3 = this->prevFile2;
    this->prevFile2 = this->prevFile;
    this->prevFile = this->currFile;
    /*origPrevLine3Col = origPrevLine2Col;
    origPrevLine2Col = origPrevLineCol;
    origPrevLineCol = origCurrLineCol;*/
}


/**
 * @brief Runs simulation
 */
void Project::run()
{
    m_simControlUnit->run();
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