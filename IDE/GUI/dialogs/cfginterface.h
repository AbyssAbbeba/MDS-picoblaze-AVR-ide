/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 * @ingroup GUI
 * @file cfginterface.h
 */


#ifndef CFGINTERFACE_H
#define CFGINTERFACE_H


#include <QWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QStringList>
#include <QStackedWidget>
#include <QDialogButtonBox>


class CfgInterface : public QWidget
{
    Q_OBJECT
    public:
        CfgInterface(QWidget *parent);
        bool addWidget(QWidget *widget, QString text, QString tabText, bool child = false);
        void fixSize();

    private slots:
        void changeWidget(QTreeWidgetItem *curr, QTreeWidgetItem *prev);
        
    private:
        QTreeWidget *menuList;
        QTreeWidgetItem *lastItem;
        QStackedWidget *tabs;
        QDialogButtonBox *buttonBox;
        QLabel *widgetLabel;
        QStringList labelTexts;
        int count;

    //protected:
    //    virtual void paintEvent(QPaintEvent *event);
};

//Q_DECLARE_INTERFACE(CfgInterface, "CfgInterface/1.0")

#endif