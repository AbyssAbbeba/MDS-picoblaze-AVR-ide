
#ifndef MAIN_C
#define MAIN_C


#include "widget.h"
#include <QtGui>
#include <QSignalMapper>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>
#include <QTextCursor>

Xxx::Xxx()
    : QWidget(0)
{


    ui.setupUi(this);
    flag_op = false;
    flag_result = 0;
    flag_operand1 = 0;
    flag_operand2 = 0;
    flag_equals   = 0;

    operand1_base = 0;
    operand2_base = 0;

    operator_out = QString("");
    operator_ = 0;
    ui.Vystup4->setReadOnly ( true );
    ui.Vystup4->setCursorWidth(0);
    ui.Vystup4->setOverwriteMode(true);


    base_indicator = 10;
    operand1 = QString("");
    operand2 = QString("");
    operator_string = QString("");
    prev_state = prev_dec;
    Cursor_textedit(Start);

    QRegExpValidator *decValidator = new QRegExpValidator(QRegExp("[0-9]{18}"), this);
        ui.Vystup->setValidator(decValidator);
        equals(operand1);

    signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(ui.push_0, 0);
    signalMapper->setMapping(ui.push_1, 1);
    signalMapper->setMapping(ui.push_2, 2);
    signalMapper->setMapping(ui.push_3, 3);
    signalMapper->setMapping(ui.push_4, 4);
    signalMapper->setMapping(ui.push_5, 5);
    signalMapper->setMapping(ui.push_6, 6);
    signalMapper->setMapping(ui.push_7, 7);
    signalMapper->setMapping(ui.push_8, 8);
    signalMapper->setMapping(ui.push_9, 9);
    signalMapper->setMapping(ui.push_add, 10);
    signalMapper->setMapping(ui.push_equals, 11);
    signalMapper->setMapping(ui.push_multiply, 12);
    signalMapper->setMapping(ui.push_substract, 13);
    signalMapper->setMapping(ui.AND, 14);
    signalMapper->setMapping(ui.OR, 15);
    signalMapper->setMapping(ui.XOR, 16);
    signalMapper->setMapping(ui.NOT, 17);
    signalMapper->setMapping(ui.Bit_shift, 18);
    signalMapper->setMapping(ui.push_div, 19);
    signalMapper->setMapping(ui.push_A, 20);
    signalMapper->setMapping(ui.push_B, 21);
    signalMapper->setMapping(ui.push_C, 22);
    signalMapper->setMapping(ui.push_D, 23);
    signalMapper->setMapping(ui.push_E, 24);
    signalMapper->setMapping(ui.push_F, 25);

    connect(ui.push_0,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_1,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_2,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_3,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_4,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_5,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_6,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_7,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_8,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_9,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_A,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_B,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_C,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_D,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_E,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_F,          SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_div,        SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_add,        SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_equals,     SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_multiply,   SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.push_substract,  SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.Bit_shift,       SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.AND       ,      SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.OR,              SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.XOR,             SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.NOT,             SIGNAL(clicked()), signalMapper, SLOT(map()));
    connect(ui.CA,              SIGNAL(clicked()), this, SLOT(CA_C()));
    connect(ui.C,               SIGNAL(clicked()), this, SLOT(CA_C()));
    connect(ui.Undo,            SIGNAL(clicked()), this, SLOT(Myundo()));
    connect(ui.Redo,            SIGNAL(clicked()), this, SLOT(Myredo()));
    connect(ui.buttonbin,       SIGNAL(clicked(bool)), this, SLOT(base_set_bin(bool)));
    connect(ui.buttondec,       SIGNAL(clicked(bool)), this, SLOT(base_set_dec(bool)));
    connect(ui.button_hex,      SIGNAL(clicked(bool)), this, SLOT(base_set_hex(bool)));

    connect(ui.Vystup,         SIGNAL(textEdited(QString)), this, SLOT( textChange2(QString)));
    // buttons
    connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(read_button(int)));
    connect(signalMapper, SIGNAL(mapped(int)),this, SLOT(Operator(int)));
}

