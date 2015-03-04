/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file helpbrowser.h
 */


#ifndef HELPBROWSER_H
#define HELPBROWSER_H


#include <QTextBrowser>
#include <QUrl>

class QHelpEngine;

class HelpBrowser : public QTextBrowser
{
    public:
        HelpBrowser(QWidget *parent);
        QVariant loadResource(int type, const QUrl &url);

    private:
        QHelpEngine *helpEngine;
};

#endif