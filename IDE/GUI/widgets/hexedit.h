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
#include <QStatusBar>
#include <QGridLayout>
#include <QByteArray>
#include "wlinecounter.h"
#include "wcolumncounter.h"


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
        HexEdit(QWidget *parent, bool AsciiPanel, int countWidth);
        void setData(int pos, char value);
        QTextEdit* getTextEdit();
        //void setByteArray();

    public slots:
        //void changeText();
        void moveCursor();

    private:
        //void clearByteArray();
        void setData(QByteArray *byteArray);

        WColumnCounter *hexColumnCount;
        WLineCounter *hexLineCount;
        QTextEdit *hexTextEdit;
        QTextEdit *hexAsciiEdit;
        QByteArray *hexByteArray;
        QStatusBar *hexStatusBar;
        QGridLayout *hexLayout;
        
        int prevPosition;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
