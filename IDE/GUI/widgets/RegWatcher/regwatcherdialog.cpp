/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup RegWatcherDialog
 * @file regwatcherdialog.cpp
 */


#include <QtGui>
#include "regwatcherdialog.h"

RegWatcherDialog::RegWatcherDialog(QWidget *parent, bool regbanks, int offset)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(emitNewItem()));
    connect(ui.leAddress, SIGNAL(textEdited(const QString&)), this, SLOT(checkIfValid()));
    connect(ui.leName, SIGNAL(textEdited(const QString&)), this, SLOT(checkIfValid()));
    connect(ui.cmbType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBanks(int)));
    ui.btnBox->button(QDialogButtonBox::Ok)->setEnabled(false);

    if (false == regbanks)
    {
        ui.lblBank->hide();
        ui.cmbBank->hide();
    }

    m_regbanks = regbanks;
    m_bankOffset = offset;
    
    QRegExpValidator *validatorAddress = new QRegExpValidator(QRegExp("0x[\\da-fA-F]+"), ui.leAddress);
    ui.leAddress->setValidator(validatorAddress);
}


void RegWatcherDialog::emitNewItem()
{
    if (true == m_regbanks)
    {
        bool ok;
        emit newItem(ui.leName->text(), ui.cmbType->currentIndex(), ui.leAddress->text().toInt(&ok, 16)+ui.cmbBank->currentIndex()*m_bankOffset);
    }
    else
    {
        bool ok;
        emit newItem(ui.leName->text(), ui.cmbType->currentIndex(), ui.leAddress->text().toInt(&ok, 16));
    }
}


void RegWatcherDialog::checkIfValid()
{
    if (true == ui.leName->text().isEmpty())
    {
        ui.btnBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        bool ok;
        ui.leAddress->text().toInt(&ok, 16);
        if (false == ok)
        {
            ui.btnBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
        else
        {
            ui.btnBox->button(QDialogButtonBox::Ok)->setEnabled(true);
        }
    }
}


void RegWatcherDialog::updateBanks(int index)
{
    if (0 == index && true == m_regbanks)
    {
        ui.lblBank->show();
        ui.cmbBank->show();
    }
    else
    {
        ui.lblBank->hide();
        ui.cmbBank->hide();
    }
}