/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup SaveDialog
 * @file savedlg.cpp
 */


#include <QtGui>
#include "savedlg.h"


/**
 * @brief
 * @param
 * @param
 */
SaveDialog::SaveDialog(QWidget *parent, QStringList lst, bool reload)
    : QDialog(parent)
{
    ui.setupUi(this);
    m_reload = reload;
    if (false == reload)
    {
        for (int i = 0; i < lst.count(); i++)
        {
            QListWidgetItem *item = new QListWidgetItem(lst.at(i).section('/', -1), ui.lstFiles);
            item->setToolTip(lst.at(i));
        }
        ui.lstFiles->setCurrentRow(0);
    }



    if (false == reload)
    {
        saveButton = new QPushButton(tr("Save"), this);
        saveAllButton = new QPushButton(tr("Save All"), this);
        discardButton = new QPushButton(tr("Discard"), this);
        discardAllButton = new QPushButton(tr("Discard All"), this);
        cancelButton = new QPushButton(tr("Cancel"), this);
    }
    else
    {
        this->setWindowTitle("Reload File");
        ui.label->setText("Some files were modified outside IDE, do you want to reload the files from disk?");
        saveButton = new QPushButton(tr("Reload"), this);
        saveAllButton = new QPushButton(tr("Reload All"), this);
        discardButton = new QPushButton(tr("Discard"), this);
        discardAllButton = new QPushButton(tr("Discard All"), this);
    }
    
    ui.buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(saveAllButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(discardButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(discardAllButton, QDialogButtonBox::ActionRole);
    if (false == reload)
    {
        ui.buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    }
    
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(saveAllButton, SIGNAL(clicked()), this, SLOT(saveAll()));
    connect(discardButton, SIGNAL(clicked()), this, SLOT(no()));
    connect(discardAllButton, SIGNAL(clicked()), this, SLOT(noAll()));
    if (false == reload)
    {
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(returnBack()));
    }
}


void SaveDialog::save()
{
    QListWidgetItem *item;
    if (ui.lstFiles->selectedItems().count() > 0)
    {
        item = ui.lstFiles->selectedItems().at(0);
    }
    else
    {
        item = ui.lstFiles->takeItem(0);
    }
    if (true == m_reload)
    {
        emit reload(item->toolTip());
    }
    else
    {
        emit save(item->toolTip());
    }
    m_files.removeAll(item->toolTip());
    delete item;
    if (ui.lstFiles->count() == 0)
    {
        this->accept();
    }
}


void SaveDialog::saveAll()
{
    QListWidgetItem *item;
    while (ui.lstFiles->count() > 0)
    {
        item = ui.lstFiles->takeItem(0);
        if (true == m_reload)
        {
            emit reload(item->toolTip());
        }
        else
        {
            emit save(item->toolTip());
        }
        m_files.removeAll(item->toolTip());
        delete item;
    }
    this->accept();
}


void SaveDialog::no()
{
    QListWidgetItem *item;
    if (ui.lstFiles->selectedItems().count() > 0)
    {
        item = ui.lstFiles->selectedItems().at(0);
    }
    else
    {
        item = ui.lstFiles->takeItem(0);
    }
    m_files.removeAll(item->toolTip());
    delete item;
    if (ui.lstFiles->count() == 0)
    {
        this->accept();
    }
}


void SaveDialog::noAll()
{
    this->accept();
}


void SaveDialog::returnBack()
{
    this->reject();
}


void SaveDialog::appendFile(QString path)
{
    if (false == m_files.contains(path))
    {
        m_files << path;
        QListWidgetItem *item = new QListWidgetItem(path.section('/', -1), ui.lstFiles);
        item->setToolTip(path);
    }
}


void SaveDialog::closeEvent(QCloseEvent *e)
{
    if (true == m_reload)
    {
        emit reloadFinished();
    }
}