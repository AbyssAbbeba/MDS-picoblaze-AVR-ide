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
#include <memory>
#include "../../../mds.h"


LicenseInitWidget::LicenseInitWidget(QWidget *parent)
    : QDialog(parent)
{
    this->setModal(true);
    ui.setupUi(this);
    this->setLayout(ui.gridLayout);
    #ifdef Q_OS_LINUX
        ui.tbInfo->setFont(QFont("Monospace", 10));
    #elif defined(Q_OS_WIN32)
        ui.tbInfo->setFont(QFont("Courier", 10));
    #endif

    ui.tbInfo->setReadOnly(true);
    #ifndef MDS_VARIANT_TRIAL
        connect(ui.btnLoad, SIGNAL(clicked()), this, SLOT(load()));
    #endif

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
    #ifndef MDS_VARIANT_TRIAL
        //qDebug() << "MDS_VARIANT_NONTRIAL";
        this->licensePath = GuiCfg::getInstance().getLicensePath();
        static const long long int MAX_SIZE = 10240;
        char data [ MAX_SIZE ];
        ssize_t len;
        QFile file(GuiCfg::getInstance().getLicensePath());
        if ((false != file.open(QIODevice::ReadOnly) ) && ( -1 !=  (len = file.read(data, MAX_SIZE))))
        {
            //load info
            LicenseCertificate crt(std::string(data,len));
            if ( true == crt.m_isValid )
            {
                this->license = true;
                QTextCursor cur = ui.tbInfo->textCursor();
                cur.setPosition(0);
                ui.tbInfo->setTextCursor(cur);
                ui.tbInfo->clear();
                ui.tbInfo->append("Licensee name:     ");
                ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_licensee.m_name.c_str()));
                ui.tbInfo->append("Product name:       ");
                ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_designation.c_str()));
                ui.tbInfo->append("Product variant:    ");
                ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_variant.c_str()));
                ui.tbInfo->append("Product grade:      ");
                ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_grade.c_str()));
                ui.tbInfo->append("Product target:     ");
                ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_target.c_str()));
                ui.tbInfo->append("Number of licenses: ");
                ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_licences.c_str()));
                this->setResult(QDialog::Accepted);
            }
        }
    #endif
    #ifdef MDS_VARIANT_TRIAL
        //qDebug() << "MDS_VARIANT_TRIAL";
        QFileInfo mdsInfo(QCoreApplication::applicationFilePath());
        QTextCursor cur = ui.tbInfo->textCursor();
        cur.setPosition(0);
        ui.tbInfo->setTextCursor(cur);
        ui.tbInfo->clear();
        ui.tbInfo->append("Trial period:\t");
        ui.tbInfo->insertPlainText(QString::number(MDS_TRIAL_PERIOD - mdsInfo.lastModified().daysTo(QDateTime::currentDateTime())) + QString(" days left"));
        this->license = true;
        this->setResult(QDialog::Accepted);
    #endif
}


