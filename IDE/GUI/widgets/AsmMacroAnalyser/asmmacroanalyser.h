/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file asmmacroanalyser.h
 */


#ifndef ASMMACROANALYSER_H
#define ASMMACROANALYSER_H

#include <QTreeWidget>
#include <QWidget>
#include <QList>
#include "../Editor/codeedit.h"

class AsmMacroAnalyser: public QTreeWidget
{
    public:
        AsmMacroAnalyser(QWidget *parent);
        void reload(QList<CodeEdit*> editList);
        void reload(QString mtblFilePath);
};

#endif
