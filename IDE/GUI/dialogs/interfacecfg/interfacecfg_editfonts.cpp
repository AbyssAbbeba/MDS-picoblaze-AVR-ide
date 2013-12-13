#include <QtGui>
#include "interfacecfg_editfonts.h"
#include "../../guicfg/guicfg.h"


InterfaceCfg_EditFonts::InterfaceCfg_EditFonts(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->show();

    QFontDatabase fdb;
    QStringList list = fdb.families();
    foreach (QString family, list)
    {
        //if (QFont(family).styleHint() == QFont::Monospace)
        //{
        //    qDebug() << "added" << family;
            QListWidgetItem * item = new QListWidgetItem(family, this->ui.listFonts);
        //}
    }
    
    for (int i = 6; i < 25; i++)
    {
        QListWidgetItem *item = new QListWidgetItem(QString::number(i), this->ui.listSize);
    }

    this->ui.listFonts->setCurrentItem(this->ui.listFonts->findItems(GuiCfg::getInstance().getEditorFont().family(),
                                                                     Qt::MatchExactly).at(0));
    this->ui.listSize->setCurrentItem(this->ui.listSize->findItems(QString::number(GuiCfg::getInstance().getEditorFont().pointSize()),
                                                                     Qt::MatchExactly).at(0));


    this->ui.lblSample->setFont(GuiCfg::getInstance().getEditorFont());

    connect(this->ui.listFonts, SIGNAL(currentTextChanged(QString)), this, SLOT(familyChanged(QString)));
    connect(this->ui.listSize, SIGNAL(currentTextChanged(QString)), this, SLOT(sizeChanged(QString)));
}


void InterfaceCfg_EditFonts::familyChanged(QString family)
{
    QFont sample(family, this->ui.listSize->currentItem()->text().toInt());
    this->ui.lblSample->setFont(sample);
}


void InterfaceCfg_EditFonts::sizeChanged(QString pointSize)
{
    QFont sample(this->ui.listFonts->currentItem()->text(), pointSize.toInt());
    this->ui.lblSample->setFont(sample);
}


void InterfaceCfg_EditFonts::save()
{
    QFont font(this->ui.listFonts->currentItem()->text(), this->ui.listSize->currentItem()->text().toInt());
    GuiCfg::getInstance().setEditorFont(font);
}