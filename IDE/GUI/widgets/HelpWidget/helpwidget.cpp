/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup HelpWidget
 * @file helpwidget.cpp
 */


#include <QtGui>
#include "helpwidget.h"
#include "../../guicfg/guicfg.h"
#include <QHelpEngine>
#include <QHelpSearchEngine>
#include <QHelpSearchQueryWidget>
#include "helpbrowser.h"



HelpWidget::HelpWidget(QWidget *parent, int width, int height)
    : QWidget(parent)
{
    this->setWindowTitle("Help");
    QVBoxLayout *layout = new QVBoxLayout(this);
    helpEngine = new QHelpEngine(GuiCfg::getInstance().getHelpPath() + "/MDS_manual.qhc", this);
    qDebug() << "HelpWidget: " << GuiCfg::getInstance().getHelpPath() + "/MDS_manual.qhc";
    if (!helpEngine->setupData())
    {
        qDebug() << "HelpWidget: setupData sucks ass";
    }
    this->textBrowser = new HelpBrowser(this);

    QTabWidget *tabs = new QTabWidget(this);

    QWidget *wCnt = new QWidget(this);
    QVBoxLayout *layoutCnt = new QVBoxLayout(wCnt);
    //QLabel *lblContent = new QLabel("Content", wCnt);
    //layoutCnt->addWidget(lblContent);
    layoutCnt->addWidget((QWidget*)(helpEngine->contentWidget()));
    wCnt->setLayout(layoutCnt);

    QWidget *wIndex = new QWidget(this);
    QVBoxLayout *layoutIndex = new QVBoxLayout(wIndex);
    //QLabel *lblIndex = new QLabel("Index", wIndex);
    //layoutIndex->addWidget(lblIndex);
    layoutIndex->addWidget((QWidget*)(helpEngine->indexWidget()));
    wIndex->setLayout(layoutIndex);

    QWidget *wSearch = new QWidget(this);
    QVBoxLayout *layoutSearch = new QVBoxLayout(wSearch);
    //QLabel *lblSearch = new QLabel("Search", wSearch);
    //layoutSearch->addWidget(lblSearch);
    layoutSearch->addWidget((QWidget*)(helpEngine->searchEngine()->queryWidget()));
    layoutSearch->addWidget((QWidget*)(helpEngine->searchEngine()->resultWidget()));
    wSearch->setLayout(layoutSearch);

    tabs->addTab(wCnt, "Content");
    tabs->addTab(wIndex, "Index");
    tabs->addTab(wSearch, "Search");

    QSplitter *helpPanel = new QSplitter(Qt::Horizontal, this);
    helpPanel->insertWidget(0, tabs);
    helpPanel->insertWidget(1, this->textBrowser);
    helpPanel->setStretchFactor(1, 1);
    this->textBrowser->show();
    //helpPanel->setFixedHeight(height);
    //helpPanel->setFixedWidth(width);
    this->showMaximized();
    layout->addWidget(helpPanel);
    this->setLayout(layout);
    //this->resize(width, height);
    //(QWidget*)(helpEngine->contentWidget())->setFixedWidth(width);
    //(QWidget*)(helpEngine->contentWidget())->setFixedHeight(height);
    //this->textBrowser->setFixedHeight(height);
    //this->textBrowser->setFixedWidth(width);

    connect((QWidget*)(helpEngine->contentWidget()),
            SIGNAL(linkActivated(const QUrl &)),
            this->textBrowser,
            SLOT(setSource(const QUrl &))
           );
    connect((QWidget*)(helpEngine->indexWidget()),
            SIGNAL(linkActivated(const QUrl &, QString)),
            this->textBrowser,
            SLOT(setSource(const QUrl &))
           );
    connect((QWidget*)(helpEngine->searchEngine()->queryWidget()),
            SIGNAL(search()),
            this,
            SLOT(querySearch())
           );
    connect((QWidget*)(helpEngine->searchEngine()->resultWidget()),
            SIGNAL(requestShowLink(const QUrl&)),
            this->textBrowser,
            SLOT(setSource(const QUrl &))
           );
    

    helpEngine->searchEngine()->reindexDocumentation();
}


void HelpWidget::querySearch()
{
    qDebug() << "HelpWidget: querySearch()";
    qDebug() << helpEngine->searchEngine()->queryWidget()->query().count();
    for (int i = 0; i < helpEngine->searchEngine()->queryWidget()->query().count(); i++)
    {
        qDebug() << helpEngine->searchEngine()->queryWidget()->query().at(i).wordList;
    }
    helpEngine->searchEngine()->search(helpEngine->searchEngine()->queryWidget()->query());
}