/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup HexEdit
 * @file hexedit.cpp
 */


#include <QtGui>
#include "hexedit.h"
#include "../Counters/wlinecounter.h"
#include "../Counters/wcolumncounter.h"




bool HexEdit::eventFilter(QObject *target, QEvent *event)
{
    if (target == hexTextEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //overit, jestli je vse v poradku (0-9, A-F, sipky)
        if ((keyEvent->key() < Qt::Key_0 ||
            keyEvent->key() > Qt::Key_9)
            && (keyEvent->key() < Qt::Key_A ||
            keyEvent->key() > Qt::Key_F)
            && (keyEvent->key() < Qt::Key_Left ||
            keyEvent->key() > Qt::Key_Down))
        {
            return true;
        }
        //else if (keyEvent->key() == Qt::Key_Right)
        //    txtCursor.setPosition(position-2);
        else if ((keyEvent->key() >= Qt::Key_0 &&
            keyEvent->key() <= Qt::Key_9)
            || (keyEvent->key() >= Qt::Key_A &&
            keyEvent->key() <= Qt::Key_F))
        {
            int position = hexTextEdit->textCursor().position();
            if ((position/3)+1 > hexByteArray->size())
            {
                return true;
            }
            QChar data;
            QString final;
            if (position % 3 == 1)
            {
                data = hexTextEdit->toPlainText()[position-1];
                final += data;
                final += keyEvent->text();
            }
            else
            {
                data = hexTextEdit->toPlainText()[position+1];
                final += keyEvent->text();
                final += data;
            }
            bool ok;
            int hex = final.toInt(&ok, 16);
            (*hexByteArray)[position/3] = hex;
            emit textChanged(position/3);
        }
    }
    else if (target == hexAsciiEdit && event->type() == QEvent::KeyPress)
    {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //overit, jestli je vse v poradku (0-9, A-F, sipky)
        if ((keyEvent->key() < 0x20 ||
            keyEvent->key() > 0x7e)
          && (keyEvent->key() < Qt::Key_Left ||
            keyEvent->key() > Qt::Key_Down))
        {
            return true;
        }
        else if (keyEvent->key() >= 0x20 &&
            keyEvent->key() <= 0x7e)
        {
            int position = hexAsciiEdit->textCursor().position() - hexAsciiEdit->textCursor().blockNumber();
            if (position+1 > hexByteArray->size())
            {
                return true;
            }
            (*hexByteArray)[position] = keyEvent->text().at(0).toAscii();
            emit asciiChanged(position);
        }
    }
    return QWidget::eventFilter(target, event);
}




