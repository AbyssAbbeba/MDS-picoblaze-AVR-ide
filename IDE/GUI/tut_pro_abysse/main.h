#ifndef MAIN_H
#define MAIN_H


#include <QWidget>
#include "ui_untitled.h"

class Xxx : public QWidget
{
    Q_OBJECT
    public:
        Xxx();
        ~Xxx();

    public slots:
        void proved();
        
    private:
        Ui_Form ui;
};


#endif