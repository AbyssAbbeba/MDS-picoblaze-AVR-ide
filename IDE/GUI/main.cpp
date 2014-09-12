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
#include "mainform/mainform.h"
#include "guicfg/guicfg.h"
#include "widgets/LicenseInitWidget/licenseinitwidget.h"
#include "errordialog/errordlg.h"
#include "../utilities/LicenseCertificate/LicenseCertificate.h"
#include <iostream>
#include "../mds.h"
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


    GuiCfg::getInstance().setDefaultAll();
    GuiCfg::getInstance().setDefaultPaths(true);
    bool openFile = false;
    if (argc > 1)
    {
        if (QString::fromLocal8Bit(argv[1]) == "--debug")
        {
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

    #ifndef MDS_VARIANT_TRIAL
        std::ifstream file(GuiCfg::getInstance().getLicensePath().toStdString(), std::ios_base::binary);
        if (!file.is_open())
        {
            qDebug() << "Main: license file not opened";
        }
        LicenseCertificate crt(file);
        file.close();

        if ( false == crt.m_isValid )
        {
            //error(ERR_NO_LICENSE);
            LicenseInitWidget *widget = new LicenseInitWidget(0);
            if (QDialog::Rejected == widget->exec())
            {
                return 1;
            }
        }
    #endif


    /*qDebug() << "CodeEdit: available encoding";
    QList<QByteArray> list = QTextCodec::availableCodecs();
    for (int i = 0; i < list.count(); i++)
    {
        qDebug() << "CodeEdit: codec" << i << list.at(i);
    }*/
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-16"));
    //qDebug() << "CodeEdit: current encoding" << QTextCodec::codecForLocale()->name();

    /*QStringList list = fdb.families();

    foreach (QString string, list)
    {
        qDebug() << string;
    }*/
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
