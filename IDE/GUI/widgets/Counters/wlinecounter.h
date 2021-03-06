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

#include <QScrollArea>
#include <QFont>
#include <QTextBlock>
#include <QLinearGradient>
#include <QString>

class WLineCounterWidget;
class QPlainTextEdit;


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
        void manageBreakpointEmit(int line);
        void manageBookmarkEmit(int line);

    private:
        QPlainTextEdit *parent;
        WLineCounterWidget *widget;

    signals:
        void breakpoint(int line);
        void bookmark(int line);
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
        void setBreakpointList(QList<int> *list);
        void setBookmarkList(QList<int> *list);
        
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
        QList<int> *breakpointList;
        QList<int> *bookmarkList;
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
        void mousePressEvent(QMouseEvent *event);

    signals:
        void bookmarkEmit(int line);
        void breakpointEmit(int line);
};

#endif
