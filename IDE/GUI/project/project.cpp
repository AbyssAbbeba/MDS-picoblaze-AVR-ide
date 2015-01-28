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
#include <QtXml>
#include "project.h"
//#include "McuSimCfgMgr.h"
#include "MCUSimControl.h"
#include "MCUSim.h"
#include "../widgets/ProjectTree/projecttree.h"
#include "../guicfg/guicfg.h"
#include "../../utilities/os/os.h"
#include "../../simulators/MCUSim/PicoBlaze/PicoBlazeInstructionSet.h"
#include "../errordialog/errordlg.h"
#include "../compatibilitymode/compatibilitymode.h"




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
        //newProject->prjDockWidget->raise();
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
    //qDebug() << "ProjectMan: setActiveByIndex name" << activeProject->prjName;
    //qDebug() << "ProjectMan: openProject count" << openProjects.count();
    if (index >= openProjects.count())
    {
        activeProject = openProjects.at(openProjects.count()-1);
    }
    else
    {
        activeProject = openProjects.at(index);
    }
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
        relativePath = QDir::cleanPath(makefileDir.relativeFilePath(projectDir.path() + "/" + activeProject->mainFilePath));
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


QList<Project*> ProjectMan::getOpenProjects()
{
    return this->openProjects;
}





//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------
//-----------------------------------------------------------


/**
 * @brief Project destructor. Saves project if not untracked.
 */
