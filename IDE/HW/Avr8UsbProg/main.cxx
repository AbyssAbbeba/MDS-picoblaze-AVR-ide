// Path to the plugin shared library
#define PATH_TO_THE_PLUGIN "libAvr8UsbProgPlugin.so"


#include "HWPluginInterface.h"

#include <cstdlib>
#include <signal.h>

#include <QDebug>
#include <QObject>
#include <QApplication>
#include <QPluginLoader>

QPluginLoader* loader;

void abortOnSignal(int signal)
{
    qDebug() << QObject::tr("Exiting on signal") << " " << signal;
    if ( nullptr != loader )
    {
        loader->unload();
    }

//         exit(0);
}

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    loader=new QPluginLoader(PATH_TO_THE_PLUGIN);
    Q_CHECK_PTR(loader);

//         signal(SIGINT, abortOnSignal);
//         signal(SIGHUP, abortOnSignal);

    QObject * plugin = loader->instance();
    if ( nullptr == plugin)
    {
            qDebug() << QObject::tr("Unable to load programmer plugin");
            qDebug() << loader->errorString();
            abort();
    }

    HWPluginInterface * programmerPlugin = qobject_cast<HWPluginInterface *>(plugin);
    if ( nullptr == plugin)
    {
            qDebug() << QObject::tr("Plugin has not compatible interface with this program");
            abort();
    }

    programmerPlugin->setStandAlone(true);
    programmerPlugin->show();

    return app.exec();
}
