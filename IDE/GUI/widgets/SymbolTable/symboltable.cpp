#include "symboltable.h"
#include "ui_symboltable1.h"
#include "ui_symboltable2.h"
#include "ui_symboltable3.h"


#include <QtGui>
#include <QHeaderView>

SymbolTable::SymbolTable(QWidget *parent) :
    QWidget(parent)
    //ui(new Ui::SymbolTable),
{
    //ui->setupUi(this);
    // ui parameters
    // main tab widget with three tabs , each for one file
    tabWidget = new QTabWidget(this);
    tabWidget->setMinimumSize(817,550);

    QWidget *symTabWidget1 = new QWidget(tabWidget);
    QWidget *symTabWidget2 = new QWidget(tabWidget);
    QWidget *symTabWidget3 = new QWidget(tabWidget);

    // setup ui to tabsSTART
    ui1.setupUi(symTabWidget1);
    ui2.setupUi(symTabWidget2);
    ui3.setupUi(symTabWidget3);

    tabWidget->addTab(symTabWidget1, "Symbol Table");
    tabWidget->addTab(symTabWidget2, "Macro Table");
    tabWidget->addTab(symTabWidget3, "String Table");


    QHBoxLayout *Hbox = new QHBoxLayout(this);
    Hbox->addWidget(tabWidget);
    this->setLayout(Hbox);

    this->setWindowTitle("Symbol table");


    modelSymbol = new QStandardItemModel(0,5,this); //2 Rows and 3 Columns
   // model->set
    modelSymbol->setHorizontalHeaderItem(0, new QStandardItem(QString("Symbol")));
    modelSymbol->setHorizontalHeaderItem(1, new QStandardItem(QString("Type")));
    modelSymbol->setHorizontalHeaderItem(2, new QStandardItem(QString("Hex")));
    modelSymbol->setHorizontalHeaderItem(3, new QStandardItem(QString("Dec")));
    modelSymbol->setHorizontalHeaderItem(4, new QStandardItem(QString("Used")));

    modelMacro = new QStandardItemModel(0,4,this); //2 Rows and 3 Columns
   // model->set
    modelMacro->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    modelMacro->setHorizontalHeaderItem(1, new QStandardItem(QString("Line")));
    modelMacro->setHorizontalHeaderItem(2, new QStandardItem(QString("Used")));
    modelMacro->setHorizontalHeaderItem(3, new QStandardItem(QString("Parameters")));

    modelString = new QStandardItemModel(0,3,this); //2 Rows and 3 Columns
   // model->set
    modelString->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    modelString->setHorizontalHeaderItem(1, new QStandardItem(QString("Line")));
    modelString->setHorizontalHeaderItem(2, new QStandardItem(QString("Characters")));

    //QStandardItem *firstRow = new QStandardItem(QString("ColumnValue"));
    connect(ui1.listDisplay,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(clicked(QModelIndex)));
    connect(ui2.listDisplay,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(clicked(QModelIndex)));
    connect(ui3.listDisplay,
            SIGNAL(clicked(QModelIndex)),
            this,
            SLOT(clicked(QModelIndex)));

    // set custom menu
    ui1.listDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui1.listDisplay,
            SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint))
            );
    ui2.listDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui2.listDisplay,
            SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint))
            );
    ui3.listDisplay->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui3.listDisplay,
            SIGNAL(customContextMenuRequested(QPoint)),
            SLOT(customMenuRequested(QPoint))
            );

    //model.
    ui1.listDisplay->setAlternatingRowColors ( true );
    ui1.listDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui1.listDisplay->setModel(modelSymbol);
    ui2.listDisplay->setAlternatingRowColors ( true );
    ui2.listDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui2.listDisplay->setModel(modelMacro);
    ui3.listDisplay->setAlternatingRowColors ( true );
    ui3.listDisplay->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui3.listDisplay->setModel(modelString);

    connectSignals();
    //QClipboard::setText("fantomas",QClipboard::Clipboard);
    //clipboard().setText("fantomas",QClipboard::Clipboard);
}

void SymbolTable::setPath(QString)
{
}

