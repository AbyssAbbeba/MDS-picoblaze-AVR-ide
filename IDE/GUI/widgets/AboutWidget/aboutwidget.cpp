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

    QVBoxLayout *coreLayout = new QVBoxLayout(this);
    this->setLayout(coreLayout);
    QTabWidget *tabWidget = new QTabWidget(this);
    coreLayout->addWidget(tabWidget);

    QWidget *tabWidgetGeneral = new QWidget(this);
    QVBoxLayout *generalLayout = new QVBoxLayout(tabWidgetGeneral);
    QTextEdit *editGeneral = new QTextEdit(tabWidgetGeneral);
    generalLayout->addWidget(editGeneral);
    tabWidgetGeneral->setLayout(generalLayout);
    tabWidget->addTab(tabWidgetGeneral, "General");

    QWidget *tabWidgetOthers = new QWidget(this);
    QVBoxLayout *othersLayout = new QVBoxLayout(tabWidgetOthers);
    QTextEdit *editOthers = new QTextEdit(tabWidgetOthers);
    othersLayout->addWidget(editOthers);
    tabWidgetOthers->setLayout(othersLayout);
    tabWidget->addTab(tabWidgetOthers, "3rd party");

    QFile fileGeneral(":/resources/html/about-general.html");
    if (fileGeneral.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        editGeneral->setHtml(fileGeneral.readAll());
        fileGeneral.close();
    }

    QFile fileOthers(":/resources/html/about-3rd.html");
    if (fileOthers.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        editOthers->setHtml(fileOthers.readAll());
        fileOthers.close();
    }
    
}