Project::~Project()
{
    if (prjName != "untracked" && prjPath != "untracked")
    {
        this->saveProject();
    }
}

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
    useMainFile = false;
    m_untitledCounter = 0;
    for (int i = 0; i < 13; i++)
    {
        compileOpt.append(false);
    }

    for (int i = 0; i < 3; i++)
    {
        compileDepths.append(-1);
    }

    fileCount = 0;
    parentManager = parent;
    this->m_simControlUnit = NULL;
    prjPath = QFileInfo(*file).filePath();
    this->simColors.append(new QColor(GuiCfg::getInstance().getCurrLineColor()));
    this->simColors.append(new QColor(GuiCfg::getInstance().getPrevLineColor()));
    this->simColors.append(new QColor(GuiCfg::getInstance().getPrevLine2Color()));
    m_asmType = 0;
    this->clock = 10.0;
    this->clockMult = 1000000;
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
                            else if (xmlGeneralElement.tagName() == "AsmType")
                            {
                                m_asmType = xmlGeneralElement.attribute("value", "").toInt(NULL);
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
                                if ("true" == xmlFilesElement.attribute("opened",""))
                                {
                                    m_fileStats.append(true);
                                }
                                else
                                {
                                    m_fileStats.append(false);
                                }
                                fileCount++;
                            }
                            xmlFilesNode = xmlFilesNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "Watchers")
                    {
                        QDomNode xmlWatchersNode = xmlElement.firstChild();
                        QDomElement xmlWatchersElement;
                        while (!xmlWatchersNode.isNull())
                        {
                            xmlWatchersElement = xmlWatchersNode.toElement();
                            if (xmlWatchersElement.tagName() == "Watcher")
                            {
                                RegWatcherExportStruct watcherStruct;
                                watcherStruct.name = xmlWatchersElement.attribute("name", "");
                                watcherStruct.address = xmlWatchersElement.attribute("address", "").toInt();
                                watcherStruct.type = xmlWatchersElement.attribute("type", "").toInt();
                                watcherStruct.regbank = xmlWatchersElement.attribute("regbank", "").toInt();
                                m_regWatchers.append(watcherStruct);
                            }
                            xmlWatchersNode = xmlWatchersNode.nextSibling();
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
                    else if (xmlElement.tagName() == "Breakpoints")
                    {
                        QDir project(QFileInfo(prjPath).dir());
                        QDomNode xmlBreakpointFileNode = xmlElement.firstChild();
                        QDomElement xmlBreakpointFileElement;
                        QString absolutePath;
                        while (!xmlBreakpointFileNode.isNull())
                        {
                            xmlBreakpointFileElement = xmlBreakpointFileNode.toElement();
                            QSet<unsigned int> set;
                            QDomNode xmlBreakpointNode = xmlBreakpointFileNode.firstChild();
                            QDomElement xmlBreakpointElement;
                            while (!xmlBreakpointNode.isNull())
                            {
                                xmlBreakpointElement = xmlBreakpointNode.toElement();
                                set << xmlBreakpointElement.attribute("line").toInt();
                                xmlBreakpointNode = xmlBreakpointNode.nextSibling();
                            }
                            absolutePath = QDir::cleanPath(project.absoluteFilePath(xmlBreakpointFileElement.attribute("path")));
                            //qDebug() << "Project: breakpoint path" << absolutePath;
                            QPair<QString, QSet<unsigned int>> pair(absolutePath, set);
                            this->breakPoints.append(pair);
                            xmlBreakpointFileNode = xmlBreakpointFileNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "Bookmarks")
                    {
                        QDir project(QFileInfo(prjPath).dir());
                        QDomNode xmlBookmarkFileNode = xmlElement.firstChild();
                        QDomElement xmlBookmarkFileElement;
                        QString absolutePath;
                        while (!xmlBookmarkFileNode.isNull())
                        {
                            xmlBookmarkFileElement = xmlBookmarkFileNode.toElement();
                            QSet<unsigned int> set;
                            QDomNode xmlBookmarkNode = xmlBookmarkFileNode.firstChild();
                            QDomElement xmlBookmarkElement;
                            while (!xmlBookmarkNode.isNull())
                            {
                                xmlBookmarkElement = xmlBookmarkNode.toElement();
                                set << xmlBookmarkElement.attribute("line").toInt();
                                xmlBookmarkNode = xmlBookmarkNode.nextSibling();
                            }
                            //TODO:
                            absolutePath = QDir::cleanPath(project.absoluteFilePath(xmlBookmarkFileElement.attribute("path")));
                            QPair<QString, QSet<unsigned int>> pair(absolutePath, set);
                            this->bookmarks.append(pair);
                            xmlBookmarkFileNode = xmlBookmarkFileNode.nextSibling();
                        }
                    }
                    else if (xmlElement.tagName() == "Simulator")
                    {
                        QDomNode xmlSimulatorNode = xmlElement.firstChild();
                        QDomElement xmlSimulatorElement;
                        while (!xmlSimulatorNode.isNull())
                        {
                            //qDebug() << "node";
                            xmlSimulatorElement = xmlSimulatorNode.toElement();
                            if (xmlSimulatorElement.tagName() == "Clock")
                            {
                                    clock = xmlSimulatorElement.attribute("clock", "").toDouble();
                                    clockMult = xmlSimulatorElement.attribute("clockMult", "").toInt();
                                    if (0.0 >= clock)
                                    {
                                        this->clock = 10.0;
                                    }
                                    if (0 >= clockMult)
                                    {
                                        this->clockMult = 1000000;
                                    }
                            }
                            xmlSimulatorNode = xmlSimulatorNode.nextSibling();
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
                            if (xmlCompilerElement.tagName() == "Depths")
                            {
                                compileDepths[0] = (xmlCompilerElement.attribute("macro", "").toInt());
                                compileDepths[1] = (xmlCompilerElement.attribute("file", "").toInt());
                                compileDepths[2] = (xmlCompilerElement.attribute("repeat", "").toInt());
                            }
                            else if (xmlCompilerElement.tagName() == "Options")
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
                                    else if (xmlCompileOptElem.tagName() == "StringFile")
                                    {
                                        if (xmlCompileOptElem.attribute("enable") == "true")
                                        {
                                            compileOpt[12] = true;
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

            //this->treeProjUntitled = new QTreeWidgetItem(treeProjName);
            //treeProjUntitled->setText(0, "Untitled");

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
                /*else if ("untitled" == filePaths.at(i))
                {
                    treeProjFile = new QTreeWidgetItem(treeProjUntitled);
                }*/
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

            connect(prjDockWidget,
                    SIGNAL(visibilityChanged(bool)),
                    this,
                    SLOT(setActive())
                   );
            connect(prjTreeWidget,
                    SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),
                    this,
                    SLOT(openItem())
                   );
            connect(prjTreeWidget,
                    SIGNAL(requestFileCount()),
                    this,
                    SLOT(emitFileCount())
                   );
            connect(prjTreeWidget,
                    SIGNAL(startProjectCfgDlgCore()),
                    this,
                    SLOT(startCfgDlgCore())
                   );
            connect(prjTreeWidget,
                    SIGNAL(setMainFile(QString, QString)),
                    this,
                    SLOT(setMainFile(QString, QString))
                   );
            connect(prjTreeWidget,
                    SIGNAL(removeFile(QString, QString)),
                    this,
                    SLOT(removeFile(QString, QString))
                   );
            connect(prjTreeWidget,
                    SIGNAL(addFile(QString, QString)),
                    this,
                    SLOT(addFile(QString, QString))
                   );
            connect(prjTreeWidget,
                    SIGNAL(closeProject()),
                    this,
                    SLOT(closeProjectSlot())
                   );
            connect(prjTreeWidget,
                    SIGNAL(requestProjectPath()),
                    this,
                    SLOT(requestProjectPath())
                   );
            connect(this,
                    SIGNAL(fileCountSignal(int)),
                    prjTreeWidget,
                    SLOT(contextP2(int))
                   );
            setupSim();
            prjTreeWidget->expandAll();
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
    m_untitledCounter = 0;

    this->simColors.append(new QColor(GuiCfg::getInstance().getCurrLineColor()));
    this->simColors.append(new QColor(GuiCfg::getInstance().getPrevLineColor()));
    this->simColors.append(new QColor(GuiCfg::getInstance().getPrevLine2Color()));

    this->langType = LANG_ASM;
    this->family = GuiCfg::getInstance().getProjectFamily();
    this->intVector = GuiCfg::getInstance().getProjectIntVector();
    this->hwBuild = GuiCfg::getInstance().getProjectHWBuild();
    this->scratchpadSize = GuiCfg::getInstance().getProjectScratchpadSize();
    this->progMemSize = GuiCfg::getInstance().getProjectProgMemSize();
    this->useMainFile = false;
    this->mainFileName = "";
    this->mainFilePath = "";
    this->compileOpt = GuiCfg::getInstance().getProjectCompOpt();
    this->compileDepths = GuiCfg::getInstance().getProjectCompDepth();
    this->defaultVHDL = GuiCfg::getInstance().getProjectDefVHDL();
    this->defaultVerilog = GuiCfg::getInstance().getProjectDefVerilog();
    this->templateVHDL =  GuiCfg::getInstance().getProjectPathVHDL();
    this->templateVerilog = GuiCfg::getInstance().getProjectPathVerilog();
    this->clock = 10.0;
    this->clockMult = 1000000;
    m_asmType = 0;

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

    //this->treeProjUntitled = new QTreeWidgetItem(treeProjName);
    //treeProjUntitled->setText(0, "Untitled");

    
    connect(prjDockWidget,
            SIGNAL(visibilityChanged(bool)),
            this,
            SLOT(setActive())
           );
    connect(prjTreeWidget,
            SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),
            this,
            SLOT(openUntrackedItem())
           );
    connect(prjTreeWidget,
            SIGNAL(requestFileCount()),
            this,
            SLOT(emitFileCount())
           );
    connect(prjTreeWidget,
            SIGNAL(startProjectCfgDlgCore()),
            this,
            SLOT(startCfgDlgCore())
           );
    //connect(prjTreeWidget, SIGNAL(setMainFile(QString, QString)), this, SLOT(setMainFile(QString, QString)));
    connect(prjTreeWidget,
            SIGNAL(removeFile(QString, QString)),
            this,
            SLOT(removeFile(QString, QString))
           );
    connect(prjTreeWidget,
            SIGNAL(addFile(QString, QString)),
            this,
            SLOT(addFile(QString, QString))
           );
    connect(prjTreeWidget,
            SIGNAL(closeProject()),
            this,
            SLOT(closeProjectSlot())
           );
    connect(prjTreeWidget,
            SIGNAL(requestProjectPath()),
            this,
            SLOT(requestProjectPath())
           );
    connect(this,
            SIGNAL(fileCountSignal(int)),
            prjTreeWidget,
            SLOT(contextP2(int))
           );
    setupSim();
    prjTreeWidget->expandAll();
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
    this->simColors.append(new QColor(GuiCfg::getInstance().getCurrLineColor()));
    this->simColors.append(new QColor(GuiCfg::getInstance().getPrevLineColor()));
    this->simColors.append(new QColor(GuiCfg::getInstance().getPrevLine2Color()));
    this->useMainFile = false;
    this->mainFileName = "";
    this->mainFilePath = "";
    this->clock = 10.0;
    this->clockMult = 1000000;
    m_asmType = 0;
    m_untitledCounter = 0;
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

    //this->treeProjUntitled = new QTreeWidgetItem(treeProjName);
    //this->treeProjUntitled->setText(0, "Untitled");

//     
    
    
    for (int i = 0; i < 3; i++)
    {
        compileDepths.append(-1);
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
    xmlIntVector.setAttribute("value", "0");
    xmlGeneral.appendChild(xmlIntVector);
    QDomElement xmlHWBuild = domDoc.createElement("HWBuild");
    xmlHWBuild.setAttribute("value", "0");
    xmlGeneral.appendChild(xmlHWBuild);
    QDomElement xmlAsmType = domDoc.createElement("AsmType");
    xmlAsmType.setAttribute("value", "0");
    xmlGeneral.appendChild(xmlAsmType);
    xmlRoot.appendChild(xmlGeneral);

    QDomElement xmlFiles = domDoc.createElement("Files");
    xmlRoot.appendChild(xmlFiles);

    QDomElement xmlMainFile = domDoc.createElement("Mainfile");
    xmlMainFile.setAttribute("name", "");
    xmlMainFile.setAttribute("path", "");
    xmlMainFile.setAttribute("enabled", "false");
    xmlRoot.appendChild(xmlMainFile);

    QDomElement xmlBreakpoints = domDoc.createElement("Breakpoints");
    xmlRoot.appendChild(xmlBreakpoints);

    QDomElement xmlBookmarks = domDoc.createElement("Bookmarks");
    xmlRoot.appendChild(xmlBookmarks);

    QDomElement xmlSimulator = domDoc.createElement("Simulator");
    QDomElement xmlClock = domDoc.createElement("Clock");
    xmlClock.setAttribute("clock", this->clock);
    xmlClock.setAttribute("clockMult", this->clockMult);
    xmlSimulator.appendChild(xmlClock);
    xmlRoot.appendChild(xmlSimulator);

    QDomElement xmlCompiler = domDoc.createElement("Compiler");

    QDomElement xmlCompilerOpt = domDoc.createElement("Options");
    QDomElement xmlSymbolTbl = domDoc.createElement("SymbolTable");
    xmlSymbolTbl.setAttribute("enable", "false");
    compileOpt.append(false);
    xmlCompilerOpt.appendChild(xmlSymbolTbl);
    QDomElement xmlMacroTbl = domDoc.createElement("MacroTable");
    xmlMacroTbl.setAttribute("enable", "false");
    compileOpt.append(false);
    xmlCompilerOpt.appendChild(xmlMacroTbl);
    QDomElement xmlDbgFile = domDoc.createElement("DebugFile");
    xmlDbgFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlDbgFile);
    QDomElement xmlCodeTree = domDoc.createElement("CodeTree");
    xmlCodeTree.setAttribute("enable", "false");
    compileOpt.append(false);
    xmlCompilerOpt.appendChild(xmlCodeTree);
    QDomElement xmlLstFile = domDoc.createElement("ListFile");
    xmlLstFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlLstFile);
    QDomElement xmlHexFile = domDoc.createElement("HexFile");
    xmlHexFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlHexFile);
    QDomElement xmlBinFile = domDoc.createElement("BinFile");
    xmlBinFile.setAttribute("enable", "false");
    compileOpt.append(false);
    xmlCompilerOpt.appendChild(xmlBinFile);
    QDomElement xmlSRecFile = domDoc.createElement("SRecFile");
    xmlSRecFile.setAttribute("enable", "false");
    compileOpt.append(false);
    xmlCompilerOpt.appendChild(xmlSRecFile);
    QDomElement xmlMemFile = domDoc.createElement("MemFile");
    xmlMemFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlMemFile);
    QDomElement xmlRawHexFile = domDoc.createElement("RawHexFile");
    xmlRawHexFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlRawHexFile);
    QDomElement xmlVerilogFile = domDoc.createElement("VerilogFile");
    xmlVerilogFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlVerilogFile);
    QDomElement xmlVHDLFile = domDoc.createElement("VHDLFile");
    xmlVHDLFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlVHDLFile);
    QDomElement xmlStringFile = domDoc.createElement("StringFile");
    xmlStringFile.setAttribute("enable", "true");
    compileOpt.append(true);
    xmlCompilerOpt.appendChild(xmlStringFile);
    xmlCompiler.appendChild(xmlCompilerOpt);

    QDomElement xmlCompilerDepth = domDoc.createElement("Depths");
    xmlCompilerDepth.setAttribute("macro", compileDepths.at(0));
    xmlCompilerDepth.setAttribute("file", compileDepths.at(1));
    xmlCompilerDepth.setAttribute("repeat", compileDepths.at(2));
    xmlCompiler.appendChild(xmlCompilerDepth);

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

    connect(prjDockWidget,
            SIGNAL(visibilityChanged(bool)),
            this,
            SLOT(setActive())
           );
    connect(prjTreeWidget,
            SIGNAL(itemDoubleClicked (QTreeWidgetItem*,int)),
            this,
            SLOT(openItem())
           );
    connect(prjTreeWidget,
            SIGNAL(requestFileCount()),
            this,
            SLOT(emitFileCount())
           );
    connect(prjTreeWidget,
            SIGNAL(startProjectCfgDlgCore()),
            this,
            SLOT(startCfgDlgCore())
           );
    connect(prjTreeWidget,
            SIGNAL(setMainFile(QString, QString)),
            this,
            SLOT(setMainFile(QString, QString))
           );
    connect(prjTreeWidget,
            SIGNAL(removeFile(QString, QString)),
            this,
            SLOT(removeFile(QString, QString))
           );
    connect(prjTreeWidget,
            SIGNAL(addFile(QString, QString)),
            this,
            SLOT(addFile(QString, QString))
           );
    connect(prjTreeWidget,
            SIGNAL(closeProject()),
            this,
            SLOT(closeProjectSlot())
           );
    connect(prjTreeWidget,
            SIGNAL(requestProjectPath()),
            this,
            SLOT(requestProjectPath())
           );
    connect(this,
            SIGNAL(fileCountSignal(int)),
            prjTreeWidget,
            SLOT(contextP2(int))
           );
    setupSim();
    prjTreeWidget->expandAll();
    //qDebug() << "Project: return Project() blank";
}