int  SymbolTable::updateValidStructString()
{
    m_stringNameBuffer.clear();
    m_stringLineBuffer.clear();
    m_stringCharactersBuffer.clear();

    // iterate structure while name is not empty
    int p = 0;
    while ( stringDataStruct[p].stringName.isEmpty() == false )
    {
        m_stringNameBuffer << stringDataStruct[p].stringName;
        m_stringLineBuffer << stringDataStruct[p].stringLine;
        m_stringCharactersBuffer << stringDataStruct[p].stringCharacters;
        p++;
    }
    return m_stringNameBuffer.size();
}
int  SymbolTable::updateValidStructMacro()
{
    //QString list m_macroNameBuffer;
    //QString macroParametersBuffer;
   // QString macroUsedBuffer;
    //QString macroLineBuffer;
    m_macroNameBuffer.clear();
    m_macroParametersBuffer.clear();
    m_macroUsedBuffer.clear();
    m_macroLineBuffer.clear();

    // iterate structure while name is not empty
    int p = 0;
    while ( macroDataStruct[p].macroName.isEmpty() == false )
    {
        QString parameters = "";
        // first examine use and unused
        // this happens when both checkboxes are selected
        if ( ui2.checkUnusedMac->isChecked() == false && ui2.checkUsedMac->isChecked() == false)
            return 0;

        if ( ui2.checkUnusedMac->isChecked() == false )//|| ui1.checkUsedSym.isChecked() == false
            {
                if ( macroDataStruct[p].macroUsed == "NO")
                {
                   p++;
                   continue;
                }
            }
        if ( ui2.checkUsedMac->isChecked() == false )//||
            {
                if ( macroDataStruct[p].macroUsed == "YES")
                {
                   p++;
                   continue;
                }
            }

        // parameters
        if ( ui2.checkParam->isChecked() == false && macroDataStruct[p].macroParameters.isEmpty() == false )//||
            {
               p++;
               continue;
            }

        // struct is examined and is valid, lets put her into a buffer
        m_macroNameBuffer       << macroDataStruct[p].macroName;

        // convert qstringlist to qstring


        for ( int i = 0; i < macroDataStruct[p].macroParameters.size(); i++ )
        {
            parameters.append(macroDataStruct[p].macroParameters.at(i) + ", ");
        }
        m_macroParametersBuffer << parameters;
        parameters.clear();
        qDebug() << m_macroParametersBuffer << "macro parameters buffer";

        m_macroUsedBuffer        << macroDataStruct[p].macroUsed;
        m_macroLineBuffer       << macroDataStruct[p].macroLine;
        p++;
    }
    return m_macroNameBuffer.size();
}

int SymbolTable::updateValidStruct()
{
    // read valid data from struct(symboldatastruct) and create another with valid values acording to the prefered checkboxes
    // display prefs, used and unused values
    m_nameBuffer.clear();
    m_typeBuffer.clear();
    m_hexBuffer.clear();
    m_decBuffer.clear();
    m_usedBuffer.clear();

    // iterate structure while name is not empty
    int p = 0;
    while ( symbolDataStruct[p].symbolName.isEmpty() == false )
    {
        // first examine use and unused
        // this happens when both checkboxes are selected
        if ( ui1.checkUnusedSym->isChecked() == false && ui1.checkUsedSym->isChecked() == false)
            return 0;

        if ( ui1.checkUnusedSym->isChecked() == false )//|| ui1.checkUsedSym.isChecked() == false
            {
                if ( symbolDataStruct[p].symbolUsed == "NO")
                {
                   p++;
                   continue;
                }
            }
        if ( ui1.checkUsedSym->isChecked() == false )//||
            {
                if ( symbolDataStruct[p].symbolUsed == "YES")
                {
                   p++;
                   continue;
                }
            }

        // NOW EXAMINE PORT box
        if ( ui1.checkPort->isChecked() == false && symbolDataStruct[p].symbolType == "PORT")
        {
           p++;
           continue;
        }
        else if ( ui1.checkPort->isChecked() == false && symbolDataStruct[p].symbolType == "PORTIN")
        {
           p++;
           continue;
        }
        else if ( ui1.checkPort->isChecked() == false && symbolDataStruct[p].symbolType == "PORTOUT")
        {
           p++;
           continue;
        }

        // now examine Data
        if ( ui1.checkData->isChecked() == false && symbolDataStruct[p].symbolType == "DATA")
        {
           p++;
           continue;
        }
        // label, expression, register, number
        if ( ui1.checkLabel->isChecked() == false && symbolDataStruct[p].symbolType == "LABEL")
        {
           p++;
           continue;
        }
        if ( ui1.checkExpresion->isChecked() == false && symbolDataStruct[p].symbolType == "EXPR.")
        {
           p++;
           continue;
        }
        if ( ui1.checkRegister->isChecked() == false && symbolDataStruct[p].symbolType == "REG.")
        {
           p++;
           continue;
        }
        if ( ui1.checkNumber->isChecked() == false && symbolDataStruct[p].symbolType == "NUMBER")
        {
           p++;
           continue;
        }

        // struct is examined and is valid, lets put her into a buffer
        m_nameBuffer    << symbolDataStruct[p].symbolName;
        m_typeBuffer    << symbolDataStruct[p].symbolType;
        m_hexBuffer     << symbolDataStruct[p].symbolHex;
        m_decBuffer     << symbolDataStruct[p].symbolDec;
        m_usedBuffer    << symbolDataStruct[p].symbolUsed;
        p++;
    }
    // return number of valid structures to create rows and set model for input data
   // qDebug() << m_nameBuffer;
   // qDebug() << m_typeBuffer;
   // qDebug() << m_hexBuffer;
   // qDebug() << m_decBuffer;
   // qDebug() << m_usedBuffer;

    return m_nameBuffer.size();
}

