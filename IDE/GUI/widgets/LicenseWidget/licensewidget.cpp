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
#include "../LicenseInitWidget/licenseinitwidget.h"
#include "../../../utilities/LicenseCertificate/LicenseCertificate.h"
#include "../../guicfg/guicfg.h"
#include <iostream>
#include "../../../mds.h"


LicenseWidget::LicenseWidget(QWidget *parent)
    : QDialog(parent)
{
    this->setModal(true);
    ui.setupUi(this);
    this->setLayout(ui.gridLayout);
    ui.teInfo->setReadOnly(true);
    #ifndef MDS_VARIANT_TRIAL
        connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(load()));
    #endif
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
    #ifndef MDS_VARIANT_TRIAL
        this->licensePath = GuiCfg::getInstance().getLicensePath();
        static const long long int MAX_SIZE = 10240;
        char data [ MAX_SIZE ];
        size_t len;
        QFile file(GuiCfg::getInstance().getLicensePath());
        if ((false != file.open(QIODevice::ReadOnly) ) && ( -1LL !=  (len = file.read(data, MAX_SIZE))))
        {
            //load info
            LicenseCertificate crt(std::string(data,len));
            if ( true == crt.m_isValid )
            {
                this->license = true;
                ui.teInfo->clear();
                QFile file(":/resources/html/license.html");
                if (file.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    ui.teInfo->setHtml(QString::fromUtf8(file.readAll()));
                    QTextCursor cur(ui.teInfo->document());
                    cur = ui.teInfo->document()->find("__LICENSES__");
                    cur.insertText(QString::fromUtf8(crt.m_product.m_licences.c_str()));
                    cur = ui.teInfo->document()->find("__NAME__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_name.c_str()));
                    cur = ui.teInfo->document()->find("__EMAIL__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_email.c_str()));
                    cur = ui.teInfo->document()->find("__PHONE__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_phone.c_str()));
                    cur = ui.teInfo->document()->find("__STREET__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_street.c_str()));
                    cur = ui.teInfo->document()->find("__NUMBER__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_number.c_str()));
                    cur = ui.teInfo->document()->find("__POST__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_post.c_str()));
                    cur = ui.teInfo->document()->find("__CITY__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_municipality.c_str()));
                    cur = ui.teInfo->document()->find("__STATE__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_country.c_str()));
                    ui.teInfo->setTextCursor(cur);
                    file.close();
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
        }
        else
        {
            ui.teInfo->clear();
            this->license = false;
            ui.teInfo->insertPlainText("<invalid certificate>");
            this->setResult(QDialog::Rejected);
        }
    #endif
    #ifdef MDS_VARIANT_TRIAL
        this->license = true;
        ui.teInfo->clear();
        QFile file(":/resources/html/license.html");
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            ui.teInfo->setHtml(QString::fromUtf8(file.readAll()));
            QTextCursor cur(ui.teInfo->document());
            cur = ui.teInfo->document()->find("__TRIAL__");
            cur.insertText(QString::number(MDS_TRIAL_PERIOD));
            ui.teInfo->setTextCursor(cur);
            file.close();
        }
        this->setResult(QDialog::Accepted);
    #endif
}


void LicenseWidget::load()
{
    /*this->licensePath = QFileDialog::getOpenFileName(this, tr("Source File"), "");
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
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_name.c_str()));
                    cur = ui.teInfo->document()->find("__EMAIL__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_email.c_str()));
                    cur = ui.teInfo->document()->find("__PHONE__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_phone.c_str()));
                    cur = ui.teInfo->document()->find("__STREET__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_street.c_str()));
                    cur = ui.teInfo->document()->find("__NUMBER__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_number.c_str()));
                    cur = ui.teInfo->document()->find("__POST__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_post.c_str()));
                    cur = ui.teInfo->document()->find("__CITY__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_municipality.c_str()));
                    cur = ui.teInfo->document()->find("__STATE__");
                    cur.insertText(QString::fromUtf8(crt.m_licensee.m_address.m_country.c_str()));
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

    }*/
    LicenseInitWidget *widget = new LicenseInitWidget(0);
    this->hide();
    widget->tryLoad();
    widget->exec();
    this->show();
    this->tryLoad();
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