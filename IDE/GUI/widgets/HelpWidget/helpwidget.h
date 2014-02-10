#ifndef HELPWIDGET_H
#define HELPWIDGET_H


#include <QWidget>
#include <QHelpEngine>
#include <QHelpSearchEngine>
#include "helpbrowser.h"



class HelpWidget : public QWidget
{
    public:
        HelpWidget(QWidget *parent, int width = 600, int height = 800);

    private:
        HelpBrowser *textBrowser;
};





#endif