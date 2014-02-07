#include <QtGui>
#include <QHelpEngine>
#include "helpwidget.h"



HelpWidget::HelpWidget(QWidget *parent, int width, int height)
    : QWidget(parent)
{
    QHelpEngine *helpEngine = new QHelpEngine("../docs/manual/MDS_manual.qhc", this);
    helpEngine->setupData();
    this->textBrowser = new QTextBrowser(this);
    QSplitter *helpPanel = new QSplitter(Qt::Horizontal, this);
    helpPanel->insertWidget(0, (QWidget*)(helpEngine->contentWidget()));
    helpPanel->insertWidget(1, this->textBrowser);
    helpPanel->setStretchFactor(1, 1);
    this->textBrowser->show();
    this->show();
}