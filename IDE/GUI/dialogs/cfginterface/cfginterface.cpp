/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup CfgInterface
 * @file cfginterface.cpp
 */


#include <QtGui>
#include "cfginterface.h"


/**
 * @brief
 * @param
 */
CfgInterface::CfgInterface(QWidget *parent)
    : QWidget(parent)
{
    //qDebug() << "CfgInterface: CfgInterface()";
    //this->setWindowTitle("Config");
    this->count = 0;
    this->tabs = new QStackedWidget(this);
    this->menuList = new QTreeWidget(this);
    this->buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    this->widgetLabel = new QLabel(this);
    this->widgetLabel->setAlignment(Qt::AlignCenter);
    this->lastItem = NULL;
    
    this->menuList->setHeaderHidden(true);
    //this->show();
    this->menuList->move(5,10);
    //this->menuList->setMaximumHeight(this->tabs->height());
    this->menuList->setMinimumWidth(160);
    this->menuList->setMaximumWidth(160);
    this->widgetLabel->move(165,5);
    this->tabs->move(165,30);
    //this->setFixedWidth(300);
    //this->setFixedHeight(300);
    //this->setFixedWidth(this->tabs->width()+this->menuList->width()+10);
    //this->setFixedHeight(this->tabs->height());

    connect(this->menuList, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
        this, SLOT(changeWidget(QTreeWidgetItem*, QTreeWidgetItem*)));

    //qDebug() << "CfgInterface: return CfgInterface()";
}


/**
 * @brief
 */
void CfgInterface::fixSize()
{
    //qDebug() << "CfgInterface: fixSize()";
    this->menuList->setMaximumHeight(this->tabs->height()+this->widgetLabel->height());
    this->menuList->setMinimumHeight(this->tabs->height()+this->widgetLabel->height());
    this->setFixedWidth(this->tabs->width()+this->menuList->width()+10);
    this->setFixedHeight(this->menuList->height()+this->buttonBox->height()+20);
    this->buttonBox->move(this->width() - buttonBox->width()-5, this->height() - buttonBox->height()-5);
    this->widgetLabel->setMaximumWidth(this->tabs->width());
    this->widgetLabel->setMinimumWidth(this->tabs->width());
    this->widgetLabel->setMaximumHeight(25);
    this->widgetLabel->setMinimumHeight(25);
    //qDebug() << "CfgInterface: return fixSize()";
}


/**
 * @brief
 * @param
 * @param
 * @param
 * @param
 */
bool CfgInterface::addWidget(QWidget *widget, QString text, QString tabText, bool child)
{
    //qDebug() << "CfgInterface: addWidget()";
    if (widget == NULL)
    {
        QWidget *plainWidget = new QWidget(this);
        this->tabs->addWidget(plainWidget);
    }
    else
    {
        this->tabs->addWidget(widget);
        if (widget->width() > this->tabs->width())
        {
            this->tabs->setFixedWidth(widget->width());
        }
        if (widget->height() > this->tabs->height())
        {
            this->tabs->setFixedHeight(widget->height());
        }
    }
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
    //qDebug() << "CfgInterface: return addWidget()";
    return true;
}



/**
 * @brief
 * @param
 * @param
 */
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