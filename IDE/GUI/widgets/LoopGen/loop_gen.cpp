/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Madron <martin.madron@moravia-microsystems.com>
 * @ingroup ComboBox
 * @file loop_gen.cpp
 */


#include "loop_gen.h"
#include "ui_loop_gen.h"
#include "../Highlighter/highlighter.h"
#include "../../guicfg/guicfg.h"
#include <QtGui>

#define max             8589803504
#define max_fornow      161631

loop_gen::loop_gen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loop_gen)
 //   loop_gen::loop_gen() :
  //      QWidget(0)
{
    ui->setupUi(this);

    index = "";
    //Frekvency = 10000;
    QString nS = "nS";
    QString uS = "uS";
    QString mS = "mS";
    QString S = "S";
    QString DJNZ   = "DJNZ";
   // QString IJNZ   = "IJNZ";
   // QString ADD    = "ADD";
    QString SUB    = "SUB";

    QString Hz     = "Hz";
    QString kHz     = "kHz";
    QString MHz     = "MHz";

    ui->Box_F->addItem(nS);
    ui->Box_F->addItem(uS);
    ui->Box_F->addItem(mS);
    ui->Box_F->addItem(S);

    ui->Box_Instructions->addItem(DJNZ);
   // ui->Box_Instructions->addItem(IJNZ);
   // ui->Box_Instructions->addItem(ADD);
    ui->Box_Instructions->addItem(SUB);

    ui->Box_Frekvency->addItem(Hz);
    ui->Box_Frekvency->addItem(kHz);
    ui->Box_Frekvency->addItem(MHz);

    ui->Box_Frekvency->setCurrentIndex(2);
    ui->Box_F->setCurrentIndex(2);

    ui->line_reg1->setDisabled(true);
    ui->line_reg2->setDisabled(true);
    ui->line_reg3->setDisabled(true);
    ui->line_reg4->setDisabled(true);
    ui->line_reg5->setDisabled(true);
    ui->line_reg6->setDisabled(true);

    QRegExpValidator *decValidator = new QRegExpValidator(QRegExp("[0-9]{18}"), this);
    ui->Vstup_Cas->setValidator(decValidator);
    ui->Vstup_Cycles->setValidator(decValidator);

    ui->Vystup_text->setFont(GuiCfg::getInstance().getEditorFont());
    Highlighter *highlighter = new Highlighter(ui->Vystup_text->document(), SourceType::PICOBLAZEASM);

    ui->Vstup_Cycles->setDisabled(true);
    
    connect(ui->Vstup_Cas,      SIGNAL(textChanged(QString)), this, SLOT(TimeChanged(QString)));
    connect(ui->push_Generate,  SIGNAL(clicked()), this, SLOT( Generate()));
    connect(ui->push_Copy,      SIGNAL(clicked()), this, SLOT(CopyToClipboard()));
    connect(ui->check_default,  SIGNAL(clicked()), this, SLOT( Default_click()));
    connect(ui->radio_Cycles,   SIGNAL(clicked()), this, SLOT( Cycles_click()));
    connect(ui->radio_Time,     SIGNAL(clicked()), this, SLOT( Time_click()));

    //register values
    temp1 = 0;
    temp2 = 0;
    temp3 = 0;
    temp4 = 0;
    temp5 = 0;
    temp6 = 0;
}


void loop_gen::Cycles_click()
{
    ui->Vstup_Cas->setDisabled(true);
}


void loop_gen::Time_click()
{
    ui->Vstup_Cas->setEnabled(true);
    ui->Vstup_Cycles->setEnabled(true);
}


void loop_gen::reg_1_Loop( unsigned long long loop, unsigned int init_nop)
{
    // number of added nops
    if ( (loop % 2) == 0 )
    {
        init_nop = 1;
    }
    else
    {
        init_nop = 0;
    }

    // calculate register numbers
    temp1 = (loop - init_nop) / 2;
}


void loop_gen::reg_2_Loop( unsigned long long loop, unsigned int init_nop)
{
    unsigned long long local3, temp_help = 0;

    // calculate register numbers
    // 1024 - 132000 - init_nop
    //temp2 = (Steps -  2 - init_nop) / 512;
    temp2 = loop;
    temp2 -= init_nop;
    temp2 /= 512;
   // temp2++;
    local3 = loop - init_nop;
    local3 -= (temp2 * 2);

    local3 -=  512 * (temp2 - 1);

    if ( local3 >= 512)
    {
        temp_help = local3 - 512;
        if ( temp_help == 0)
        {

            //temp1 = temp_help + 1;
            temp1 = 255;
        }
        else
        {//rolback

            temp_help /= 2;
            temp_help--;
            if ( temp_help == 0)
            {
                init_nop += 2;
            }
            else
            {
                temp2++;
                qDebug() << "help number" << temp_help;
            }
            temp1 = temp_help;
        }
    }
    else
    {
    local3 /=  2;
    temp1 = local3;
    }
    if ( temp2 == 256)
    {
        temp2 = 0;
    }
    return;
}


unsigned long long loop_gen::reg_3_Loop( unsigned long long loop, unsigned int init_nop)
{
    // calculate register numbers
    // 1024 - 132000 - init_nop
    unsigned long long local3 = 0, temp_cycles = 0;
    //-------------------------------------------
    temp_cycles = loop - init_nop;

    temp3 = temp_cycles / 131584;
    temp3++;
    temp_cycles -= ((temp3) * 2);

    qDebug() << "temp3 - deleni: " << temp3;
    // s3 mame
    // ted zbytek

    temp_cycles -= ( 131584 *  (temp3 - 1));

    if ( temp3 == 256)
    {
        temp3 = 0;
    }

    qDebug() << "predana hodnota  :" << temp_cycles;
    return temp_cycles;
}


unsigned long long loop_gen::reg_4_Loop( unsigned long long loop, unsigned int init_nop)
{
    // calculate register numbers
    // 1024 - 132000 - init_nop
    unsigned long long local3 = 0, temp_cycles = 0;
    //-------------------------------------------
    temp_cycles = loop - init_nop;

    temp4 = temp_cycles / 33686016;
    temp4++;
    temp_cycles -= ((temp4) * 2);

    qDebug() << "temp3 - deleni: " << temp4;
    // s4 mame
    // ted zbytek

    temp_cycles -= ( 33686016 *  (temp4 - 1));

    if ( temp4 == 256)
    {
        temp4 = 0;
    }

    qDebug() << "predana hodnota  :" << temp_cycles;
    return temp_cycles;
}


unsigned long long loop_gen::reg_5_Loop( unsigned long long loop, unsigned int init_nop)
{
    // calculate register numbers
    // 1024 - 132000 - init_nop
    unsigned long long local3 = 0, temp_cycles = 0;
    //-------------------------------------------
    temp_cycles = loop - init_nop;

    temp5 = temp_cycles / 8623620608ULL;
    temp5++;
    temp_cycles -= ((temp5) * 2);

    qDebug() << "temp5 - deleni: " << temp5;
    // s4 mame
    // ted zbytek

    temp_cycles -= ( 8623620608ULL *  (temp5 - 1));

    if ( temp5 == 256)
    {
        temp5 = 0;
    }

    qDebug() << "predana hodnota  :" << temp_cycles;
    return temp_cycles;
}


