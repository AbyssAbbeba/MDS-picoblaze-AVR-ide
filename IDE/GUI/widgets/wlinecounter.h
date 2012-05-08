#ifndef WLINECOUNTER_H
#define WLINECOUNTER_H

#include <QTextEdit>
#include <QScrollArea>

class WLineCounterWidget;


class WLineCounter : public QScrollArea
{
    Q_OBJECT   
    public:
        WLineCounter(QTextEdit *parent, bool icons, int width);
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
        WLineCounterWidget(WLineCounter *parent, bool icons, int width);
    //private slots:
    //    void change();
    private:
        WLineCounter *parent;
        bool icons;
        QList<bool> *breakpointList;
        QList<bool> *errorList;
        QList<bool> *bookmarkList;
    protected:
        void paintEvent(QPaintEvent *);
};

#endif
