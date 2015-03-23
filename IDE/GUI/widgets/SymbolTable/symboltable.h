#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <QWidget>
#include <QtGui>
//#include "linkedlist.h"
#include "ui_symboltable1.h"
#include "ui_symboltable2.h"
#include "ui_symboltable3.h"

namespace Ui {
class SymbolTable;
}
//class LinkedList;

class SymbolTable : public QWidget
{
    Q_OBJECT
   // LinkedList symbolList;
    
public:
    explicit SymbolTable(QWidget *parent = 0);
    ~SymbolTable();

    QTabWidget *tabWidget;
    // variables
    QStandardItemModel *model, *modelSymbol, *modelMacro, *modelString;
    QString loadPath;
    QFile   file;
    QStringList simplifiedLineList;
    int iGlobal;
    int validNum;
    QStringList m_nameBuffer;
    QStringList m_typeBuffer;
    QStringList m_hexBuffer;
    QStringList m_decBuffer;
    QStringList m_usedBuffer;

    QStringList m_macroNameBuffer;
    QStringList m_macroParametersBuffer;
    QStringList m_macroUsedBuffer;
    QStringList m_macroLineBuffer;

    QStringList m_stringNameBuffer;
    QStringList m_stringLineBuffer;
    QStringList m_stringCharactersBuffer;

    struct clicked
    {
        QString clickedName;
        QString clickedHex;
        QString clickedDec;
        QString clickedLine;
    } clickedStruct;

    struct stringStruct
    {
        QString stringName;
        QString stringLine;
        QString stringCharacters;
    } stringDataStruct[4096];

    struct macroStruct
    {
        QString macroName;
        QStringList macroParameters;
        QString macroUsed;
        QString macroLine;
        bool    display;
    } macroDataStruct[4096];

    struct symbolStruct
    {
        QString symbolName;
        QString symbolType;
        QString symbolAttribute;
        QString symbolHex;
        QString symbolDec;
        QString symbolUsed;
        bool    display;
    } symbolDataStruct[4096];
    
private:
    Ui_SymbolTable1 ui1;
    Ui_SymbolTable2 ui2;
    Ui_SymbolTable3 ui3;
    // functions
    QString setPath();
    void connectSignals();
    void loadFile(int purpose);
    void readFileTable( QIODevice *device);
    void readFileMacro( QIODevice *device);
    void readFileString( QIODevice *device);

    void setDataColor(QStandardItem*, int);
    void setDataColorMacro(QStandardItem*, int);
    void setDataColorString(QStandardItem*, int);

    void editTable();
    void editMacro();
    void editString();
    int  updateValidStruct();
    int  updateValidStructMacro();
    int  updateValidStructString();

    // slots
private slots:
    void clicked(QModelIndex index);
    void pushOpen();
    void pushReload();
    void searchChanged(QString);
    void displayPrefChanged();
    void sortPrefChanged();
    // custom menu
    void customMenuRequested(QPoint);
    void copySymbolName();
    void copyLine();
    void copyDecValue();
    void copyHexValue();

    void on_searchEdit_textChanged(const QString &arg1);
    void macroTableClicked();
    void stringTableClicked();
};

#endif // SYMBOLTABLE_H
