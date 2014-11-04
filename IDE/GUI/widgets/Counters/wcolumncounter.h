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
 * @file wcolumncounter.h
 */


#ifndef WCOLUMNCOUNTER_H
#define WCOLUMNCOUNTER_H

#include <QScrollArea>
#include <QFont>

class WColumnCounterWidget;
class QPlainTextEdit;


/**
 * @brief Counts columns and paints the numbers. Horizontal.
 * @ingroup GUI
 * @class WColumnCounter
 */
class WColumnCounter : public QScrollArea
{
    Q_OBJECT   
    public:
        WColumnCounter(QPlainTextEdit *parent, QFont font, int columns);
        QPlainTextEdit* getTextEdit();
        WColumnCounterWidget* getWidget();
    private slots:
        void change(int value);
    private:
        QPlainTextEdit *parent;
        WColumnCounterWidget *widget;
};


class WColumnCounterWidget : public QWidget
{   
    Q_OBJECT   
    public:
        WColumnCounterWidget(WColumnCounter *parent, QFont font, int columns);
    private:
        WColumnCounter *parent;
        int columns;
        int fontHeight;
        int fontWidth;
    protected:
        void paintEvent(QPaintEvent *);
};


#endif
