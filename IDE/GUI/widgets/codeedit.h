#ifndef CODEEDIT_H
#define CODEEDIT_H


#include <QPlainTextEdit>
#include <QTabWidget>


//editor kodu s pouzitim QSyntaxHighlight a pocitanim radku
class CodeEdit : public QPlainTextEdit
{
    Q_OBJECT   
    public:
        CodeEdit(QTabWidget *parent, QString wName, QString wPath);
        QString getName();
        QString getPath();
        void setName(QString wName);
        void setPath(QString wPath);
        void setSaved();
        bool isChanged();
        void connectAct();

    public slots:
        void setChanged();

    private:
        QString name;
        QString path;
        bool changed;
        QTabWidget *parentWidget;
};


#endif
