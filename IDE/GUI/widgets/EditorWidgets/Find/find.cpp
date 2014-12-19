/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup Find
 * @file find.cpp
 */


#include <QtGui>
#include "find.h"



/**
 * @brief Constructor.
 * @param parent Parent QWidget.
 */
Find::Find(QWidget *parent, QString query)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.leQuery->setText(query);
    ui.leQuery->installEventFilter(this);
    connect(ui.btnNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(ui.btnPrev, SIGNAL(clicked()), this, SLOT(findPrevious()));
    connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(ui.leQuery, SIGNAL(returnPressed()), ui.btnNext, SIGNAL(clicked()));
}


bool Find::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui.leQuery && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //overit, jestli je vse v poradku (0-9, A-F, sipky)
        if (keyEvent->key() == Qt::Key_Escape)
        {
            closeClicked();
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}


void Find::reloadFocus()
{
    ui.leQuery->setFocus();
}


void Find::setQuery(QString query)
{
    ui.leQuery->setText(query);
}


void Find::findNext()
{
    if (false == ui.leQuery->text().isEmpty())
    {
        emit find(ui.leQuery->text(), true, ui.chckCase->isChecked());
    }
}


void Find::findPrevious()
{
    if (false == ui.leQuery->text().isEmpty())
    {
        emit find(ui.leQuery->text(), false, ui.chckCase->isChecked());
    }
}


void Find::closeClicked()
{
    emit closeWidget(CodeEditBottomWidget::WFIND);
}