unsigned long long loop_gen::reg_6_Loop( unsigned long long loop, unsigned int init_nop)
{
    // calculate register numbers
    // 1024 - 132000 - init_nop
    unsigned long long local3 = 0, temp_cycles = 0;
    //-------------------------------------------
    temp_cycles = loop - init_nop;

    temp6 = temp_cycles / 2207646876160ULL;
    temp6++;
    temp_cycles -= ((temp6) * 2);

    qDebug() << "temp5 - deleni: " << temp6;
    // s4 mame
    // ted zbytek

    temp_cycles -= ( 2207646876160ULL *  (temp6 - 1));

    if ( temp6 == 256)
    {
        temp6 = 0;
    }

    qDebug() << "predana hodnota  :" << temp_cycles;
    return temp_cycles;
}


double loop_gen::TimeToCycles()
{
    QString temp1 = 0;
    unsigned long setTimeFrame = 0;

    if ( ui->radio_Cycles->isChecked() == true)
    {
        temp1 = ui->Vstup_Cycles->text();        
       // ui->Vstup_Cas->setDisabled();

        // get inserted frekvency value
       // setTimeFrame = get_frekvency();

        // calculate number of cycles
        //CycleTime = ((1 / Frekvency) * setTimeFrame) * temp1;

        //ui->Vstup_Cas->setText( QString::number( CycleTime ));
        Steps = temp1.toDouble() / 2;
        return temp1.toDouble();                         // dod2lat steps
    }
    ui->Vstup_Cycles->setDisabled(false);
    // get inserted frekvency value
    setTimeFrame = get_frekvency();
    qDebug()<< " f:"<< Frekvency;

    // calculate number of cycles
    CycleTime = (1 / Frekvency) * (double)setTimeFrame;
    qDebug()<< " Cycle Time:"<< CycleTime;

    // get time of the step
    StepTime = CycleTime * 2;
    qDebug()<< " Step Time:"<< StepTime;

        //Cycles = ;
        qDebug()<< " Time:"<< Time;
        if ( (Time / StepTime) < 1)
        {
            qDebug() << " Cycles lesser than inserted time";
            return 0;
        }
        Cycles = Time / CycleTime;
        qDebug() << " Cycles" << Cycles;

        Steps = Time / StepTime;
        qDebug() << " Steps" << Steps;

    return Steps;
}


void loop_gen::TimeChanged ( QString Time_change )
{
    //double cyka;
    Time = Time_change.toDouble();

  //  cyka = TimeToCycles();
  //  qDebug() << "Time has changed :" << Time;
}


void loop_gen::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        Generate();
    }
}


void loop_gen::Generate ( void)
{
    // chceck if Frekvency field is empty
    if ( ui->Vstup_Frekvency->text() == NULL )
    {
        ui->Vystup_text->clear();
        ui->Vystup_text->setText( " Insert valid Frekvency number" );
        return;
    }
    CyclesFinal = TimeToCycles();
    if ( CyclesFinal == 0)
    {
        ui->Vystup_text->setText(" Cycles lesser than inserted time");
        return;
    }
    if ( ui->radio_Cycles->isChecked() == false )
    {
        ui->Vstup_Cycles->setText(QString::number( Cycles, 'f', 0 ) );
    }
    
    GenerateLoop();
}


void loop_gen::GenerateLoop(void)
{

    if ( 3 >= Steps)
    {
        loop( nop_);
    }
    else if ( (512 >= Steps) && (Steps >= 3) )
    {
        loop( first_1r);
    }
    else if ( (768 >= Steps) && (Steps >= 512) )
    {
        loop( second_1r);
    }
    else if ( (1024 >= Steps) && (Steps >= 768) )
    {
        loop( third_1r);
    }
    else if ( (132086 >= Steps) && (Steps >= 1024) )
    {
        loop( first_2r);
    }
    else if ( (196351 >= Steps) && (Steps >= 131586) )
    {
        loop( second_2r);
    }
    else if ( (261631 >= Steps) && (Steps >= 196351) )
    {
        loop( third_2r);
    }
    else if ( (33686019 >= Steps) && (Steps >= 261631) )
    {
        loop( first_3r);
    }
    else if ( ( 8623620868ULL>= Steps) && (Steps >= 33686019ULL) )
    {
        loop( first_4r);
    }
    else if ( ( 2207646876165ULL >= Steps) && (Steps >= 8623620868ULL) )
    {
        loop( first_5r);
    }
    else if ( ( 565157600297478ULL >= Steps) && (Steps >= 2207646876165ULL ) )
    {
        loop( first_6r);
    }
    else
    {
        loop( impossible);
    }
    return;
}


