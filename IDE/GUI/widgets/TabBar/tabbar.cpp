#include <QtGui>
#include "tabbar.h"


TabBar::TabBar(QWidget *parent)
    : QTabBar(parent)
{
    //this->setDrawBase(false);
    connect(this, SIGNAL(tabMoved(int, int)), this, SLOT(tabStatsMoved(int, int)));
}


void TabBar::tabRemoved(int index)
{
    qDebug() << "tabbar: tab removed";
    tabStats.removeAt(index);
}


void TabBar::tabAdded()
{
    qDebug() << "tabbar: tab added";
    tabStats.append(false);
}


void TabBar::tabChanged(int index, bool changed)
{
    if (tabStats.at(index) != changed)
    {
        qDebug() << "tabbar: tab status changed";
        tabStats.replace(index, changed);
    }
}


void TabBar::tabStatsMoved(int from, int to)
{
    tabStats.swap(from, to);
}


void TabBar::paintEvent(QPaintEvent *)
{

    //Q_D(QTabBar);

    //QStyleOptionTabBarBaseV2 optTabBase;

    //this->d_func()->initStyleBaseOption(&optTabBase, this, size());

    QStylePainter p(this);
    QPainter paint;
    QPen pen;
    QPen pen2(QColor::fromRgbF(0.5, 0.2, 0.2, 1));
    pen2.setStyle(Qt::SolidLine);
    QStyleOptionTab tab;
    /*int selected = -1;

    int cut = -1;

    bool rtl = optTabBase.direction == Qt::RightToLeft;

    bool vertical = this->shape();

    QStyleOptionTab cutTab;

    //selected = d->currentIndex;

    //if (d->dragInProgress)

    //    selected = d->pressedIndex;

    for (int i = 0; i < d->tabList.count(); ++i)

         optTabBase.tabBarRect |= tabRect(i);

    optTabBase.selectedTabRect = tabRect(selected);

    if (d->drawBase)

        p.drawPrimitive(QStyle::PE_FrameTabBarBase, optTabBase);*/

    for (int i = 0; i < this->count(); ++i) {


        initStyleOption(&tab, i);

        QLinearGradient gradient(tab.rect.width()/2, tab.rect.bottom(), tab.rect.width()/2, tab.rect.top());
        /*if (d->paintWithOffsets && d->tabList[i].dragOffset != 0) {

            if (vertical) {

                tab.rect.moveTop(tab.rect.y() + d->tabList[i].dragOffset);

            } else {

                tab.rect.moveLeft(tab.rect.x() + d->tabList[i].dragOffset);

            }

        }*/

        /*if (!(tab.state & QStyle::State_Enabled)) {

            tab.palette.setCurrentColorGroup(QPalette::Disabled);

        }*/

        // If this tab is partially obscured, make a note of it so that we can pass the information

        // along when we draw the tear.

        /*if (((!vertical && (!rtl && tab.rect.left() < 0)) || (rtl && tab.rect.right() > width()))

            || (vertical && tab.rect.top() < 0)) {

            cut = i;

            cutTab = tab;

        }*/

        // Don't bother drawing a tab if the entire tab is outside of the visible tab bar.

        /*if ((!vertical && (tab.rect.right() < 0 || tab.rect.left() > width()))

            || (vertical && (tab.rect.bottom() < 0 || tab.rect.top() > height())))

            continue;

        optTabBase.tabBarRect |= tab.rect;

        if (i == selected)

            continue;*/

        //QPalette a(tab.palette);
        /*a.setColor(QPalette::Window, Qt::blue);
        a.setColor(QPalette::Base, Qt::blue);
        a.setColor(QPalette::WindowText, Qt::blue);
        a.setColor(QPalette::AlternateBase, Qt::blue);
        a.setColor(QPalette::ToolTipBase, Qt::blue);
        a.setColor(QPalette::Button, Qt::blue);*/


        //tab.palette.setColor(QPalette::WindowText, Qt::black);
        //tab.palette.setColor(QPalette::Base, Qt::blue);
        //tab.palette.setColor(QPalette::Window, Qt::blue);
        p.drawControl(QStyle::CE_TabBarTab, tab);
        paint.begin(this);
        gradient.setColorAt(0, QColor::fromRgbF(1, 1, 1, 0));
        gradient.setColorAt(1, QColor::fromRgbF(1, 1, 1, 0));
        int index = tab.text.lastIndexOf(".");
        if (index > 0)
        {
            QString text(tab.text.right(tab.text.size() - index));
            if (text == ".h")
            {
                gradient.setColorAt(0, QColor::fromRgbF(0.8, 0.3, 0, 0.35));
                gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
            }
            else if (text == ".cpp" || text == ".cxx" || text == ".cc")
            {
                gradient.setColorAt(0, QColor::fromRgbF(0.5, 0, 0.5, 0.35));
                gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
            }
            else if (text == ".c")
            {
                gradient.setColorAt(0, QColor::fromRgbF(0, 0, 1, 0.35));
                gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
            }
            else if (text == ".asm")
            {
                gradient.setColorAt(0, QColor::fromRgbF(1, 1, 0, 0.35));
                gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
            }
        }
        QBrush brush(gradient);
        paint.setBrush(brush);
        pen.setStyle(Qt::NoPen);
        paint.setPen(pen);
        paint.drawRect(tab.rect);
        pen.setStyle(Qt::SolidLine);
        paint.setPen(pen);
        if (i != this->currentIndex())
        {
            QRect rect(tab.rect);
            rect.setY(rect.y()+2);
            if (tabStats.at(i) == true)
            {
                paint.setPen(pen2);
                paint.drawText(rect, Qt::AlignCenter, tab.text);
                paint.setPen(pen);
            }
            else
            {
                paint.drawText(rect, Qt::AlignCenter, tab.text);
            }
        }
        else
        {
            if (tabStats.at(i) == true)
            {
                paint.setPen(pen2);
                paint.drawText(tab.rect, Qt::AlignCenter, tab.text);
                paint.setPen(pen);
            }
            else
            {
                paint.drawText(tab.rect, Qt::AlignCenter, tab.text);
            }
        }
        paint.end();
        

    }

    // Draw the selected tab last to get it "on top"

    /*if (selected >= 0) {

        QStyleOptionTabV3 tab;

        initStyleOption(&tab, selected);

        if (d->paintWithOffsets && d->tabList[selected].dragOffset != 0) {

            if (vertical)

                tab.rect.moveTop(tab.rect.y() + d->tabList[selected].dragOffset);

            else

                tab.rect.moveLeft(tab.rect.x() + d->tabList[selected].dragOffset);

        }

        if (!d->dragInProgress)

            p.drawControl(QStyle::CE_TabBarTab, tab);

        else {

            int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);

            d->movingTab->setGeometry(tab.rect.adjusted(-taboverlap, 0, taboverlap, 0));

        }

    }

    // Only draw the tear indicator if necessary. Most of the time we don't need too.

    if (d->leftB->isVisible() && cut >= 0) {

        cutTab.rect = rect();

        cutTab.rect = style()->subElementRect(QStyle::SE_TabBarTearIndicator, &cutTab, this);

        p.drawPrimitive(QStyle::PE_IndicatorTabTear, cutTab);*/

    //}
    

}