void SymbolTable::editString()
{
    QStringList buffer;

    // main update function
    // updates struture to displaz with actual values and returns row number
    int rows;
    int cols = 3;
    rows = updateValidStructString();
    qDebug() << rows << "rows";

    modelString->setRowCount(rows);

    for( int r=0; r<rows; r++ )
    {
        // fill buffer with buffer >]
        buffer.clear();
        buffer << m_stringNameBuffer[r];
        buffer << m_stringLineBuffer[r];
        buffer << m_stringCharactersBuffer[r];

        for( int c=0; c<cols; c++)
        {
            QStandardItem* item = new QStandardItem(buffer[c]);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            if ( c == 0)
            {
                item->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
            }
            else if ( c == 2)
            {
                item->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
            }
            else
            {
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
            }
            setDataColorString(item,c);
            modelString->setItem(r, c, item);
          //  }
        }
    }
    ui3.listDisplay->setModel(modelString);
}

void SymbolTable::editMacro()
{
    QStringList buffer;

    // main update function
    // updates struture to displaz with actual values and returns row number
    int rows;
    int cols = 4;
    rows = updateValidStructMacro();
    qDebug() << rows << "rows";

    modelMacro->setRowCount(rows);

    for( int r=0; r<rows; r++ )
    {
        // fill buffer with buffer >]
        buffer.clear();
        buffer << m_macroNameBuffer[r];
        buffer << m_macroLineBuffer[r];
        buffer << m_macroUsedBuffer[r];
        buffer << m_macroParametersBuffer[r];

        for( int c=0; c<cols; c++)
        {
            QStandardItem* item = new QStandardItem(buffer[c]);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            if ( c == 0)
            {
                item->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
            }
            else if ( c == 3)
            {
                item->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
            }
            else
            {
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
            }
            setDataColorMacro(item,c);
            modelMacro->setItem(r, c, item);
          //  }
        }
    }
    ui2.listDisplay->setModel(modelMacro);

}

void SymbolTable::editTable()
{
    QStringList buffer;

    // main update function
    // updates struture to displaz with actual values and returns row number
    int rows;
    int cols = 5;
    rows = updateValidStruct();

   // set rows of valid structure
    modelSymbol->setRowCount(rows);

    for( int r=0; r<rows; r++ )
    {
        // fill buffer with buffer >]
        buffer.clear();
        buffer << m_nameBuffer[r];
        buffer << m_typeBuffer[r];
        buffer << m_hexBuffer[r];
        buffer << m_decBuffer[r];
        buffer << m_usedBuffer[r];

        for( int c=0; c<cols; c++)
        {
            QStandardItem* item = new QStandardItem(buffer[c]);
            item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            if ( c == 0)
            {
                item->setData(Qt::AlignLeft, Qt::TextAlignmentRole);
            }
            else
            {
                item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
            }
            setDataColor(item,c);
            modelSymbol->setItem(r, c, item);
          //  }
        }
    }
    ui1.listDisplay->setModel(modelSymbol);
}

//When you receive the signal, you call itemFromIndex() on the given model index to get a pointer to the item:

