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
        
    private:

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

        QLineEdit *bitLE7Name;
        QLineEdit *bitLE6Name;
        QLineEdit *bitLE5Name;
        QLineEdit *bitLE4Name;
        QLineEdit *bitLE3Name;
        QLineEdit *bitLE2Name;
        QLineEdit *bitLE1Name;
        QLineEdit *bitLE0Name;
        QLineEdit *bitLE7STip;
        QLineEdit *bitLE6STip;
        QLineEdit *bitLE5STip;
        QLineEdit *bitLE4STip;
        QLineEdit *bitLE3STip;
        QLineEdit *bitLE2STip;
        QLineEdit *bitLE1STip;
        QLineEdit *bitLE0STip;
        QLineEdit *bitLE7TTip;
        QLineEdit *bitLE6TTip;
        QLineEdit *bitLE5TTip;
        QLineEdit *bitLE4TTip;
        QLineEdit *bitLE3TTip;
        QLineEdit *bitLE2TTip;
        QLineEdit *bitLE1TTip;
        QLineEdit *bitLE0TTip;

        //general stuff
        QLabel *indexLbl;
        int index;
};

#endif //IOREGSWIDGET_H