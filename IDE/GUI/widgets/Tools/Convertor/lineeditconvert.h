#ifndef LINEEDITCONVERT_H
#define LINEEDITCONVERT_H

#include <QLineEdit>
#include <QWidget>


class LineEditConvert : public QLineEdit
{
    Q_OBJECT
    public:
        LineEditConvert(QWidget *parent, int base);

    public slots:
        void setTextSlot(const QString &text, int base);
        void catchEditSignal(const QString &text);

    signals:
        void textEditedSig(const QString &text, int base);

    private:
        int base;
};


#endif