void SymbolTable::clicked(QModelIndex index)
{
    // To actually capture the selection you use the item view's selection model
    //    to get a list of indices. Given that you have a QTableView * called
    //   view you get the selection this way:
    QAbstractItemModel * model;
    QItemSelectionModel * selection;

    if ( tabWidget->currentIndex() == 0 )
    {
         model = ui1.listDisplay->model();
         selection = ui1.listDisplay->selectionModel();
         QModelIndexList indexes = selection->selectedIndexes();
        //Then loop through the index list calling model->data(index) on each index.
            //    Convert the data to a string if it isn't already and concatenate each string together.
         clickedStruct.clickedLine.clear();
         for ( int i = 0; i < indexes.size(); i++)
         {
             clickedStruct.clickedLine.append( model->data(indexes.at(i)).toString() + "\t" );
         }
         clickedStruct.clickedDec  = model->data(indexes.at(3)).toString();
         clickedStruct.clickedHex  = model->data(indexes.at(2)).toString();
         clickedStruct.clickedName = model->data(indexes.at(0)).toString();
    }
    if ( tabWidget->currentIndex() == 1 )
    {
         model = ui2.listDisplay->model();
         selection = ui2.listDisplay->selectionModel();
         QModelIndexList indexes = selection->selectedIndexes();
        //Then loop through the index list calling model->data(index) on each index.
            //    Convert the data to a string if it isn't already and concatenate each string together.
         clickedStruct.clickedLine.clear();
         for ( int i = 0; i < indexes.size(); i++)
         {
             clickedStruct.clickedLine.append( model->data(indexes.at(i)).toString() + "\t" );
         }
         clickedStruct.clickedName = model->data(indexes.at(0)).toString();
    }
    if ( tabWidget->currentIndex() == 2 )
    {
         model = ui3.listDisplay->model();
         selection = ui3.listDisplay->selectionModel();
         QModelIndexList indexes = selection->selectedIndexes();
        //Then loop through the index list calling model->data(index) on each index.
            //    Convert the data to a string if it isn't already and concatenate each string together.
         clickedStruct.clickedLine.clear();
         for ( int i = 0; i < indexes.size(); i++)
         {
             clickedStruct.clickedLine.append( model->data(indexes.at(i)).toString() + "\t" );
         }
         clickedStruct.clickedName = model->data(indexes.at(0)).toString();
    }
}

void SymbolTable::setDataColorString(QStandardItem * itemCopy, int c)
{
    switch ( c )
    {
        case 0:
            itemCopy->setForeground(QBrush(QColor(Qt::black))); break;
        case 1: // line
            itemCopy->setForeground(QBrush(QColor(Qt::darkCyan))); break;
        case 2: // string
            itemCopy->setForeground(QBrush(QColor(Qt::darkBlue))); break;
    }
}
void SymbolTable::setDataColorMacro(QStandardItem * itemCopy, int c)
{
    switch ( c )
    {
        case 0:
            itemCopy->setForeground(QBrush(QColor(Qt::black))); break;
        case 1: // line number
            itemCopy->setForeground(QBrush(QColor(Qt::darkCyan))); break;
        case 2: // used
        {
            QString data = itemCopy->text();
            if ( data == "YES")
            {
                itemCopy->setForeground(QBrush(QColor(Qt::darkGreen)));
            }
            else
            {
                itemCopy->setForeground(QBrush(QColor(Qt::red)));
            }
            break;
        }
        case 3:
            //itemCopy->setForeground(QBrush(QColor(Qt::darkRed))); break;
            itemCopy->setForeground(QBrush(QColor(Qt::darkGreen))); break;
    }
}

void SymbolTable::setDataColor(QStandardItem  * itemCopy, int c)
{
    switch ( c )
    {
        case 0:
            itemCopy->setForeground(QBrush(QColor(Qt::black))); break;
        case 1: // type
            {
                QString data = itemCopy->text();
                if ( data == "PORT" || data == "PORTOUT" || data == "PORTIN" )
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkGreen)));
                }
                else if ( data == "LABEL")
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkYellow)));
                }
                else if ( data == "REG.")
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkRed)));
                }
                else if ( data == "DATA")
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkBlue)));
                }
                else if ( data == "EXPRESION")
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::red)));
                }
                else if ( data == "NUMBER")
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkBlue)));
                }
                else
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkCyan)));
                }
                //QString data = itemCopy->data().toString();
                //qDebug() << data << "datatoColor";
                break;
            }
        case 2:
            itemCopy->setForeground(QBrush(QColor(Qt::black))); break;
        case 3:
            itemCopy->setForeground(QBrush(QColor(Qt::black))); break;
        case 4: // used
            {
                QString data = itemCopy->text();
                if ( data == "YES")
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::darkGreen)));
                }
                else
                {
                    itemCopy->setForeground(QBrush(QColor(Qt::red)));
                }
                break;
            }
    }
}

