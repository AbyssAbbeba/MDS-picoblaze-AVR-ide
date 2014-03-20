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

int main(int argc, char *argv[])
{
    QApplication::setDesktopSettingsAware(false);
    QApplication::setStyle("Plastique");
    QApplication app(argc, argv);
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

    /*QStringList list = fdb.families();

    foreach (QString string, list)
    {
        qDebug() << string;
    }*/
    //QResource::registerResource("icons.rcc");
    MainForm MainGUI;
    MainGUI.showMaximized();
    
    if (true == openFile)
    {
        MainGUI.openProject(QString::fromLocal8Bit(argv[1]));
    }
    else
    {
        //if session restore == true - restore
        if (true == GuiCfg::getInstance().loadSession())
        {
            qDebug() << "Main: session restoration";
            //open projects and files
            QList<QString> projectPaths = GuiCfg::getInstance().getSessionProjectPaths();
            QList<QString> filePaths = GuiCfg::getInstance().getSessionFilePaths();
            for (int i = 0; i < projectPaths.count(); i++)
            {
                MainGUI.openProject(projectPaths.at(i));
            }
            for (int i = 0; i < filePaths.count(); i++)
            {
                MainGUI.openFilePath(filePaths.at(i));
            }
            GuiCfg::getInstance().sessionClear();
        }
    }
    
    return app.exec();
    
}
