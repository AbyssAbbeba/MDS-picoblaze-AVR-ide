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


RegWatcherDialog::RegWatcherDialog(QWidget *parent, bool regbanks)
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
    
    QRegExpValidator *validatorAddress = new QRegExpValidator(QRegExp("0x[\\da-fA-F]+"), ui.leAddress);
    ui.leAddress->setValidator(validatorAddress);
    ui.leName->setFocus(Qt::OtherFocusReason);
}


RegWatcherDialog::RegWatcherDialog(QWidget *parent, bool regbanks, QString name, int address, int type, int regbank)
    : QDialog(parent)
{
    ui.setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(emitEditItem()));
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

    QRegExpValidator *validatorAddress = new QRegExpValidator(QRegExp("0x[\\da-fA-F]+"), ui.leAddress);
    ui.leAddress->setValidator(validatorAddress);
    ui.leName->setFocus(Qt::OtherFocusReason);

    ui.leName->setText(name);
    ui.leAddress->setText("0x" + QString::number(address, 16));
    ui.cmbType->setCurrentIndex(type);
    ui.cmbBank->setCurrentIndex(regbank);
}


void RegWatcherDialog::emitNewItem()
{
    if (true == m_regbanks)
    {
        bool ok;
        emit newItem(ui.leName->text(), ui.cmbType->currentIndex(), ui.leAddress->text().toInt(&ok, 16), ui.cmbBank->currentIndex());
    }
    else
    {
        bool ok;
        emit newItem(ui.leName->text(), ui.cmbType->currentIndex(), ui.leAddress->text().toInt(&ok, 16));
    }
}


void RegWatcherDialog::emitEditItem()
{
    if (true == m_regbanks)
    {
        bool ok;
        emit editItem(ui.leName->text(), ui.cmbType->currentIndex(), ui.leAddress->text().toInt(&ok, 16), ui.cmbBank->currentIndex());
    }
    else
    {
        bool ok;
        emit editItem(ui.leName->text(), ui.cmbType->currentIndex(), ui.leAddress->text().toInt(&ok, 16));
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