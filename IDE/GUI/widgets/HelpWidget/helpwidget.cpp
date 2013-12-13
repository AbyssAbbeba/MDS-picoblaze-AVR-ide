#include <QtGui>
#include "helpwidget.h"



HelpWidget::HelpWidget(QWidget *parent, int width, int height)
    : QWidget(parent)
{
    this->textBrowser = new QTextBrowser(this);
    QFile file("../docs/manual/MDS_manual/index.html");
    QStringList pathList;
    pathList << "../docs/manual/MDS_manual/";
    this->textBrowser->setSearchPaths(pathList);
    
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        this->textBrowser->setHtml(in.readAll());
    }
    this->textBrowser->setMinimumWidth(width);
    this->textBrowser->setMinimumHeight(height);
    this->textBrowser->show();
    this->show();
}