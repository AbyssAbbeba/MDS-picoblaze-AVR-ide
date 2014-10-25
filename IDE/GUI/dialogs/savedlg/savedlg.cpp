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
SaveDialog::SaveDialog(QWidget *parent, QStringList lst)
    : QDialog(parent)
{
    ui.setupUi(this);
    for (int i = 0; i < lst.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(lst.at(i).section('/', -1), ui.lstFiles);
        item->setToolTip(lst.at(i));
    }
    ui.lstFiles->setCurrentRow(0);

    QPushButton *saveButton = new QPushButton(tr("Save"), this);
    QPushButton *saveAllButton = new QPushButton(tr("Save All"), this);
    QPushButton *discardButton = new QPushButton(tr("Discard"), this);
    QPushButton *discardAllButton = new QPushButton(tr("Discard All"), this);
    QPushButton *cancelButton = new QPushButton(tr("Cancel"), this);
    
    ui.buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(saveAllButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(discardButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(discardAllButton, QDialogButtonBox::ActionRole);
    ui.buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);
    
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(saveAllButton, SIGNAL(clicked()), this, SLOT(saveAll()));
    connect(discardButton, SIGNAL(clicked()), this, SLOT(no()));
    connect(discardAllButton, SIGNAL(clicked()), this, SLOT(noAll()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(returnBack()));
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
    emit save(item->toolTip());
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
        emit save(item->toolTip());
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