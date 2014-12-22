/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup ExtAppOutput
 * @file extappoutput.cpp
 */


#include <QtGui>
#include "extappoutput.h"

ExtAppOutput::ExtAppOutput(QWidget *parent)
    : QWidget(parent)
{
    //this->setFont(QFont("Andale Mono", 10));
    ui.setupUi(this);
}


QPlainTextEdit* ExtAppOutput::getTextEdit(int tab)
{
    QPlainTextEdit* textEdit = NULL;
    switch (tab)
    {
        case 0:
        {
            textEdit = ui.pteApp1;
            break;
        }
        case 1:
        {
            textEdit = ui.pteApp2;
            break;
        }
        case 2:
        {
            textEdit = ui.pteApp3;
            break;
        }
    }
    return textEdit;
}


void ExtAppOutput::cleanOutput(int tab)
{
    switch (tab)
    {
        case 0:
        {
            ui.pteApp1->clear();
            break;
        }
        case 1:
        {
            ui.pteApp2->clear();
            break;
        }
        case 2:
        {
            ui.pteApp3->clear();
            break;
        }
    }
}


void ExtAppOutput::setActiveTab(int index)
{
    ui.tabWidget->setCurrentIndex(index);
}


void ExtAppOutput::setTabStats(int index, bool enabled, QString name)
{
    ui.tabWidget->setTabEnabled(index, enabled);
    ui.tabWidget->setTabText(index, name);
    ui.tabWidget->update();
}