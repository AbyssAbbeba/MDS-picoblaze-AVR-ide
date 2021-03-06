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
 * @file combobox.h
 */


#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QString>

class ComboBox : public QComboBox
{
    Q_OBJECT
    public:
        ComboBox(QWidget *parent, int index);
        int cbIndex;
        bool conflict;
        int previousPin;
        
    signals:
        void changed(ComboBox *box, int index);

    public slots:
        void changedEmit(int index);
};


#endif