void loop_gen::loop(unsigned int type)
{
    QTextCursor cursor(ui->Vystup_text->textCursor());

    unsigned long long pokus;
    QString s1,s2,s3,s4,s5,s6;
    QString line1,line2,line3, line4, line5, line6, line7, line8, line9, line10,line11, line12, line13, line14, line15, line16, line17, line18, line19, line20, line21, line22, linemacro, endmacro, localline, line_nop,line_nop1,line_nop2;
    QString instr1 = "nop", instr2 = "sub", instr3 = "djnz", instr4 = "Delay", instr5 = "load"
            , instr6 = "jump", instr7, macro = "Macro", macroend = "endm", wait = "Wait", local = "local";

    unsigned int init_nop;
    // get names of used registers

        s1 = ui->line_reg1->text();
        s2 = ui->line_reg2->text();
        s3 = ui->line_reg3->text();
        s4 = ui->line_reg4->text();
        s5 = ui->line_reg5->text();
        s6 = ui->line_reg6->text();

        // printing
        ui->Vystup_text->clear();

        if ( ui->check_Comments->isChecked() == true )
        {
            if ( ui->radio_Cycles->isChecked() == true )
            {
                cursor.insertText(";Generated loop for " + QString::number(CyclesFinal, 'f', 0)
                                  + " cycles \n");
                cursor.insertText(";----------------------------------\n");
                ui->Vystup_text->setTextCursor(cursor);
            }
            else
            {
                cursor.insertText(";Generated loop for " + QString::number(CyclesFinal * 2, 'f', 0)
                                  + " cycles, " + QString::number(Time, 'f', 0) + " " + index + "\n");
                cursor.insertText(";----------------------------------\n");
                ui->Vystup_text->setTextCursor(cursor);

            }
        }


        // if macro is ON, add MACRO SZNTAX
        if ( true == ui->Button_Macro->isChecked())
        {
            if ( ui->check_upper->isChecked() == true )
            {
                linemacro = QString ("%1             %2\n").arg(wait.toUpper(),macro.toUpper());
                localline = QString ("             %1      %2\n\n").arg(local.toUpper(), instr4.toUpper());
            }
            else
            {
                linemacro = QString ("%2             %1\n").arg(macro, wait);
                localline = QString ("             %1      %2\n\n").arg(local, instr4);
            }
            cursor.insertText(linemacro);
            cursor.insertText(localline);
        }


    // main switchhhhhh
    switch ( type )
    {
        case nop_:

            cursor.insertText("DELAY:\n");
            for (int i=1; i <= CyclesFinal; i++)
            {
                cursor.insertText("         NOP\n");
            }
            ui->Vystup_text->setTextCursor(cursor);
            break;
    //=======================================================================================================
        case first_1r:


            pokus = Steps;
            // number of added nops
            if ( (pokus % 2) == 0 )
            {
                init_nop = 1;
            }
            else
            {
                init_nop = 0;
            }

            // calculate register numbers
            temp1 = (Steps - init_nop) / 2;
            qDebug() << " register value" << temp1;

            // printing

            // print delay loop
            //          NOP
            //          LOAD    s1,num
            //
            //Delay:
            //          SUB     s1,1
            //          JUMP    NZ,Delay
            if ( ui->check_upper->isChecked() == true )
            {
                if ( init_nop == 1)
                {
                    line1 = QString      ("             %1\n").arg(instr1.toUpper());
                }
                    line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n\n").arg(instr5.toUpper(),s1.toUpper());
                    line3 = QString      ("%1:\n").arg(instr4.toUpper());
                    if ( ui->Box_Instructions->currentText() == "DJNZ")
                    {
                        line4 = QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                    }
                    else
                    {
                        line4 = QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                        line5 = QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                    }
            }
            else
            {
                if ( init_nop == 1)
                {
                        line1 = QString  ("             %1\n").arg(instr1);
                }
                    line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n\n").arg(instr5,s1);
                    line3 = QString      ("%1:\n").arg(instr4);
                    if ( ui->Box_Instructions->currentText() == "DJNZ")
                    {
                        line4 = QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                    }
                    else
                    {
                        line4 = QString  ("             %1           %2,#1\n").arg(instr2,s1);
                        line5 = QString  ("             %1        NZ,%2\n").arg(instr6,instr4);
                    }
            }


           if ( ui->Box_Instructions->currentText() == "DJNZ")
           {
                cursor.insertText(line1);
                cursor.insertText(line2);
                cursor.insertText(line3);
                cursor.insertText(line4 + "\n");
                if ( true == ui->Button_Macro->isChecked())
                {
                    if ( ui->check_upper->isChecked() == true )
                    {
                        endmacro = QString ("%1").arg(macroend.toUpper());
                        cursor.insertText(endmacro + "\n");
                    }
                    else
                    {
                        endmacro = QString ("%1").arg(macroend);
                        cursor.insertText(endmacro + "\n");
                    }
                }
           }
           else
           {
                cursor.insertText(line1);
                cursor.insertText(line2);
                cursor.insertText(line3);
                cursor.insertText(line4);
                cursor.insertText(line5 + "\n");
                if ( true == ui->Button_Macro->isChecked())
                {
                    if ( ui->check_upper->isChecked() == true )
                    {
                        endmacro = QString ("%1").arg(macroend.toUpper());
                        cursor.insertText(endmacro + "\n");
                    }
                    else
                    {
                        endmacro = QString ("%1").arg(macroend);
                        cursor.insertText(endmacro + "\n");
                    }
                }
           }
            cursor.insertText(";---------------------------------------------");
            ui->Vystup_text->setTextCursor(cursor);
            break;
        //=======================================================================================================
            case second_1r:

        pokus = Steps;
        // number of added nops
        if ( (pokus % 3) == 0 )
        {
            init_nop = 2;
            qDebug() << "init nops " << init_nop;
        }
        if ( (pokus % 3) == 1 )
        {
            init_nop = 0;
            qDebug() << "init nops " << init_nop;
        }
        if ( (pokus % 3) == 2 )
        {
            init_nop = 1;
            qDebug() << "init nops " << init_nop;
        }

        // calculate register numbers
        temp1 = (Steps - init_nop) / 3;
        qDebug() << " register value" << temp1;

        // printing

        // print delay loop
        //          NOP
        //          NOP
        //          LOAD    s1,num
        //
        //Delay:
        //          SUB     s1,1
        //          JUMP    NZ,Delay
        if ( ui->check_upper->isChecked() == true )
        {
            if ( init_nop == 1)
            {
                line1 = QString      ("             %1\n").arg(instr1.toUpper());
            }
            if ( init_nop == 2)
            {
                line1 = QString      ("             %1\n             %1\n").arg(instr1.toUpper());
               // line1 = QString      ("             %1\n").arg(instr1.toUpper());
            }
                line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n\n").arg(instr5.toUpper(),s1.toUpper());
                line3 = QString      ("%1:\n").arg(instr4.toUpper());
                if ( ui->Box_Instructions->currentText() == "DJNZ")
                {
                    line_nop = QString  ("             %1\n").arg(instr1.toUpper());
                    line4 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                }
                else
                {
                    line_nop = QString  ("             %1\n").arg(instr1.toUpper());
                    line4 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                    line5 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                }
        }
        else
        {
            if ( init_nop == 1)
            {
                line1 = QString      ("             %1\n").arg(instr1);
            }
            else if ( init_nop == 2)
            {
                line1 = QString      ("             %1\n").arg(instr1);
                line1 = QString      ("             %1\n").arg(instr1);
            }
                line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n\n").arg(instr5,s1);
                line3 = QString      ("%1:\n").arg(instr4);
                if ( ui->Box_Instructions->currentText() == "DJNZ")
                {
                    line_nop = QString  ("             %1\n").arg(instr1);
                    line4 = QString     ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                }
                else
                {
                    line_nop = QString  ("             %1\n").arg(instr1);
                    line4 = QString     ("             %1           %2,#1\n").arg(instr2,s1);
                    line5 = QString     ("             %1         NZ,%2\n").arg(instr6,instr4);
                }
        }


       if ( ui->Box_Instructions->currentText() == "DJNZ")
       {
            cursor.insertText(line1);
            cursor.insertText(line2);
            cursor.insertText(line3);
            cursor.insertText(line_nop);
            cursor.insertText(line4 + "\n");
            if ( true == ui->Button_Macro->isChecked())
            {
                if ( ui->check_upper->isChecked() == true )
                {
                    endmacro = QString ("%1").arg(macroend.toUpper());
                    cursor.insertText(endmacro + "\n");
                }
                else
                {
                    endmacro = QString ("%1").arg(macroend);
                    cursor.insertText(endmacro + "\n");
                }
            }
       }
       else
       {
            cursor.insertText(line1);
            cursor.insertText(line2);
            cursor.insertText(line3);
            cursor.insertText(line_nop);
            cursor.insertText(line4);
            cursor.insertText(line5 + "\n");
            if ( true == ui->Button_Macro->isChecked())
            {
                if ( ui->check_upper->isChecked() == true )
                {
                    endmacro = QString ("%1").arg(macroend.toUpper());
                    cursor.insertText(endmacro + "\n");
                }
                else
                {
                    endmacro = QString ("%1").arg(macroend);
                    cursor.insertText(endmacro + "\n");
                }
            }
       }
        cursor.insertText(";---------------------------------------------");
        ui->Vystup_text->setTextCursor(cursor);
        break;
        //=======================================================================================================
            case third_1r:
            break;
        //=======================================================================================================
            case first_2r:

                registers = 2;

                pokus = Steps - 2;
                // number of added nops
                if ( (pokus % 2) == 0 )
                {
                    init_nop = 0;
                    qDebug() << "init nops " << init_nop;
                }
                if ( (pokus % 2) == 1 )
                {
                    init_nop = 1;
                    qDebug() << "init nops " << init_nop;
                }

                reg_2_Loop(pokus, init_nop);


                qDebug() << " register value 1 " << temp1;
                qDebug() << " register value 2 " << temp2;

                // now print

                if ( ui->check_upper->isChecked() == true )
                {
                    if ( init_nop == 1)
                    {
                        line1 = QString      ("             %1\n").arg(instr1.toUpper());
                    }
                        line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
                        line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n\n").arg(instr5.toUpper(),s2.toUpper());
                        line4 = QString      ("%1:\n").arg(instr4.toUpper());
                        if ( ui->Box_Instructions->currentText() == "DJNZ")
                        {
                            line5 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                            line6 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                        }
                        else
                        {
                            line5 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                            line6 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                            line7 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                            line8 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                        }
                }
                else
                {
                    if ( init_nop == 1)
                    {
                        line1 = QString      ("             %1\n").arg(instr1);
                    }
                        line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
                        line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n\n").arg(instr5,s2);
                        line4 = QString      ("%1:\n").arg(instr4);
                        if ( ui->Box_Instructions->currentText() == "DJNZ")
                        {
                            line5 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                            line6 =    QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                        }
                        else
                        {
                            line5 =    QString  ("             %1          %2,#1\n").arg(instr2,s1);
                            line6 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                            line7 =    QString  ("             %1          %2,#1\n").arg(instr2,s2);
                            line8 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                        }
                }

            // 2400 nefacha
                if ( ui->Box_Instructions->currentText() == "DJNZ")
                {
                     cursor.insertText(line1);
                     cursor.insertText(line2);
                     cursor.insertText(line3);
                     cursor.insertText(line4);
                     cursor.insertText(line5);
                     cursor.insertText(line6 + "\n");
                     if ( true == ui->Button_Macro->isChecked())
                     {
                         if ( ui->check_upper->isChecked() == true )
                         {
                             endmacro = QString ("%1").arg(macroend.toUpper());
                             cursor.insertText(endmacro + "\n");
                         }
                         else
                         {
                             endmacro = QString ("%1").arg(macroend);
                             cursor.insertText(endmacro + "\n");
                         }
                     }
                }
                else
                {
                     cursor.insertText(line1);
                     cursor.insertText(line2);
                     cursor.insertText(line3);
                     cursor.insertText(line4);
                     cursor.insertText(line5);
                     cursor.insertText(line6);
                     cursor.insertText(line7);
                     cursor.insertText(line8 + "\n");
                     if ( true == ui->Button_Macro->isChecked())
                     {
                         if ( ui->check_upper->isChecked() == true )
                         {
                             endmacro = QString ("%1").arg(macroend.toUpper());
                             cursor.insertText(endmacro + "\n");
                         }
                         else
                         {
                             endmacro = QString ("%1").arg(macroend);
                             cursor.insertText(endmacro + "\n");
                         }
                     }
                }
                 cursor.insertText(";---------------------------------------------");
                 ui->Vystup_text->setTextCursor(cursor);
            break;

         //=======================================================================================================
             case second_2r:
                 pokus = Steps;

                 // number of added nops
                 if ( (pokus % 3) == 1 )
                 {
                     init_nop = 0;
                     qDebug() << "init nops " << init_nop;
                 }
                 if ( (pokus % 3) == 2 )
                 {
                     init_nop = 1;
                     qDebug() << "init nops " << init_nop;
                 }
                 if ( (pokus % 3) == 0 )
                 {
                     init_nop = 2;
                     qDebug() << "init nops " << init_nop;
                 }

                 // calculate register numbers
                 // 1024 - 132000 - init_nop
                 unsigned long long local;
                 temp2 = (Steps -  2 - init_nop) / 768;
                 temp1 = (Steps -  2 - init_nop) - 768 * (temp2 - 1);

                 local = temp1;
                 local -= (temp2 * 2);
                 local /=  3;
                 qDebug() << " Local :" << local;
                 if ( local > 255 )
                 {
                     temp2++;
                     temp1 -= temp2 * 2;
                     temp1 -= 768;
                     temp1 /=  3;
                    // temp1 -= 255;
                 }
                 else
                {
                    temp1 -= (temp2 * 2);
                    temp1 /=  3;
                }

                 qDebug() << " register value" << temp1;
                 qDebug() << " register value" << temp2;

                 // now print
                 // print delay loop
                 //          NOP
                 //          LOAD    s1,num
                 //          LOAD    s2,num
                 //Delay:
                 //          NOP
                 //          SUB     s1,1
                 //          JUMP    NZ,Delay
                 //          SUB     s2,1
                 //          JUMP    NZ,Delay
                 if ( ui->check_upper->isChecked() == true )
                 {
                     if ( init_nop == 1)
                     {
                         line1 = QString      ("             %1\n").arg(instr1.toUpper());
                     }
                     if ( init_nop == 2)
                     {
                         line1 = QString      ("             %1\n").arg(instr1.toUpper());
                         line_nop1 = QString      ("             %1\n").arg(instr1.toUpper());
                     }
                         line3 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
                         line4 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n\n").arg(instr5.toUpper(),s2.toUpper());
                         line5 = QString      ("%1:\n").arg(instr4.toUpper());
                         if ( ui->Box_Instructions->currentText() == "DJNZ")
                         {
                             line_nop =  QString  ("             %1\n").arg(instr1.toUpper());
                             line6 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                             line7 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                         }
                         else
                         {
                             line_nop =  QString  ("             %1\n").arg(instr1.toUpper());
                             line6 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                             line7 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                             line8 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                             line9 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                         }
                 }
                 else
                 {
                     if ( init_nop == 1)
                     {
                         line1 = QString      ("             %1\n").arg(instr1);
                     }
                     if ( init_nop == 2)
                     {
                         line1 = QString      ("             %1\n").arg(instr1);
                         line_nop1 = QString      ("             %1\n").arg(instr1);
                     }
                         line3 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
                         line4 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n\n").arg(instr5,s2);
                         line5 = QString      ("%1:\n").arg(instr4);
                         if ( ui->Box_Instructions->currentText() == "DJNZ")
                         {
                             line_nop =  QString  ("             %1\n").arg(instr1);
                             line6 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                             line7 =    QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                         }
                         else
                         {
                             line_nop =  QString  ("             %1\n").arg(instr1);
                             line6 =    QString  ("             %1           %2,#1\n").arg(instr2,s1);
                             line7 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                             line8 =    QString  ("             %1           %2,#1\n").arg(instr2,s2);
                             line9 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                         }
                 }

             // 2400 nefacha
                 if ( ui->Box_Instructions->currentText() == "DJNZ")
                 {
                      cursor.insertText(line1);
                      if ( init_nop == 2)
                      {
                        cursor.insertText(line_nop1);
                      }
                      cursor.insertText(line2);
                      cursor.insertText(line3);
                      cursor.insertText(line4);
                      cursor.insertText(line5);
                      cursor.insertText(line_nop);
                      cursor.insertText(line6);
                      cursor.insertText(line7 + "\n");
                      if ( true == ui->Button_Macro->isChecked())
                      {
                          if ( ui->check_upper->isChecked() == true )
                          {
                              endmacro = QString ("%1").arg(macroend.toUpper());
                              cursor.insertText(endmacro + "\n");
                          }
                          else
                          {
                              endmacro = QString ("%1").arg(macroend);
                              cursor.insertText(endmacro + "\n");
                          }
                      }
                 }
                 else
                 {
                      cursor.insertText(line1);
                      if ( init_nop == 2)
                      {
                        cursor.insertText(line_nop1);
                      }
                      cursor.insertText(line2);
                      cursor.insertText(line3);
                      cursor.insertText(line4);
                      cursor.insertText(line5);
                      cursor.insertText(line_nop);
                      cursor.insertText(line6);
                      cursor.insertText(line7);
                      cursor.insertText(line8);
                      cursor.insertText(line9 + "\n");
                      if ( true == ui->Button_Macro->isChecked())
                      {
                          if ( ui->check_upper->isChecked() == true )
                          {
                              endmacro = QString ("%1").arg(macroend.toUpper());
                              cursor.insertText(endmacro + "\n");
                          }
                          else
                          {
                              endmacro = QString ("%1").arg(macroend);
                              cursor.insertText(endmacro + "\n");
                          }
                      }
                 }
                  cursor.insertText(";---------------------------------------------");
                  ui->Vystup_text->setTextCursor(cursor);
             break;

          //=======================================================================================================
              case third_2r:
                  pokus = Steps;

                  // number of added nops
                  if ( (pokus % 2) == 0 )
                  {
                      init_nop = 0;
                      qDebug() << "init nops " << init_nop;
                  }
                  else if ( (pokus % 2) == 1 )
                  {
                      init_nop = 1;
                      qDebug() << "init nops " << init_nop;
                  }

                  // calculate register numbers
                  // 1024 - 132000 - init_nop
                  temp2 = (Steps -  2) / 512;
                  qDebug() << " prvni ardek" << temp2;
                  temp1 = (Steps -  2) - 512 * (temp2 - 1);
                  qDebug() << " druhy ardek" << temp1;
                  temp1 -= (temp2 * 2);
                  qDebug() << " druhy ardek" << temp1;
                  temp1 /=  2;

                  qDebug() << " register value" << temp1;
                  qDebug() << " register value" << temp2;
                  if ( temp1 > 255)
                  {
                      temp1 -= 257;
                      temp2++;
                  }

                  // now print
                  if ( ui->check_upper->isChecked() == true )
                  {
                      if ( init_nop == 1)
                      {
                          line1 = QString      ("             %1\n").arg(instr1.toUpper());
                      }
                          line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
                          line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n\n").arg(instr5.toUpper(),s2.toUpper());
                          line4 = QString      ("%1:\n").arg(instr4.toUpper());
                          if ( ui->Box_Instructions->currentText() == "DJNZ")
                          {
                              line5 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                              line6 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                          }
                          else
                          {
                              line5 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                              line6 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                              line7 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                              line8 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                          }
                  }
                  else
                  {
                      if ( init_nop == 1)
                      {
                          line1 = QString      ("             %1\n").arg(instr1);
                      }
                          line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
                          line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n\n").arg(instr5,s2);
                          line4 = QString      ("%1:\n").arg(instr4);
                          if ( ui->Box_Instructions->currentText() == "DJNZ")
                          {
                              line5 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                              line6 =    QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                          }
                          else
                          {
                              line5 =    QString  ("             %1           %2,#1\n").arg(instr2,s1);
                              line6 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                              line7 =    QString  ("             %1           %2,#1\n").arg(instr2,s2);
                              line8 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                          }
                  }

              // 2400 nefacha
                  if ( ui->Box_Instructions->currentText() == "DJNZ")
                  {
                       cursor.insertText(line1);
                       cursor.insertText(line2);
                       cursor.insertText(line3);
                       cursor.insertText(line4);
                       cursor.insertText(line5);
                       cursor.insertText(line6 + "\n");
                       if ( true == ui->Button_Macro->isChecked())
                       {
                           if ( ui->check_upper->isChecked() == true )
                           {
                               endmacro = QString ("%1").arg(macroend.toUpper());
                               cursor.insertText(endmacro + "\n");
                           }
                           else
                           {
                               endmacro = QString ("%1").arg(macroend);
                               cursor.insertText(endmacro + "\n");
                           }
                       }
                  }
                  else
                  {
                       cursor.insertText(line1);
                       cursor.insertText(line2);
                       cursor.insertText(line3);
                       cursor.insertText(line4);
                       cursor.insertText(line5);
                       cursor.insertText(line6);
                       cursor.insertText(line7);
                       cursor.insertText(line8 + "\n");
                       if ( true == ui->Button_Macro->isChecked())
                       {
                           if ( ui->check_upper->isChecked() == true )
                           {
                               endmacro = QString ("%1").arg(macroend.toUpper());
                               cursor.insertText(endmacro + "\n");
                           }
                           else
                           {
                               endmacro = QString ("%1").arg(macroend);
                               cursor.insertText(endmacro + "\n");
                           }
                       }
                  }
                   cursor.insertText(";---------------------------------------------");
                   ui->Vystup_text->setTextCursor(cursor);
              break;

       //=======================================================================================================
           case first_3r:

            registers = 3;
            pokus = Steps - 3;
            // number of added nops
          if ( (pokus % 2) == 1 )
          {
              init_nop = 1;
              qDebug() << "init nops " << init_nop;
          }
          if ( (pokus % 2) == 0 )
          {
              init_nop = 0;
              qDebug() << "init nops " << init_nop;
          }

   //-------------------------------------------

        reg_2_Loop( reg_3_Loop(pokus, init_nop), 0);

        qDebug() << " register value 1 " << temp1;
        qDebug() << " register value 2 " << temp2;
        qDebug() << " register value 3 " << temp3;

        // print out
        if ( ui->check_upper->isChecked() == true )
        {
            if ( init_nop == 1)
            {
                line1 = QString      ("             %1\n").arg(instr1.toUpper());
            }
                line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
                line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5.toUpper(),s2.toUpper());
                line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n\n").arg(instr5.toUpper(),s3.toUpper());
                line5 = QString      ("%1:\n").arg(instr4.toUpper());
                if ( ui->Box_Instructions->currentText() == "DJNZ")
                {
                    line6 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                    line7 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                    line8 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s3.toUpper(),instr4.toUpper());
                }
                else
                {
                    line6 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                    line7 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                    line8 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                    line9 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                    line10 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s3.toUpper());
                    line11 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                }
        }
        else
        {
            if ( init_nop == 1)
            {
                line1 = QString      ("             %1\n").arg(instr1);
            }
                line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
                line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5,s2);
                line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n\n").arg(instr5,s3);
                line5 = QString      ("%1:\n").arg(instr4);
                if ( ui->Box_Instructions->currentText() == "DJNZ")
                {
                    line6 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                    line7 =    QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                    line8 =    QString  ("             %1          %2,%3\n").arg(instr3,s3,instr4);
                }
                else
                {
                    line6 =    QString  ("             %1           %2,#1\n").arg(instr2,s1);
                    line7 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                    line8 =    QString  ("             %1           %2,#1\n").arg(instr2,s2);
                    line9 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                    line10 =   QString  ("             %1           %2,#1\n").arg(instr2,s3);
                    line11 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                }
        }

        // 2400 nefacha
            if ( ui->Box_Instructions->currentText() == "DJNZ")
            {
                 cursor.insertText(line1);
                 cursor.insertText(line2);
                 cursor.insertText(line3);
                 cursor.insertText(line4);
                 cursor.insertText(line5);
                 cursor.insertText(line6);
                 cursor.insertText(line7);
                 cursor.insertText(line8 + "\n");
                 if ( true == ui->Button_Macro->isChecked())
                 {
                     if ( ui->check_upper->isChecked() == true )
                     {
                         endmacro = QString ("%1").arg(macroend.toUpper());
                         cursor.insertText(endmacro + "\n");
                     }
                     else
                     {
                         endmacro = QString ("%1").arg(macroend);
                         cursor.insertText(endmacro + "\n");
                     }
                 }
            }
            else
            {
                 cursor.insertText(line1);
                 cursor.insertText(line2);
                 cursor.insertText(line3);
                 cursor.insertText(line4);
                 cursor.insertText(line5);
                 cursor.insertText(line6);
                 cursor.insertText(line7);
                 cursor.insertText(line8);
                 cursor.insertText(line9);
                 cursor.insertText(line10);
                 cursor.insertText(line11 + "\n");
                 if ( true == ui->Button_Macro->isChecked())
                 {
                     if ( ui->check_upper->isChecked() == true )
                     {
                         endmacro = QString ("%1").arg(macroend.toUpper());
                         cursor.insertText(endmacro + "\n");
                     }
                     else
                     {
                         endmacro = QString ("%1").arg(macroend);
                         cursor.insertText(endmacro + "\n");
                     }
                 }
            }
             cursor.insertText(";---------------------------------------------");
             ui->Vystup_text->setTextCursor(cursor);
        break;


        //=======================================================================================================
        case first_4r:

             registers = 4;
             pokus = Steps - 4;
             // number of added nops
           if ( (pokus % 2) == 1 )
           {
               init_nop = 1;
               qDebug() << "init nops " << init_nop;
           }
           if ( (pokus % 2) == 0 )
           {
               init_nop = 0;
               qDebug() << "init nops " << init_nop;
           }

    //-------------------------------------------

         reg_2_Loop( reg_3_Loop( reg_4_Loop(pokus, init_nop), 0), 0);

         qDebug() << " register value 1 " << temp1;
         qDebug() << " register value 2 " << temp2;
         qDebug() << " register value 3 " << temp3;
         qDebug() << " register value 4 " << temp4;

         // print out
         if ( ui->check_upper->isChecked() == true )
         {
             if ( init_nop == 1)
             {
                 line1 = QString      ("             %1\n").arg(instr1.toUpper());
             }
                 line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
                 line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5.toUpper(),s2.toUpper());
                 line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n").arg(instr5.toUpper(),s3.toUpper());
                 line5 = QString      ("             %1          %2,#" + QString::number(temp4) + "\n\n").arg(instr5.toUpper(),s4.toUpper());
                 line6 = QString      ("%1:\n").arg(instr4.toUpper());
                 if ( ui->Box_Instructions->currentText() == "DJNZ")
                 {
                     line7 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                     line8 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                     line9 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s3.toUpper(),instr4.toUpper());
                     line10 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s4.toUpper(),instr4.toUpper());

                 }
                 else
                 {
                     line7 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                     line8 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                     line9 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                     line10 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                     line11 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s3.toUpper());
                     line12 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                     line13 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s4.toUpper());
                     line14 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                 }
         }
         else
         {
             if ( init_nop == 1)
             {
                 line1 = QString      ("             %1\n").arg(instr1);
             }
                 line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
                 line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5,s2);
                 line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n").arg(instr5,s3);
                 line5 = QString      ("             %1          %2,#" + QString::number(temp4) + "\n\n").arg(instr5,s4);
                 line6 = QString      ("%1:\n").arg(instr4);
                 if ( ui->Box_Instructions->currentText() == "DJNZ")
                 {
                     line7 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                     line8 =    QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                     line9 =    QString  ("             %1          %2,%3\n").arg(instr3,s3,instr4);
                     line10 =   QString  ("             %1          %2,%3\n").arg(instr3,s4,instr4);

                 }
                 else
                 {
                     line7 =    QString  ("             %1           %2,#1\n").arg(instr2,s1);
                     line8 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                     line9 =    QString  ("             %1           %2,#1\n").arg(instr2,s2);
                     line10 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                     line11 =   QString  ("             %1           %2,#1\n").arg(instr2,s3);
                     line12 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                     line13 =   QString  ("             %1           %2,#1\n").arg(instr2,s4);
                     line14 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                 }
         }

         // 2400 nefacha
             if ( ui->Box_Instructions->currentText() == "DJNZ")
             {
                  cursor.insertText(line1);
                  cursor.insertText(line2);
                  cursor.insertText(line3);
                  cursor.insertText(line4);
                  cursor.insertText(line5);
                  cursor.insertText(line6);
                  cursor.insertText(line7);
                  cursor.insertText(line8);
                  cursor.insertText(line9);
                  cursor.insertText(line10 + "\n");
                  if ( true == ui->Button_Macro->isChecked())
                  {
                      if ( ui->check_upper->isChecked() == true )
                      {
                          endmacro = QString ("%1").arg(macroend.toUpper());
                          cursor.insertText(endmacro + "\n");
                      }
                      else
                      {
                          endmacro = QString ("%1").arg(macroend);
                          cursor.insertText(endmacro + "\n");
                      }
                  }
             }
             else
             {
                  cursor.insertText(line1);
                  cursor.insertText(line2);
                  cursor.insertText(line3);
                  cursor.insertText(line4);
                  cursor.insertText(line5);
                  cursor.insertText(line6);
                  cursor.insertText(line7);
                  cursor.insertText(line8);
                  cursor.insertText(line9);
                  cursor.insertText(line10);
                  cursor.insertText(line11);
                  cursor.insertText(line12);
                  cursor.insertText(line13);
                  cursor.insertText(line14 + "\n");
                  if ( true == ui->Button_Macro->isChecked())
                  {
                      if ( ui->check_upper->isChecked() == true )
                      {
                          endmacro = QString ("%1").arg(macroend.toUpper());
                          cursor.insertText(endmacro + "\n");
                      }
                      else
                      {
                          endmacro = QString ("%1").arg(macroend);
                          cursor.insertText(endmacro + "\n");
                      }
                  }
             }
              cursor.insertText(";---------------------------------------------");
              ui->Vystup_text->setTextCursor(cursor);
         break;
  //--------------------------------------------------------------------------------
        case    first_5r:

        registers = 5;
        pokus = Steps - 5;
        // number of added nops
      if ( (pokus % 2) == 1 )
      {
          init_nop = 1;
          qDebug() << "init nops " << init_nop;
      }
      if ( (pokus % 2) == 0 )
      {
          init_nop = 0;
          qDebug() << "init nops " << init_nop;
      }

