/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup GUI
 * @file WelcomeWidget.h
 */

#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

// Qt header files.
#include <QWidget>
#include <QString>

// UI header files.
#include "ui_WelcomeWidget.h"

/**
 * @brief
 * @ingroup GUI
 * @class WelcomeWidget
 */
class WelcomeWidget : public QWidget,
                      private Ui::WelcomeWidget
{
    Q_OBJECT

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] parent
         */
        WelcomeWidget ( QWidget * parent = nullptr );

    ////    Inline Private Operations    ////
    public:
        /**
         * @brief
         */
        inline void setupConnections();
};

#endif // WELCOMEWIDGET_H
