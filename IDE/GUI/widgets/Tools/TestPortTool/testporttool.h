#ifndef TESTPORTTOOL_H
#define TESTPORTTOOL_H

#include <QWidget>
#include <QtGui>

namespace Ui {
class TestPortTool;
}

class TestPortTool : public QWidget
{
    Q_OBJECT
    
public:
    explicit TestPortTool(QWidget *parent = 0);
    ~TestPortTool();
    bool status;
    QStringList fileBuffer;
    QTextCursor * cursor;
    QTextStream * outFileText;


private slots:
    void activatePushed();
    void cancelApp();
    void readFileClicked();
    void outFileClicked();
    void inputEvent();

private:
    QStringList address, data;
    QFile inFile,outFile;
    QString loadPath;
    Ui::TestPortTool *ui;

    void fillBuffer();
    int randInt(int,int);
};

#endif // TESTPORTTOOL_H
