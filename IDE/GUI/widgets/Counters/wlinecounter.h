/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file wlinecounter.h
 */


#ifndef WLINECOUNTER_H
#define WLINECOUNTER_H

#include <QPlainTextEdit>
#include <QScrollArea>
#include <QFont>

class WLineCounterWidget;


/**
 * @brief Line Counter base
 * @ingroup GUI
 * @class WLineCounter
 */
class WLineCounter : public QScrollArea
{
    Q_OBJECT   
    public:
        WLineCounter(QPlainTextEdit *parent, bool icons, bool hex, int offset, QFont font);
        QPlainTextEdit* getTextEdit();
        WLineCounterWidget* getWidget();

    public slots:
        void changeFont(QFont font);
        
    private slots:
        void change(int value);

    private:
        QPlainTextEdit *parent;
        WLineCounterWidget *widget;
};

/**
 * @brief Line Counter (hex/dec) with bookmarks, breakpoints and error highlight
 * @ingroup GUI
 * @class WLineCounterWidget
 */
class WLineCounterWidget : public QWidget
{   
    Q_OBJECT   
    public:
        WLineCounterWidget(WLineCounter *parent, bool icons, bool hex, int offset, QFont font);
        void changeHeight();
        void changeFont(QFont font);
        
    private:
        WLineCounter *parent;
        bool icons;
        bool hex;
        int offset;
        int fontHeight;
        int fontWidth;
        //QList<bool> *breakpointList;
        //QList<bool> *errorList;
        //QList<bool> *bookmarkList;

    protected:
        void paintEvent(QPaintEvent *);
};

#endif
