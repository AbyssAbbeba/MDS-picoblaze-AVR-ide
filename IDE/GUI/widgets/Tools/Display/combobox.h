#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QString>

class ComboBox : public QComboBox
{
    Q_OBJECT
    public:
        ComboBox(QWidget *parent);
        
    signals:
        void changed(ComboBox *box, int index);

    public slots:
        void changedEmit(int index);
};


#endif