HexEdit::HexEdit(QWidget *parent, bool AsciiPanel, int countSize, int columns)
    :QWidget(parent)
{
    this->fontSize = 10;
    QFont font("Ubuntu Mono");
    font.setPointSize(fontSize);
    font.setHintingPreference(QFont::PreferFullHinting);
    font.setFixedPitch(true);
    font.setKerning(false);
    font.setStyleStrategy(QFont::ForceIntegerMetrics);
    this->columns=columns;
    this->ascii = AsciiPanel;
    hexAsciiEdit = NULL;
    this->prevBlock = 0;
    hexLayout = new QGridLayout(this);
    hexLayout->setHorizontalSpacing(0);
    hexLayout->setRowStretch(0,0);
    hexLayout->setColumnStretch(0,0);
    hexTextEdit = new QPlainTextEdit(this);
    //hexTextEdit->setReadOnly(true);
    hexTextEdit->setOverwriteMode(true);
    hexTextEdit->setWordWrapMode(QTextOption::NoWrap);
    hexTextEdit->setFont(font);
    QFontMetrics metrics(hexTextEdit->font());
    hexTextEdit->setFixedWidth(columns*3*metrics.averageCharWidth()+5);
    hexTextEdit->setMinimumHeight(175);
    hexTextEdit->setStyleSheet(QString(
                                    "QScrollBar:vertical"
                                    "{"
                                        "border: 0px; "
                                        "background: white;"
                                        "width: 2px;"
                                        "margin: 0px 0 0px 0;"
                                    "}"
                                    "QScrollBar:horizontal"
                                    "{"
                                        "border: 0px; "
                                        "background: white;"
                                        "width: 0px;"
                                        "margin: 0px 0 0px 0;"
                                    "}"
                                    "QScrollBar::handle:vertical"
                                    "{"
                                        "background: black;"
                                    "}"
                                    )
                              );

    if (AsciiPanel == true)
    {
        hexAsciiEdit = new QPlainTextEdit(this);
        hexAsciiEdit->setFont(font);
        //hexAsciiEdit->resize(columns*15,250);
        hexAsciiEdit->setFixedWidth(columns*3*metrics.averageCharWidth());
        hexAsciiEdit->setMinimumHeight(175);
        hexAsciiEdit->setOverwriteMode(true);
        hexAsciiEdit->setWordWrapMode(QTextOption::NoWrap);
        //hexAsciiEdit->verticalScrollBar()->hide();
        //hexAsciiEdit->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
    }

    hexColumnCount = new WColumnCounter((QPlainTextEdit*)hexTextEdit, this->hexTextEdit->font(), columns);
    hexLineCount = new WLineCounter((QPlainTextEdit*)hexTextEdit, false, true, columns, this->hexTextEdit->font());
    hexByteArray = new QList<unsigned char>();
    hexByteArray->reserve(countSize);
    for (int i = 0; i < countSize; i++)
    {
        hexByteArray->append(0);
    }
    //hexStatusBar = new QStatusBar(this);
    //hexStatusLabel = new QLabel(this);
    //hexStatusBar->addPermanentWidget(hexStatusLabel);
    /*if (hexTextEdit->isReadOnly() == true)
    {
        hexStatusLabel->setText("Read Only");
    }
    else
    {
        hexStatusLabel->setText("Read/Write");
    }*/

    hexLayout->setSpacing(0);


    //hexLayout->addWidget(nullWidget, 0, 0);
    hexLayout->addWidget(hexLineCount, 1, 0);
    hexLayout->addWidget(hexColumnCount, 0, 1);
    hexLayout->addWidget(hexTextEdit, 1, 1);
    //hexLayout->addWidget(hexStatusLabel, 2, 1);

    if (AsciiPanel == true)
    {
        hexLayout->addWidget(hexAsciiEdit, 1, 2);
        hexAsciiEdit->installEventFilter(this);
    }
    hexTextEdit->installEventFilter(this);

    changable = false;
    setData(hexByteArray);
    connect( hexTextEdit,
             SIGNAL(cursorPositionChanged()),
             this,
             SLOT(moveCursor())
           );
    connect( hexTextEdit->horizontalScrollBar(),
             SIGNAL(valueChanged(int)),
             this,
             SLOT(resetHorizontalScrollBar())
           );
    if (AsciiPanel == true)
    {
        connect( hexAsciiEdit,
                 SIGNAL(cursorPositionChanged()),
                 this,
                 SLOT(moveAsciiCursor())
               );
        connect( this,
                 SIGNAL(textChanged(int)),
                 this,
                 SLOT(changeAscii(int))
               );
        connect( this,
                 SIGNAL(asciiChanged(int)),
                 this,
                 SLOT(changeText(int))
               );
        connect( hexTextEdit->verticalScrollBar(),
                 SIGNAL(valueChanged(int)),
                 hexAsciiEdit->verticalScrollBar(),
                 SLOT(setValue(int))
               );
        connect( hexAsciiEdit->verticalScrollBar(),
                 SIGNAL(valueChanged(int)),
                 hexTextEdit->verticalScrollBar(),
                 SLOT(setValue(int))
               );
    }


    changable = true;
    prevPosition = 0;
    asciiPrevPosition = 0;


    this->hexTextEdit->horizontalScrollBar()->setMaximum(0);
}


HexEdit::~HexEdit()
{
    delete hexByteArray;
}


QPlainTextEdit* HexEdit::getTextEdit()
{
    return hexTextEdit;
}








