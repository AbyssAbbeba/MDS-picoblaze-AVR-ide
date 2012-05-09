
#include "RegDisplay.h"

#include <QApplication>

int main(int argc, char ** argv) {
	QApplication app(argc, argv);

	QStringList * bitNames = new QStringList;
	*bitNames << "C" << "Z" << "N" << "V" << "S" << "H" << "T" << "I";
	QStringList * toolsTips = new QStringList;
	*toolsTips << "some text for I" << "some text for T" << "some text for H" << "some text for S" << "some text for V" << "some text for N" << "some text for Z" << "some text for C";

	RegDisplay * regDisplay = new RegDisplay("SREG", "The AVR Status Register", 0xFC, bitNames, toolsTips);
	regDisplay->show();

// 	regDisplay->setHighlighted(true);
// 	regDisplay->setReadOnly(true);

	return app.exec();
}
