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
    this->setLayout(ui.gridLayout);
    ui.teInfo->setReadOnly(true);
    connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(load()));
    connect(ui.btnPrint, SIGNAL(clicked()), this, SLOT(print()));
    //connect(this, SIGNAL(accepted()), this, SLOT(tryAccept()));
    //connect(this, SIGNAL(rejected()), this, SLOT(tryReject()));

    this->license = false;
    this->setResult(QDialog::Rejected);
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
            QFile file(":/resources/html/license.html");
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                ui.teInfo->setHtml(QString::fromUtf8(file.readAll()));
                QTextCursor cur(ui.teInfo->document());
                cur = ui.teInfo->document()->find("__NAME__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_name));
                cur = ui.teInfo->document()->find("__EMAIL__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_email));
                cur = ui.teInfo->document()->find("__PHONE__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_phone));
                cur = ui.teInfo->document()->find("__STREET__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_street));
                cur = ui.teInfo->document()->find("__NUMBER__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_number));
                cur = ui.teInfo->document()->find("__POST__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_post));
                cur = ui.teInfo->document()->find("__CITY__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_municipality));
                cur = ui.teInfo->document()->find("__STATE__");
                cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_country));
                ui.teInfo->setTextCursor(cur);
                file.close();
                this->setResult(QDialog::Accepted);
            }
        }
    }
}


void LicenseWidget::load()
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
                ui.teInfo->clear();
                QFile file(":/resources/html/license.html");
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui.teInfo->setHtml(file.readAll());
                    QTextCursor cur(ui.teInfo->document());
                    cur = ui.teInfo->document()->find("__NAME__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_name));
                    cur = ui.teInfo->document()->find("__EMAIL__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_email));
                    cur = ui.teInfo->document()->find("__PHONE__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_phone));
                    cur = ui.teInfo->document()->find("__STREET__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_street));
                    cur = ui.teInfo->document()->find("__NUMBER__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_number));
                    cur = ui.teInfo->document()->find("__POST__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_post));
                    cur = ui.teInfo->document()->find("__CITY__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_municipality));
                    cur = ui.teInfo->document()->find("__STATE__");
                    cur.insertText(QString::fromStdString(crt.m_licensee.m_address.m_country));
                    ui.teInfo->setTextCursor(cur);
                    file.close();
                }
                if (QFile::copy(this->licensePath, GuiCfg::getInstance().getLicensePath()) == false)
                {
                    QFile::remove(GuiCfg::getInstance().getLicensePath());
                    QFile::copy(this->licensePath, GuiCfg::getInstance().getLicensePath());
                    this->setResult(QDialog::Accepted);
                }
            }
            else
            {
                ui.teInfo->clear();
                this->license = false;
                ui.teInfo->insertPlainText("<invalid certificate>");
                this->setResult(QDialog::Rejected);
            }

            ifs.close();
        }

    }
}


/*void LicenseWidget::closeEvent(QCloseEvent *)
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


void LicenseWidget::print()
{
    QPrinter printer;
    printer.setPageSize(QPrinter::A4);
    printer.setFullPage(true);
    QPrintDialog *dialog = new QPrintDialog (&printer, this);
    if (dialog->exec() != QDialog::Accepted)
    {
        return;
    }
    ui.teInfo->print(&printer);
}


/*void LicenseWidget::tryAccept()
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
}*/