void HexEdit::moveCursor()
{
    if (changable == true)
    {
        changable = false;
        QTextCursor txtCursor = hexTextEdit->textCursor();
        int position = txtCursor.position();

        txtCursor.setPosition(prevPosition);
        txtCursor.select(QTextCursor::WordUnderCursor);
        QTextCharFormat format = txtCursor.charFormat();
        if (txtCursor.blockNumber() % 2 == 0)
        {
            format.setBackground(Qt::white);
        }
        else
        {
            format.setBackground(Qt::lightGray);
        }
        txtCursor.setCharFormat(format);
        txtCursor.setPosition(position);

        if (position % 3 == 2)
        {
            if (prevPosition == position - 1 && position != hexTextEdit->document()->characterCount() - 1)
            {
               position++;
               txtCursor.setPosition(position);
            }
            else
            {
               position -= 1;
               txtCursor.setPosition(position);
            }

            hexTextEdit->setTextCursor(txtCursor);
        }
        else if (hexTextEdit->textCursor().hasSelection() == true)
        {
            txtCursor.setPosition(position);
            hexTextEdit->setTextCursor(txtCursor);
        }

        prevPosition = position;
        txtCursor.select(QTextCursor::WordUnderCursor);
        format = txtCursor.charFormat();
        format.setBackground(Qt::green);
        txtCursor.setCharFormat(format);

        if (ascii == true)
        {
            QTextCursor asciiCursor = hexAsciiEdit->textCursor();
            asciiCursor.setPosition(position/3);
            int prevBlock = asciiCursor.blockNumber();

            int asciiPosition = prevBlock + position/3;
            asciiCursor.setPosition(asciiPosition);
            if (asciiCursor.positionInBlock() == columns)
            {
                asciiPosition += 1;
            }

            if (prevBlock != asciiCursor.blockNumber())
            {
                asciiPosition += 1;
            }

            asciiCursor.setPosition(asciiPosition);
            asciiCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
            format = asciiCursor.charFormat();
            format.setBackground(Qt::green);
            asciiCursor.setCharFormat(format);
            if (asciiPrevPosition != asciiPosition)
            {
                asciiCursor.setPosition(asciiPrevPosition);
                asciiCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
                if (asciiCursor.blockNumber() % 2 == 0)
                {
                    format.setBackground(Qt::white);
                }
                else
                {
                    format.setBackground(Qt::lightGray);
                }
                asciiCursor.setCharFormat(format);
                asciiCursor.setPosition(asciiPosition);
                asciiPrevPosition = asciiPosition;
            }
        }
        if (hexTextEdit->textCursor().blockNumber() != this->prevBlock)
        {
            this->prevBlock = hexTextEdit->textCursor().blockNumber();
            this->hexLineCount->getWidget()->update();
        }
        changable = true;
    }
}



void HexEdit::moveAsciiCursor()
{
    if (changable == true)
    {
        changable = false;
        QTextCursor asciiCursor = hexAsciiEdit->textCursor();
        int position = asciiCursor.position();
        if (asciiCursor.positionInBlock() == columns)
        {
            position++;
            asciiCursor.setPosition(position);
            hexAsciiEdit->setTextCursor(asciiCursor);
        }
        else if (asciiCursor.hasSelection() == true)
        {
            //int position = asciiCursor.position();
            asciiCursor.setPosition(position);
            hexAsciiEdit->setTextCursor(asciiCursor);
        }
        QTextCharFormat format = asciiCursor.charFormat();
        asciiCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
        format = asciiCursor.charFormat();
        format.setBackground(Qt::green);
        asciiCursor.setCharFormat(format);
        asciiCursor.setPosition(asciiPrevPosition);
        asciiCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
        if (asciiCursor.blockNumber() % 2 == 0)
        {
            format.setBackground(Qt::white);
        }
        else
        {
            format.setBackground(Qt::lightGray);
        }
        asciiCursor.setCharFormat(format);
        asciiCursor.setPosition(position);
        asciiPrevPosition = position;

        QTextCursor textCursor = hexTextEdit->textCursor();
        int textPosition = (asciiCursor.position() - asciiCursor.blockNumber() )*3;
        textCursor.setPosition(textPosition);
        format = textCursor.charFormat();
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
        format = textCursor.charFormat();
        format.setBackground(Qt::green);
        textCursor.setCharFormat(format);
        textCursor.setPosition(prevPosition);
        textCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 2);
        if (textCursor.blockNumber()%2 == 0)
        {
            format.setBackground(Qt::white);
        }
        else
        {
            format.setBackground(Qt::lightGray);
        }
        textCursor.setCharFormat(format);
        textCursor.setPosition(textPosition);
        prevPosition = textPosition;
        hexTextEdit->setTextCursor(textCursor);

        changable = true;
    }
}




