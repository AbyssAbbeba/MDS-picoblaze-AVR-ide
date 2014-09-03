// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup AdjSimProcDef
 * @file AdjSimProcDefGui.h
 */
// =============================================================================

#ifndef ADJSIMPROCDEFGUI_H
#define ADJSIMPROCDEFGUI_H

#include <QWidget>

#include "ui_AdjSimProcDefGuiWidget.h"

/**
 * @brief
 * @ingroup AdjSimProcDef
 * @class AdjSimProcDefGui
 */
class AdjSimProcDefGui : public QWidget,
                         private Ui::AdjSimProcDefGuiWidget
{
    Q_OBJECT

    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] parent
         */
        AdjSimProcDefGui ( QWidget * parent = nullptr );
};

#endif // ADJSIMPROCDEFGUI_H
