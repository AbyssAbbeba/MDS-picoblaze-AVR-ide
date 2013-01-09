#ifndef IOREGSWIDGET_H
#define IOREGSWIDGET_H

#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>


class IORegsWidget : public QWidget
{
    Q_OBJECT
    public:
        IORegsWidget(QWidget *parent, int index);

        QPushButton *nextBtn;
        QPushButton *previousBtn;

        QGridLayout *ioRegsLayout;


        //IORegs
        QLabel *ioRegsLblAddr;
        QLabel *ioRegsLblValue;
        QLabel *ioRegsLblReadMask;
        QLabel *ioRegsLblWriteMask;
        QLabel *ioRegsLblRandomMask;
        QLabel *ioRegsLblReserved;
        QLabel *ioRegsLblVirtual;
        QLabel *ioRegsLblName;
        QLabel *ioRegsLblDesc;

        QCheckBox *ioRegsChckReserved;
        QCheckBox *ioRegsChckVirtual;

        QLineEdit *ioRegsLEAddr;
        QLineEdit *ioRegsLEValue;
        QLineEdit *ioRegsLEReadMask;
        QLineEdit *ioRegsLEWriteMask;
        QLineEdit *ioRegsLERandomMask;
        QLineEdit *ioRegsLEName;
        QLineEdit *ioRegsLEDesc;
        
        //BITS
        QLabel *bitLblEnable;
        QLabel *bitLbl7;
        QLabel *bitLbl6;
        QLabel *bitLbl5;
        QLabel *bitLbl4;
        QLabel *bitLbl3;
        QLabel *bitLbl2;
        QLabel *bitLbl1;
        QLabel *bitLbl0;
        QLabel *bitLblSTip7;
        QLabel *bitLblSTip6;
        QLabel *bitLblSTip5;
        QLabel *bitLblSTip4;
        QLabel *bitLblSTip3;
        QLabel *bitLblSTip2;
        QLabel *bitLblSTip1;
        QLabel *bitLblSTip0;
        QLabel *bitLblTTip7;
        QLabel *bitLblTTip6;
        QLabel *bitLblTTip5;
        QLabel *bitLblTTip4;
        QLabel *bitLblTTip3;
        QLabel *bitLblTTip2;
        QLabel *bitLblTTip1;
        QLabel *bitLblTTip0;
        
        QCheckBox *bitChckEnable;

        QList<QLineEdit*> bitLEName;
        QList<QLineEdit*> bitLESTip;
        QList<QLineEdit*> bitLETTip;

        //general stuff
        QLabel *indexLbl;
        int index;
};

#endif //IOREGSWIDGET_H