/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup HelpDockWidget
 * @file helpdockwidget.cpp
 */


#include <QtGui>
#include "helpdockwidget.h"
#include "../../guicfg/guicfg.h"
#include <QHelpEngine>
#include <QHelpSearchEngine>



HelpDockWidget::HelpDockWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle("Help");
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    QHelpEngine *helpEngine = new QHelpEngine(GuiCfg::getInstance().getHelpPath() + "/MDS_manual.qhc", this);
    qDebug() << "HelpDockWidget: " << GuiCfg::getInstance().getHelpPath() + "/MDS_manual.qhc";
    if (!helpEngine->setupData())
    {
        qDebug() << "HelpDockWidget: setupData sucks ass";
    }

    QTabWidget *tabs = new QTabWidget(this);

    QWidget *wCnt = new QWidget(this);
    QVBoxLayout *layoutCnt = new QVBoxLayout(wCnt);
    QLabel *lblContent = new QLabel("Content", wCnt);
    layoutCnt->addWidget(lblContent);
    layoutCnt->addWidget((QWidget*)(helpEngine->contentWidget()));
    wCnt->setLayout(layoutCnt);

    QWidget *wIndex = new QWidget(this);
    QVBoxLayout *layoutIndex = new QVBoxLayout(wIndex);
    QLabel *lblIndex = new QLabel("Index", wIndex);
    layoutIndex->addWidget(lblIndex);
    layoutIndex->addWidget((QWidget*)(helpEngine->indexWidget()));
    wIndex->setLayout(layoutIndex);

    /*QWidget *wSearch = new QWidget(this);
    QVBoxLayout *layoutSearch = new QVBoxLayout(wSearch);
    QLabel *lblSearch = new QLabel("Search", wSearch);
    layoutSearch->addWidget(lblSearch);
    layoutSearch->addWidget((QWidget*)(helpEngine->searchEngine()->queryWidget()));
    layoutSearch->addWidget((QWidget*)(helpEngine->searchEngine()->resultWidget()));
    wSearch->setLayout(layoutSearch);*/

    tabs->addTab(wCnt, "Content");
    tabs->addTab(wIndex, "Index");
    
    layout->addWidget(tabs);
    this->setLayout(layout);
    this->show();
    //tabs->addTab(wSearch, "Search");

    //this->resize(width, height);
    //(QWidget*)(helpEngine->contentWidget())->setFixedWidth(width);
    //(QWidget*)(helpEngine->contentWidget())->setFixedHeight(height);
    //this->textBrowser->setFixedHeight(height);
    //this->textBrowser->setFixedWidth(width);

    connect((QWidget*)(helpEngine->contentWidget()),
            SIGNAL(linkActivated(const QUrl &)),
            this,
            SIGNAL(showHelpContent(const QUrl &))
           );
    connect((QWidget*)(helpEngine->indexWidget()),
            SIGNAL(linkActivated(const QUrl &, QString)),
            this,
            SIGNAL(showHelpContent(const QUrl &))
           );
}