void Project::saveProject()
{
    qDebug() << "saveProject()";
    if (QDir(GuiCfg::getInstance().getExamplePath() + "/MDSExample.mds-project").absolutePath() == prjPath)
    {
        qDebug() << "example read only";
        return;
    }
    QDir project(QFileInfo(prjPath).dir());

    QFile *file = new QFile(prjPath);
    if(!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        error(ERR_OPENFILE, prjPath);
        qDebug() << "return saveProject() error";
        return;
    }

    QDomDocument domDoc("MDSProject");
    QDomElement xmlRoot = domDoc.createElement("MDSProject");
    domDoc.appendChild(xmlRoot);

    QDomElement xmlGeneral = domDoc.createElement("General");
    QDomElement xmlName = domDoc.createElement("Name");
    xmlName.setAttribute("name", this->prjName);
    xmlGeneral.appendChild(xmlName);
    QDomElement xmlArch = domDoc.createElement("Family");
    xmlArch.setAttribute("family", this->family);
    xmlGeneral.appendChild(xmlArch);
    QDomElement xmlLang = domDoc.createElement("Language");
    xmlLang.setAttribute("language", this->langType);
    xmlGeneral.appendChild(xmlLang);
    QDomElement xmlScratchpad = domDoc.createElement("ScratchpadSize");
    xmlScratchpad.setAttribute("value", this->scratchpadSize);
    xmlGeneral.appendChild(xmlScratchpad);
    QDomElement xmlProgMem = domDoc.createElement("ProgMemSize");
    xmlProgMem.setAttribute("value", this->progMemSize);
    xmlGeneral.appendChild(xmlProgMem);
    QDomElement xmlIntVector = domDoc.createElement("IntVector");
    xmlIntVector.setAttribute("value", this->intVector);
    xmlGeneral.appendChild(xmlIntVector);
    QDomElement xmlHWBuild = domDoc.createElement("HWBuild");
    xmlHWBuild.setAttribute("value", this->hwBuild);
    xmlGeneral.appendChild(xmlHWBuild);
    QDomElement xmlAsmType = domDoc.createElement("AsmType");
    xmlAsmType.setAttribute("value", m_asmType);
    xmlGeneral.appendChild(xmlAsmType);
    xmlRoot.appendChild(xmlGeneral);

    QDomElement xmlRegs = domDoc.createElement("Watchers");
    qDebug() << "saving watchers" << m_regWatchers.count();
    for (int i = 0; i < m_regWatchers.count(); i++)
    {
        QDomElement xmlReg = domDoc.createElement("Watcher");
        xmlReg.setAttribute("name", m_regWatchers.at(i).name);
        xmlReg.setAttribute("address", m_regWatchers.at(i).address);
        xmlReg.setAttribute("type", m_regWatchers.at(i).type);
        xmlReg.setAttribute("regbank", m_regWatchers.at(i).regbank);
        xmlRegs.appendChild(xmlReg);
    }
    xmlRoot.appendChild(xmlRegs);

    QDomElement xmlFiles = domDoc.createElement("Files");
    //qDebug() << "Project: saving" << this->fileCount << "files";
    for (int i = 0; i < this->fileCount; i++)
    {
        if ( "untracked" == this->filePaths.at(i))
        {
            continue;
        }
        QDomElement xmlFile = domDoc.createElement("File");
        xmlFile.setAttribute("path", this->filePaths.at(i));
        xmlFile.setAttribute("name", this->fileNames.at(i));
        if (true == m_fileStats.at(i))
        {
            xmlFile.setAttribute("opened", "true");
        }
        else
        {
            xmlFile.setAttribute("opened", "false");
        }
        xmlFiles.appendChild(xmlFile);
    }
    xmlRoot.appendChild(xmlFiles);

    QDomElement xmlMainFile = domDoc.createElement("Mainfile");
    xmlMainFile.setAttribute("name", this->mainFileName);
    xmlMainFile.setAttribute("path", this->mainFilePath);
    if (true == this->useMainFile)
    {
        xmlMainFile.setAttribute("enabled", "true");
    }
    else
    {
        xmlMainFile.setAttribute("enabled", "false");
    }
    xmlRoot.appendChild(xmlMainFile);

    QString relativePath;
    QDomElement xmlBreakpoints = domDoc.createElement("Breakpoints");
    for (int i = 0; i < this->breakPoints.count(); i++)
    {

        relativePath = QDir::cleanPath(project.relativeFilePath(this->breakPoints.at(i).first));
        //qDebug() << "Project: breakpoint relative path" << relativePath;
        QDomElement xmlBreakpointFile = domDoc.createElement("BreakpointFile");
        xmlBreakpointFile.setAttribute("path", relativePath);
        foreach (unsigned int value, this->breakPoints.at(i).second)
        {
            QDomElement xmlBreakpoint = domDoc.createElement("Breakpoint");
            xmlBreakpoint.setAttribute("line", value);
            xmlBreakpointFile.appendChild(xmlBreakpoint);
        }
        xmlBreakpoints.appendChild(xmlBreakpointFile);
    }
    xmlRoot.appendChild(xmlBreakpoints);

    QDomElement xmlBookmarks = domDoc.createElement("Bookmarks");
    for (int i = 0; i < this->bookmarks.count(); i++)
    {
        relativePath = QDir::cleanPath(project.relativeFilePath(this->bookmarks.at(i).first));
        QDomElement xmlBookmarkFile = domDoc.createElement("BookmarkFile");
        xmlBookmarkFile.setAttribute("path", relativePath);
        foreach (unsigned int value, this->bookmarks.at(i).second)
        {
            QDomElement xmlBookmark = domDoc.createElement("Bookmark");
            xmlBookmark.setAttribute("line", value);
            xmlBookmarkFile.appendChild(xmlBookmark);
        }
        xmlBookmarks.appendChild(xmlBookmarkFile);
    }
    xmlRoot.appendChild(xmlBookmarks);

    QDomElement xmlSimulator = domDoc.createElement("Simulator");
    QDomElement xmlClock = domDoc.createElement("Clock");
    xmlClock.setAttribute("clock", this->clock);
    xmlClock.setAttribute("clockMult", this->clockMult);
    xmlSimulator.appendChild(xmlClock);
    xmlRoot.appendChild(xmlSimulator);

    QDomElement xmlCompiler = domDoc.createElement("Compiler");

    QDomElement xmlCompilerOpt = domDoc.createElement("Options");
    QDomElement xmlSymbolTbl = domDoc.createElement("SymbolTable");
    if (true == this->compileOpt.at(0))
    {
        xmlSymbolTbl.setAttribute("enable", "true");
    }
    else
    {
        xmlSymbolTbl.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlSymbolTbl);
    QDomElement xmlMacroTbl = domDoc.createElement("MacroTable");
    if (true == this->compileOpt.at(1))
    {
        xmlMacroTbl.setAttribute("enable", "true");
    }
    else
    {
        xmlMacroTbl.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlMacroTbl);
    QDomElement xmlDbgFile = domDoc.createElement("DebugFile");
    if (true == this->compileOpt.at(2))
    {
        xmlDbgFile.setAttribute("enable", "true");
    }
    else
    {
        xmlDbgFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlDbgFile);
    QDomElement xmlCodeTree = domDoc.createElement("CodeTree");
    if (true == this->compileOpt.at(3))
    {
        xmlCodeTree.setAttribute("enable", "true");
    }
    else
    {
        xmlCodeTree.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlCodeTree);
    QDomElement xmlLstFile = domDoc.createElement("ListFile");
    if (true == this->compileOpt.at(4))
    {
        xmlLstFile.setAttribute("enable", "true");
    }
    else
    {
        xmlLstFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlLstFile);
    QDomElement xmlHexFile = domDoc.createElement("HexFile");
    if (true == this->compileOpt.at(5))
    {
        xmlHexFile.setAttribute("enable", "true");
    }
    else
    {
        xmlHexFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlHexFile);
    QDomElement xmlBinFile = domDoc.createElement("BinFile");
    if (true == this->compileOpt.at(6))
    {
        xmlBinFile.setAttribute("enable", "true");
    }
    else
    {
        xmlBinFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlBinFile);
    QDomElement xmlSRecFile = domDoc.createElement("SRecFile");
    if (true == this->compileOpt.at(7))
    {
        xmlSRecFile.setAttribute("enable", "true");
    }
    else
    {
        xmlSRecFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlSRecFile);
    QDomElement xmlMemFile = domDoc.createElement("MemFile");
    if (true == this->compileOpt.at(8))
    {
        xmlMemFile.setAttribute("enable", "true");
    }
    else
    {
        xmlMemFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlMemFile);
    QDomElement xmlRawHexFile = domDoc.createElement("RawHexFile");
    if (true == this->compileOpt.at(9))
    {
        xmlRawHexFile.setAttribute("enable", "true");
    }
    else
    {
        xmlRawHexFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlRawHexFile);
    QDomElement xmlVerilogFile = domDoc.createElement("VerilogFile");
    if (true == this->compileOpt.at(10))
    {
        xmlVerilogFile.setAttribute("enable", "true");
    }
    else
    {
        xmlVerilogFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlVerilogFile);
    QDomElement xmlVHDLFile = domDoc.createElement("VHDLFile");
    if (true == this->compileOpt.at(11))
    {
        xmlVHDLFile.setAttribute("enable", "true");
    }
    else
    {
        xmlVHDLFile.setAttribute("enable", "false");
    }
    QDomElement xmlStringFile = domDoc.createElement("StringFile");
    if (true == this->compileOpt.at(12))
    {
        xmlStringFile.setAttribute("enable", "true");
    }
    else
    {
        xmlStringFile.setAttribute("enable", "false");
    }
    xmlCompilerOpt.appendChild(xmlVHDLFile);
    xmlCompiler.appendChild(xmlCompilerOpt);

    QDomElement xmlCompilerDepth = domDoc.createElement("Depths");
    xmlCompilerDepth.setAttribute("macro", compileDepths.at(0));
    xmlCompilerDepth.setAttribute("file", compileDepths.at(1));
    xmlCompilerDepth.setAttribute("repeat", compileDepths.at(2));
    xmlCompiler.appendChild(xmlCompilerDepth);

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
    for (int i = 0; i < this->compileIncPaths.count(); i++)
    {
        QDomElement xmlIncPath = domDoc.createElement("Path");
        xmlIncPath.setAttribute("path", this->compileIncPaths.at(i));
        xmlCompilerInclude.appendChild(xmlIncPath);
    }
    xmlCompiler.appendChild(xmlCompilerInclude);
    xmlRoot.appendChild(xmlCompiler);

    QTextStream xmlStream(file);
    xmlStream << domDoc.toString();
    qDebug() << "return saveProject()";
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
        /*for (int i = 0; i < this->fileCount; i++)
        {
            if (path == prjPath.section('/', 0, -2) + "/" + filePaths.at(i))
            {
                return "";
            }
        }*/
        QDir project(QFileInfo(this->prjPath).dir());
        QString relativePath = QDir::cleanPath(project.relativeFilePath(path));
        if (true == filePaths.contains(relativePath))
        {
            qDebug() << "Project: file already in project";
            return "";
        }
        fileNames.append(name);
        m_fileStats.append(false);
        fileCount++;
        filePaths.append(relativePath);

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
        /*else if ("untitled" == path)
        {
            treeProjFile = new QTreeWidgetItem(treeProjUntitled);
        }*/
        else
        {
            treeProjFile = new QTreeWidgetItem(treeProjOther);
        }
        treeProjFile->setText(0, name);
        treeProjFile->setData(0, Qt::ToolTipRole, path);
    }
    else
    {
        for (int i = 0; i < filePaths.count(); i++)
        {
            if (path == filePaths.at(i))
            {
                return "";
            }
        }
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
            fileNames.append(fileName);
            filePaths.append("untracked");
            m_fileStats.append(false);
        }
        else
        {
            treeProjFile->setText(0, name);
            treeProjFile->setData(0, Qt::ToolTipRole, path);
            fileNames.append(name);
            filePaths.append(path);
            m_fileStats.append(false);
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
    QString relativePath = QDir::cleanPath(project.relativeFilePath(path));

    mainFileName = name;
    mainFilePath = relativePath;
    useMainFile = true;

}

