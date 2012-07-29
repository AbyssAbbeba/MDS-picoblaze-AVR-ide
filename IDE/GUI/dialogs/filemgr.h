#ifndef FILEMGR_H
#define FILEMGR_H


#include <QListWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include "projectcfgdlg_core.h"

class Project;
class ProjectConfigDialog_Core;


/**
 * @brief
 * @ingroup GUI
 * @class FileMgr
 */
class FileMgr : public QWidget
{
    Q_OBJECT
    public:
        FileMgr(ProjectConfigDialog_Core *parentWidget, Project *currProject);

        bool reloadFiles;

    private slots:
        void deleteFile();
        void newFile();
        void addFile();
        void setMainFile();

    private:
        Project *project;
        ProjectConfigDialog_Core *parent;
        QPushButton *deleteBtn;
        QPushButton *addBtn;
        QPushButton *newBtn;
        QPushButton *setMainBtn;
        QListWidget *fileList;
        QGridLayout *layout;
        QVBoxLayout *btnLayout;
	QWidget *btnWidget;
};



#endif
