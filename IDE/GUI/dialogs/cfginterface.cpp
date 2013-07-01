/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Erik Chalupa <xchalu10@stud.fit.vutbr.cz>, (C) 2012
 *
 */


#include <QtGui>
#include "cfginterface.h"


CfgInterface::CfgInterface(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "CfgInterface: CfgInterface()";
    //this->setWindowTitle("Config");
    this->count = 0;
    this->tabs = new QStackedWidget(this);
    this->menuList = new QTreeWidget(this);
    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    this->widgetLabel = new QLabel(this);
    this->widgetLabel->setAlignment(Qt::AlignCenter);
    this->lastItem = NULL;
    
    this->menuList->setHeaderHidden(true);
    this->show();
    this->menuList->move(5,10);
    //this->menuList->setMaximumHeight(this->tabs->height());
    this->menuList->setMaximumWidth(160);
    this->widgetLabel->move(165,5);
    this->tabs->move(165,30);
    //this->setFixedWidth(300);
    //this->setFixedHeight(300);
    //this->setFixedWidth(this->tabs->width()+this->menuList->width()+10);
    //this->setFixedHeight(this->tabs->height());

    connect(this->menuList, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
        this, SLOT(changeWidget(QTreeWidgetItem*, QTreeWidgetItem*)));

    qDebug() << "CfgInterface: return CfgInterface()";
}


void CfgInterface::fixSize()
{
    qDebug() << "CfgInterface: fixSize()";

    this->menuList->setMaximumHeight(this->tabs->height()+this->widgetLabel->height());
    this->menuList->setMinimumHeight(this->tabs->height()+this->widgetLabel->height());
    this->setFixedWidth(this->tabs->width()+this->menuList->width()+10);
    this->setFixedHeight(this->menuList->height()+this->buttonBox->height()+15);
    this->buttonBox->move(this->width() - buttonBox->width()-5, this->height() - buttonBox->height()-5);
    this->widgetLabel->setMaximumWidth(this->tabs->width());
    this->widgetLabel->setMinimumWidth(this->tabs->width());
    this->widgetLabel->setMaximumHeight(25);
    this->widgetLabel->setMinimumHeight(25);
    qDebug() << "CfgInterface: return fixSize()";
}


bool CfgInterface::addWidget(QWidget *widget, QString text, QString tabText, bool child)
{
    qDebug() << "CfgInterface: addWidget()";
    this->tabs->addWidget(widget);
    this->labelTexts.append(tabText);
    QTreeWidgetItem *item;
    if (true == child)
    {
        item = new QTreeWidgetItem(this->lastItem, this->count);
    }
    else
    {
        if (NULL == lastItem)
        {
            item = new QTreeWidgetItem(this->menuList, this->count);
            this->lastItem = item;
        }
        else
        {
            item = new QTreeWidgetItem(this->menuList, this->lastItem, this->count);
            this->lastItem = item;
        }
    }
    item->setText(0, text);
    this->count++;
    return true;
    qDebug() << "CfgInterface: return addWidget()";
}



void CfgInterface::changeWidget(QTreeWidgetItem *curr, QTreeWidgetItem *prev)
{
    this->tabs->setCurrentIndex(curr->type());
    this->widgetLabel->setText(labelTexts[curr->type()]);
}

/*void CfgInterface::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}*/