/**
 * @brief Sets usage of main file
 * @param enabled indicator
 */
void Project::setUseMainFile(bool enabled)
{
    useMainFile = enabled;

    //qDebug() << "Project: return setUseMainFile()";
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setIntVector(int value)
{
    intVector = value;
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setHWBuild(int value)
{
    hwBuild = value;
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setScratchpad(int value)
{
    scratchpadSize = value;
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setProgMem(int value)
{
    progMemSize = value;
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setName(QString name)
{
    prjName = name;
}


/**
 * @brief Sets int vector
 * @param value value
 */
void Project::setFamily(QString family)
{
    this->family = family;
    emit changeFamily(family);
}


void Project::setTemplates(bool verilog, QString verilogTemplate, bool VHDL, QString VHDLTemplate)
{
    this->defaultVerilog = verilog;
    this->defaultVHDL = VHDL;
    this->templateVerilog = verilogTemplate;
    this->templateVHDL = VHDLTemplate;
}


void Project::setClock(double clock, int mult)
{
    this->clock = clock;
    this->clockMult = mult;
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
    QString relativePath = QDir::cleanPath(project.relativeFilePath(path));
    int index = filePaths.indexOf(relativePath);
    if (-1 == index)
    {
        qDebug() << "Project: File" << relativePath << "not in project";
        return;
    }
    fileNames.removeAt(index);
    filePaths.removeAt(index);
    m_fileStats.removeAt(index);
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
        //qDebug() << "Project: setupSim() family:" << family;
        this->m_simControlUnit = new MCUSimControl(family.toUtf8().constData());
        //qDebug() << "Project: setupSim() done";
        connect(m_simControlUnit, SIGNAL(updateRequest(int)), this, SLOT(handleUpdateRequest(int)));
        connect(m_simControlUnit, SIGNAL(breakpointReached()), this, SLOT(breakpointReachedSlot()));
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
int Project::start(QString file, QString dumpFiles, DbgFile *dbgFile, DataFile *dataFile)
{
    //qDebug() << "Project: start()";
    //parentWindow->getWDockManager()->setEditorsReadOnly(true);
    if (langType == LANG_ASM)
    {
        //qDebug() << "Project: start() after ";
        //QString hexPath = prjPath.section('/',0, -2) + "/build/" + mainFileName.section('.',0,-2);
        if (NULL != dbgFile && NULL != dataFile)
        {
            PicoBlazeInstructionSet *set = dynamic_cast<PicoBlazeInstructionSet*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU));
            set->m_config.m_interruptVector = (unsigned int)this->intVector;
            set->m_config.m_hwbuild = (uint8_t)this->hwBuild;

            if ( false == m_simControlUnit->startSimulation(dbgFile, dataFile) )
            {
                qDebug() << "Project: m_simControlUnit->startSimulation() returned false";
                std::vector<std::string> messages = m_simControlUnit->getMessages();
                for (unsigned int i = 0; i < messages.size(); i++)
                {
                    qDebug() << QString::fromStdString(messages.at(i));
                }
                //qDebug() << "Project: start return 1";
                return 1;
            }
        }
        else
        {
            QString hexPath;
            QString asmPath;
            if (file != "")
            {
                asmPath = file;
                if (dumpFiles != "")
                {
                    hexPath = dumpFiles.section('.',0,-2);
                    if (hexPath == "")
                    {
                        hexPath = dumpFiles;
                    }
                }
                else
                {
                    hexPath = file.section('.',0,-2);
                    if (hexPath == "")
                    {
                        hexPath = dumpFiles;
                    }
                }
            }
            else
            {
                QDir dir(prjPath.section('/',0, -2));
                hexPath = QDir::cleanPath(dir.absoluteFilePath(mainFilePath.section('.',0,-2)));
                asmPath = QDir::cleanPath(dir.absoluteFilePath(mainFilePath));
                if (hexPath == "")
                {
                    hexPath = asmPath;
                }
            }
            //QString hexPath = prjPath.section('/',0, -2) + "/" + mainFileName.section('.',0,-2);
            QFileInfo infoAsm(asmPath);
            QFileInfo infoHex(hexPath + ".ihex");
            QFileInfo infoDbg(hexPath + ".dbg");
            //qDebug() << "Project: sim file" << asmPath;
            //qDebug() << "Project: sim dump file" << hexPath;
            if ( false == infoHex.exists()
            || false == infoDbg.exists()
            || 0 == infoHex.size()
            || 0 == infoDbg.size()
            )
            {
                //qDebug() << "Project: files do not exist";
                return 3;
            }
            if (infoAsm.lastModified() > infoHex.lastModified() || infoAsm.lastModified() > infoDbg.lastModified())
            {
                qDebug() << "Project: file modified" << infoAsm.lastModified();
                qDebug() << "Project: hex modified" << infoHex.lastModified();
                qDebug() << "Project: dbg modified" << infoDbg.lastModified();
                return 4;
            }
            std::string stdPath = hexPath.toLocal8Bit().constData();

            PicoBlazeInstructionSet *set = dynamic_cast<PicoBlazeInstructionSet*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_CPU));
            set->m_config.m_interruptVector = (unsigned int)this->intVector;
            set->m_config.m_hwbuild = (uint8_t)this->hwBuild;

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
                //qDebug() << "Project: start return 1";
                return 1;
            }
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
        std::string stdPath = hexPath.toLocal8Bit().constData();
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
    if (file != "")
    {
        this->simulatedFile = file;
        //qDebug() << "Project: start simulatedFile =" << simulatedFile;
    }
    else
    {
        QDir dir(prjPath.section('/',0, -2));
        this->simulatedFile = QDir::cleanPath(dir.absoluteFilePath(mainFilePath));
    }
    //qDebug() << "Project: getLineNumber";
    //std::vector<std::pair<std::string, std::set<unsigned int>>> breakpointsVector;
    //m_simControlUnit->setBreakPoints(breakpointsVector);
    m_simControlUnit->getLineNumber(m_currSim);
    //qDebug() << "Project: getLineNumber check";
    if (m_currSim.empty() == true)
    {
        //qDebug() << "Project: start return 2";
        //return 2;
    }
    m_prevSim.clear();
    m_prevSim2.clear();
    //qDebug() << "Project: getLineNumber done";
    /*for (int i = 0; i < m_currSim.size(); i++)
    {
        qDebug() << QString::fromStdString(*(m_currSim.at(i).first));
    }*/
    emit setEditorReadOnly(true);
    emit simHighlightLines(m_currSim, m_prevSim, m_prevSim2, this->simColors);
    m_prevSim = m_currSim;
    //qDebug() << "Project: return start()";
    return 0;
}


void Project::setBreakpoints(bool set, bool compatibilityMode, CompatibilityMode *mode)
{
    using namespace boost::filesystem;
    if (true == set)
    {
        //qDebug() << "Project: setBreakpoints(true)";
        if (this->breakPoints.count() > 0)
        {
            if (false == m_simControlUnit->breakPointsEnabled())
            {
                m_simControlUnit->enableBreakPoints(true);
            }
            std::vector<std::pair<std::string, std::set<unsigned int>>> breakpointsVector;
            QString relativePath;
            QString absolutePath;
            QDir prjDir(simulatedFile.section('/', 0, -2));
            if (false == compatibilityMode)
            {
                for (int i = 0; i < this->breakPoints.count(); i++)
                {
                    //qDebug() << "Project: breakpoint list at" << i;
                    std::set<unsigned int> breakpointsSet;
                    foreach (const unsigned int &value, this->breakPoints.at(i).second)
                    {
                        breakpointsSet.insert(value);
                    }
                    //qDebug() << "Path" << this->breakPoints.at(i).first;
                    const std::string file = breakPoints.at(i).first.toLocal8Bit().constData();
                    relativePath = QDir::cleanPath(prjDir.relativeFilePath(QString::fromStdString(file)));
                    //qDebug() << "Project: absoluteFilePath" << relativePath;
                    breakpointsVector.push_back ( std::make_pair ( relativePath.toStdString(), breakpointsSet ) );
                }
            }
            else
            {
                for (int i = 0; i < this->breakPoints.count(); i++)
                {
                    //qDebug() << "Project: breakpoint list at" << i;
                    const std::string file = breakPoints.at(i).first.toLocal8Bit().constData();
                    if (true == QString::fromStdString(file).section('.',0,-2).isEmpty())
                    {
                        relativePath = QDir::cleanPath(prjDir.relativeFilePath(QString::fromStdString(file) + ".asm"));
                    }
                    else
                    {
                        relativePath = QDir::cleanPath(prjDir.relativeFilePath(QString::fromStdString(file).section('.',0,-2) + ".asm"));
                    }
                    std::set<unsigned int> breakpointsSet;
                    foreach (const unsigned int &value, this->breakPoints.at(i).second)
                    {
                        breakpointsSet.insert(mode->getNewLine(QString::fromStdString(file), value));
                    }
                    //qDebug() << "Path" << this->breakPoints.at(i).first;
                    //qDebug() << "Project: absoluteFilePath" << relativePath;
                    breakpointsVector.push_back ( std::make_pair ( relativePath.toStdString(), breakpointsSet ) );
                }
            }
            /*for (unsigned int i = 0; i < breakpointsVector.size(); i++)
            {
                qDebug() << "Project: breakpoint file" << QString::fromStdString(breakpointsVector.at(i).first);
                foreach (const unsigned int &value, breakpointsVector.at(i).second)
                {
                    qDebug() << "Project: breakpoint line" << value;
                }
            }*/
            m_simControlUnit->setBreakPoints(breakpointsVector);
        }
    }
    else
    {
        //qDebug() << "Project: setBreakpoints(false)";
        m_simControlUnit->enableBreakPoints(false);
    }
}




/**
 * @brief Stops simulation
 */
void Project::stop()
{

    m_simControlUnit->stopSimulation();
    this->simulatedFile = "";
    emit setEditorReadOnly(false);
    //qDebug() << "Project: return stop()";
}


/**
 * @brief Resets simulation
 */
void Project::reset()
{
    m_simControlUnit->resetProgram();
    m_prevSim.clear();
    m_prevSim2.clear();
    m_simControlUnit->getLineNumber(m_currSim);
    if (m_currSim.empty() == true)
    {
        return;
    }
    emit simHighlightLines(m_currSim, m_prevSim, m_prevSim2, this->simColors);
    m_prevSim = m_currSim;
}


void Project::handleUpdateRequest(int mask)
{
    //run
    if ("" == this->simulatedFile)
    {
        return;
    }
    if (4 & mask)
    {
        //std::string fileName; //= new std::string();
        m_simControlUnit->getLineNumber(m_currSim);

        m_prevSim.clear();
        m_prevSim2.clear();
        emit simHighlightLines(m_currSim, m_prevSim, m_prevSim2, this->simColors);

        m_prevSim = m_currSim;
    }
    else if (2 & mask)
    {
        m_simControlUnit->getLineNumber(m_currSim);


        emit simHighlightLines(m_currSim, m_prevSim, m_prevSim2, this->simColors);


        m_prevSim2 = m_prevSim;
        m_prevSim = m_currSim;
    }
}


/**
 * @brief Makes step in simulation
 */
void Project::step()
{
    m_simControlUnit->stepProgram();
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
}


void Project::setCompileDepths(QList<int> depths)
{
    for (int i = 0; i < depths.size(); i++)
    {
        this->compileDepths[i] = depths.at(i);
    }
}


void Project::setCompileIncPaths(QList<QString> paths)
{
    this->compileIncPaths.clear();
    for (int i = 0; i < paths.size(); i++)
    {
        this->compileIncPaths.append(paths.at(i));
    }
}


void Project::closeProjectSlot()
{
    emit closeProject();
}


//-1 nothing, 0 add, 1 remove
int Project::handleBreakpoint(QString file, int line)
{
    QString fileRelative;
    if (this->prjPath == "untracked")
    {
        fileRelative = file;
    }
    else
    {
        QDir prjDir(this->prjPath.section('/', 0, -2));
        fileRelative = QDir::cleanPath(prjDir.relativeFilePath(file));
    }
    //for (int i = 0; i < this->filePaths.count(); i++)
    //{
    if (true == this->filePaths.contains(fileRelative))
    {
        //qDebug() << "Project: contains" << fileRelative;
        //if (this->filePaths.at(i) == fileRelative)
        //{
            //qDebug() << "Project:" << this->prjName << "contains file" << fileRelative;
        int found = -1;
        for (int j = 0; j < this->breakPoints.count() && found == -1; j++)
        {
            //qDebug() << "Project: matching file" << file << "with" << this->breakPoints.at(j).first;
            if (this->breakPoints.at(j).first == file)
            {
                found = j;
            }
        }
        if (found > -1)
        {
            if (false == this->breakPoints.at(found).second.contains(line))
            {
                //qDebug() << "Project: breakpoint file found, added";
                QSet<unsigned int> set = this->breakPoints.at(found).second;
                set.insert(line);
                this->breakPoints.removeAt(found);
                QPair<QString, QSet<unsigned int>> pair(file, set);
                this->breakPoints.append(pair);
                //emit breakpointAppend(file, line);
                /*if (this->prjPath != "untracked")
                {
                    this->xmlBreakpointAdd(file, line);
                }*/
                if (simulatedFile != "" && true == m_simControlUnit->breakPointsEnabled())
                {
                    this->setBreakpoints(true);
                }
                return 0;
            }
            else
            {
                QSet<unsigned int> set = this->breakPoints.at(found).second;
                set.remove(line);
                this->breakPoints.removeAt(found);
                QPair<QString, QSet<unsigned int>> pair(file, set);
                this->breakPoints.append(pair);
                /*if (this->prjPath != "untracked")
                {
                    this->xmlBreakpointRemove(file, line);
                }*/
                if (simulatedFile != "" && true == m_simControlUnit->breakPointsEnabled())
                {
                    this->setBreakpoints(true);
                }
                return 1;
                //emit breakpointRemove(file, line);
            }
            //this->breakPoints.at(found).second;
        }
        else
        {
            //qDebug() << "Project: breakpoint file not found, added";
            QSet<unsigned int> set;
            set.insert(line);
            QPair<QString, QSet<unsigned int>> pair(file, set);
            this->breakPoints.append(pair);
            /*if (this->prjPath != "untracked")
            {
                this->xmlBreakpointAdd(file, line);
            }*/
            if (simulatedFile != "" && true == m_simControlUnit->breakPointsEnabled())
            {
                this->setBreakpoints(true);
            }
            return 0;
            //emit breakpointAppend(file, line);
        }
    }
    //qDebug() << "Project: does not contain" << fileRelative;
    return -1;
}


void Project::moveBreakpointsAdd(QString file, int line, unsigned int linesAdded)
{
    QString fileRelative;
    if (this->prjPath == "untracked")
    {
        fileRelative = file;
    }
    else
    {
        QDir prjDir(this->prjPath.section('/', 0, -2));
        fileRelative = QDir::cleanPath(prjDir.relativeFilePath(file));
    }
    if (true == this->filePaths.contains(fileRelative))
    {
        for (int i = 0; i < this->breakPoints.count(); i++)
        {
            if (this->breakPoints.at(i).first == file)
            {
                //handle move
                bool changed = false;
                QSet<unsigned int> set;
                QList<unsigned int> setValues = this->breakPoints.at(i).second.toList();
                for (int j = 0; j < setValues.count(); j++)
                {
                    if (setValues.at(j) > (unsigned int)line)
                    {
                        set << setValues.at(j) + linesAdded;
                        changed = true;
                    }
                    else
                    {
                        set << setValues.at(j);
                    }
                }
                if (true == changed)
                {
                    this->breakPoints.removeAt(i);
                    QPair<QString, QSet<unsigned int>> pair(file, set);
                    this->breakPoints.append(pair);
                }
                break;
            }
        }
    }
}


void Project::moveBreakpointsRemove(QString file, int line, unsigned int linesRemoved)
{
    QString fileRelative;
    if (this->prjPath == "untracked")
    {
        fileRelative = file;
    }
    else
    {
        QDir prjDir(this->prjPath.section('/', 0, -2));
        fileRelative = QDir::cleanPath(prjDir.relativeFilePath(file));
    }
    if (true == this->filePaths.contains(fileRelative))
    {
        for (int i = 0; i < this->breakPoints.count(); i++)
        {
            if (this->breakPoints.at(i).first == file)
            {
                //handle move
                bool changed = false;
                QSet<unsigned int> set;
                QList<unsigned int> setValues = this->breakPoints.at(i).second.toList();
                for (int j = 0; j < setValues.count(); j++)
                {
                    if (setValues.at(j) >= (unsigned int)line + linesRemoved)
                    {
                        set << setValues.at(j) - linesRemoved;
                        changed = true;
                    }
                    else
                    {
                        if (setValues.at(j) < (unsigned int)line)
                        {
                            set << setValues.at(j);
                        }
                        else
                        {
                        //else (==) do nothing, breakpoint removed
                            changed = true;
                        }
                    }
                }
                if (true == changed)
                {
                    this->breakPoints.removeAt(i);
                    if (false == set.isEmpty())
                    {
                        QPair<QString, QSet<unsigned int>> pair(file, set);
                        this->breakPoints.append(pair);
                        /*if (this->prjPath != "untracked")
                        {
                            this->xmlBreakpointFileReplace(file);
                        }*/
                    }
                }
                break;
            }
        }
    }
}


//-1 nothing, 0 add, 1 remove
int Project::handleBookmark(QString file, int line)
{
    QString fileRelative;
    if (this->prjPath == "untracked")
    {
        fileRelative = file;
    }
    else
    {
        QDir prjDir(this->prjPath.section('/', 0, -2));
        fileRelative = QDir::cleanPath(prjDir.relativeFilePath(file));
    }
    //for (int i = 0; i < this->filePaths.count(); i++)
    //{
    if (true == this->filePaths.contains(fileRelative))
    {
        //qDebug() << "Project: contains" << fileRelative;
        //if (this->filePaths.at(i) == fileRelative)
        //{
            //qDebug() << "Project:" << this->prjName << "contains file" << fileRelative;
        int found = -1;
        for (int j = 0; j < this->bookmarks.count() && found == -1; j++)
        {
            //qDebug() << "Project: matching file" << file << "with" << this->bookmarks.at(j).first;
            if (this->bookmarks.at(j).first == file)
            {
                found = j;
            }
        }
        if (found > -1)
        {
            if (false == this->bookmarks.at(found).second.contains(line))
            {
                //qDebug() << "Project: breakpoint file found, added";
                QSet<unsigned int> set = this->bookmarks.at(found).second;
                set.insert(line);
                this->bookmarks.removeAt(found);
                QPair<QString, QSet<unsigned int>> pair(file, set);
                this->bookmarks.append(pair);
                //emit breakpointAppend(file, line);
                return 0;
            }
            else
            {
                QSet<unsigned int> set = this->bookmarks.at(found).second;
                set.remove(line);
                this->bookmarks.removeAt(found);
                QPair<QString, QSet<unsigned int>> pair(file, set);
                this->bookmarks.append(pair);
                return 1;
                //emit breakpointRemove(file, line);
            }
            //this->bookmarks.at(found).second;
        }
        else
        {
            //qDebug() << "Project: breakpoint file not found, added";
            QSet<unsigned int> set;
            set.insert(line);
            QPair<QString, QSet<unsigned int>> pair(file, set);
            this->bookmarks.append(pair);
            return 0;
            //emit breakpointAppend(file, line);
        }
    }
    //qDebug() << "Project: does not contain" << fileRelative;
    return -1;
}


void Project::moveBookmarksAdd(QString file, int line, unsigned int linesAdded)
{
    QString fileRelative;
    if (this->prjPath == "untracked")
    {
        fileRelative = file;
    }
    else
    {
        QDir prjDir(this->prjPath.section('/', 0, -2));
        fileRelative = QDir::cleanPath(prjDir.relativeFilePath(file));
    }
    if (true == this->filePaths.contains(fileRelative))
    {
        for (int i = 0; i < this->bookmarks.count(); i++)
        {
            if (this->bookmarks.at(i).first == file)
            {
                //handle move
                bool changed = false;
                QSet<unsigned int> set;
                QList<unsigned int> setValues = this->bookmarks.at(i).second.toList();
                for (int j = 0; j < setValues.count(); j++)
                {
                    if (setValues.at(j) > (unsigned int)line)
                    {
                        set << setValues.at(j) + linesAdded;
                        changed = true;
                    }
                    else
                    {
                        set << setValues.at(j);
                    }
                }
                if (true == changed)
                {
                    this->bookmarks.removeAt(i);
                    QPair<QString, QSet<unsigned int>> pair(file, set);
                    this->bookmarks.append(pair);
                    /*if (this->prjPath != "untracked")
                    {
                        this->xmlBreakpointFileReplace(file);
                    }*/
                }
                break;
            }
        }
    }
}


void Project::moveBookmarksRemove(QString file, int line, unsigned int linesRemoved)
{
    QString fileRelative;
    if (this->prjPath == "untracked")
    {
        fileRelative = file;
    }
    else
    {
        QDir prjDir(this->prjPath.section('/', 0, -2));
        fileRelative = QDir::cleanPath(prjDir.relativeFilePath(file));
    }
    if (true == this->filePaths.contains(fileRelative))
    {
        for (int i = 0; i < this->bookmarks.count(); i++)
        {
            if (this->bookmarks.at(i).first == file)
            {
                //handle move
                bool changed = false;
                QSet<unsigned int> set;
                QList<unsigned int> setValues = this->bookmarks.at(i).second.toList();
                for (int j = 0; j < setValues.count(); j++)
                {
                    if (setValues.at(j) >= (unsigned int)line + linesRemoved)
                    {
                        set << setValues.at(j) - linesRemoved;
                        changed = true;
                    }
                    else
                    {
                        if (setValues.at(j) < (unsigned int)line)
                        {
                            set << setValues.at(j);
                        }
                        else
                        {
                        //else (==) do nothing, breakpoint removed
                            changed = true;
                        }
                    }
                }
                if (true == changed)
                {
                    this->bookmarks.removeAt(i);
                    if (false == set.isEmpty())
                    {
                        QPair<QString, QSet<unsigned int>> pair(file, set);
                        this->bookmarks.append(pair);
                        /*if (this->prjPath != "untracked")
                        {
                            this->xmlBreakpointFileReplace(file);
                        }*/
                    }
                }
                break;
            }
        }
    }
}



void Project::breakpointReachedSlot()
{
    //qDebug() << "Project: breakpoint reached";
    emit breakpointReached();
}


QList<QPair<QString, QSet<unsigned int>>>* Project::getBreakpointsListRef()
{
    return &breakPoints;
}


QList<QPair<QString, QSet<unsigned int>>>* Project::getBookmarksListRef()
{
    return &bookmarks;
}



QList<unsigned int> Project::getBreakpointsForFileAbsolute(QString file)
{
    for (int i = 0; i < this->breakPoints.count(); i++)
    {
        if (this->breakPoints.at(i).first == file)
        {
            return this->breakPoints.at(i).second.toList();
        }
    }
    return QList<unsigned int>();
}


QList<unsigned int> Project::getBookmarksForFileAbsolute(QString file)
{
    for (int i = 0; i < this->bookmarks.count(); i++)
    {
        if (this->bookmarks.at(i).first == file)
        {
            return this->bookmarks.at(i).second.toList();
        }
    }
    return QList<unsigned int>();
}


void Project::reloadProjectTree()
{
    //qDebug() << "Project: reloadProjectTree()";
    prjTreeWidget->clear();
    //qDebug() << this->treeProjName->text(0);
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

    //this->treeProjUntitled = new QTreeWidgetItem(treeProjName);
    //treeProjUntitled->setText(0, "Untitled");


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
        /*else if ("untitled" == filePaths.at(i))
        {
            treeProjFile = new QTreeWidgetItem(treeProjUntitled);
        }*/
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
    prjTreeWidget->expandAll();
}


void Project::requestProjectPath()
{
    prjTreeWidget->requestProjectPathAnswer(QDir(this->prjPath.section('/',0, -2)).absolutePath());
}


//in case of untracked project, oldPath = oldName, because untracked files have path "untracked"
void Project::renameFile(QString oldPath, QString newPath)
{
    QDir project(QFileInfo(prjPath).dir());
    QString relativePath;
    int index = 0;
    if ("untracked" == prjPath)
    {
        index = fileNames.indexOf(oldPath);
        if (-1 == index)
        {
            //relativePath = QDir::cleanPath(project.relativeFilePath(newPath));
            this->addFile(newPath, newPath.section('/', -1));
            return;
        }
    }
    else
    {
        relativePath = QDir::cleanPath(project.relativeFilePath(oldPath));
        index = filePaths.indexOf(relativePath);
        if (-1 == index)
        {
            //qDebug() << "Project: File" << relativePath << "not in project";
            //relativePath = QDir::cleanPath(project.relativeFilePath(newPath));
            this->addFile(newPath, newPath.section('/', -1));
            return;
        }
    }
    relativePath = QDir::cleanPath(project.relativeFilePath(newPath));
    qDebug() << "index  found" << relativePath << newPath;
    fileNames[index] = relativePath.section('/', -1);
    filePaths[index] = relativePath;
    this->reloadProjectTree();
}


void Project::setAsmType(int type)
{
    m_asmType = type;
}


int Project::getAsmType()
{
    return m_asmType;
}


void Project::setFileOpened(QString path, bool opened)
{
    QDir project(QFileInfo(prjPath).dir());
    QString relativePath = QDir::cleanPath(project.relativeFilePath(path));
    for (int i = 0; i < filePaths.count(); i++)
    {
        //qDebug() << "Project: setFileOpened" << relativePath << filePaths.at(i);
        if (filePaths.at(i) == relativePath)
        {
            //qDebug() << "Project: setFileOpened - file found and set";
            m_fileStats[i] = opened;
            break;
        }
    }
}


void Project::setRegWatchers(QList<RegWatcherExportStruct> regWatchers)
{
    m_regWatchers = regWatchers;
}


QList<RegWatcherExportStruct> Project::getRegWatchers()
{
    return m_regWatchers;
}