void HexEdit::changeText(int position)
{
    if (changable == true)
    {
        changable = false;
        QTextCursor textCursor = hexTextEdit->textCursor();
        textCursor.setPosition(position*3);
        textCursor.deleteChar();
        textCursor.deleteChar();
        //qDebug() << "HexEdit: text changed to" << (int)(hexByteArray->at(position));
        if (hexByteArray->at(position) > 15)
        {
            textCursor.insertText((QString::number((int)(hexByteArray->at(position)), 16)).toUpper());
        }
        else
        {
            textCursor.insertText("0" + (QString::number((int)(hexByteArray->at(position)), 16)).toUpper());
        }
        hexTextEdit->setTextCursor(textCursor);
        /*if (position != hexByteArray->size() && position != 0)
        {
            qDebug() << "HexEdit: characters"
                    << (hexTextEdit->document()->characterAt(position*3-2))
                    << (hexTextEdit->document()->characterAt(position*3-1))
                    << (hexTextEdit->document()->characterAt(position*3))
                    << (hexTextEdit->document()->characterAt(position*3+1))
                    << (hexTextEdit->document()->characterAt(position*3+2));
        }*/
        if (hexTextEdit->textCursor().blockNumber() != this->prevBlock)
        {
            this->prevBlock = hexTextEdit->textCursor().blockNumber();
            this->hexLineCount->getWidget()->update();
        }
        changable = true;
    }
}



void HexEdit::changeAscii(int position)
{
    if (changable == true)
    {
        changable = false;
        int prevBlock = 0;
        QTextCursor asciiCursor = hexAsciiEdit->textCursor();
        asciiCursor.setPosition(position);
        prevBlock = asciiCursor.blockNumber();

        int asciiPosition = prevBlock + position;
        asciiCursor.setPosition(asciiPosition);
        if (asciiCursor.positionInBlock() == columns)
        {
            asciiPosition += 1;
        }

        if (prevBlock != asciiCursor.blockNumber())
        {
            asciiPosition += 1;
        }

        asciiCursor.setPosition(asciiPosition);
        asciiCursor.deleteChar();
        if ((unsigned char)(hexByteArray->at(position)) >= 32 && (unsigned char)(hexByteArray->at(position)) < 127)
        {
            asciiCursor.insertText(QString((unsigned char)(hexByteArray->at(position))));
        }
        else
        {
            asciiCursor.insertText(".");
        }
        asciiCursor.setPosition(asciiPosition);
        hexAsciiEdit->setTextCursor(asciiCursor);
        changable = true;
    }
}





void HexEdit::setData(QList<unsigned char> *byteArray)
{
    int line = 0;
    if (hexTextEdit->isReadOnly())
    {
        //hexTextEdit->setTextColor(Qt::darkGray);
        QPalette p = hexTextEdit->palette();
        p.setColor(QPalette::Text, Qt::darkGray);
        hexTextEdit->setPalette(p);
    }
    else
    {
        //hexTextEdit->setTextColor(Qt::black);
        QPalette p = hexTextEdit->palette();
        p.setColor(QPalette::Text, Qt::black);
        hexTextEdit->setPalette(p);
    }
    //QString tmp(*byteArray);
    for (int i=0; i<byteArray->size(); i++)
    {
        /*if (line == 0)
        {
            qDebug() << back;
            if (back == true)
            {
                //hexTextEdit->setTextBackgroundColor(Qt::lightGray);
                QPalette p = hexTextEdit->palette();
                p.setColor(QPalette::Base, Qt::white);
                hexTextEdit->setPalette(p);
                if (ascii == true)
                {
                    //hexAsciiEdit->setTextBackgroundColor(Qt::lightGray);
                    QPalette p = hexAsciiEdit->palette();
                    p.setColor(QPalette::Base, Qt::white);
                    hexAsciiEdit->setPalette(p);
                }
                back = false;
            }
            else
            {
                //hexTextEdit->setTextBackgroundColor(Qt::white);
                QPalette p = hexTextEdit->palette();
                p.setColor(QPalette::Base, Qt::lightGray);
                hexTextEdit->setPalette(p);
                if (ascii == true)
                {
                    //hexAsciiEdit->setTextBackgroundColor(Qt::white);
                    QPalette p = hexAsciiEdit->palette();
                    p.setColor(QPalette::Base, Qt::lightGray);
                    hexAsciiEdit->setPalette(p);
                }
                back = true;
            }
        }*/
        
        if (byteArray->at(i) > 15)
        {
            hexTextEdit->insertPlainText((QString::number(byteArray->at(i), 16)).toUpper());
        }
        else
        {
            hexTextEdit->insertPlainText("0" + (QString::number(byteArray->at(i), 16)).toUpper());
        }
        
        if (ascii == true)
        {
            if (byteArray->at(i) < 127 && byteArray->at(i) >= 32)
            {
                hexAsciiEdit->insertPlainText(QString(byteArray->at(i)));
            }
            else
            {
                hexAsciiEdit->insertPlainText(".");
            }
        }
        
        line++;
        
        if (line == this->columns)
        {
            if (i != byteArray->size()-1)
            {
                hexTextEdit->insertPlainText("\n");
                if (ascii == true)
                {
                    hexAsciiEdit->insertPlainText("\n");
                }
                line = 0;
            }
        }
        else
        {
            hexTextEdit->insertPlainText(" ");
        }
    }
    this->hexByteArray = byteArray;

    QTextBlockFormat blockFormat;
    QTextBlock block;
    for (int i = 0; i < hexTextEdit->document()->blockCount(); i++)
    {
        block = hexTextEdit->document()->findBlockByNumber(i);
        QTextCursor cur(block);
        blockFormat = cur.blockFormat();
        if (i%2 == 0)
        {
            blockFormat.setBackground(Qt::white);
        }
        else
        {
            blockFormat.setBackground(Qt::lightGray);
        }
        cur.setBlockFormat(blockFormat);
        if (ascii == true)
        {
            block = hexAsciiEdit->document()->findBlockByNumber(i);
            QTextCursor cur(block);
            blockFormat = cur.blockFormat();
            if (i%2 == 0)
            {
                blockFormat.setBackground(Qt::white);
            }
            else
            {
                blockFormat.setBackground(Qt::lightGray);
            }
            cur.setBlockFormat(blockFormat);
        }
    }
    //this->hexTextEdit->verticalScrollBar()->setValue(0);
    QTextCursor cur(hexTextEdit->textCursor());
    cur.setPosition(0);
    hexTextEdit->setTextCursor(cur);
    hexTextEdit->ensureCursorVisible();
}


