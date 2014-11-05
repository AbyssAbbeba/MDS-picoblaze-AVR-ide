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
 * @file compilewidget.h
 */


#ifndef COMPILEWIDGET_H
#define COMPILEWIDGET_H


#include <QObject>
#include <QString>
#include "../../enums/enums.h"

class CompileWidget : public QObject
{
    Q_OBJECT
    public:
        CompileWidget(QString mainFileName, QString path, LangType langType);

    public slots:
        void compile();

    signals:
        void finished();
        void write(QString text);

    private:
         QString mainFileName;
         QString path;
         LangType langType;
};


#endif