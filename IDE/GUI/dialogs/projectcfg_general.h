#ifndef PROJECTCFG_GENERAL_H
#define PROJECTCFG_GENERAL_H

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>


class Project;


/**
 * @brief
 * @ingroup GUI
 * @class ProjectCfg_General
 */
class ProjectCfg_General : public QWidget
{
    Q_OBJECT
    public:
        ProjectCfg_General(QWidget *parentWidget, Project *currProject);
    private:
        Project *project;
        QWidget *parent;
        QVBoxLayout *layout;
        QDialogButtonBox *buttonBox;
        QLineEdit *projName;
        QLabel *labelName;
};



#endif
