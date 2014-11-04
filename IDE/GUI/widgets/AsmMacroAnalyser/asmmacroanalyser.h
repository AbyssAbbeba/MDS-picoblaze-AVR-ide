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
#include <QList>
#include <QRegExp>
#include <QString>

class CodeEdit;
class QWidget;
class QMenu;
class QTreeWidgetItem;
class QContextMenuEvent;


class AsmMacroAnalyser: public QTreeWidget
{
    Q_OBJECT
    public:
        AsmMacroAnalyser(QWidget *parent);

    public slots:
        void reload(QList<CodeEdit*> editList);
        void reload(QString mtblFilePath);

    private slots:
        void requestRefresh();
        void macroClickedSlot(QTreeWidgetItem *item, int column);

    private:
        QRegExp macroEditRegExp;
        QRegExp macroFileRegExp;
        QMenu *popupMenu;

    signals:
        void requestCodeEdits();
        void macroClicked(QString file, int line);
        
    protected:
        void contextMenuEvent(QContextMenuEvent *event);
};

#endif
