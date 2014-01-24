#ifndef SHOWHIDEWIDGET_H
#define SHOWHIDEWIDGET_H


#include <QWidget>

class ShowHideWidget : public QWidget
{
    Q_OBJECT
    public:
        ShowHideWidget(QWidget *parent);

    signals:
        void show(bool enable);

    protected:
        virtual void paintEvent ( QPaintEvent * event );

    private:
        bool isShown;
};



#endif