//-------------------------------------------

    reg_2_Loop( reg_3_Loop( reg_4_Loop ( reg_5_Loop (pokus, init_nop),0), 0), 0);

    qDebug() << " register value 1 " << temp1;
    qDebug() << " register value 2 " << temp2;
    qDebug() << " register value 3 " << temp3;
    qDebug() << " register value 4 " << temp4;
    qDebug() << " register value 5 " << temp5;

    // print out
    if ( ui->check_upper->isChecked() == true )
    {
        if ( init_nop == 1)
        {
            line1 = QString      ("             %1\n").arg(instr1.toUpper());
        }
            line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
            line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5.toUpper(),s2.toUpper());
            line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n").arg(instr5.toUpper(),s3.toUpper());
            line5 = QString      ("             %1          %2,#" + QString::number(temp4) + "\n").arg(instr5.toUpper(),s4.toUpper());
            line6 = QString      ("             %1          %2,#" + QString::number(temp5) + "\n\n").arg(instr5.toUpper(),s5.toUpper());
            line7 = QString      ("%1:\n").arg(instr4.toUpper());
            if ( ui->Box_Instructions->currentText() == "DJNZ")
            {
                line8 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                line9 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                line10 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s3.toUpper(),instr4.toUpper());
                line11 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s4.toUpper(),instr4.toUpper());
                line12 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s5.toUpper(),instr4.toUpper());
            }
            else
            {
                line8 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                line9 =    QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line10 =    QString  ("             %1          %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                line11 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line12 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s3.toUpper());
                line13 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line14 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s4.toUpper());
                line15 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line16 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s5.toUpper());
                line17 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
            }
    }
    else
    {
        if ( init_nop == 1)
        {
            line1 = QString      ("             %1\n").arg(instr1);
        }
            line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
            line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5,s2);
            line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n").arg(instr5,s3);
            line5 = QString      ("             %1          %2,#" + QString::number(temp4) + "\n").arg(instr5,s4);
            line6 = QString      ("             %1          %2,#" + QString::number(temp5) + "\n\n").arg(instr5,s5);
            line7 = QString      ("%1:\n").arg(instr4);
            if ( ui->Box_Instructions->currentText() == "DJNZ")
            {
                line8 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                line9 =    QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                line10 =   QString  ("             %1          %2,%3\n").arg(instr3,s3,instr4);
                line11 =   QString  ("             %1          %2,%3\n").arg(instr3,s4,instr4);
                line12 =   QString  ("             %1          %2,%3\n").arg(instr3,s5,instr4);
            }
            else
            {
                line8 =    QString  ("             %1           %2,#1\n").arg(instr2,s1);
                line9 =    QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line10 =    QString  ("             %1          %2,#1\n").arg(instr2,s2);
                line11 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line12 =   QString  ("             %1           %2,#1\n").arg(instr2,s3);
                line13 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line14 =   QString  ("             %1           %2,#1\n").arg(instr2,s4);
                line15 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line16 =   QString  ("             %1           %2,#1\n").arg(instr2,s5);
                line17 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
            }
    }

    // 2400 nefacha
        if ( ui->Box_Instructions->currentText() == "DJNZ")
        {
             cursor.insertText(line1);
             cursor.insertText(line2);
             cursor.insertText(line3);
             cursor.insertText(line4);
             cursor.insertText(line5);
             cursor.insertText(line6);
             cursor.insertText(line7);
             cursor.insertText(line8);
             cursor.insertText(line9);
             cursor.insertText(line10);
             cursor.insertText(line11);
             cursor.insertText(line12 + "\n");
             if ( true == ui->Button_Macro->isChecked())
             {
                 if ( ui->check_upper->isChecked() == true )
                 {
                     endmacro = QString ("%1").arg(macroend.toUpper());
                     cursor.insertText(endmacro + "\n");
                 }
                 else
                 {
                     endmacro = QString ("%1").arg(macroend);
                     cursor.insertText(endmacro + "\n");
                 }
             }
        }
        else
        {
             cursor.insertText(line1);
             cursor.insertText(line2);
             cursor.insertText(line3);
             cursor.insertText(line4);
             cursor.insertText(line5);
             cursor.insertText(line6);
             cursor.insertText(line7);
             cursor.insertText(line8);
             cursor.insertText(line9);
             cursor.insertText(line10);
             cursor.insertText(line11);
             cursor.insertText(line12);
             cursor.insertText(line13);
             cursor.insertText(line14);
             cursor.insertText(line15);
             cursor.insertText(line16);
             cursor.insertText(line17 + "\n");
             if ( true == ui->Button_Macro->isChecked())
             {
                 if ( ui->check_upper->isChecked() == true )
                 {
                     endmacro = QString ("%1").arg(macroend.toUpper());
                     cursor.insertText(endmacro + "\n");
                 }
                 else
                 {
                     endmacro = QString ("%1").arg(macroend);
                     cursor.insertText(endmacro + "\n");
                 }
             }
        }
         cursor.insertText(";---------------------------------------------");
         ui->Vystup_text->setTextCursor(cursor);
        break;

        case first_6r:

        registers = 6;
        pokus = Steps - 6;
        // number of added nops
      if ( (pokus % 2) == 1 )
      {
          init_nop = 1;
          qDebug() << "init nops " << init_nop;
      }
      if ( (pokus % 2) == 0 )
      {
          init_nop = 0;
          qDebug() << "init nops " << init_nop;
      }

