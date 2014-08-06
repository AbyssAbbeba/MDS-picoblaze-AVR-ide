/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup HelpWidget
 * @file helpwidget.cpp
 */


#include <QtGui>
#include "licenseinitwidget.h"
#include "../../../utilities/LicenseCertificate/LicenseCertificate.h"
#include "../../guicfg/guicfg.h"
#include <iostream>


LicenseInitWidget::LicenseInitWidget(QWidget *parent)
    : QDialog(parent)
{
    this->setModal(true);
    ui.setupUi(this);
    this->setLayout(ui.verticalLayout);
    ui.tbInfo->setReadOnly(true);
    connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(load()));

    connect(ui.btnBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui.btnBox, SIGNAL(rejected()), this, SLOT(reject()));
    
    connect(this, SIGNAL(accepted()), this, SLOT(tryAccept()));
    connect(this, SIGNAL(rejected()), this, SLOT(tryReject()));

    this->license = false;
    this->setResult(QDialog::Rejected);
}

//copy to path after ok
//QFile::copy("/path/file", "/path/copy-of-file");


void LicenseInitWidget::tryLoad()
{
    this->licensePath = GuiCfg::getInstance().getLicensePath();
    std::ifstream ifs;

    ifs.open (this->licensePath.toStdString(), std::ios_base::binary);

    if (ifs.is_open())
    {
        //load info
        LicenseCertificate crt(ifs);
        if ( true == crt.m_isValid )
        {
            this->license = true;
            ui.tbInfo->clear();
            this->setResult(QDialog::Accepted);
        }
    }
}


void LicenseInitWidget::load()
{
    this->licensePath = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (this->licensePath != NULL)
    {
        //ui.lePath->setText(this->licensePath);
        std::ifstream ifs;

        ifs.open (this->licensePath.toStdString(), std::ios_base::binary);

        if (ifs.is_open())
        {
            //load info
            LicenseCertificate crt(ifs);
            if ( true == crt.m_isValid )
            {
                this->license = true;
                ui.tbInfo->clear();
            }
            else
            {
                ui.tbInfo->clear();
                this->license = false;
                ui.tbInfo->insertPlainText("NOT VALID!");
            }

            ifs.close();
        }

    }
}


/*void LicenseInitWidget::closeEvent(QCloseEvent *)
{
    if (true == this->license)
    {
        this->setResult(QDialog::Accepted);
    }
    else
    {
        this->setResult(QDialog::Rejected);
    }
}*/


void LicenseInitWidget::tryAccept()
{
    if (true == this->license)
    {
        if (QFile::copy(this->licensePath, GuiCfg::getInstance().getLicensePath()) == false)
        {
            QFile::remove(GuiCfg::getInstance().getLicensePath());
            QFile::copy(this->licensePath, GuiCfg::getInstance().getLicensePath());
        }
        this->setResult(QDialog::Accepted);
    }
    else
    {
        this->setResult(QDialog::Rejected);
    }
    this->close();
}


void LicenseInitWidget::tryReject()
{
    this->setResult(QDialog::Rejected);
    this->close();
}