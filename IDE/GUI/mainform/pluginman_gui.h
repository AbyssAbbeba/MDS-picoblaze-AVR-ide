#ifndef PLUGINMAN_GUI_H
#define PLUGINMAN_GUI_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QtNetwork/QNetworkReply>
#include <QList>
#include <QtXml>
#include "../errordialog/errordlg.h"

typedef struct {
    QString name;
    int uid;
    double version;
    QString description;
    bool active;
    bool bought;
    bool downloaded;
} PluginStruct;

class PluginMan_GUI : public QWidget
{
    Q_OBJECT
    public:
        PluginMan_GUI(QWidget *parent);

    private slots:
        void reloadPlugins();
        void downloadFinished(QNetworkReply *reply);
        void currentChanged(const QString &text);
        
    private:
        void loadPlugins();
        
        QLabel *availableLbl;
        QLabel *usingLbl;
        QLabel *descLbl;
        QListWidget *availableLW;
        QListWidget *usingLW;
        QTextEdit *descTE;
        QPushButton *activateBtn;
        QPushButton *reloadBtn;
        QList<PluginStruct*> pluginStructs;
        int currRevision;
        bool changing;
        PluginStruct *currPlugin;
};

#endif