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
 * @file hexedit.h
 */


#ifndef HEXEDIT_H
#define HEXEDIT_H


#include <QWidget>


class WLineCounter;
class WColumnCounter;
class QGridLayout;
class QPlainTextEdit;
class QScrollBar;
class QLabel;
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
        void scrollToTop();

    public slots:
        void changeAscii(int position);
        void changeText(int position);
        void moveCursor();
        void moveAsciiCursor();
        void resetHorizontalScrollBar();

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
        int prevBlock;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


#endif
