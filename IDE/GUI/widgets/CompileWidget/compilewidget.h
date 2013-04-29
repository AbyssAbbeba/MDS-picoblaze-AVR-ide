#ifndef COMPILEWIDGET_H
#define COMPILEWIDGET_H


#include <QWidget>
#include <QPlainTextEdit>
#include "../../enums/enums.h"

class CompileWidget : public QObject
{
    Q_OBJECT
    public:
        CompileWidget(QString mainFileName, QString path, LangType langType);

    public slots:
        void compile();

    signals:
        void finished();
        void write(QString text);

    private:
         QString mainFileName;
         QString path;
         LangType langType;
};


#endif