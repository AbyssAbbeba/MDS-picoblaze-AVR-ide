/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup AboutWidget
 * @file aboutwidget.cpp
 */


#include <QtGui>
#include "aboutwidget.h"
#include "../../guicfg/guicfg.h"



AboutWidget::AboutWidget(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);
    //this->setLayout(ui.verticalLayout);
    this->setWindowTitle("About MDS");
    this->setMinimumWidth(800);
    this->setMinimumHeight(420);

    QVBoxLayout *coreLayout = new QVBoxLayout(this);
    this->setLayout(coreLayout);
    QTabWidget *tabWidget = new QTabWidget(this);
    coreLayout->addWidget(tabWidget);

    QWidget *tabWidgetGeneral = new QWidget(this);
    QVBoxLayout *generalLayout = new QVBoxLayout(tabWidgetGeneral);
    QTextBrowser *editGeneral = new QTextBrowser(tabWidgetGeneral);
    //editGeneral->setOpenLinks(false);
    editGeneral->setOpenExternalLinks(true);
    editGeneral->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    generalLayout->addWidget(editGeneral);
    tabWidgetGeneral->setLayout(generalLayout);
    tabWidget->addTab(tabWidgetGeneral, "About MDS");

    QWidget *tabWidgetOthers = new QWidget(this);
    QVBoxLayout *othersLayout = new QVBoxLayout(tabWidgetOthers);
    QTextBrowser *editOthers = new QTextBrowser(tabWidgetOthers);
    //editOthers->setOpenLinks(false);
    editOthers->setOpenExternalLinks(true);
    editOthers->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    othersLayout->addWidget(editOthers);
    tabWidgetOthers->setLayout(othersLayout);
    tabWidget->addTab(tabWidgetOthers, "Thanks to");

    //editGeneral->setSource(QUrl("qrc:/resources/html/about-general.html"));
    //editOthers->setSource(QUrl("qrc:/resources/html/about-3rd.html"));

    QFile fileGeneral(":/resources/html/about-general.html");
    if (fileGeneral.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        editGeneral->setHtml(QString::fromUtf8(fileGeneral.readAll()));
        fileGeneral.close();
    }

    QFile fileOthers(":/resources/html/about-3rd.html");
    if (fileOthers.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        editOthers->setHtml(QString::fromUtf8(fileOthers.readAll()));
        fileOthers.close();
    }
    
}