void LicenseInitWidget::load()
{
    #ifndef MDS_VARIANT_TRIAL
        this->licensePath = QFileDialog::getOpenFileName(this, tr("Certificate File"), QDir::homePath(), tr("MDS Certificate (*.cert)"));
        if (this->licensePath != NULL)
        {
            ui.lePath->setText(this->licensePath);
            static const long long int MAX_SIZE = 10240;
            std::unique_ptr<char[]> data ( new char [ MAX_SIZE ] );
            ssize_t len;
            QFile file(this->licensePath);
            if ((false != file.open(QIODevice::ReadOnly) ) && ( -1 !=  (len = file.read(data.get(), MAX_SIZE))))
            {
                //load info
                LicenseCertificate crt(std::string(data.get(),len));
                if ( true == crt.m_isValid )
                {
                    #ifdef MDS_VARIANT_COMMERCIAL
                        if (crt.m_product.m_variant != "Commercial")
                        {
                            qDebug() << "LicenseInitWidget: not commercial";
                            qDebug() << QString::fromUtf8(crt.m_product.m_variant.c_str());
                            printError();
                            return;
                        }
                    #elif defined(MDS_VARIANT_NONCOMMERCIAL)
                        if (crt.m_product.m_variant != "Noncommercial")
                        {
                            qDebug() << "LicenseInitWidget: not noncommercial";
                            qDebug() << QString::fromUtf8(crt.m_product.m_variant.c_str());
                            printError();
                            return;
                        }
                    #endif

                    #if defined(MDS_GRADE_BASIC)
                        if (crt.m_product.m_grade != "Basic")
                        {
                            qDebug() << "LicenseInitWidget: not basic";
                            qDebug() << QString::fromUtf8(crt.m_product.m_grade.c_str());
                            printError();
                            return;
                        }
                    #elif defined(MDS_GRADE_PREMIUM)
                        if (crt.m_product.m_grade != "Premium")
                        {
                            qDebug() << "LicenseInitWidget: not premium";
                            qDebug() << QString::fromUtf8(crt.m_product.m_grade.c_str());
                            printError();
                            return;
                        }
                    #elif defined(MDS_GRADE_PROFESSIONAL)
                        if (crt.m_product.m_grade != "Professional")
                        {
                            qDebug() << "LicenseInitWidget: not professional";
                            qDebug() << QString::fromUtf8(crt.m_product.m_grade.c_str());
                            printError();
                            return;
                        }
                    #elif defined(MDS_GRADE_ULTIMATE)
                        if (crt.m_product.m_grade != "Ultimate")
                        {
                            qDebug() << "LicenseInitWidget: not ultimate";
                            qDebug() << QString::fromUtf8(crt.m_product.m_grade.c_str());
                            printError();
                            return;
                        }
                    #endif

                    #if defined(MDS_TARGET_PICOBLAZE)
                        if (crt.m_product.m_target != "PicoBlaze")
                        {
                            qDebug() << "LicenseInitWidget: not PicoBlaze";
                            qDebug() << QString::fromUtf8(crt.m_product.m_target.c_str());
                            printError();
                            return;
                        }
                    #endif
                    this->license = true;
                    this->setResult(QDialog::Accepted);
                    QTextCursor cur = ui.tbInfo->textCursor();
                    cur.setPosition(0);
                    ui.tbInfo->setTextCursor(cur);
                    ui.tbInfo->clear();
                    ui.tbInfo->append("Licensee name:     ");
                    ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_licensee.m_name.c_str()));
                    ui.tbInfo->append("Product name:       ");
                    ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_designation.c_str()));
                    ui.tbInfo->append("Product variant:    ");
                    ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_variant.c_str()));
                    ui.tbInfo->append("Product grade:      ");
                    ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_grade.c_str()));
                    ui.tbInfo->append("Product target:     ");
                    ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_target.c_str()));
                    ui.tbInfo->append("Number of licenses: ");
                    ui.tbInfo->insertPlainText(QString::fromUtf8(crt.m_product.m_licences.c_str()));
                }
                else
                {
                    printError();
                }
                file.close();
            }

        }
    #endif
    #ifdef MDS_VARIANT_TRIAL
        QFileInfo mdsInfo(QCoreApplication::applicationFilePath());
        ui.tbInfo->clear();
        ui.tbInfo->append("Trial period:\t");
        ui.tbInfo->insertPlainText(QString::number(MDS_TRIAL_PERIOD - mdsInfo.lastModified().daysTo(QDateTime::currentDateTime())) + QString(" days left"));
        this->license = true;
        this->setResult(QDialog::Accepted);
    #endif
}


void LicenseInitWidget::printError()
{
    ui.tbInfo->clear();
    this->license = false;
    ui.tbInfo->setHtml(
        QString("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd\">"
                "<html><head><meta name=\"qrichtext\" content=q\"1\" /><style type=\"text/css\">"
                "p, li { white-space: pre-wrap; }"
                "</style></head><body style=\" font-family:'Ubuntu'; font-size:11pt; font-weight:400; font-style:normal;\">"
                "<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><img src=\":/resources/icons/dialog-warning.png\" /><span style=\" font-size:30px; color:#ff0000;\">CERTIFICATE NOT VALID!</span></p></body></html>"
                )
    );
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
        //qDebug() << "LicenseInitWidget: trying to copy to" << GuiCfg::getInstance().getLicensePath();
        #ifndef MDS_VARIANT_TRIAL
            if (QFile::copy(this->licensePath, GuiCfg::getInstance().getLicensePath()) == false)
            {
                QFile::remove(GuiCfg::getInstance().getLicensePath());
                QFile::copy(this->licensePath, GuiCfg::getInstance().getLicensePath());
                /*{
                    qDebug() << "LicenseInitWidget: copy done";
                }
                else
                {
                    qDebug() << "LicenseInitWidget: copy not done";
                }*/
            }
        #endif
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