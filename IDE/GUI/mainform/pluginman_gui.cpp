/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup PluginMan_GUI
 * @file pluginman_gui.cpp
 */


#include <QtGui>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include "pluginman_gui.h"


PluginMan_GUI::PluginMan_GUI(QWidget *parent)
    : QWidget(parent)
{
    this->currRevision = 0;
    this->changing = false;

    this->setMinimumHeight(425);
    this->setMaximumHeight(425);
    this->setMinimumWidth(650);
    this->setMaximumWidth(650);
    
    availableLbl = new QLabel("Available", this);
    usingLbl = new QLabel("Using", this);
    descLbl = new QLabel("Description", this);
    availableLW = new QListWidget(this);
    usingLW = new QListWidget(this);
    descTE = new QTextEdit(this);
    activateBtn = new QPushButton("<-", this);
    reloadBtn = new QPushButton("Reload", this);
    
    usingLbl->move(10, 10);
    availableLbl->move(385, 10);
    usingLW->move(10, 30);
    availableLW->move(385, 30);
    descLbl->move(10,250);
    descTE->move(10,275);
    descTE->setMaximumHeight(100);
    descTE->setMaximumWidth(630);
    descTE->setMinimumWidth(630);
    descTE->setReadOnly(true);
    activateBtn->move(285,100);
    reloadBtn->move(285, 140);
    this->show();
    activateBtn->setMaximumWidth(reloadBtn->width());
    activateBtn->setMinimumWidth(reloadBtn->width());
    
    connect(reloadBtn, SIGNAL(clicked()), this, SLOT(reloadPlugins()));
    connect(availableLW, SIGNAL(currentTextChanged(const QString &)), this, SLOT(currentChanged(const QString &)));
    connect(usingLW, SIGNAL(currentTextChanged(const QString &)), this, SLOT(currentChanged(const QString &)));
    
    this->loadPlugins();
}


void PluginMan_GUI::reloadPlugins()
{
    QNetworkAccessManager *netAccessMan = new QNetworkAccessManager(this);
    connect(netAccessMan, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
    QNetworkRequest request(QString("http://192.168.1.10/pluginlist"));
    QNetworkReply *reply = netAccessMan->get(request);
}


void PluginMan_GUI::downloadFinished(QNetworkReply *reply)
{
    QFile file(QString("plugins/pluginlist"));
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.close();
    }
    reply->deleteLater();
    this->loadPlugins();
}


void PluginMan_GUI::loadPlugins()
{
    pluginStructs.clear();
    QFile file(QString("plugins/pluginlist.xml"));
    if (!file.open(QIODevice::ReadOnly))
    {
        error(ERR_OPENFILE);
    }
    QDomDocument domDoc("MDSPluginList");
    if (!domDoc.setContent(&file))
    {
        error(ERR_XML_ASSIGN);
    }
    else
    {
        QDomElement xmlRoot = domDoc.documentElement();
        if (xmlRoot.tagName() != "mdspluginlist")
        {
            error(ERR_XML_CONTENT);
        }
        else
        {
            QDomNode xmlNode = xmlRoot.firstChild();
            QDomElement xmlElement;
            while (!xmlNode.isNull())
            {
                xmlElement = xmlNode.toElement();
                if (!xmlElement.isNull())
                {
                    if (xmlElement.tagName() == "plugin")
                    {
                        PluginStruct *plugin = new PluginStruct;
                        plugin->active = false;
                        plugin->bought = false;
                        plugin->downloaded = false;
                        plugin->name = xmlElement.attribute("name", "");
                        plugin->version = xmlElement.attribute("version", "").toDouble();
                        plugin->uid = xmlElement.attribute("uid", "").toInt();
                        QDomNode xmlPluginNode = xmlElement.firstChild();
                        if (!xmlPluginNode.isNull())
                        {
                            QDomElement xmlPluginElement = xmlPluginNode.toElement();
                            if (xmlPluginElement.tagName() == "description")
                            {
                                plugin->description = xmlPluginElement.attribute("desc", "");
                                pluginStructs.append(plugin);
                            }
                            else
                            {
                                error(ERR_XML_CONTENT);
                            }
                        }
                    }
                }
                xmlNode = xmlNode.nextSibling();
            }
        }
    }
    file.close();

    availableLW->clear();
    usingLW->clear();
    for (int i = 0; i < pluginStructs.count(); i++)
    {
        if (pluginStructs.at(i)->active == false)
        {
            availableLW->addItem(pluginStructs.at(i)->name);
        }
        else
        {
            usingLW->addItem(pluginStructs.at(i)->name);
        }
    }
}


void PluginMan_GUI::currentChanged(const QString &text)
{
    for (int i = 0; i < pluginStructs.count(); i++)
    {
        if (text == pluginStructs.at(i)->name)
        {
            currPlugin =  pluginStructs.at(i);
            descTE->setPlainText(currPlugin->description);
            QString version("Version: ");
            version += QString::number(currPlugin->version);
            descTE->append(version);
            if (currPlugin->bought == false)
            {
                activateBtn->setText("Buy");
            }
            else if (currPlugin->downloaded == false)
            {
                activateBtn->setText("Download");
            }
            else if (currPlugin->active == false)
            {
                activateBtn->setText("<-");
            }
            else
            {
                activateBtn->setText("->");
            }
            break;
        }
    }
}