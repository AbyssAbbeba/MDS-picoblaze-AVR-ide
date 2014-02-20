#include "compilewidget.h"
#include <QtGui>


CompileWidget::CompileWidget(QString mainFileName, QString path, LangType langType)
    : QObject(0)
{
    this->mainFileName = mainFileName;
    this->path = path;
    this->langType = langType;
}


void CompileWidget::compile()
{
    //qDebug() << "CompileWidget: compile()";
    //QStringList args;
    //args << " -g -Os -mmcu=atmega8 -c ../" + this->mainFileName;
    QProcess compiler(this);
    compiler.setProcessChannelMode(QProcess::MergedChannels);

    compiler.setWorkingDirectory(path.section('/',0, -2));
    compiler.start("mkdir build");
    compiler.waitForFinished();

    compiler.setWorkingDirectory(this->path.section('/',0, -2) + "/build/");
    qDebug() << compiler.workingDirectory();

    compiler.start("cp ../" + this->mainFileName + " " + this->mainFileName);
    compiler.waitForFinished();

    if (this->langType == LangType::LANG_ASM)
    {
        compiler.start("avra -I /usr/share/avra -l " + mainFileName.section('.',0,-2) + ".lst " + this->mainFileName);
        if (!compiler.waitForFinished())
        {
            emit write(compiler.errorString());
        }
        else
        {
            emit write(compiler.readAll() + "\n\n");
        }
    }
    else if (this->langType == LangType::LANG_C)
    {
        compiler.start("avr-gcc -g -Os -mmcu=atmega8 -c " + this->mainFileName);
        if (!compiler.waitForFinished())
        {
            emit write(compiler.errorString());
            //qDebug() << "CompileWidget: return compile()";
            emit finished();
            return;
        }
        else
        {
            emit write(compiler.readAll() + "\n\n");
        }


        compiler.setProcessChannelMode(QProcess::MergedChannels);
        compiler.start("avr-gcc -g -mmcu=atmega8 -o " + mainFileName + ".elf " + mainFileName + ".o");
        if (!compiler.waitForFinished())
        {
            emit write(compiler.errorString());
            //qDebug() << "CompileWidget: return compile()";
            emit finished();
            return;
        }
        else
        {
            emit write(compiler.readAll() + "\n\n");
        }


        compiler.setProcessChannelMode(QProcess::MergedChannels);
        compiler.start("avr-objcopy -j .text -j .data -O ihex " + mainFileName + ".elf " + mainFileName + ".hex");
        if (!compiler.waitForFinished())
        {
            emit write(compiler.errorString());
        }
        else
        {
            emit write(compiler.readAll() + "\n\n");
        }
    }
    //qDebug() << "CompileWidget: return compile()";
    emit finished();
}