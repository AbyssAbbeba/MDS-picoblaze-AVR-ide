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
        else
        {
            int position = hexTextEdit->textCursor().position();
            QChar data;
            QString final;
            if (position % 2 == 1)
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
        }
    }
    return QWidget::eventFilter(target, event);
}




HexEdit::HexEdit(QWidget *parent)
    :QWidget(parent)
{
    hexLayout = new QGridLayout(this);
    hexColumnCount = new VerticalNumberCount(this);
    hexLineCount = new HorizontalNumberCount(this);
    hexTextEdit = new QTextEdit(this);
    hexByteArray = new QByteArray(190, 10);
    hexStatusBar = new QStatusBar(this);
    hexTextEdit->setReadOnly(true);
    hexTextEdit->setOverwriteMode(true); 
    hexTextEdit->setWordWrapMode(QTextOption::NoWrap);
    hexTextEdit->setFont (QFont ("Andale Mono", 10));
    hexTextEdit->setMinimumWidth(475);
    //hexTextEdit->setMaximumWidth(475);
    hexTextEdit->setMinimumHeight(175);

    hexLayout->setSpacing(0);
    hexLayout->addWidget(hexLineCount, 1, 0);
    hexLayout->addWidget(hexColumnCount, 0, 1);
    hexLayout->addWidget(hexTextEdit, 1, 1);
    hexLayout->addWidget(hexStatusBar, 2, 1);
    //hexColumnCount->repaint();
    hexTextEdit->installEventFilter(this);

    setData(hexByteArray);
    //connect(hexTextEdit, SIGNAL(textChanged()), this, SLOT(changeText()));
    connect(hexTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(moveCursor()));
    //changable = true;
}


QTextEdit* HexEdit::getTextEdit()
{
    return hexTextEdit;
}




void HexEdit::moveCursor()
{
    //changable = false;
    int position = hexTextEdit->textCursor().position();
    if (position % 3 == 2)
    {
        QTextCursor txtCursor = hexTextEdit->textCursor();
        txtCursor.setPosition(position-2);
        hexTextEdit->setTextCursor(txtCursor);
    }
    else if (hexTextEdit->textCursor().hasSelection() == true)
    {
        QTextCursor txtCursor = hexTextEdit->textCursor();
        txtCursor.setPosition(position);
        hexTextEdit->setTextCursor(txtCursor);
    }
    //changable = true;
}


//void HexEdit::changeText()
//{
    //if (changable == true)
    //{
    //int column = hexTextEdit->textCursor().columnNumber();
    //int line = hexTextEdit->textCursor().blockNumber();
       // int position = hexTextEdit->textCursor().position();
       // if (position % 3 == 2)
      //      position = position-2;
     //   QString data = hexTextEdit->toPlainText();
    //    printf("%c%c, %d, %d\n", data[position].toAscii(),data[position+1].toAscii(), position, position/3);
        //zmeni se bytearray na pozici x,y (position/neco) na hodnotu position+(position+1) -> na QString::toInteger?
    //}
//}





void HexEdit::setData(QByteArray *byteArray)
{
    bool back = false;
    int line = 0;
    if (hexTextEdit->isReadOnly())
        hexTextEdit->setTextColor(Qt::darkGray);
    else
        hexTextEdit->setTextColor(Qt::black);
    for (int i=0; i<byteArray->size(); i++)
    {
        if (line == 0)
        {
            if (back == true)
            {
                hexTextEdit->setTextBackgroundColor(Qt::lightGray);
                back = false;
            }
            else
            {
                hexTextEdit->setTextBackgroundColor(Qt::white);
                back = true;
            }
        }
        if (byteArray->at(i) < 10)
            hexTextEdit->insertPlainText((QString::number(byteArray->at(i), 16)).toUpper());
        else
            hexTextEdit->insertPlainText("0" + (QString::number(byteArray->at(i), 16)).toUpper());
        line++;
        if (line == 16) 
        {
            hexTextEdit->insertPlainText("\n");
            line = 0;
        }
        else
            hexTextEdit->insertPlainText(" ");
    }
}



VerticalNumberCount::VerticalNumberCount(QWidget *parent)
{
    this->setMaximumWidth(455);
    this->setMaximumHeight(20);
    this->setMinimumWidth(455);
    this->setMinimumHeight(20);
}

void VerticalNumberCount::paintEvent(QPaintEvent *)
{
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,25,20);
    QPointF point;
    point.setY(0);
    QBrush brush(Qt::darkCyan);
    paint.setBackground(brush);
    QPen pen(Qt::darkCyan);
    paint.setBrush(brush);
    paint.setPen(pen);
    for (int i = 0; i<16; i++)
    {
        point.setX(i*24);
        rect.moveTopLeft(point);
        paint.drawRect(rect);
        pen.setColor(Qt::black);
        paint.setPen(pen);
        paint.drawText(rect, Qt::AlignCenter, "0" + QString::number(i, 16).toUpper());
        pen.setColor(Qt::darkCyan);
        paint.setPen(pen);
    }
    paint.end();
}




HorizontalNumberCount::HorizontalNumberCount(HexEdit *parent)
{
    this->parent = parent;
    //this->setMaximumWidth(20);
    this->setMaximumHeight(175);
    this->setMinimumWidth(20);
    this->setMinimumHeight(175);
}

void HorizontalNumberCount::paintEvent(QPaintEvent *)
{
    QPainter paint;
    paint.begin(this);
    QRectF rect(0,0,15,19);
    QPointF point;
    point.setX(0);
    QBrush brush(Qt::darkCyan);
    paint.setBackground(brush);
    QPen pen(Qt::darkCyan);
    paint.setBrush(brush);
    paint.setPen(pen);
    for (int i = 0; i<parent->getTextEdit()->document()->lineCount()-1 && i<150; i++)
    {
        point.setY(i*17);
        rect.moveTopLeft(point);
        paint.drawRect(rect);
        pen.setColor(Qt::black);
        paint.setPen(pen);
        paint.drawText(rect, Qt::AlignCenter, "0" + QString::number(i, 16).toUpper());
        pen.setColor(Qt::darkCyan);
        paint.setPen(pen);
    }
    paint.end();
}


