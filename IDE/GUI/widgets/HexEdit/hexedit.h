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
 * @file hexedit.h
 */



#ifndef HEXEDIT_H
#define HEXEDIT_H


#include <QTextEdit>
//#include <QStatusBar>
#include <QGridLayout>
#include <QByteArray>
#include <QLabel>
#include <QScrollBar>
#include "../Counters/wlinecounter.h"
#include "../Counters/wcolumncounter.h"


class WLineCounter;
class WColumnCounter;



//hex editor (modifikace QTextEdit) s pouzitim QStatusBar
/**
 * @brief
 * @ingroup GUI
 * @class HexEdit
 */
class HexEdit : public QWidget
{
    Q_OBJECT   
    public:
        HexEdit(QWidget *parent, bool AsciiPanel, int countSize, int columns);
        ~HexEdit();
        //void setData(int pos, char value);
        QPlainTextEdit* getTextEdit();
        //void setByteArray();
        void setVal(int pos, unsigned char val);
        unsigned char getVal(int pos);
        void setReadOnly(bool readonly);
        void fixHeight();
        void setHighlighted(int pos, bool highlight);
        QScrollBar* verticalScrollBar();
        QScrollBar* horizontalScrollBar();
        void unhighlight();

    public slots:
        void changeAscii(int position);
        void changeText(int position);
        void moveCursor();
        void moveAsciiCursor();

    signals:
        void textChanged(int position);
        void asciiChanged(int position);

    private:
        //void clearByteArray();
        void setData(QList<unsigned char> *array);

        WColumnCounter *hexColumnCount;
        WLineCounter *hexLineCount;
        QPlainTextEdit *hexTextEdit;
        QPlainTextEdit *hexAsciiEdit;
        QList<unsigned char> *hexByteArray;
        //QStatusBar *hexStatusBar;
        QLabel *hexStatusLabel;
        QGridLayout *hexLayout;
        
        int prevPosition;
        int asciiPrevPosition;
        int columns;
        int fontSize;
        bool changable;
        bool ascii;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
