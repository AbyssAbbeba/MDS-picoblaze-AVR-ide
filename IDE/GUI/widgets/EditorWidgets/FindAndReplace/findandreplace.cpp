/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup FindAndReplace
 * @file findandreplace.cpp
 */


#include <QtGui>
#include "findandreplace.h"



/**
 * @brief Constructor.
 * @param parent Parent QWidget.
 */
FindAndReplace::FindAndReplace(QWidget *parent, QString query)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.leFind->setText(query);
    ui.leFind->setFocus();
    ui.leFind->installEventFilter(this);
    ui.leReplace->installEventFilter(this);
    connect(ui.btnFindNext, SIGNAL(clicked()), this, SLOT(findNext()));
    connect(ui.btnFindPrev, SIGNAL(clicked()), this, SLOT(findPrevious()));
    connect(ui.btnReplace, SIGNAL(clicked()), this, SLOT(replace()));
    connect(ui.btnReplaceAll, SIGNAL(clicked()), this, SLOT(replaceAll()));
    connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(closeClicked()));
    connect(ui.leFind, SIGNAL(returnPressed()), ui.btnFindNext, SIGNAL(clicked()));
    connect(ui.leReplace, SIGNAL(returnPressed()), ui.btnReplace, SIGNAL(clicked()));
}


bool FindAndReplace::eventFilter(QObject *target, QEvent *event)
{
    if ((target == ui.leFind || target == ui.leReplace) && event->type() == QEvent::KeyPress)
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


void FindAndReplace::setQuery(QString query)
{
    ui.leFind->setText(query);
}


void FindAndReplace::findNext()
{
    if (false == ui.leFind->text().isEmpty())
    {
        emit find(ui.leFind->text(), true, ui.chckCase->isCheckable());
    }
}


void FindAndReplace::findPrevious()
{
    if (false == ui.leFind->text().isEmpty())
    {
        emit find(ui.leFind->text(), false, ui.chckCase->isCheckable());
    }
}


void FindAndReplace::replace()
{
    if (false == ui.leFind->text().isEmpty())
    {
        emit findAndReplace(ui.leFind->text(), ui.leReplace->text(), false, ui.chckCase->isCheckable());
    }
}


void FindAndReplace::replaceAll()
{
    if (false == ui.leFind->text().isEmpty())
    {
        emit findAndReplace(ui.leFind->text(), ui.leReplace->text(), true, ui.chckCase->isCheckable());
    }
}


void FindAndReplace::closeClicked()
{
    emit closeWidget(CodeEditBottomWidget::WFINDANDREPLACE);
}