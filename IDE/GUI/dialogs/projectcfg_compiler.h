#ifndef PROJECTCFG_COMPILER_H
#define PROJECTCFG_COMPILER_H


#include <QWidget>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include "../project/project.h"


class Project;


/**
 * @brief Project Dialog Config compiler tab
 * @ingroup GUI
 * @class ProjectCfg_Compiler
 */
class ProjectCfg_Compiler : public QWidget
{
    public:
        ProjectCfg_Compiler(QWidget *parentWidget, Project *currProject);

    private:
        Project *project;
        QLabel *lblMain;
        QLabel *lblSymbolTbl;
        QLabel *lblMacroTbl;
        QLabel *lblDbgFile;
        QLabel *lblCodeTree;
        QLabel *lblLstFile;
        QLabel *lblHexFile;
        QLabel *lblBinFile;
        QLabel *lblSRecFile;

        QLineEdit *leMain;

        QCheckBox *chckMain;
        QCheckBox *chckSymbolTbl;
        QCheckBox *chckMacroTbl;
        QCheckBox *chckDbgFile;
        QCheckBox *chckCodeTree;
        QCheckBox *chckLstFile;
        QCheckBox *chckHexFile;
        QCheckBox *chckBinFile;
        QCheckBox *chckSRecFile;
};

#endif