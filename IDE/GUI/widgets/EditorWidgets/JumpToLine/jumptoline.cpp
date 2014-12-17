/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup JumpToLine
 * @file jumptoline.cpp
 */


#include <QtGui>
#include "jumptoline.h"



/**
 * @brief Constructor.
 * @param parent Parent QWidget.
 */
JumpToLine::JumpToLine(QWidget *parent, int currLine, int maxLines)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.spinLine->setMaximum(maxLines);
    ui.spinLine->setValue(currLine);
    ui.spinLine->setFocus();
    ui.spinLine->installEventFilter(this);
    //this->installEventFilter(this);
    connect(ui.btnJump, SIGNAL(clicked()), this, SLOT(emitJump()));
    connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(closeClicked()));
    //connect(ui.spinLine, SIGNAL(returnPressed()), ui.btnJump, SIGNAL(clicked()));
}

bool JumpToLine::eventFilter(QObject *target, QEvent *event)
{
    if (target == ui.spinLine && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //overit, jestli je vse v poradku (0-9, A-F, sipky)
        if ((keyEvent->key() == Qt::Key_Enter ||
            keyEvent->key() == Qt::Key_Return))
        {
            emitJump();
            return true;
        }
        else if (keyEvent->key() == Qt::Key_Escape)
        {
            closeClicked();
            return true;
        }
    }
    return QWidget::eventFilter(target, event);
}


void JumpToLine::emitJump()
{
    emit jmpToLine(ui.spinLine->value());
}


void  JumpToLine::closeClicked()
{
    emit closeWidget(CodeEditBottomWidget::WJUMPTOLINE);
}