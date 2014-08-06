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
 * @file licenseinitwidget.h
 */


#ifndef LICENSEINITWIDGET_H
#define LICENSEINITWIDGET_H


#include <QDialog>
#include "ui_licenseinitwidget.h"



class LicenseInitWidget : public QDialog
{
    Q_OBJECT
    public:
        LicenseInitWidget(QWidget *parent);
        void tryLoad();

    private slots:
        void load();
        void tryAccept();
        void tryReject();

    private:
        Ui_LicenseInitWidget ui;
        bool license;
        QString licensePath;

    //protected:
    //    virtual void closeEvent(QCloseEvent *e);

};





#endif