void HexEdit::setVal(int pos, unsigned char val)
{
    //qDebug() << "-----------------HexEdit: setVal()";
    (*hexByteArray)[pos] = val;
    //qDebug() << "HexEdit: val" << (int)val;
    //qDebug() << "HexEdit: array val" << (int)hexByteArray->at(pos);
    this->changeText(pos);
    //qDebug() << "-----------------HexEdit: return setVal()";
}


unsigned char HexEdit::getVal(int pos)
{
    return hexByteArray->at(pos);
}


void HexEdit::setReadOnly(bool readonly)
{
    this->hexTextEdit->setReadOnly(readonly);
}


void HexEdit::fixHeight()
{
    //if (this->hexTextEdit->verticalScrollBar()->isVisible())
    //{
        //hexTextEdit->setFixedWidth(columns*3*metrics.averageCharWidth());
        this->hexLineCount->getWidget()->changeHeight();
    //}
    //else
    //{
    //    hexTextEdit->setMinimumWidth((columns*3-4)*fontSize);
    //    hexTextEdit->setMaximumWidth((columns*3-4)*fontSize);
    //}
}


QScrollBar* HexEdit::verticalScrollBar()
{
    return this->hexTextEdit->verticalScrollBar();
}


QScrollBar* HexEdit::horizontalScrollBar()
{
    return this->hexTextEdit->horizontalScrollBar();
}


void HexEdit::setHighlighted(int pos, bool highlight)
{
    if (changable == true)
    {
        changable = false;
        int position = pos * 3;
        QTextCursor txtCursor = hexTextEdit->textCursor();
        int prevPosition = txtCursor.position();
        txtCursor.setPosition(position);
        txtCursor.select(QTextCursor::WordUnderCursor);
        QTextCharFormat format = txtCursor.charFormat();
        if (true == highlight)
        {
            //qDebug() << "HexEdit: setHighlighted";
            format.setBackground(Qt::yellow);
        }
        else
        {
            if (txtCursor.blockNumber() % 2 == 0)
            {
                format.setBackground(Qt::white);
            }
            else
            {
                format.setBackground(Qt::lightGray);
            }
        }
        txtCursor.setCharFormat(format);
        txtCursor.setPosition(prevPosition);
        hexTextEdit->setTextCursor(txtCursor);
        changable = true;
    }
}


void HexEdit::unhighlight()
{
    for (int i = 0; i < hexByteArray->size(); i++)
    {
        this->setHighlighted(i, false);
    }
}


void HexEdit::scrollToTop()
{
    QTextCursor txtCursor = hexTextEdit->textCursor();
    txtCursor.setPosition(0);
    hexTextEdit->setTextCursor(txtCursor);
}


void HexEdit::resetHorizontalScrollBar()
{
    this->hexTextEdit->horizontalScrollBar()->setValue(0);
}