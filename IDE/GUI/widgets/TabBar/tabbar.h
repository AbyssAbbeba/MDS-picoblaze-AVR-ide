#ifndef TABBAR_H
#define TABBAR_H


#include <QTabBar>
#include <QList>



class TabBar : public QTabBar
{
    Q_OBJECT
    public:
        TabBar(QWidget *parent);
        void tabAdded();
        void tabRemoved(int index);
        void tabChanged(int index, bool changed);
    private slots:
        void tabStatsMoved(int from, int to);
    private:
        QList<bool> tabStats;
    protected:
        void paintEvent(QPaintEvent *);
};


#endif