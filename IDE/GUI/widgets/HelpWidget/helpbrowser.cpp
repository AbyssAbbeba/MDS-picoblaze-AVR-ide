#include <QtGui>
#include "helpbrowser.h"


HelpBrowser::HelpBrowser(QWidget *parent)
    : QTextBrowser(parent)
{
    helpEngine = new QHelpEngine("../docs/manual/MDS_manual.qhc", this);
    helpEngine->setupData();
}

QVariant HelpBrowser::loadResource(int type, const QUrl &url)
{
    if (url.scheme() == "qthelp")
    {
        return QVariant(helpEngine->fileData(url));
    }
    else
    {
        return QTextBrowser::loadResource(type, url);
    }
}