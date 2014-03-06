/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup GUI
 * @file regdisplay.h
 */


#ifndef REGDISPLAY_H
#define REGDISPLAY_H

// Forward declarations
class QHBoxLayout;
class QLineEdit;
class QLabel;
class QPushButton;
class QStringList;

#include <cstdint>
#include <QWidget>
#include <QString>
#include <QFont>
#include <QPalette>

class RegDisplay : public QWidget {
	Q_OBJECT
public:
	RegDisplay (
		uint address,
		const QString & regName,
		const QString & regNameTip = "",
		uint8_t mask = 0xFFu,
		const QStringList * bitNames = NULL,
		const QStringList * toolsTips = NULL,
		const QStringList * statusTips = NULL,
		QWidget * parent = 0);

	virtual ~RegDisplay();

	static const QFont m_nameFont;
	static const QFont m_bitNormalFont;

	uint getValue();

public slots:
	void setValue(uint value);
	void setReadOnly(bool readOnly);
	void setHighlighted(bool highlighted);

private slots:
	void bit0ButtonClicked();
	void bit1ButtonClicked();
	void bit2ButtonClicked();
	void bit3ButtonClicked();
	void bit4ButtonClicked();
	void bit5ButtonClicked();
	void bit6ButtonClicked();
	void bit7ButtonClicked();
	void hexTextEdited(const QString & text);
	void hexTextCurChange(int , int);

signals:
	void valueChanged(uint address, uint value);

private:
	RegDisplay();
 
        QWidget *parentWidget;

	const int m_address;
	const uint8_t m_bitEnableMask;
	bool m_readOnly;
	bool m_bitButtonsAvailable;

	QHBoxLayout * m_primaryLayout;
	QLabel * m_regNameLabel;
	QLineEdit * m_hexLineEdit;
	QPushButton * m_bitButtons[8];

	QPalette m_log0palette;
	QPalette m_log1palette;
	QPalette m_highlightedPalette;
	QPalette m_normalPalette;

	inline void bitButtonClicked(int i);
	inline void refreshBitButtons(uint value);

	inline void setPalettes();
	inline void createLayout();
	inline void createWidgets(
		const QString & regName,
		const QString & regNameTip,
		const QStringList * bitNames,
		const QStringList * toolsTips,
		const QStringList * statusTips);
	inline void setupConnections();
};

#endif // REGDISPLAY_H