void SymbolTable::loadFile(int purpose)
{
    // check what kind of file is needed
    // 1 stbl, 2 mactbl, 3 stringtbl
    loadPath.clear();
    int p = 0;

    switch (purpose)
    {
        case 1:
            while ( symbolDataStruct[p].symbolName.isEmpty() == false )
            {
                symbolDataStruct[p].symbolName.clear();
                symbolDataStruct[p].symbolType.clear();
                symbolDataStruct[p].symbolHex.clear();
                symbolDataStruct[p].symbolDec.clear();
                symbolDataStruct[p].symbolUsed.clear();
                p++;
            }
            loadPath = QFileDialog::getOpenFileName(this,"Input file",".","*.sym");
            file.setFileName(loadPath);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            readFileTable(&file);
            ui1.labelFileName->setText(loadPath);
            editTable();
        break;
        case 2:
            while ( macroDataStruct[p].macroName.isEmpty() == false )
            {
                macroDataStruct[p].macroName.clear();
                macroDataStruct[p].macroLine.clear();
                macroDataStruct[p].macroParameters.clear();
                macroDataStruct[p].macroUsed.clear();
                p++;
            }
            loadPath = QFileDialog::getOpenFileName(this,"Input file",".","*.mtbl");
            file.setFileName(loadPath);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            readFileMacro(&file);
            ui2.labelFileName->setText(loadPath);
            editMacro();
        break;
        case 3:
            while ( stringDataStruct[p].stringName.isEmpty() == false )
            {
                stringDataStruct[p].stringName.clear();
                stringDataStruct[p].stringLine.clear();
                stringDataStruct[p].stringCharacters.clear();
                p++;
            }
            loadPath = QFileDialog::getOpenFileName(this,"Input file",".","*.strtbl");
            file.setFileName(loadPath);
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            readFileString(&file);
            ui3.labelFileName->setText(loadPath);
            editString();
        break;
    }

}

void SymbolTable::pushReload()
{
    if ( loadPath.isEmpty() == true )
        return;

    if ( tabWidget->currentIndex() == 0 )
    {
        int p = 0;
        while ( symbolDataStruct[p].symbolName.isEmpty() == false )
        {
            symbolDataStruct[p].symbolName.clear();
            symbolDataStruct[p].symbolType.clear();
            symbolDataStruct[p].symbolHex.clear();
            symbolDataStruct[p].symbolDec.clear();
            symbolDataStruct[p].symbolUsed.clear();
            p++;
        }
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        readFileTable(&file);
        ui3.labelFileName->setText(loadPath);
        editTable();
    }
    if ( tabWidget->currentIndex() == 1 )
    {
        int p = 0;
        while ( macroDataStruct[p].macroName.isEmpty() == false )
        {
            macroDataStruct[p].macroName.clear();
            macroDataStruct[p].macroLine.clear();
            macroDataStruct[p].macroParameters.clear();
            macroDataStruct[p].macroUsed.clear();
            p++;
        }
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        readFileMacro(&file);
        ui3.labelFileName->setText(loadPath);
        editMacro();
    }
    if ( tabWidget->currentIndex() == 2 )
    {
        int p = 0;
        while ( stringDataStruct[p].stringName.isEmpty() == false )
        {
            stringDataStruct[p].stringName.clear();
            stringDataStruct[p].stringLine.clear();
            stringDataStruct[p].stringCharacters.clear();
            p++;
        }
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        readFileString(&file);
        ui3.labelFileName->setText(loadPath);
        editString();
    }
    // call read fnc
    //loadFile( tabWidget->currentIndex() + 1 );
}


