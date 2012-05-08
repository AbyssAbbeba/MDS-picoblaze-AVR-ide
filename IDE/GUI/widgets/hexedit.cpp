#include <QtGui>
#include "hexedit.h"




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
            return true;
        //else if (keyEvent->key() == Qt::Key_Right)
        //    txtCursor.setPosition(position-2); 
        else if ((keyEvent->key() >= Qt::Key_0 &&
          keyEvent->key() <= Qt::Key_9)
          || (keyEvent->key() >= Qt::Key_A &&
          keyEvent->key() <= Qt::Key_F))
        {
            int position = hexTextEdit->textCursor().position();
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
            return true;
        else if (keyEvent->key() >= 0x20 &&
          keyEvent->key() <= 0x7e)
        {
            int position = hexAsciiEdit->textCursor().position() - hexAsciiEdit->textCursor().blockNumber();
            (*hexByteArray)[position] = keyEvent->text().at(0).toAscii();
            emit asciiChanged(position);
        }
    }
    return QWidget::eventFilter(target, event);
}




HexEdit::HexEdit(QWidget *parent, bool AsciiPanel, int countSize, int columns)
    :QWidget(parent)
{
    this->columns=columns;
    hexLayout = new QGridLayout(this);
    hexTextEdit = new QTextEdit(this);
    //hexTextEdit->setReadOnly(true);
    hexTextEdit->setOverwriteMode(true);
    hexTextEdit->setWordWrapMode(QTextOption::NoWrap);
    hexTextEdit->setFont(QFont("Andale Mono", 10));
    //hexTextEdit->resize((columns*2-1)*10,5);
    hexTextEdit->setMinimumWidth(175);
    //hexTextEdit->setMaximumWidth(475);
    hexTextEdit->setMinimumHeight(175);

    if (AsciiPanel == true)
    {
        hexAsciiEdit = new QTextEdit(this);
        hexAsciiEdit->setFont(QFont("Andale Mono", 10));
        //hexAsciiEdit->resize(columns*15,250);
        hexAsciiEdit->setMinimumHeight(175);
        hexAsciiEdit->setMinimumWidth(columns*10);
        hexAsciiEdit->setOverwriteMode(true);
        hexAsciiEdit->setWordWrapMode(QTextOption::NoWrap);
        hexAsciiEdit->verticalScrollBar()->hide();
    }

    hexColumnCount = new WColumnCounter(hexTextEdit, countSize, columns);
    hexLineCount = new WLineCounter(hexTextEdit, false, countSize);
    hexByteArray = new QByteArray(190, 126);
    hexStatusBar = new QStatusBar(this);
    hexStatusLabel = new QLabel(this);
    hexStatusBar->addPermanentWidget(hexStatusLabel);
    if (hexTextEdit->isReadOnly() == true)
        hexStatusLabel->setText("Read Only");
    else
        hexStatusLabel->setText("Read/Write");

    hexLayout->setSpacing(0);
    hexLayout->addWidget(hexLineCount, 1, 0);
    hexLayout->addWidget(hexColumnCount, 0, 1);
    hexLayout->addWidget(hexTextEdit, 1, 1);
    hexLayout->addWidget(hexStatusBar, 2, 1);
    if (AsciiPanel == true)
    {
        hexLayout->addWidget(hexAsciiEdit, 1, 2);
        hexAsciiEdit->installEventFilter(this);
    }
    hexTextEdit->installEventFilter(this);

    changable = false;
    setData(hexByteArray);
    connect(hexTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(moveCursor()));
    if (AsciiPanel == true)
    {
        connect(hexAsciiEdit, SIGNAL(cursorPositionChanged()), this, SLOT(moveAsciiCursor()));
        connect(this, SIGNAL(textChanged(int)), this, SLOT(changeAscii(int)));
        connect(this, SIGNAL(asciiChanged(int)), this, SLOT(changeText(int)));
        connect(hexTextEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), hexAsciiEdit->verticalScrollBar(), SLOT(setValue(int)));
        connect(hexAsciiEdit->verticalScrollBar(), SIGNAL(valueChanged(int)), hexTextEdit->verticalScrollBar(), SLOT(setValue(int)));
    }


    changable = true;
    prevPosition = 0;
    asciiPrevPosition = 0;
}


