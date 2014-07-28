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
            //ui.teInfo->insertPlainText("Version:\t");

            //ui.teInfo->insertPlainText(QString::fromStdString(crt.m_version) + "\n");
            //ui.teInfo->insertPlainText("Date:\t");
            //ui.teInfo->insertPlainText(QString::number(crt.m_date) + "\n");
            //ui.teInfo->insertPlainText("UserID:\t");
            //ui.teInfo->insertPlainText(QString::number(crt.m_userid) + "\n");
            ui.teInfo->insertPlainText("User:\t");
            ui.teInfo->insertPlainText(QString::fromStdString(crt.m_name) + "\n");
            //ui.teInfo->insertPlainText("Expiration:\t");
            //ui.teInfo->insertPlainText(QString::number(crt.m_expiry) + "\n");
            //ui.teInfo->insertPlainText("LicenseID:\t");
            //ui.teInfo->insertPlainText(QString::number(crt.m_licenseid) + "\n");
            ui.teInfo->insertPlainText("Products:");
            for (unsigned int i = 0; i < crt.m_products.size(); i++)
            {
                ui.teInfo->insertPlainText("\t" + QString::fromStdString(crt.m_products.at(i)) + "\n");
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
                //ui.teInfo->insertPlainText("Version:\t");
                
                //ui.teInfo->insertPlainText(QString::fromStdString(crt.m_version) + "\n");
                //ui.teInfo->insertPlainText("Date:\t");
                //ui.teInfo->insertPlainText(QString::number(crt.m_date) + "\n");
                //ui.teInfo->insertPlainText("UserID:\t");
                //ui.teInfo->insertPlainText(QString::number(crt.m_userid) + "\n");
                ui.teInfo->insertPlainText("User:\t");
                ui.teInfo->insertPlainText(QString::fromStdString(crt.m_name) + "\n");
                //ui.teInfo->insertPlainText("Expiration:\t");
                //ui.teInfo->insertPlainText(QString::number(crt.m_expiry) + "\n");
                //ui.teInfo->insertPlainText("LicenseID:\t");
                //ui.teInfo->insertPlainText(QString::number(crt.m_licenseid) + "\n");
                ui.teInfo->insertPlainText("Products:");
                for (unsigned int i = 0; i < crt.m_products.size(); i++)
                {
                   ui.teInfo->insertPlainText("\t" + QString::fromStdString(crt.m_products.at(i)) + "\n");
                }
                /*std::cout << "Certificate is valid." << std::endl;
                std::cout << std::endl;
                std::cout << "<certificate>" << std::endl;
                std::cout << "  ├─ version: " << crt.m_version << std::endl;
                std::cout << "  ├─ user ID: " << crt.m_userid << std::endl;
                std::cout << "  ├─ license ID: " << crt.m_licenseid << std::endl;
                std::cout << "  ├─ date: " << crt.m_date << std::endl;
                std::cout << "  ├─ expiry: " << crt.m_expiry << std::endl;
                std::cout << "  ├─ products:";
                for ( const auto & product : crt.m_products )
                {
                    std::cout << ' ' << product;
                }
                std::cout << std::endl;
                std::cout << "  ├─ <address>" << std::endl;
                std::cout << "  │    ├─ street1: \"" << crt.m_addrStreet1 << '"' << std::endl;
                std::cout << "  │    ├─ street2: \"" << crt.m_addrstreet2 << '"' << std::endl;
                std::cout << "  │    ├─ city: \"" << crt.m_addrcity << '"' << std::endl;
                std::cout << "  │    ├─ post: \"" << crt.m_addrpost << '"' << std::endl;
                std::cout << "  │    ├─ state1: \"" << crt.m_addrstate1 << '"' << std::endl;
                std::cout << "  │    ├─ state2: \"" << crt.m_addrstate2 << '"' << std::endl;
                std::cout << "  │    ╰─ state3: \"" << crt.m_addrstate3 << '"' << std::endl;
                std::cout << "  ├─ name: \"" << crt.m_name << '"' << std::endl;
                std::cout << "  ╰─ <details>" << std::endl;
                std::cout << "       ├─ type: \"" << crt.m_detailsType << '"' << std::endl;
                std::cout << "       ├─ <contact>" << std::endl;
                std::cout << "       │    ├─ email: " << crt.m_detailsContactEmail << std::endl;
                std::cout << "       │    ├─ phone: \"" << crt.m_detailsContactPhone << '"' << std::endl;
                std::cout << "       │    ╰─ address: \"" << crt.m_detailsContactAddress << '"' << std::endl;
                std::cout << "       ╰─ department: \"" << crt.m_detailsDepartment << '"' << std::endl;*/
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