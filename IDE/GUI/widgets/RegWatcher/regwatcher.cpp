/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup RegWatcher
 * @file regwatcher.cpp
 */


#include <QtGui>
#include "regwatcher.h"
#include "regwatcherwidget.h"
#include "regwatcherdialog.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../../../simulators/MCUSim/MCUSim.h"

RegWatcher::RegWatcher(QWidget *parent, MCUSimControl *controlUnit)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_simControl = controlUnit;

    ui.btnRemove->setEnabled(false);
    ui.btnEdit->setEnabled(false);

    connect(ui.btnRemove,
            SIGNAL(clicked()),
            this,
            SLOT(removeItem())
           );

    connect(ui.btnAdd,
            SIGNAL(clicked()),
            this,
            SLOT(addItem())
           );
    
    connect(ui.btnEdit,
            SIGNAL(clicked()),
            this,
            SLOT(editItem())
           );
}


void RegWatcher::addItem()
{
    if (MCUSim::FAMILY_KCPSM6 == m_simControl->getFamily())
    {
        RegWatcherDialog dlg(this, true);
        connect(&dlg,
                SIGNAL(newItem(QString, int, int, int)),
                this,
                SLOT(createItem(QString, int, int, int))
               );
        dlg.exec();
    }
    else
    {
        RegWatcherDialog dlg(this, false);
        connect(&dlg,
                SIGNAL(newItem(QString, int, int, int)),
                this,
                SLOT(createItem(QString, int, int, int))
               );
        dlg.exec();
    }
}


void RegWatcher::removeItem()
{
    if (ui.lstItems->currentItem() != NULL)
    {
        QListWidgetItem *item = ui.lstItems->takeItem(ui.lstItems->currentRow());
        delete item;
    }
    else if (ui.lstItems->count() > 0)
    {
        QListWidgetItem *item = ui.lstItems->takeItem(0);
        delete item;
    }
    
    if (ui.lstItems->count() == 0)
    {
        ui.btnRemove->setEnabled(false);
        ui.btnEdit->setEnabled(false);
    }
}

void RegWatcher::createItem(QString name, int type, int address, int regbank)
{
    //qDebug() << "Add item";
    QListWidgetItem *item = new QListWidgetItem(ui.lstItems);

    RegWatcherWidget *itemWidget = new RegWatcherWidget(this, m_simControl, name, type, address, regbank);
    itemWidget->show();

    ui.lstItems->setItemWidget(item, itemWidget);
    item->setSizeHint(QSize(itemWidget->width(),itemWidget->height()));
    ui.btnRemove->setEnabled(true);
    ui.btnEdit->setEnabled(true);
}


void RegWatcher::editItem()
{
    if (ui.lstItems->currentItem() != NULL)
    {
        RegWatcherWidget *itemWidget = (RegWatcherWidget*)(ui.lstItems->itemWidget(ui.lstItems->currentItem()));
        QString name = itemWidget->getName();
        int type = itemWidget->getType();
        int address = itemWidget->getAddress();
        int regbank = itemWidget->getRegbank();
        if (MCUSim::FAMILY_KCPSM6 == m_simControl->getFamily())
        {
            RegWatcherDialog dlg(this, true, name, address, type, regbank);
            connect(&dlg,
                    SIGNAL(editItem(QString, int, int, int)),
                    this,
                    SLOT(editCurrentItem(QString, int, int, int))
                );
            dlg.exec();
        }
        else
        {
            RegWatcherDialog dlg(this, false, name, address, type, regbank);
            connect(&dlg,
                    SIGNAL(editItem(QString, int, int, int)),
                    this,
                    SLOT(editCurrentItem(QString, int, int, int))
                );
            dlg.exec();
        }
    }
}


void RegWatcher::editCurrentItem(QString name, int type, int address, int regbank)
{
    if (ui.lstItems->currentItem() != NULL)
    {
        RegWatcherWidget *itemWidget = (RegWatcherWidget*)(ui.lstItems->itemWidget(ui.lstItems->currentItem()));
        itemWidget->setRegbank(regbank);
        itemWidget->setType(type);
        itemWidget->setAddress(address);
        itemWidget->setName(name);
        itemWidget->deviceChanged();
    }
}