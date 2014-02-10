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
    fdb.addApplicationFont("./resources/fonts/FreeMono/FreeMono.ttf");
    fdb.addApplicationFont("./resources/fonts/FreeMono/FreeMonoBold.ttf");
    fdb.addApplicationFont("./resources/fonts/FreeMono/FreeMonoOblique.ttf");
    fdb.addApplicationFont("./resources/fonts/FreeMono/FreeMonoBoldOblique.ttf");

    GuiCfg::getInstance().setDefaultAll();
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
