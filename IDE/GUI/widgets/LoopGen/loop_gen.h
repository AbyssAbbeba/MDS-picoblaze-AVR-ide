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
 * @file loop_gen.h
 */


#ifndef LOOP_GEN_H
#define LOOP_GEN_H

#include <QWidget>

namespace Ui {
class loop_gen;
}

class loop_gen : public QWidget
{
        Q_OBJECT

    public:
        enum  { nop_, first_1r, second_1r,third_1r, first_2r, second_2r, first_3r, third_2r, first_4r, first_5r, impossible,first_6r};
        double Cycles,CycleTime,Frekvency, StepTime, Steps;
        double CyclesFinal;
        QString index;
        unsigned long long temp1, temp2, temp3, temp4, temp5,temp6,registers;

        double Time;
        explicit loop_gen(QWidget *parent = 0);
        void setFlags(void);
        void reg_1_Loop( unsigned long long loop, unsigned int init_nop);
        void reg_2_Loop( unsigned long long loop, unsigned int init_nop);
        unsigned long long reg_3_Loop( unsigned long long loop, unsigned int init_nop);
        unsigned long long reg_4_Loop( unsigned long long loop, unsigned int init_nop);
        unsigned long long reg_5_Loop( unsigned long long loop, unsigned int init_nop);
        unsigned long long reg_6_Loop( unsigned long long loop, unsigned int init_nop);
        void loop(unsigned int type);
        unsigned int get_frekvency(void);
        void PrintToText(void);
        void keyPressEvent(QKeyEvent *);
        ~loop_gen();

    private slots:
        double TimeToCycles();
        void TimeChanged(QString);
        void Generate(void);
        void GenerateLoop( void );
        void Default_click(void);
        void Time_click(void);
        void Cycles_click (void);
        void CopyToClipboard();

    private:


        Ui::loop_gen *ui;
};

#endif // LOOP_GEN_H
