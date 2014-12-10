/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file savedlg.h
 */


#ifndef SAVEDLG_H
#define SAVEDLG_H



#include <QWidget>
#include "ui_savedlg.h"


/**
 * @brief Save Dialog
 * @ingroup GUI
 * @class SaveDialog
 * @details Used in creating new project
 */
class SaveDialog : public QDialog
{
    Q_OBJECT
    public:
        SaveDialog(QWidget *parent, QStringList lst, bool reload = false);
        void appendFile(QString path);

    private slots:
        void save();
        void saveAll();
        void no();
        void noAll();
        void returnBack();

    signals:
        void save(QString path);
        void reload(QString path);
        void reloadFinished();

    private:
        Ui_SaveDlg ui;
        
        QPushButton *saveButton;
        QPushButton *saveAllButton;
        QPushButton *discardButton;
        QPushButton *discardAllButton;
        QPushButton *cancelButton;

        QStringList m_files;

        bool m_reload;

    protected:
        void closeEvent(QCloseEvent *e);
        
};

#endif