Xxx::~Xxx()
{

}
void Xxx::textChange2()
{
    if ( flag_result == 1)
    {
        qDebug() << "done reset?";
        Cursor_textedit(reset);
        flag_result = 0;
    }
    equals(ui.Vystup->text());
}
void Xxx::textChange2(QString s)
{
    // display in bin
    // variables
    //unsigned long long int f = 0, g = 0, z = 0;

    // check set base and adjust input characters
    // decimal
    qDebug() << "flag result" << flag_result ;
    if ( flag_result == 1)
    {
        qDebug() << "done reset? in textchange";
        Cursor_textedit(reset);
        flag_result = 0;
    }
    s_perm = s;
    qDebug() << "nechcu : "<< s_perm;
    equals(ui.Vystup->text());

}
void Xxx::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "key";
   if(event->key() == Qt::Key_0)
   {
       read_button(0);
       textChange2();
   }
   if(event->key() == Qt::Key_1)
   {

       read_button(1);
       textChange2();

   }
   if(event->key() == Qt::Key_2)
   {
       read_button(2);
       textChange2();
   }
   if(event->key() == Qt::Key_3)
   {
       read_button(3);
       textChange2();
   }
   if(event->key() == Qt::Key_4)
   {
       read_button(4);
       textChange2();
   }
   if(event->key() == Qt::Key_5)
   {
       read_button(5);
       textChange2();
   }
   if(event->key() == Qt::Key_6)
   {
       read_button(6);
       textChange2();
   }
   if(event->key() == Qt::Key_7)
   {
       read_button(7);
       textChange2();
   }
   if(event->key() == Qt::Key_8)
   {
       read_button(8);
       textChange2();
   }
   if(event->key() == Qt::Key_9)
   {
       read_button(9);
       textChange2();
   }
   if(event->key() == Qt::Key_Enter)
   {
       Operator(11);

   }
   if(event->key() == Qt::Key_Return)
   {
       Operator(11);
   }
   if(event->key() == Qt::Key_Plus)
   {
       Operator(10);
   }
   if(event->key() == Qt::Key_Minus)
   {
       Operator(13);
   }
   if(event->key() == Qt::Key_Slash)
   {
       Operator(19);
   }
   if(event->key() == Qt::Key_Equal)
   {
       Operator(11);
   }
   if(event->key() == Qt::Key_A)
   {
       read_button(20);
       textChange2();
   }
   if(event->key() == Qt::Key_B)
   {
       read_button(21);
       textChange2();
   }
   if(event->key() == Qt::Key_C)
   {
       read_button(22);
       textChange2();
   }
   if(event->key() == Qt::Key_D)
   {
       read_button(23);
       textChange2();
   }
   if(event->key() == Qt::Key_E)
   {
       read_button(24);
       textChange2();
   }
   if(event->key() == Qt::Key_F)
   {
       read_button(25);
       textChange2();
   }
   if(event->key() == Qt::Key_Asterisk)
   {
       Operator(12);
   }
   if(event->key() == Qt::Key_division)
   {
       Operator(19);
   }
}


