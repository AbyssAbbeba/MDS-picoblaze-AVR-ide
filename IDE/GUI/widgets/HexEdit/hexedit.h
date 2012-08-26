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
        //void setData(int pos, char value);
        QTextEdit* getTextEdit();
        //void setByteArray();
        void setVal(int pos, char val);
        char getVal(int pos);

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
        void setData(QByteArray *byteArray);

        WColumnCounter *hexColumnCount;
        WLineCounter *hexLineCount;
        QTextEdit *hexTextEdit;
        QTextEdit *hexAsciiEdit;
        QByteArray *hexByteArray;
        //QStatusBar *hexStatusBar;
        QLabel *hexStatusLabel;
        QGridLayout *hexLayout;
        
        int prevPosition;
        int asciiPrevPosition;
        int columns;
        bool changable;
        bool ascii;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
