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


class VerticalNumberCount;
class HorizontalNumberCount;


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
        HexEdit(QWidget *parent);
        void setData(int pos, char value);
        QTextEdit* getTextEdit();
        //void setByteArray();

    public slots:
        //void changeText();
        void moveCursor();

    private:
        //void clearByteArray();
        void setData(QByteArray *byteArray);

        VerticalNumberCount *hexColumnCount;
        HorizontalNumberCount *hexLineCount;
        QTextEdit *hexTextEdit;
        QByteArray *hexByteArray;
        QStatusBar *hexStatusBar;
        QGridLayout *hexLayout;

    protected:
        bool eventFilter(QObject *target, QEvent *event);
};


class VerticalNumberCount : public QWidget
{
    Q_OBJECT   
    public:
        VerticalNumberCount(QWidget *parent);
    protected:
        void paintEvent(QPaintEvent *);
};


class HorizontalNumberCount : public QWidget
{
    Q_OBJECT   
    public:
        HorizontalNumberCount(HexEdit *parent);
    private:
        HexEdit *parent;
    protected:
        void paintEvent(QPaintEvent *);
};


#endif
