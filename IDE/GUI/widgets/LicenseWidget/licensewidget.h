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
 * @file helpwidget.h
 */


#ifndef LICENSEWIDGET_H
#define LICENSEWIDGET_H


#include <QDialog>
#include "ui_licensewidget.h"



class LicenseWidget : public QDialog
{
    Q_OBJECT
    public:
        LicenseWidget(QWidget *parent);

    private slots:
        void load();

    private:
        Ui_LicenseWidget ui;

};





#endif