#ifndef WLINECOUNTER_H
#define WLINECOUNTER_H

#include <QTextEdit>
#include <QScrollArea>

class WLineCounterWidget;


class WLineCounter : public QScrollArea
{
    Q_OBJECT   
    public:
        WLineCounter(QTextEdit *parent, bool icons, bool hex, int width);
        QTextEdit* getTextEdit();
        WLineCounterWidget* getWidget();
    private slots:
        void change(int value);
    private:
        QTextEdit *parent;
        WLineCounterWidget *widget;
};


class WLineCounterWidget : public QWidget
{   
    Q_OBJECT   
    public:
        WLineCounterWidget(WLineCounter *parent, bool icons, bool hex, int width);
    //private slots:
    //    void change();
    private:
        WLineCounter *parent;
        bool icons;
        bool hex;
        QList<bool> *breakpointList;
        QList<bool> *errorList;
        QList<bool> *bookmarkList;
    protected:
        void paintEvent(QPaintEvent *);
};

#endif
