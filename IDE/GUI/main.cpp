/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup MainGUI
 * @file main.cpp
 */


#include <QtGui>
#include "../mds.h"
#include "mainform/mainform.h"
#include "guicfg/guicfg.h"
#ifndef MDS_FEATURE_TRIAL
    #include "widgets/LicenseInitWidget/licenseinitwidget.h"
#endif // MDS_FEATURE_TRIAL
#include "errordialog/errordlg.h"
#ifndef MDS_FEATURE_TRIAL
    #include "../utilities/LicenseCertificate/LicenseCertificate.h"
#endif // MDS_FEATURE_TRIAL
#include <iostream>
#ifdef MDS_FEATURE_TRIAL
    #include "dialogs/TrialExpired/trialexpired.h"
#endif // MDS_FEATURE_TRIAL

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication::setStyle("Plastique");
    QApplication app(argc, argv);
    #ifdef MDS_VARIANT_TRIAL
        QFileInfo mdsInfo(QCoreApplication::applicationFilePath());
        if (  MDS_TRIAL_PERIOD - mdsInfo.lastModified().daysTo(QDateTime::currentDateTime()) <= 0
           || MDS_TRIAL_PERIOD - mdsInfo.lastModified().daysTo(QDateTime::currentDateTime()) > MDS_TRIAL_PERIOD
           )
        {
            TrialExpired trial(0);
            trial.exec();
            return 0;
        }
    #endif
    //qDebug() << QStyleFactory::keys();
    Q_INIT_RESOURCE(icons);
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QFontDatabase fdb;
    //fdb.addApplicationFont("./resources/fonts/MostlyMono/MostlyMono.ttf");
    if (fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-R.ttf") < 0 )
    {
        qDebug() << "Main: already loaded";
    }
    if (fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-RI.ttf") < 0 )
    {
        qDebug() << "Main: already loaded";
    }
    if (fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-B.ttf") < 0 )
    {
        qDebug() << "Main: already loaded";
    }
    if (fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-BI.ttf") < 0 )
    {
        qDebug() << "Main: already loaded";
    }

    //foreach (const QString &pattern, fdb.families())
    //{
    //    qDebug() << pattern;
    //}

    /*QList<QByteArray> codecs = QTextCodec::availableCodecs();
    foreach (QByteArray array, codecs)
    {
        qDebug() << array;
    }*/
    
    GuiCfg::getInstance().setDefaultAll();
    GuiCfg::getInstance().setDefaultPaths(true);
    bool openFile = false;
    bool release = true;
    if (argc > 1)
    {
        if (QString::fromLocal8Bit(argv[1]) == "--debug")
        {
            release = false;
            GuiCfg::getInstance().setDefaultPaths(false);
        }
        else if (QString::fromLocal8Bit(argv[1]).section('.',-1) == "mds-project")
        {
            qDebug() << "Main: received file as argv[1] " << QString::fromLocal8Bit(argv[1]);
            openFile = true;
        }
        else
        {
            qDebug() << "Main: recieved something on argv[1]" << QString::fromLocal8Bit(argv[1]);
        }
    }
    GuiCfg::getInstance().loadConfig();

    if (true == release)
    {
        QDir homeDir(QDir::homePath() + "/.mds");
        if (false == homeDir.exists())
        {
            homeDir.mkpath(".");
        }
    }

    #ifndef MDS_VARIANT_TRIAL
        //std::ifstream file(GuiCfg::getInstance().getLicensePath().replace('/','\\').toStdString(), std::ios_base::binary);
        //qDebug() << GuiCfg::getInstance().getLicensePath();
        static const long long int MAX_SIZE = 10240;
        char data [ MAX_SIZE ];
        size_t len;
        QFile file(GuiCfg::getInstance().getLicensePath());
        //qDebug() << "Main: 3";
        if ((false != file.open(QIODevice::ReadOnly) ) && ( -1 !=  (len = file.read(data, MAX_SIZE))))
        {
            file.close();
            LicenseCertificate crt(std::string(data,len));
            if ( false == crt.m_isValid )
            {
                LicenseInitWidget *widget = new LicenseInitWidget(0);
                if (QDialog::Rejected == widget->exec())
                {
                    return 1;
                }
            }
            else
            {
                #ifdef MDS_VARIANT_COMMERCIAL
                    if (crt.m_product.m_variant != "Commercial")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #elif defined(MDS_VARIANT_NONCOMMERCIAL)
                    if (crt.m_product.m_variant != "Noncommercial")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #elif defined(MDS_GRADE_BASIC)
                    if (crt.m_product.m_grade != "Basic")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #elif defined(MDS_GRADE_PREMIUM)
                    if (crt.m_product.m_grade != "Premium")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #elif defined(MDS_GRADE_PROFESSIONAL)
                    if (crt.m_product.m_grade != "Professional")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #elif defined(MDS_GRADE_ULTIMATE)
                    if (crt.m_product.m_grade != "Ultimate")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #elif defined(MDS_TARGET_PICOBLAZE)
                    if (crt.m_product.m_target != "PicoBlaze")
                    {
                        LicenseInitWidget *widget = new LicenseInitWidget(0);
                        if (QDialog::Rejected == widget->exec())
                        {
                            return 1;
                        }
                    }
                #else
                    qDebug() << "Main: Error";
                    LicenseInitWidget *widget = new LicenseInitWidget(0);
                    if (QDialog::Rejected == widget->exec())
                    {
                        return 1;
                    }
                #endif
            }
        }
        else
        {
            qDebug() << "Main: else";
            LicenseInitWidget *widget = new LicenseInitWidget(0);
            if (QDialog::Rejected == widget->exec())
            {
                return 1;
            }
        }
    #endif

    //QResource::registerResource("icons.rcc");
    MainForm MainGUI;
    MainGUI.showMaximized();
    //qDebug() << "Main: height" << MainGUI.height();
    MainGUI.startHeight = MainGUI.height();

    if (true == openFile)
    {
        QTimer::singleShot(200, &MainGUI, SLOT(openProject(QString::fromLocal8Bit(argv[1]))));
    }
    else
    {
        //if session restore == true - restore
        if (true == GuiCfg::getInstance().loadSession())
        {
            //MainGUI.emitSessionRestorationSignal();
            QTimer::singleShot(200, &MainGUI, SLOT(sessionRestorationSlot()));
        }
        else
        {
            MainGUI.createWelcome();
        }
    }

    return app.exec();

}
