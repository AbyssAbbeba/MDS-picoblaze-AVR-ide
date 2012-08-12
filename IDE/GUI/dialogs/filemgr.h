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
    Q_OBJECT
    public:
        FileMgr(QWidget *parentWidget, Project *currProject);


    private slots:
        void deleteFile();
        void newFile();
        void addFile();
        void setMainFile();

    signals:
        void reloadTree();

    private:
        Project *project;
        QWidget *parent;
        QPushButton *deleteBtn;
        QPushButton *addBtn;
        QPushButton *newBtn;
        QPushButton *setMainBtn;
        QListWidget *fileList;
        QGridLayout *layout;
        QVBoxLayout *btnLayout;
	QWidget *btnWidget;

        bool reloadFiles;
};



#endif
