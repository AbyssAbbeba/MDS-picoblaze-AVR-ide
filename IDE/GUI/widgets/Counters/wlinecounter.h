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
 * @file wlinecounter.h
 */


#ifndef WLINECOUNTER_H
#define WLINECOUNTER_H

#include <QPlainTextEdit>
#include <QScrollArea>
#include <QFont>
#include <QTextBlock>
#include <QLinearGradient>
#include <QString>

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
        void setBreakpointList(QList<unsigned int> *list);
        void setBookmarkList(QList<unsigned int> *list);
        
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
        QList<unsigned int> *breakpointList;
        QList<unsigned int> *bookmarkList;
        QLinearGradient gradient;
        QPointF point;
        QPen pen;
        QTextBlock lineBlock;
        QRect cursorRect;
        QString decDraw;
        int doneZeros;
        int len;
        int zeros;

    protected:
        void paintEvent(QPaintEvent *);
};

#endif
