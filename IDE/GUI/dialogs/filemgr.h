#ifndef FILEMGR_H
#define FILEMGR_H


#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>

class Project;


/**
 * @brief
 * @ingroup GUI
 * @class FileMgr
 */
class FileMgr : public QWidget
{
    //Q_OBJECT
    public:
        FileMgr(QWidget *parentWidget, Project *currProject);
    private:
        Project *project;
        QWidget *parent;
        QPushButton *deleteBut;
        QPushButton *addBut;
        QPushButton *newBut;
        QPushButton *setMainBut;
        QListWidget *fileList;
        QGridLayout *layout;
        QVBoxLayout *btnLayout;
};



#endif
