#ifndef WELCOMESCR_H
#define WELCOMESCR_H

#include <QWidget>
#include <QListWidget>
#include "ui_welcomescr.h"


class WelcomeScr : public QWidget
{
    Q_OBJECT
    public:
        WelcomeScr(QWidget *parent = 0);
        //~welcome_screen();
        
    private slots:
        //slots
        void listClickSlot(QListWidgetItem* item);
        void exampleSlot();
        void openProjectSlot();
        void newProjectSlot();
        void manualSlot();
        void quickGuideSlot();

    signals:
        void example();
        void openProject();
        void newProject();
        void manual();
        void quickGuide();
        void recentProject(QString path);
        
    private:
        Ui_WelcomeScr ui;
};

#endif // WELCOMESCR_H