void SymbolTable::readFileMacro( QIODevice *device)
{
    QString line;
    QTextStream in(device);
    int f = 1;
    int p = 0;

    qDebug()<< "readable : "<< device->isReadable() << "file read";

    line = in.readLine();
    while (!line.isNull())
    {
        simplifiedLineList.clear();
        //simplifiedLine = line.simplified();

       // simplifiedLineList = simplifiedLine.split(" ");
        //qDebug() << simplifiedLineList << "simplified list";
        // first symbol is a name
        //qDebug()<< line << "line";
        line.remove(",");

        simplifiedLineList = line.split(" ", QString::SkipEmptyParts );
        qDebug()<< simplifiedLineList << "line splited";

        macroDataStruct[p].macroName = simplifiedLineList[0];

        if ( simplifiedLineList[1] == QString("(") && simplifiedLineList[2] == QString(")") )
        {
            QString lineNumber, finalNumber;
            int help;
            // skip all dots
            f = 3;
            while( simplifiedLineList[f] == QString(".") )
            {
                f++;
                if ( f > 4000)
                break;
            }
            macroDataStruct[p].macroUsed = simplifiedLineList[f];

            if ( macroDataStruct[p].macroUsed == "USED")
            {
                f++;
                macroDataStruct[p].macroUsed = "YES";
            }
            else
            {
                macroDataStruct[p].macroUsed = "NO";
                f = f + 2;
            }
            help = simplifiedLineList.at(f).indexOf(":") + 1;

            lineNumber = simplifiedLineList[f];
            while ( lineNumber.at(help) != QChar('.') )
            {
                finalNumber.append(lineNumber[help]);
                help++;
            }
            macroDataStruct[p].macroLine = finalNumber;
            if ( help == -1)
                qDebug() << "problem in parsing file";
        }
        else
        {
            // simplifiedLineList[1] == QString("(") && simplifiedLineList[3] == QString(")")
            f = 2;
            while( simplifiedLineList[f] != QString(")") )
            {
                macroDataStruct[p].macroParameters << simplifiedLineList[f];
                f++;
            }
            QString lineNumber, finalNumber;
            int help;
            // skip all dots
            f++;
            while( simplifiedLineList[f] == QString(".") )
            {
                f++;
                if ( f > 4000)
                break;
            }
            // read type and other values after dots
            macroDataStruct[p].macroUsed = simplifiedLineList[f];

            // if it is used or not
            if ( macroDataStruct[p].macroUsed == "USED")
            {
                f++;
                macroDataStruct[p].macroUsed = "YES";
            }
            else
            {
                macroDataStruct[p].macroUsed = "NO";
                f = f + 2;
            }
            help = simplifiedLineList.at(f).indexOf(":") + 1;

            lineNumber = simplifiedLineList[f];
            while ( lineNumber.at(help) != QChar('.') )
            {
                finalNumber.append(lineNumber[help]);
                help++;
            }
            macroDataStruct[p].macroLine = finalNumber;
        }
        // read line, remove long spaces, create list of words
        line = in.readLine();
        p++;
    }
    //iGlobal = i;
    qDebug()     << "macros: "<< macroDataStruct[p].macroName
                 << macroDataStruct[p].macroParameters
                 << macroDataStruct[p].macroLine
                 << macroDataStruct[p].macroUsed;

    file.close();
}

void SymbolTable::readFileString( QIODevice *device)
{
    QString line;
    QString simplifiedLine;
    QTextStream in(device);
    int f = 1;
    int p = 0;

    line = in.readLine();
    while (!line.isNull())
    {
        // variables for reading line number
        QString lineNumber, finalNumber;
        int help;

        // parse
        simplifiedLineList.clear();
        simplifiedLine = line.simplified();

        simplifiedLineList = simplifiedLine.split(" ");
        qDebug() << simplifiedLineList << "simplified list";

        // first symbol is a name
        stringDataStruct[p].stringName = simplifiedLineList[0];

        // skip all dots
        f = 1;
        while( simplifiedLineList[f] == QString(".") )
        {
            f++;
        }

        // read type and other values after dots
        help = simplifiedLineList.at(f).indexOf(":") + 1;

        lineNumber = simplifiedLineList[f];
        qDebug() << lineNumber << "line number string";

        while ( lineNumber.at(help) != QChar('.') )
        {
            finalNumber.append(lineNumber[help]);
            help++;
        }
        stringDataStruct[p].stringLine = finalNumber;
        f++;
        stringDataStruct[p].stringCharacters = simplifiedLineList[f];
        for ( int i = (f + 1); i < simplifiedLineList.size(); i++)
        {
            stringDataStruct[p].stringCharacters.append(simplifiedLineList[i]);
        }
        qDebug() << stringDataStruct[p].stringCharacters << " <<<<<<  STRING FINAL";

        // continue reading
        line = in.readLine();
        p++;
    }
    file.close();
}

