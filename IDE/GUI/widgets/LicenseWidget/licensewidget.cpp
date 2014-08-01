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
#include "licensewidget.h"
#include "../../../utilities/LicenseCertificate/LicenseCertificate.h"
#include "../../guicfg/guicfg.h"
#include <iostream>


LicenseWidget::LicenseWidget(QWidget *parent)
    : QDialog(parent)
{
    this->setModal(true);
    ui.setupUi(this);
    ui.teInfo->setReadOnly(true);
    connect(ui.btnPath, SIGNAL(clicked()), this, SLOT(load()));
    connect(this, SIGNAL(accepted()), this, SLOT(tryAccept()));
    connect(this, SIGNAL(rejected()), this, SLOT(tryReject()));

    this->license = false;
}

//copy to path after ok
//QFile::copy("/path/file", "/path/copy-of-file");


void LicenseWidget::tryLoad()
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
            ui.teInfo->clear();
            QFile file = QFile(":resources/html/license.html")
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ui.teInfo->setHtml(file.readAll())
                file.close()
            }
        }
    }
}


void LicenseWidget::load()
{
    this->licensePath = QFileDialog::getOpenFileName(this, tr("Source File"), "");
    if (this->licensePath != NULL)
    {
        ui.lePath->setText(this->licensePath);
        std::ifstream ifs;

        ifs.open (this->licensePath.toStdString(), std::ios_base::binary);

        if (ifs.is_open())
        {
            //load info
            LicenseCertificate crt(ifs);
            if ( true == crt.m_isValid )
            {
                this->license = true;
                ui.teInfo->clear();
                QFile file = QFile(":resources/html/license.html")
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui.teInfo->setHtml(file.readAll())
                    file.close()
                }
            }
            else
            {
                ui.teInfo->clear();
                ui.teInfo->insertPlainText("<invalid certificate>");
            }

            ifs.close();
        }

    }
}


void LicenseWidget::tryAccept()
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


void LicenseWidget::tryReject()
{
    this->setResult(QDialog::Rejected);
    this->close();
}