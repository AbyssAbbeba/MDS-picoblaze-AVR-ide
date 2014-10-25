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

// Qt header files.
#include <QFile>

WelcomeWidget::WelcomeWidget ( QWidget * parent ) : QWidget(parent)
{
    setupUi(this);
    setupConnections();

    textBrowser->setOpenExternalLinks(true);

    QFile file(":/resources/html/welcome.html");
    if ( true == file.open ( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        textBrowser->setHtml ( QString::fromUtf8 ( file.readAll() ) );
        file.close();
    }
}

inline void WelcomeWidget::setupConnections()
{
    connect(pushButtonDismiss,  SIGNAL(clicked()), SLOT(close()));
    connect(pushButtonManual,   SIGNAL(clicked()), SLOT(close()));
    connect(pushButtonTutorial, SIGNAL(clicked()), SLOT(close()));

    connect(pushButtonTutorial, SIGNAL(clicked()), SIGNAL(tutorial()));
    connect(pushButtonManual,   SIGNAL(clicked()), SIGNAL(manual()));
}