void Xxx::equals(QString s_result)
{
    display_bin(s_result);
    if ( 10 == base_indicator)
    {
        if ( prev_state == prev_bin)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,2),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,2),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,2),8));
        }
        if ( prev_state == prev_dec)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,10),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,10),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,10),8));
        }
        if ( prev_state == prev_hex)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,16),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,16),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,16),8));
        }
    ui.push_A->setDisabled(true);
    ui.push_B->setDisabled(true);
    ui.push_C->setDisabled(true);
    ui.push_D->setDisabled(true);
    ui.push_E->setDisabled(true);
    ui.push_F->setDisabled(true);
    ui.push_2->setEnabled(true);
    ui.push_3->setEnabled(true);
    ui.push_4->setEnabled(true);
    ui.push_5->setEnabled(true);
    ui.push_6->setEnabled(true);
    ui.push_7->setEnabled(true);
    ui.push_8->setEnabled(true);
    ui.push_9->setEnabled(true);


    }
    if ( 16 == base_indicator)
    {
        if ( prev_state == prev_bin)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,2),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,2),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,2),8));
        }
        if ( prev_state == prev_dec)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,10),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,10),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,10),8));
        }
        if ( prev_state == prev_hex)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,16),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,16),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,16),8));
        }
    ui.push_A->setEnabled(true);
    ui.push_B->setEnabled(true);
    ui.push_C->setEnabled(true);
    ui.push_D->setEnabled(true);
    ui.push_E->setEnabled(true);
    ui.push_F->setEnabled(true);
    ui.push_2->setEnabled(true);
    ui.push_3->setEnabled(true);
    ui.push_4->setEnabled(true);
    ui.push_5->setEnabled(true);
    ui.push_6->setEnabled(true);
    ui.push_7->setEnabled(true);
    ui.push_8->setEnabled(true);
    ui.push_9->setEnabled(true);

    }
    if ( 2 == base_indicator)
    {
        if ( prev_state == prev_bin)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,2),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,2),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,2),8));
        }
        if ( prev_state == prev_dec)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,10),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,10),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,10),8));
        }
        if ( prev_state == prev_hex)
        {
            ui.VystupHex->setText(QString::number(s_result.toLongLong(0,16),16).toUpper());
            ui.VystupDec->setText(QString::number(s_result.toLongLong(0,16),10));
            ui.VystupOct->setText(QString::number(s_result.toLongLong(0,16),8));
        }
    ui.push_A->setDisabled(true);
    ui.push_B->setDisabled(true);
    ui.push_C->setDisabled(true);
    ui.push_D->setDisabled(true);
    ui.push_E->setDisabled(true);
    ui.push_F->setDisabled(true);
    ui.push_2->setDisabled(true);
    ui.push_3->setDisabled(true);
    ui.push_4->setDisabled(true);
    ui.push_5->setDisabled(true);
    ui.push_6->setDisabled(true);
    ui.push_7->setDisabled(true);
    ui.push_8->setDisabled(true);
    ui.push_9->setDisabled(true);

    }
    return;
}

void Xxx::Cursor_textedit(int stav)//const char Number_char)
{
    QTextCursor cursor(this->ui.Vystup4->textCursor());

    if (  Start == stav)
    {
        ui.Vystup4->clear();
       // cursor.insertBlock();
       // cursor.insertBlock();
        //cursor.insertBlock();
        ui.Vystup4->setTextCursor(cursor);
        return;
    }
    if ( reset == stav)
    {
        ui.Vystup4->clear();
        //cursor.movePosition(QTextCursor::Start);
        //ui.Vystup4->setTextCursor(cursor);
        return;
    }

    if( prvni == stav)
    {
    // FIRST BLOCK
        cursor.movePosition(QTextCursor::Start);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.clearSelection();
        cursor.insertText(QString::number(operand1.toLongLong(0,operand1_base),10) + '\n');
        ui.Vystup4->setTextCursor(cursor);
        return;
    }

    if ( druhy == stav)
    {
     // SECOND BLOCK

        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.clearSelection();
        cursor.insertText(operator_out + '\n');
        ui.Vystup4->setTextCursor(cursor);
        return;
    }

    if ( treti == stav)
    {
     // THIRD BLOLOCK
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::Down);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.clearSelection();
        cursor.insertText(QString::number(operand1.toLongLong(0,operand2_base),10) + '\n');
        ui.Vystup4->setTextCursor(cursor);
        return;
    }
    if ( result == stav)
    {
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::Down);
        cursor.movePosition(QTextCursor::Down);
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.clearSelection();
        cursor.insertText(QString::number(end_result,10) + '\n');
        ui.Vystup4->setTextCursor(cursor);
        return;
    }
    return;
}

