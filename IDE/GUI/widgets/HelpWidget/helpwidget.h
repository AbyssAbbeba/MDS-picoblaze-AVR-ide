#ifndef HELPWIDGET_H
#define HELPWIDGET_H


#include <QWidget>
#include <QTextBrowser>



class HelpWidget : public QWidget
{
    public:
        HelpWidget(QWidget *parent, int width = 600, int height = 750);

    private:
        QTextBrowser *textBrowser;
};





#endif