QTextEdit* HexEdit::getTextEdit()
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
            format.setBackground(Qt::white);
        else
            format.setBackground(Qt::lightGray);
        txtCursor.setCharFormat(format);
        txtCursor.setPosition(position);

        if (position % 3 == 2)
        {
            if (prevPosition == position - 1)
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
        

        QTextCursor asciiCursor = hexAsciiEdit->textCursor();
        asciiCursor.setPosition(position/3);
        int prevBlock = asciiCursor.blockNumber();

        int asciiPosition = prevBlock + position/3;
        asciiCursor.setPosition(asciiPosition);
        if (asciiCursor.positionInBlock() == columns)
            asciiPosition += 1;

        if (prevBlock != asciiCursor.blockNumber())
            asciiPosition += 1;

        asciiCursor.setPosition(asciiPosition);
        asciiCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
        format = asciiCursor.charFormat();
        format.setBackground(Qt::green);
        asciiCursor.setCharFormat(format);
        if (asciiPrevPosition != asciiPosition)
        {
            asciiCursor.setPosition(asciiPrevPosition);
            asciiCursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, 1);
            if (asciiCursor.blockNumber()%2 == 0)
                format.setBackground(Qt::white);
            else
                format.setBackground(Qt::lightGray);
            asciiCursor.setCharFormat(format);
            asciiCursor.setPosition(asciiPosition);
            asciiPrevPosition = asciiPosition;
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
        if (asciiCursor.blockNumber()%2 == 0)
            format.setBackground(Qt::white);
        else
            format.setBackground(Qt::lightGray);
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
            format.setBackground(Qt::white);
        else
            format.setBackground(Qt::lightGray);
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
        if (hexByteArray->at(position) > 10)
            textCursor.insertText((QString::number(hexByteArray->at(position), 16)).toUpper());
        else
            textCursor.insertText("0" + (QString::number(hexByteArray->at(position), 16)).toUpper());
        hexTextEdit->setTextCursor(textCursor);
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
            asciiPosition += 1;

        if (prevBlock != asciiCursor.blockNumber())
            asciiPosition += 1;
                 
        asciiCursor.setPosition(asciiPosition);
        asciiCursor.deleteChar();
        if ((unsigned char)hexByteArray->at(position) >= 32 && (unsigned char)hexByteArray->at(position) < 127)
            asciiCursor.insertText(QString((unsigned char)hexByteArray->at(position)));
        else
            asciiCursor.insertText(".");
        asciiCursor.setPosition(asciiPosition);
        hexAsciiEdit->setTextCursor(asciiCursor);
        changable = true;
    }
}





void HexEdit::setData(QByteArray *byteArray)
{
    bool back = false;
    //int line is for counting rows (bad naming, heh...)
    int line = 0;
    if (hexTextEdit->isReadOnly())
        hexTextEdit->setTextColor(Qt::darkGray);
    else
        hexTextEdit->setTextColor(Qt::black);
    QString tmp(*byteArray);
    for (int i=0; i<byteArray->size(); i++)
    {
        if (line == 0)
        {
            if (back == true)
            {
                hexTextEdit->setTextBackgroundColor(Qt::lightGray);
                hexAsciiEdit->setTextBackgroundColor(Qt::lightGray);
                back = false;
            }
            else
            {
                hexTextEdit->setTextBackgroundColor(Qt::white);
                hexAsciiEdit->setTextBackgroundColor(Qt::white);
                back = true;
            }
        }
        if (byteArray->at(i) > 10)
            hexTextEdit->insertPlainText((QString::number(byteArray->at(i), 16)).toUpper());
        else
            hexTextEdit->insertPlainText("0" + (QString::number(byteArray->at(i), 16)).toUpper());
        if (byteArray->at(i) < 127 && byteArray->at(i) >= 32)
            hexAsciiEdit->insertPlainText(tmp.at(i));
        else
            hexAsciiEdit->insertPlainText(".");
        line++;
        if (line == 16) 
        {
            hexTextEdit->insertPlainText("\n");
            hexAsciiEdit->insertPlainText("\n");
            line = 0;
        }
        else
            hexTextEdit->insertPlainText(" ");
    }
}