void Xxx::Operator(int i)
{
    // if pushbutton 0-9, return
    if ( 10 > i)
    {
        return;
    }
    //
    if ( 1 == flag_equals)
    {
        calc_reset();
    }

    switch (i)
    {
        case 19:    // display text of operand
                    ui.Vystup3->setText("    /");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    ui.Vystup->setText("");
                    operator_ = div_;
                    operator_out = QString(" / ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;

        case 10: // display text of operand
                    ui.Vystup3->setText("    +");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = add;
                    operator_out = QString(" + ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;


        case 12: // display text of operand
                    ui.Vystup3->setText("    *");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = mul;
                    operator_out = QString(" * ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;

        case 13: // display text of operand
                    ui.Vystup3->setText("    -");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = sub;
                    operator_out = QString(" - ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;

        case 14: // display text of operand
                    ui.Vystup3->setText("    AND");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = and_;
                    operator_out = QString(" AND ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;
//OR
        case 15:
                    ui.Vystup3->setText("    OR");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = or_;
                    operator_out = QString(" OR ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;

        case 16:
                    ui.Vystup3->setText("    XOR");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = xor_;
                    operator_out = QString(" XOR ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;

        case 17:
                    ui.Vystup3->setText("    NOT");
                    // if op1 doesnt exist, create one
                    if ( 0 == flag_operand1)
                    {
                        operand1 = ui.Vystup->text();
                        operand1_base = base_indicator;
                        flag_operand1 = 1;
                        ui.Vystup->setText("");
                    }
                    // set operator
                    operator_ = xor_;
                    operator_out = QString(" NOT ");

                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
        break;

        case 18: ui.Vystup3->setText("   >>");
                    operand1 = ui.Vystup->text();
                    ui.Vystup->setText("");
                    Cursor_textedit(prvni);
                    Cursor_textedit(druhy);
                    break;

// special button
        case 11:
       // flag_result,flag_operand1, flag_operand2, flag_equals;
            flag_equals = 1;
            if ( operator_ == NULL || operand1 == NULL)
            {
                return;
            }
            operand2 = ui.Vystup->text();
            operand1_base = base_indicator;
            flag_operand2 = 1;
            Cursor_textedit(treti);
            qDebug() << " pred result 1 :" << operand1;
            qDebug() << " pred result 2 :" << operand2;
            get_result();
            // dodelat pocatecni status here
            Cursor_textedit(result);
        break;
     //
        default:
            return;
        break;
    }
    return;
}
void Xxx::get_result()
{

    operand_long1 = 0;
    operand_long2 = 0;
    end_result = 0;

    // get operands in longlong int
    operand_long1   =   operand1.toLongLong(0, base_indicator);
    operand_long2   =   operand2.toLongLong(0, base_indicator);
            qDebug() << " op1 long long :" << operand_long1;
            qDebug() << " op2 longlong :" << operand_long2;
    switch (operator_)
        {
            case div_:
                    operator_string = QString ("/");
                    end_result = operand_long1 / operand_long2;
                    qDebug() << " div result :" << end_result;
                break;
            case mul:
                    operator_string = QString ("*");
                    end_result = operand_long1 * operand_long2;
                    qDebug() << " mul result :" << end_result;
                break;
            case add:
                    operator_string = QString ("+");
                    end_result = operand_long1 + operand_long2;
                    qDebug() << " add result :" << end_result;
                break;
            case sub:
                    operator_string = QString ("-");
                    end_result = operand_long1 - operand_long2;
                    qDebug() << " sub result :" << end_result;
                break;
            case xor_:
                    operator_string = QString ("XoR");
                    end_result = operand_long1 ^ operand_long2;
                    qDebug() << " sub result :" << end_result;
                break;
            case or_:
                    operator_string = QString ("oR");
                    end_result = operand_long1 | operand_long2;
                    qDebug() << " sub result :" << end_result;
                break;
            case and_:
                    operator_string = QString ("AND");
                    end_result = operand_long1 & operand_long2;
                    qDebug() << " sub result :" << end_result;
                break;
            case not_:
                    operator_string = QString ("NoT");
                    end_result = operand_long1 + operand_long2;
                    qDebug() << " sub result :" << end_result;
                break;
            default:
                return;
            break;
        }
    qDebug() << "nastavi se flag ? :";
    flag_result = 1;
    return;
}

void Xxx::read_button(int i)
{
    if ( 1 == flag_equals)
    {
        calc_reset();
    }

    switch  (i)
    {
        case 0:
                    ui.Vystup->setText(ui.Vystup->text() + "0");
                  //  textChange2(QString (ui.Vystup->text() + "0"));
            break;
        case 1:
                    ui.Vystup->setText(ui.Vystup->text() + "1");
            break;
        case 2:
                    ui.Vystup->setText(ui.Vystup->text() + "2");
            break;
        case 3:
                    ui.Vystup->setText(ui.Vystup->text() + "3");
            break;
        case 4:
                    ui.Vystup->setText(ui.Vystup->text() + "4");
            break;
        case 5:
                    ui.Vystup->setText(ui.Vystup->text() + "5");
            break;
        case 6:
                    ui.Vystup->setText(ui.Vystup->text() + "6");
            break;
        case 7:
                    ui.Vystup->setText(ui.Vystup->text() + "7");
            break;
        case 8:
                    ui.Vystup->setText(ui.Vystup->text() + "8");
            break;
        case 9:
                    ui.Vystup->setText(ui.Vystup->text() + "9");
            break;
        case 20:
                    ui.Vystup->setText(ui.Vystup->text() + "A");
            break;
        case 21:
                    ui.Vystup->setText(ui.Vystup->text() + "B");
            break;
        case 22:
                    ui.Vystup->setText(ui.Vystup->text() + "C");
            break;
        case 23:
                    ui.Vystup->setText(ui.Vystup->text() + "D");
            break;
        case 24:
                    ui.Vystup->setText(ui.Vystup->text() + "E");
            break;
        case 25:
                    ui.Vystup->setText(ui.Vystup->text() + "F");
            break;
    // return
    default:
            return;
        break;
            // translation to other bases
            equals(ui.Vystup->text());
    }
}

void Xxx:: CA_C()
{
    //chop ( int n )
    ui.Vystup->clear();
    ui.Vystup3->clear();
    ui.Vystup4->clear();
    ui.VystupHex->clear();
    ui.VystupDec->clear();
    ui.VystupOct->clear();
    //qDebug() << "LineEditConvert: Text set:" << Number_int;
}

void Xxx:: Myredo()
{
    ui.Vystup->redo();
    ui.Vystup3->redo();
    ui.Vystup4->redo();
    ui.VystupHex->redo();
    ui.VystupDec->redo();
    ui.VystupOct->redo();
}
void Xxx:: Myundo()
{
    ui.Vystup->undo();
    ui.Vystup3->undo();
    ui.Vystup4->undo();
    ui.VystupHex->undo();
    ui.VystupDec->undo();
    ui.VystupOct->undo();
}

void Xxx::base_set_bin(bool b)
{
    if ( true == b)
    {
    base_indicator = 2;
    s_perm = ui.Vystup->text();
    qDebug() << "sperm u bin: "<< s_perm;


    QRegExpValidator *binValidator = new QRegExpValidator(QRegExp("[01]{63}"), this);
    ui.Vystup->setValidator(binValidator);

    if ( prev_state == prev_hex)
    {
        ui.Vystup->setText(QString::number(s_perm.toLongLong(0,16),2));
    }
    if ( prev_state == prev_bin)
    {
        ui.Vystup->setText(QString::number(s_perm.toLongLong(0,2),2));
    }
    if ( prev_state == prev_dec)
    {
        ui.Vystup->setText(QString::number(s_perm.toLongLong(0,10),2));
    }
    qDebug() << "za u bin: "<< s_perm;
    }
    equals(s_perm);
    prev_state = prev_bin;
}
void Xxx::base_set_hex(bool h)
{

    if ( true == h)
    {
    base_indicator = 16;
    s_perm = ui.Vystup->text();
    qDebug() << "sperm u hex: "<< s_perm;


    QRegExpValidator *hexValidator = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{16}"), this);
    ui.Vystup->setValidator(hexValidator);
    if ( prev_state == prev_hex)
    {
        ui.Vystup->setText(QString::number(s_perm.toLongLong(0,16),16).toUpper());
    }
    if ( prev_state == prev_bin)
    {
        ui.Vystup->setText(QString::number(s_perm.toLongLong(0,2),16).toUpper());
    }
    if ( prev_state == prev_dec)
    {
        ui.Vystup->setText(QString::number(s_perm.toLongLong(0,10),16).toUpper());
    }
    }
    equals(s_perm);
    prev_state = prev_hex;
}
void Xxx::base_set_dec(bool d)
{
    if ( true == d)
    {

    base_indicator = 10;

    s_perm = ui.Vystup->text();
    qDebug() << "sperm u dec: "<< s_perm;

    QRegExpValidator *decValidator = new QRegExpValidator(QRegExp("[0-9]{18}"), this);
    ui.Vystup->setValidator(decValidator);

        if ( prev_state == prev_hex)
        {
            ui.Vystup->setText(QString::number(s_perm.toLongLong(0,16),10));
        }
        if ( prev_state == prev_bin)
        {
            ui.Vystup->setText(QString::number(s_perm.toLongLong(0,2),10));
        }
        if ( prev_state == prev_dec)
        {
            ui.Vystup->setText(QString::number(s_perm.toLongLong(0,10),10));
        }
    }
    equals(s_perm);
    prev_state = prev_dec;
}

void Xxx::calc_reset(void)
{
    flag_op = false;
    flag_result = 0;
    flag_operand1 = 0;
    flag_operand2 = 0;
    flag_equals   = 0;


    operand1_base = 0;
    operand2_base = 0;

    operator_out = QString("");
    operator_ = 0;

    operand1 = QString("");
    operand2 = QString("");
    operator_string = QString("");
    Cursor_textedit(reset);
    return;
}

void Xxx::display_bin(QString bin_string)
{
    // variables
    QString variable = 0;
    long long int   display = 0;
    int i = 0;

        if ( prev_state == prev_hex)
        {
            i = 16;
        }
        if ( prev_state == prev_bin)
        {
            i = 2;
        }
        if ( prev_state == prev_dec)
        {
            i = 10;
        }
    // go >>

    display = bin_string.toLongLong(0,i);
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L1->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 4;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L2->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 8;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L3->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 12;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L4->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 16;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L5->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 20;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L6->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 24;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L7->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 28;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L8->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 32;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L9->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 36;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L10->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 40;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L11->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 44;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L12->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 48;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L13->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 52;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L14->setText(variable);
//-----------------------------------------
    display = bin_string.toLongLong(0,i);
    display >>= 56;
    display &= 0x000000000000000F;
    variable = QString::number(display,2);
        for (int i = variable.size(); i < 4 ; i++)
        {
            variable.prepend("0");
        }
    ui.L15->setText(variable);
//-----------------------------------------
        display = bin_string.toLongLong(0,i);
        display >>= 60;
        display &= 0x000000000000000F;
        variable = QString::number(display,2);
            for (int i = variable.size(); i < 4 ; i++)
            {
                variable.prepend("0");
            }
        ui.L16->setText(variable);
    return;
}



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Xxx xxx;
   // Calc calc;
    xxx.show();
    return app.exec();
}
#endif
