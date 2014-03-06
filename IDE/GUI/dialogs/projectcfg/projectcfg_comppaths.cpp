/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file projectcfg_comppaths.cpp
 */


#include "projectcfg_comppaths.h"
#include "../project/project.h"
#include <QFileDialog>
#include "../errordialog/errordlg.h"


/**
 * @brief
 * @param
 * @param
 */
ProjectCfg_CompPaths::ProjectCfg_CompPaths(QWidget *parentWidget, Project *currProject)
    : QWidget(parentWidget)
{
    this->project = currProject;

    ui.setupUi(this);
    
    if (this->project != NULL)
    {
        this->load();
    }

    connect(this->ui.btnNew, SIGNAL(clicked()), this, SLOT(newPath()));
    connect(this->ui.btnEdit, SIGNAL(clicked()), this, SLOT(editPath()));
    connect(this->ui.btnDelete, SIGNAL(clicked()), this, SLOT(deletePath()));
    //connect(this, SIGNAL(savePaths(QList<QString>)), this->project, SLOT(setCompileIncPaths(QList<QString>)));
}


/**
 * @brief
 */
void ProjectCfg_CompPaths::newPath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "");
    if (NULL != path)
    {
        QListWidgetItem *item = new QListWidgetItem(path, ui.lstPaths);
    }
}


/**
 * @brief
 */
void ProjectCfg_CompPaths::editPath()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "");
    if (NULL != path)
    {
        this->ui.lstPaths->currentItem()->setText(path);
    }
}


/**
 * @brief
 */
void ProjectCfg_CompPaths::deletePath()
{
    if (0 < this->ui.lstPaths->count())
    {
        if (NULL == this->ui.lstPaths->currentItem())
        {
            delete this->ui.lstPaths->item(0);
        }
        else
        {
            delete this->ui.lstPaths->currentItem();
        }
    }
}


/**
 * @brief
 */
void ProjectCfg_CompPaths::load()
{
    //qDebug() << "ProjectCfg_CompPaths: load()";
    for (int i = 0; i < this->project->compileIncPaths.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(this->project->compileIncPaths.at(i), this->ui.lstPaths);
    }
    //qDebug() << "ProjectCfg_CompPaths: load()";
}


/**
 * @brief
 */
void ProjectCfg_CompPaths::save()
{
    QStringList paths;
    for (int i = 0; i < this->ui.lstPaths->count(); i++)
    {
        paths.append(this->ui.lstPaths->item(i)->text());
    }
    this->project->setCompileIncPaths(paths);
}


/**
 * @brief
 */
QStringList ProjectCfg_CompPaths::getPaths()
{
    QStringList paths;
    for (int i = 0; i < this->ui.lstPaths->count(); i++)
    {
        paths.append(this->ui.lstPaths->item(i)->text());
    }
    return paths;
}