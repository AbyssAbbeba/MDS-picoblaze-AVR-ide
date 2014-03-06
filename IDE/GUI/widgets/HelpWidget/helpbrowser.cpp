/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup HelpBrowser
 * @file helpbrowser.cpp
 */


#include <QtGui>
#include "helpbrowser.h"
#include "../../guicfg/guicfg.h"


HelpBrowser::HelpBrowser(QWidget *parent)
    : QTextBrowser(parent)
{
    helpEngine = new QHelpEngine(GuiCfg::getInstance().getHelpPath() + "/MDS_manual.qhc", this);
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