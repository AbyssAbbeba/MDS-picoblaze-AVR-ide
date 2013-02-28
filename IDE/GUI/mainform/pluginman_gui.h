#ifndef PLUGINMAN_GUI_H
#define PLUGINMAN_GUI_H

#include <QWidget>
#include <QLabel>
#include <QListWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QtNetwork/QNetworkReply>

class PluginMan_GUI : public QWidget
{
    Q_OBJECT
    public:
        PluginMan_GUI(QWidget *parent);

    private slots:
        void reloadPlugins();
        void downloadFinished(QNetworkReply *reply);
        
    private:
        QLabel *availableLbl;
        QLabel *usingLbl;
        QLabel *descLbl;
        QListWidget *availableLW;
        QListWidget *usingLW;
        QTextEdit *descTE;
        QPushButton *activateBtn;
        QPushButton *reloadBtn;
};

#endif