void SymbolTable::readFileTable( QIODevice *device)
{

    QString line;
    QString simplifiedLine;
    QTextStream in(device);
    int f = 1;
    int i = 0;

    qDebug()<< "readable : "<< device->isReadable() << "file read";

    line = in.readLine();
    while (!line.isNull())
    {
        simplifiedLineList.clear();
        simplifiedLine = line.simplified();

        simplifiedLineList = simplifiedLine.split(" ");
        //qDebug() << simplifiedLineList << "simplified list";

        // first symbol is a name
        symbolDataStruct[i].symbolName = simplifiedLineList[0];

        // skip all dots
        f = 1;
        while( simplifiedLineList[f] == QString(".") )
        {
            f++;
        }
        // read type and other values after dots
        symbolDataStruct[i].symbolType = simplifiedLineList[f];
        symbolDataStruct[i].symbolHex = simplifiedLineList[f+1];
        symbolDataStruct[i].symbolDec = simplifiedLineList[f+1];

        // transform to dec number
        symbolDataStruct[i].symbolDec.remove(0,2);
        symbolDataStruct[i].symbolHex.remove(0,2);

        long dec;
        dec = symbolDataStruct[i].symbolDec.toLong(0,16);

        symbolDataStruct[i].symbolDec = QString::number(dec,10);

        // if it is used or not
        if ( simplifiedLineList.at(f + 2) == "USED")
        {
            symbolDataStruct[i].symbolUsed = "YES";
        }
        else
        {
            symbolDataStruct[i].symbolUsed = "NO";
        }
        symbolDataStruct[i].display = true;
        // read attribute
        //symbolDataStruct[i].symbolAttribute = simplifiedLineList.at(f + 3);

        // read line, remove long spaces, create list of words
        line = in.readLine();
        i++;
    }
    iGlobal = i;
    file.close();
}


void SymbolTable::on_searchEdit_textChanged(const QString &arg1)
{
    // clear color when  empty
    if ( ui1.searchEdit->text().isEmpty() == true )
    {
        ui1.searchEdit->setStyleSheet("QLineEdit{background: white;}");
        ui1.listDisplay->selectionModel()->clearSelection();
    }
    if ( ui2.searchEdit->text().isEmpty() == true )
    {
        ui2.searchEdit->setStyleSheet("QLineEdit{background: white;}");
        ui2.listDisplay->selectionModel()->clearSelection();
    }
    if ( ui3.searchEdit->text().isEmpty() == true )
    {
        ui3.searchEdit->setStyleSheet("QLineEdit{background: white;}");
        ui3.listDisplay->selectionModel()->clearSelection();
    }
    // do nothing when all empty
    if ( ui1.searchEdit->text().isEmpty() == true && ui2.searchEdit->text().isEmpty() == true && ui3.searchEdit->text().isEmpty() == true)
        return;

    QModelIndex start;
    // index list for each view
    QModelIndexList symbolIndexList, macroIndexList, stringIndexList;


    start = modelSymbol->index(0, 0);
    symbolIndexList = modelSymbol->match(
                start, Qt::DisplayRole,
                arg1, -1,Qt::MatchContains);
    qDebug() << "index list:" << symbolIndexList;

    start = modelMacro->index(0, 0);
    symbolIndexList = modelMacro->match(
                start, Qt::DisplayRole,
                arg1, -1,Qt::MatchContains);
    qDebug() << "index list:" << macroIndexList;

    start = modelString->index(0, 0);
    symbolIndexList = modelString->match(
                start, Qt::DisplayRole,
                arg1, -1,Qt::MatchContains);
    qDebug() << "index list:" << stringIndexList;

    if ( symbolIndexList.size() == 0)
    {
        ui1.searchEdit->setStyleSheet("QLineEdit{background: red;}");
        ui1.listDisplay->selectionModel()->clearSelection();
    }
    else
    {
        ui1.searchEdit->setStyleSheet("QLineEdit{background: #BCED91;}");
        for ( int i = 0; i < symbolIndexList.size(); i++)
        {
            ui1.listDisplay->selectionModel()->select(symbolIndexList[i],QItemSelectionModel::Select);
        }
    }

    if ( macroIndexList.size() == 0)
    {
        ui2.searchEdit->setStyleSheet("QLineEdit{background: red;}");
        ui2.listDisplay->selectionModel()->clearSelection();
    }
    else
    {
        ui2.searchEdit->setStyleSheet("QLineEdit{background: #BCED91;}");
        for ( int i = 0; i < symbolIndexList.size(); i++)
        {
            ui2.listDisplay->selectionModel()->select(macroIndexList[i],QItemSelectionModel::Select);
        }
    }

    if ( stringIndexList.size() == 0)
    {
        ui3.searchEdit->setStyleSheet("QLineEdit{background: red;}");
        ui3.listDisplay->selectionModel()->clearSelection();
    }
    else
    {
        ui3.searchEdit->setStyleSheet("QLineEdit{background: #BCED91;}");
        for ( int i = 0; i < stringIndexList.size(); i++)
        {
            ui3.listDisplay->selectionModel()->select(stringIndexList[i],QItemSelectionModel::Select);
        }
    }
    //ui1->listDisplay->setFocus();

    //ui1->listDisplay->setModel(model);
   // qDebug() << "arg1:" << arg1;
}

