/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file interfacecfg_editfonts.h
 */


#ifndef INTERFACECFG_EDITFONTS_H
#define INTERFACECFG_EDITFONTS_H

#include <QWidget>
#include "ui_interfacecfg_editfonts.h"


class InterfaceCfg_EditFonts : public QWidget
{
    Q_OBJECT
    public:
        InterfaceCfg_EditFonts(QWidget *parent);
        void save();

    private:
        Ui_InterfaceCfg_EditFonts ui;

    private slots:
        void familyChanged(QString family);
        void sizeChanged(QString pointSize);

};


#endif