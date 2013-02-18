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
ProjectMan::ProjectMan(MainForm *qMainWindow)
{
    //openProjects = new QList<Project*>;
    projectCount = 0;
    mainWindow = qMainWindow;
    activeProject = NULL;
}


/**
 * @brief Opens and loads project according to opened xml (.mmp) file.
 * @param file Opened xml (.mmp) file.
 */
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
    {
        mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    }
    //else
    //    mainWindow->CreateDockWidgets();

    projectCount++;
}


/**
 * @brief Adds file to active project.
 * @param path Path to the file
 * @param name Name of the file
 */
void ProjectMan::addFile(QString path, QString name)
{
    getActive()->addFile(path, name);
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
    Project *newProject = new Project(name, path, architecture, langType, mainWindow, file, this);
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
    {
        mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);
    }
    else
    {
        mainWindow->CreateDockWidgets();
    }

    openProjects.append(newProject);
    activeProject = newProject;
    projectCount++;
}


/**
 * @brief Sets project selected by programmer as active
 * @param activePrj Pointer to the soon-to-be active project
 */
void ProjectMan::setActive(Project *activePrj)
{
    activeProject = activePrj;
}


/**
 * @brief Checks if project selected by programmer is active.
 * @param project Pointer to the checked project
 * @return True if project is active, otherwise false.
 */
bool ProjectMan::isActiveProject(Project *project)
{
    return (project==activeProject);
}


/**
 * @brief Returns pointer to the active project.
 * @return Pointer to the active project.
 */
Project* ProjectMan::getActive()
{
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
Project::~Project()
{
    delete this->prjDockWidget;
    
}

//otevreni projektu
/**
 * @brief Opens project from opened xml (.mmp) file.
 * @param file Opened xml (.mmp) file.
 * @param mainWindow 
 * @param parent Project parent project manager.
 */
Project::Project(QFile *file, MainForm* mainWindow, ProjectMan *parent)
{
    mainFileName = "";
    mainFilePath = "";
    
    errorFlag = ERR_OK;
    fileCount = 0;
    parentManager = parent;
    parentWindow = mainWindow;
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
                }
                xmlNode = xmlNode.nextSibling();
            }
            prjDockWidget = new QDockWidget(prjName, mainWindow);
            prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
            prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            prjTreeWidget = new ProjectTree(prjDockWidget, this);
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
            setupSim();
        }
    }
}


//vytvoreni prazdneho projektu
/**
 * @brief Constructor. Creates a blank new project
 * @param name The name of the project
 * @param path The path to xml (.mmp) file
 * @param arch Compiler option, architecture
 * @param langType Compiler option, language
 * @param mainWindow Pointer to parent main window
 * @param file Opened file for xml (.mmp) stream
 * @param parent Pointer to parent (project manager)
 */
Project::Project(QString name, QString path, QString arch, LangType lang, MainForm* mainWindow, QFile *file, ProjectMan *parent)
{
    errorFlag = ERR_OK;
    parentManager = parent;
    parentWindow = mainWindow;

    if (name != NULL)
    {
        prjDockWidget = new QDockWidget(name, mainWindow);
    }
    else
    {
        prjDockWidget = new QDockWidget("Projects", mainWindow);
    }
    prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    prjTreeWidget = new ProjectTree(prjDockWidget, this);

    prjDockWidget->setWidget(prjTreeWidget);
    //prjDockWidget->setMinimumWidth(150);
    prjName=name;
    prjPath=path;
    QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
    treeProjName->setText(0, name);
    treeProjName->setData(0, Qt::ToolTipRole, path);
    fileCount=0;

    architecture = arch;
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
    xmlRoot.appendChild(xmlCompiler);

    QTextStream xmlStream(file);
    xmlStream << domDoc.toString();

    connect(prjDockWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(setActive()));
    connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)), this, SLOT(openItem()));
    setupSim();
}



/**
 * @brief Adds file to the active project
 * @param path The path to the file
 * @param name The name of the file
 */
void Project::addFile(QString path, QString name)
{
    QString relativePath;
    QDomDocument domDoc("MMProject");
    QFile *file = new QFile(prjPath);
    if(!file->open(QIODevice::ReadWrite | QIODevice::Text))
    {
        error(ERR_OPENFILE);
        return;
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



/**
 * @brief Slot. Sets current project as active
 */
void Project::setActive()
{
    if (false == parentManager->isActiveProject(this))
    {
        parentManager->setActive(this);
    }
}



/**
 * @brief Slot. Opens selected item from project's tree widget
 */
void Project::openItem()
{
    if (prjTreeWidget->currentItem() != NULL)
    {
        parentManager->mainWindow->openFilePath(prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
    }
}



/**
 * @brief Sets selected name and path by programmer as main file
 * @param path The path to the selected file
 * @param name The name of the selected file
 */
void Project::setMainFile(QString path, QString name)
{
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
}


/**
 * @brief Removes selected file by programmer from project
 * @param path The path to the file
 * @param name The name of the file
 */
void Project::removeFile(QString path, QString name)
{
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
}



/**
 * @brief Inits simulation control unit
 */
void Project::setupSim()
{
    McuSimCfgMgr::getInstance()->openConfigFile("../simulators/MCUSim/McuSimCfgMgr/mcuspecfile.xml");
    //"ATmega8A"
    this->m_simControlUnit = new MCUSimControl(architecture.toUtf8().constData());
}


/**
 * @brief Starts simulation
 */
void Project::start()
{
    if (langType == LANG_ASM)
    {
        QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        qDebug() << "ASM:" << hexPath;
        std::string stdPath = hexPath.toUtf8().constData();
        m_simControlUnit->start(stdPath, m_simControlUnit->COMPILER_AVRA, m_simControlUnit->DBGFILEID_HEX);
    }
    else if (langType == LANG_C)
    {
        QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        qDebug() << "C:" << hexPath;
        std::string stdPath = hexPath.toUtf8().constData();
        m_simControlUnit->start(stdPath, m_simControlUnit->COMPILER_GCC, m_simControlUnit->DBGFILEID_HEX);
    }
}



/**
 * @brief Stops simulation
 */
void Project::stop()
{
    m_simControlUnit->stop();
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
    std::string *fileName;
    int line = m_simControlUnit->getLineNumber(fileName);
    parentWindow->getWDockManager()->setCentralByName(QString::fromStdString(*fileName));
    parentWindow->getWDockManager()->getCentralTextEdit()->highlightLine(line);
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
    if (this->m_simControlUnit == NULL)
    {
        qDebug() << "losdaskdfdsjfdsfsdfs";
    }
    return this->m_simControlUnit;
}