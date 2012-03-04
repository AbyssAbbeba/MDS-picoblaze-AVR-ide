#ifndef PROJECTDLG_H
#define PROJECTDLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include "../mainform/project.h"

class ProjectDialog : public QDialog
{
    Q_OBJECT
    public:
        ProjectDialog(QMainWindow *dialogParent, ProjectMan *dialogProjectMan);

    private slots:
        void bSetPath();
        void bCreate();
        void bReject();

    private:
        void freeDialog();
        QGridLayout *layout;
        QLineEdit *projName, *projDir;
        QLabel *labelName, *labelDir;
        QPushButton *chooseName;
        QDialogButtonBox *buttonBox;
        QMainWindow *parent;
        ProjectMan *projectMan;
        
};

#endif
