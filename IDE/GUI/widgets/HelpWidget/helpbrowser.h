
#ifndef HELPBROWSER_H
#define HELPBROWSER_H


#include <QTextBrowser>
#include <QHelpEngine>

class HelpBrowser : public QTextBrowser
{
    public:
        HelpBrowser(QWidget *parent);
        QVariant loadResource(int type, const QUrl &url);

    private:
        QHelpEngine *helpEngine;
};

#endif