/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup GUI
 * @file WelcomeWidget.cpp
 */

#include "WelcomeWidget.h"

WelcomeWidget::WelcomeWidget ( QWidget * parent ) : QWidget(parent)
{
    setupUi(this);
    textBrowser->setHtml(":/html/welcome.html");
}

inline void WelcomeWidget::setupConnections()
{
    connect(pushButtonDismiss, SIGNAL(clicked()), SLOT(close()));
}
