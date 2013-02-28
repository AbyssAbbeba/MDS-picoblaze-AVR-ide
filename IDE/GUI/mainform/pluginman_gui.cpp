#include <QtGui>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include "pluginman_gui.h"

PluginMan_GUI::PluginMan_GUI(QWidget *parent)
    : QWidget(parent)
{
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
    activateBtn->move(285,100);
    reloadBtn->move(285, 140);
    this->show();
    activateBtn->setMaximumWidth(reloadBtn->width());
    activateBtn->setMinimumWidth(reloadBtn->width());
    
    connect(reloadBtn, SIGNAL(clicked()), this, SLOT(reloadPlugins()));
    
    this->reloadPlugins();
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
    QFile file(QString("pluginlist"));
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.close();
    }
    reply->deleteLater();
}