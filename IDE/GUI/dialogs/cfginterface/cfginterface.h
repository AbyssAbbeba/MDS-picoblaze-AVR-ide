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
        
        QDialogButtonBox *buttonBox;

    private slots:
        void changeWidget(QTreeWidgetItem *curr, QTreeWidgetItem *prev);
        
    private:
        QTreeWidget *menuList;
        QTreeWidgetItem *lastItem;
        QStackedWidget *tabs;
        QLabel *widgetLabel;
        QStringList labelTexts;
        int count;

    //protected:
    //    virtual void paintEvent(QPaintEvent *event);
};

//Q_DECLARE_INTERFACE(CfgInterface, "CfgInterface/1.0")

#endif