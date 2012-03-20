#include <QtGui>
#include "project.h"

ProjectMan::ProjectMan(MainForm* qMainWindow)
{
    //openProjects = new QList<Project*>;
    projectCount = 0;
    mainWindow = qMainWindow;
    
}


void ProjectMan::openProject(QFile *file)
{
    Project *newProject = new Project(file, mainWindow, this);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);
    if (projectCount == 1 && openProjects.at(0)->prjName == NULL)
    {
	delete openProjects.at(0);
        openProjects.removeAt(0);
        projectCount--;
    }
    else if (projectCount > 0) 
        mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);

    openProjects.append(newProject);
    activeProject = newProject;
    projectCount++;
}


void ProjectMan::addFile(QFile *file, QString path, QString name)
{
    getActive()->addFile(file, path, name);
}

void ProjectMan::addProject(QString name, QString path, QFile *file)
{
    Project *newProject = new Project(name, path, mainWindow, file, this);
    mainWindow->addDockWidget(Qt::LeftDockWidgetArea, newProject->prjDockWidget);

    if (projectCount == 1 && openProjects.at(0)->prjName == NULL) {
        delete openProjects.at(0);
        openProjects.removeAt(0);
        projectCount--;
    }
    else if (projectCount > 0) 
        mainWindow->tabifyDockWidget(openProjects.at(0)->prjDockWidget, newProject->prjDockWidget);

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

}





Project::~Project()
{
    delete this->prjDockWidget;
    
}

//otevreni projektu
Project::Project(QFile *file, QMainWindow * mainWindow, ProjectMan *parent)
{
    errorFlag = ERR_OK;
    fileCount = 0;
    parentManager = parent;

    //nacteni ze souboru
    QDomDocument domDoc("Project");
    if (!domDoc.setContent(file))
    {
        errorFlag = ERR_ASSIGN;
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "Project")
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
                    if (xmlElement.tagName() == "Attributes")
                    {
                        prjName = xmlElement.attribute("name", "");
                        prjPath = xmlElement.attribute("path", "");
                    }
                    if (xmlElement.tagName() == "File")
                    {
                        fileNames.append(xmlElement.attribute("name", ""));
                        filePaths.append(xmlElement.attribute("path", ""));
                        fileCount++;
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
            prjDockWidget = new QDockWidget(prjName, mainWindow);
            prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
            prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
            prjTreeWidget = new QTreeWidget(prjDockWidget);
            prjDockWidget->setWidget(prjTreeWidget);

            QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
            treeProjName->setText(0, prjName);
            treeProjName->setData(0, Qt::ToolTipRole, prjPath);

            for (int i=0; i<fileCount; ++i)
            {
                 QTreeWidgetItem *treeProjFile = new QTreeWidgetItem(treeProjName);
                 treeProjFile->setText(0, fileNames.at(i));
                 treeProjFile->setData(0, Qt::ToolTipRole, filePaths.at(i));
            }
            connect(prjDockWidget, SIGNAL(visibilityChanged(bool)),this,SLOT(setActive()));  
            connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),this,SLOT(openItem()));  
        }
    }
}

//vytvoreni prazdneho projektu
Project::Project(QString name, QString path, QMainWindow * mainWindow, QFile *file, ProjectMan *parent)
{
    errorFlag = ERR_OK;
    parentManager = parent;

    if (name != NULL)
        prjDockWidget = new QDockWidget(name, mainWindow);
    else
        prjDockWidget = new QDockWidget("Projects", mainWindow);
    prjDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    prjDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    prjTreeWidget = new QTreeWidget(prjDockWidget);
    prjDockWidget->setWidget(prjTreeWidget);
    prjName=name;
    //pri nacteni programu neni nacteny projekt
    if (name != NULL) {
        prjPath=path;
        QTreeWidgetItem *treeProjName = new QTreeWidgetItem(prjTreeWidget);
    	treeProjName->setText(0, name);
        treeProjName->setData(0, Qt::ToolTipRole, path);
    	fileCount=0;
        
        //a zapsani do souboru
        QDomDocument domDoc("Project");
        QDomElement xmlRoot = domDoc.createElement("Project");
        domDoc.appendChild(xmlRoot);

        QDomElement prjAttr = domDoc.createElement("Attributes");
        prjAttr.setAttribute("name", name);
        prjAttr.setAttribute("path", path);
        xmlRoot.appendChild(prjAttr);

        QTextStream xmlStream(file);
        xmlStream << domDoc.toString();

        connect(prjDockWidget, SIGNAL(visibilityChanged(bool)),this,SLOT(setActive()));
        connect(prjTreeWidget, SIGNAL(itemDoubleClicked (QTreeWidgetItem *,int)),this,SLOT(openItem()));  
        
    }

}



void Project::addFile(QFile *file, QString path, QString name)
{
    QDomDocument domDoc("Project");
    if (!domDoc.setContent(file))
    {
        errorFlag = ERR_ASSIGN;
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "Project")
        {
            errorFlag = ERR_CONTENT;
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomElement prjFile = domDoc.createElement("File");
            prjFile.setAttribute("name", name);
            prjFile.setAttribute("path", path);
            xmlRoot.appendChild(prjFile);

            file->close();
            file->open(QIODevice::WriteOnly);

            QTextStream xmlStream(file);
            xmlStream << domDoc.toString();

            fileNames.append(name);
            filePaths.append(path);
            fileCount++;
            QTreeWidgetItem *treeProjFile = new QTreeWidgetItem(prjTreeWidget->topLevelItem(0));
            treeProjFile->setText(0, name);
            treeProjFile->setData(0, Qt::ToolTipRole, path);
            
        }
    }
}



void Project::setActive()
{
    if (parentManager->isActiveProject(this))
        parentManager->setActive(this);
}




void Project::openItem()
{
    if (prjTreeWidget->currentItem() != NULL)
        parentManager->mainWindow->openFilePath(prjTreeWidget->currentItem()->data(0, Qt::ToolTipRole).toString());
}