//-------------------------------------------

    reg_2_Loop( reg_3_Loop( reg_4_Loop ( reg_5_Loop ( reg_6_Loop (pokus, init_nop),0) ,0), 0), 0);

    qDebug() << " register value 1 " << temp1;
    qDebug() << " register value 2 " << temp2;
    qDebug() << " register value 3 " << temp3;
    qDebug() << " register value 4 " << temp4;
    qDebug() << " register value 5 " << temp5;
    qDebug() << " register value 6 " << temp6;

    // print out
    if ( ui->check_upper->isChecked() == true )
    {
        if ( init_nop == 1)
        {
            line1 = QString      ("             %1\n").arg(instr1.toUpper());
        }
            line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5.toUpper(),s1.toUpper());
            line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5.toUpper(),s2.toUpper());
            line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n").arg(instr5.toUpper(),s3.toUpper());
            line5 = QString      ("             %1          %2,#" + QString::number(temp4) + "\n").arg(instr5.toUpper(),s4.toUpper());
            line6 = QString      ("             %1          %2,#" + QString::number(temp5) + "\n").arg(instr5.toUpper(),s5.toUpper());
            line7 = QString      ("             %1          %2,#" + QString::number(temp6) + "\n\n").arg(instr5.toUpper(),s6.toUpper());
            line8 = QString      ("%1:\n").arg(instr4.toUpper());
            if ( ui->Box_Instructions->currentText() == "DJNZ")
            {
                line9 =    QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s1.toUpper(),instr4.toUpper());
                line10 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s2.toUpper(),instr4.toUpper());
                line11 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s3.toUpper(),instr4.toUpper());
                line12 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s4.toUpper(),instr4.toUpper());
                line13 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s5.toUpper(),instr4.toUpper());
                line14 =   QString  ("             %1          %2,%3\n").arg(instr3.toUpper(),s6.toUpper(),instr4.toUpper());
            }
            else
            {
                line9 =    QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s1.toUpper());
                line10 =    QString  ("             %1        NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line11 =    QString  ("             %1          %2,#1\n").arg(instr2.toUpper(),s2.toUpper());
                line12 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line13 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s3.toUpper());
                line14 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line15 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s4.toUpper());
                line16 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line17 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s5.toUpper());
                line18 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
                line19 =   QString  ("             %1           %2,#1\n").arg(instr2.toUpper(),s6.toUpper());
                line20 =   QString  ("             %1         NZ,%2\n").arg(instr6.toUpper(),instr4.toUpper());
           }
    }
    else
    {
        if ( init_nop == 1)
        {
            line1 = QString      ("             %1\n").arg(instr1);
        }
            line2 = QString      ("             %1          %2,#" + QString::number(temp1) + "\n").arg(instr5,s1);
            line3 = QString      ("             %1          %2,#" + QString::number(temp2) + "\n").arg(instr5,s2);
            line4 = QString      ("             %1          %2,#" + QString::number(temp3) + "\n").arg(instr5,s3);
            line5 = QString      ("             %1          %2,#" + QString::number(temp4) + "\n").arg(instr5,s4);
            line6 = QString      ("             %1          %2,#" + QString::number(temp5) + "\n").arg(instr5,s5);
            line7 = QString      ("             %1          %2,#" + QString::number(temp6) + "\n\n").arg(instr5,s6);
            line8 = QString      ("%1:\n").arg(instr4);
            if ( ui->Box_Instructions->currentText() == "DJNZ")
            {
                line9 =    QString  ("             %1          %2,%3\n").arg(instr3,s1,instr4);
                line10 =   QString  ("             %1          %2,%3\n").arg(instr3,s2,instr4);
                line11 =   QString  ("             %1          %2,%3\n").arg(instr3,s3,instr4);
                line12 =   QString  ("             %1          %2,%3\n").arg(instr3,s4,instr4);
                line13 =   QString  ("             %1          %2,%3\n").arg(instr3,s5,instr4);
                line14 =   QString  ("             %1          %2,%3\n").arg(instr3,s6,instr4);
            }
            else
            {
                line9 =    QString  ("             %1           %2,#1\n").arg(instr2,s1);
                line10 =    QString  ("             %1        NZ,%2\n").arg(instr6,instr4);
                line11 =    QString  ("             %1          %2,#1\n").arg(instr2,s2);
                line12 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line13 =   QString  ("             %1           %2,#1\n").arg(instr2,s3);
                line14 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line15 =   QString  ("             %1           %2,#1\n").arg(instr2,s4);
                line16 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line17 =   QString  ("             %1           %2,#1\n").arg(instr2,s5);
                line18 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
                line19 =   QString  ("             %1           %2,#1\n").arg(instr2,s6);
                line20 =   QString  ("             %1         NZ,%2\n").arg(instr6,instr4);
           }
    }

    // 2400 nefacha
        if ( ui->Box_Instructions->currentText() == "DJNZ")
        {
             cursor.insertText(line1);
             cursor.insertText(line2);
             cursor.insertText(line3);
             cursor.insertText(line4);
             cursor.insertText(line5);
             cursor.insertText(line6);
             cursor.insertText(line7);
             cursor.insertText(line8);
             cursor.insertText(line9);
             cursor.insertText(line10);
             cursor.insertText(line11);
             cursor.insertText(line12);
             cursor.insertText(line13);
             cursor.insertText(line14 + "\n");
             if ( true == ui->Button_Macro->isChecked())
             {
                 if ( ui->check_upper->isChecked() == true )
                 {
                     endmacro = QString ("%1").arg(macroend.toUpper());
                     cursor.insertText(endmacro + "\n");
                 }
                 else
                 {
                     endmacro = QString ("%1").arg(macroend);
                     cursor.insertText(endmacro + "\n");
                 }
             }
        }
        else
        {
             cursor.insertText(line1);
             cursor.insertText(line2);
             cursor.insertText(line3);
             cursor.insertText(line4);
             cursor.insertText(line5);
             cursor.insertText(line6);
             cursor.insertText(line7);
             cursor.insertText(line8);
             cursor.insertText(line9);
             cursor.insertText(line10);
             cursor.insertText(line11);
             cursor.insertText(line12);
             cursor.insertText(line13);
             cursor.insertText(line14);
             cursor.insertText(line15);
             cursor.insertText(line16);
             cursor.insertText(line17);
             cursor.insertText(line18);
             cursor.insertText(line19);
             cursor.insertText(line20 + "\n");
             if ( true == ui->Button_Macro->isChecked())
             {
                 if ( ui->check_upper->isChecked() == true )
                 {
                     endmacro = QString ("%1").arg(macroend.toUpper());
                     cursor.insertText(endmacro + "\n");
                 }
                 else
                 {
                     endmacro = QString ("%1").arg(macroend);
                     cursor.insertText(endmacro + "\n");
                 }
             }
        }
         cursor.insertText(";---------------------------------------------");
         ui->Vystup_text->setTextCursor(cursor);
        break;
