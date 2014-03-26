
#ifndef MAIN_H
#define MAIN_H

#include <QSignalMapper>
#include <QWidget>
#include "ui_untitled.h"
#include <string.h>
#include <stdio.h>



class Xxx : public QWidget
{
    Q_OBJECT
    public:
         //= new KeyPress();
        Xxx();
        ~Xxx();

        // variables
        int operand1_base, operand2_base;
        int flag_result,flag_operand1, flag_operand2, flag_equals;
        long long int operand_long1,operand_long2;
        QString operator_out;
        int base_indicator;
        int operator_;
        long long int end_result;
        enum {Start, prvni, druhy, treti, result, reset};
        enum { nop, add, sub, mul, xor_, or_, not_, and_, bshift,div_ };
        enum { dec_, hex_, bin_ };
        enum {prev_hex, prev_dec, prev_bin};
        int prev_state;
        QString operand1;
        QString operand2;
        QString s_perm;
        QString operator_string;
        int     number_cnt;
        float digit_1;
        bool flag_op;
        // functions
        void calc_reset();
        void textChange2();
        void equals(QString s_result);
        void get_result(void);
        void Cursor_textedit(int stav);//const char Number_char);
        // events
        void keyPressEvent(QKeyEvent *);


    public slots:
        void base_set_hex(bool h);
        void base_set_bin(bool b);
        void base_set_dec(bool d);
        void display_bin(QString bin_string);
        void Operator(int i);
        void CA_C();
        void Myundo();
        void Myredo();
        void read_button(int i);
        void textChange2(QString s);
        //void proved_5(const QString xxx);

    private:
        Ui_Form ui;
        QSignalMapper *signalMapper;
        //QKeyEvent KeyPress *keyPress;
};


#endif