void SymbolTable::customMenuRequested(QPoint pos)
{
    QMenu *menu = new QMenu(this);
    QMenu *menuMacro = new QMenu(this);
    QMenu *menuString = new QMenu(this);
    //menu->addAction(new QAction("Remove", this));
    menu->addAction("Copy symbol name", this,   SLOT(copySymbolName()));
    menu->addAction("Copy hex value", this,     SLOT(copyHexValue()));
    menu->addAction("Copy dec value", this,     SLOT(copyDecValue()));
    menu->addAction("Copy line", this,          SLOT(copyLine()));

    menuMacro->addAction("Copy macro name", this,   SLOT(copySymbolName()));
    menuMacro->addAction("Copy line", this,          SLOT(copyLine()));

    menuString->addAction("Copy string name", this,   SLOT(copySymbolName()));
    menuString->addAction("Copy line", this,          SLOT(copyLine()));

    if ( tabWidget->currentIndex() == 0 )
    {
        menu->popup(ui1.listDisplay->viewport()->mapToGlobal(pos));
    }
    if ( tabWidget->currentIndex() == 1 )
    {
        menuMacro->popup(ui2.listDisplay->viewport()->mapToGlobal(pos));
    }
    if ( tabWidget->currentIndex() == 2 )
    {
        menuString->popup(ui3.listDisplay->viewport()->mapToGlobal(pos));
    }
}

void SymbolTable::copySymbolName()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clickedStruct.clickedName);
}
void SymbolTable::copyLine()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clickedStruct.clickedLine);
}
void SymbolTable::copyDecValue()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clickedStruct.clickedDec);
}
void SymbolTable::copyHexValue()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(clickedStruct.clickedHex);
}

void SymbolTable::pushOpen()
{
    loadFile(1);
}

void SymbolTable::macroTableClicked()
{
    loadFile(2);
}

void SymbolTable::stringTableClicked()
{
    loadFile(3);
}

void SymbolTable::searchChanged(QString)
{}
void SymbolTable::displayPrefChanged()
{
    if ( tabWidget->currentIndex() == 0 )
    {
        editTable();
    }
    if ( tabWidget->currentIndex() == 1 )
    {
        editMacro();
    }
}
void SymbolTable::sortPrefChanged()
{}

void SymbolTable::connectSignals()
{
    // slots
    //      void pushOpen();
    //      void pushReload();
    //      void searchChanged();
    //      void displayPrefChanged();
    //      void sortPrefChanged();

    // push buttons
    connect(ui1.pushOpen,
            SIGNAL(clicked()),
            this,
            SLOT(pushOpen())
           );
    connect(ui1.pushReload,
            SIGNAL(clicked()),
            this,
            SLOT(pushReload())
           );
    connect(ui1.searchEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(searchChanged(QString))
           );

    // display preferencs checkboxes
    connect(ui1.checkData,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkExpresion,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkLabel,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkNumber,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkPort,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkRegister,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkUnusedSym,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.checkUsedSym,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui1.searchEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(on_searchEdit_textChanged(QString)));


    // UI 2 ---------------
    connect(ui2.pushMacro,
            SIGNAL(clicked()),
            this,
            SLOT(macroTableClicked())
           );
    connect(ui2.searchEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(on_searchEdit_textChanged(QString)));
    connect(ui2.pushReload,
            SIGNAL(clicked()),
            this,
            SLOT(pushReload())
           );
    connect(ui2.checkUsedMac,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui2.checkUnusedMac,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );
    connect(ui2.checkParam,
            SIGNAL(clicked()),
            this,
            SLOT(displayPrefChanged())
           );

    // UI 3 ---------------
    connect(ui3.searchEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(on_searchEdit_textChanged(QString)));

    connect(ui3.pushString,
            SIGNAL(clicked()),
            this,
            SLOT(stringTableClicked())
           );
    connect(ui3.pushReload,
            SIGNAL(clicked()),
            this,
            SLOT(pushReload())
           );
}


SymbolTable::~SymbolTable()
{
  //  delete ui1;
}
