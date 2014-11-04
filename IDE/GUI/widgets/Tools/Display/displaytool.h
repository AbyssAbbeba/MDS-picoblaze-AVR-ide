/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file displaytool.h
 */


#ifndef DISPLAYTOOL_H
#define DISPLAYTOOL_H


#include <QWidget>
#include <QString>

class QLabel;
class QGridLayout;
class QVBoxLayout;
class DisplaySegment;
class ComboBox;

/**
 * @brief
 * @ingroup GUI
 * @class DisplayTool
 */
class DisplayTool : public QWidget
{
    Q_OBJECT
    public:
        DisplayTool(QWidget *parent);
        ~DisplayTool();

    private:
        void updateLEGlobal(int pin, bool active);
        
        DisplaySegment *display;
        QLineEdit *cathodeLE[3];
        QLineEdit *anodeLE[3];
        QLabel *cathodeLabels[4];
        QLabel *anodeLabels[4];
        QLabel *labelsUp[3];
        QLabel *labelsLED[8];
        QLabel *labels[8];
        ComboBox *comboBoxes[8];
        bool config[8];
        
    public slots:
        void segmentPressed(int index);
        void pinEdited(int pin, bool active);
        void updateComboBoxes(ComboBox *box, int index);
        void updateLEHexCat(const QString &text);
        void updateLEDecCat(const QString &text);
        void updateLEBinCat(const QString &text);
        void updateLEHexAn(const QString &text);
        void updateLEDecAn(const QString &text);
        void updateLEBinAn(const QString &text);
};




#endif
