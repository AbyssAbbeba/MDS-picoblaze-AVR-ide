/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
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
    fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-R.ttf");
    fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-RI.ttf");
    fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-B.ttf");
    fdb.addApplicationFont(":/resources/fonts/Ubuntu/UbuntuMono-BI.ttf");

    GuiCfg::getInstance().setDefaultAll();
    GuiCfg::getInstance().setDefaultPaths(true);
    if (argc > 1)
    {
        if (QString::fromLocal8Bit(argv[1]) == "--debug")
        {
            GuiCfg::getInstance().setDefaultPaths(false);
        }
        else if (QString::fromLocal8Bit(argv[1]).section('.',0,-2) == "mds_project")
        {
            qDebug() << "Main: received file as argv[1] " << QString::fromLocal8Bit(argv[1]);
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
    
    return app.exec();
    
}