//------------------------------------------------------------------------------------
//+-------------------------------------------------------------------------------------
         case impossible:
                ui->Vystup_text->clear();
                cursor.insertText("Cannot evaluate");
                ui->Vystup_text->setTextCursor(cursor);
            break;

        default:
        qDebug() <<"chyba";
        break;
    }

    return;
}


unsigned int loop_gen::get_frekvency ( void )
{
    QString temp1 = 0;
    unsigned long setTimeFrame = 1;

    if ( ui->radio_Time->isChecked() == true)
    {
        if (ui->Box_Frekvency->currentText() == "Hz")
        {
            temp1 = ui->Vstup_Frekvency->text();
            Frekvency = temp1.toDouble();
        }
        if (ui->Box_Frekvency->currentText() == "kHz")
        {
            temp1 = ui->Vstup_Frekvency->text();
            Frekvency = temp1.toDouble() * 1000;
        }
        if (ui->Box_Frekvency->currentText() == "MHz")
        {
            temp1 = ui->Vstup_Frekvency->text();
            Frekvency = temp1.toDouble() * 1000000;
        }
    }

    // set selected frekvency
    if (ui->Box_F->currentText() == "S")
    {
        setTimeFrame = 1;
        index = QString("S");
    }
    if (ui->Box_F->currentText() == "nS")
    {
        setTimeFrame = 1000000000;
        index = QString("nS");
    }
    if (ui->Box_F->currentText() == "uS")
    {
        setTimeFrame = 1000000;
        index = QString("uS");
    }
    if (ui->Box_F->currentText() == "mS")
    {
        setTimeFrame = 1000;
        index = QString("mS");
    }
    return setTimeFrame;
}


void loop_gen::setFlags(void)
{
    if ( ui->check_upper->isChecked() == true)
    {
        qDebug() << "jjj :" << Time;
    }
}


void loop_gen::Default_click(void)
{
    if ( true == ui->check_default->isChecked())
    {
        ui->line_reg1->setDisabled(true);
        ui->line_reg2->setDisabled(true);
        ui->line_reg3->setDisabled(true);
        ui->line_reg4->setDisabled(true);
        ui->line_reg5->setDisabled(true);
        ui->line_reg6->setDisabled(true);

        ui->line_reg1->setText("S1");
        ui->line_reg2->setText("S2");
        ui->line_reg3->setText("S3");
        ui->line_reg4->setText("S4");
        ui->line_reg5->setText("S5");
        ui->line_reg6->setText("S6");
    }
    else
    {
        ui->line_reg1->setEnabled(true);
        ui->line_reg2->setEnabled(true);
        ui->line_reg3->setEnabled(true);
        ui->line_reg4->setEnabled(true);
        ui->line_reg5->setEnabled(true);
        ui->line_reg6->setEnabled(true);
    }

}


// destructor
loop_gen::~loop_gen()
{
    delete ui;
}


void loop_gen::CopyToClipboard()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->Vystup_text->toPlainText());
}
