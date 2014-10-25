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
    connect(ui.buttonBox->button(QDialogButtonBox::Save), SIGNAL(clicked()), this, SLOT(save()));
    connect(ui.buttonBox->button(QDialogButtonBox::SaveAll), SIGNAL(clicked()), this, SLOT(saveAll()));
    connect(ui.buttonBox->button(QDialogButtonBox::No), SIGNAL(clicked()), this, SLOT(no()));
    connect(ui.buttonBox->button(QDialogButtonBox::NoToAll), SIGNAL(clicked()), this, SLOT(noAll()));
    connect(ui.buttonBox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), this, SLOT(returnBack()));
}


void SaveDialog::save()
{
    QListWidgetItem *item = ui.lstFiles->takeItem(0);
    emit save(item->toolTip());
    delete item;
    if (ui.lstFiles->count() == 0)
    {
        this->accept();
    }
}


void SaveDialog::saveAll()
{
    while (ui.lstFiles->count() > 0)
    {
        QListWidgetItem *item = ui.lstFiles->takeItem(0);
        emit save(item->toolTip());
        delete item;
    }
    this->accept();
}


void SaveDialog::no()
{
    QListWidgetItem *item = ui.lstFiles->takeItem(0);
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