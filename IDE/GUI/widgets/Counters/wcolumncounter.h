#ifndef WCOLUMNCOUNTER_H
#define WCOLUMNCOUNTER_H

#include <QTextEdit>
#include <QScrollArea>

class WColumnCounterWidget;


class WColumnCounter : public QScrollArea
{
    Q_OBJECT   
    public:
        WColumnCounter(QTextEdit *parent, int height, int columns);
        QTextEdit* getTextEdit();
        WColumnCounterWidget* getWidget();
    private slots:
        void change(int value);
    private:
        QTextEdit *parent;
        WColumnCounterWidget *widget;
};


class WColumnCounterWidget : public QWidget
{   
    Q_OBJECT   
    public:
        WColumnCounterWidget(WColumnCounter *parent, int height, int columns);
    private:
        WColumnCounter *parent;
        int columns;
    protected:
        void paintEvent(QPaintEvent *);
};


#endif
