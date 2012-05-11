/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author: Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 *
 */

#include "RegDisplay.h"

#include <cassert>

#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QStringList>

const QFont RegDisplay::m_nameFont = QFont("Courier New", 10, QFont::Normal);
const QFont RegDisplay::m_bitNormalFont = QFont("Courier New", 10, QFont::DemiBold);

RegDisplay::RegDisplay(
		const QString & regName,
		const QString & regNameTip,
		uint8_t mask,
		const QStringList * bitNames,
		const QStringList * toolsTips,
		const QStringList * statusTips
	) :
		m_bitEnableMask(mask),
		m_readOnly(true)
{
	setPalettes();
	createLayout();
	createWidgets(regName, regNameTip, bitNames, toolsTips, statusTips);
	setupConnections();
}

inline void RegDisplay::setPalettes() {
	m_log0palette.setColor(QPalette::WindowText, QColor(0xFF, 0x00, 0x00));
	m_log1palette.setColor(QPalette::WindowText, QColor(0x00, 0xCC, 0x00));
	m_highlightedPalette.setColor(QPalette::Text, QColor(0xDD, 0x88, 0x00));
	m_normalPalette.setColor(QPalette::Text, QColor(0x00, 0x00, 0x00));
}

inline void RegDisplay::createLayout() {
	m_primaryLayout = new QHBoxLayout(this);
	m_primaryLayout->setSpacing(0);
	m_primaryLayout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_primaryLayout);
}

inline void RegDisplay::createWidgets(
	const QString & regName,
	const QString & regNameTip,
	const QStringList * bitNames,
	const QStringList * toolsTips,
	const QStringList * statusTips)
{
	m_regNameLabel = new QLabel(regName + ": ", this);
	m_regNameLabel->setFont(m_nameFont);
	if ( 0 != regNameTip.size() ) {
		m_regNameLabel->setToolTip(regNameTip);
		m_regNameLabel->setStatusTip(regNameTip);
	}
	m_primaryLayout->addWidget(m_regNameLabel);

	m_hexLineEdit = new QLineEdit(this);
	m_hexLineEdit->setInputMask(">HH;");
	m_hexLineEdit->setText("00");
	m_hexLineEdit->setMinimumWidth(30);
	m_hexLineEdit->setMaximumWidth(30);
	m_hexLineEdit->setFont(m_nameFont);
	m_primaryLayout->addWidget(m_hexLineEdit);

	m_bitButtonsAvailable = false;
	if ( NULL != bitNames ) {
		m_bitButtonsAvailable = true;

		assert ( 8 == bitNames->size() );

		for ( int i = 7; i >= 0; i-- ) {
			m_bitButtons[i] = new QPushButton(bitNames->at(i), this);
			m_bitButtons[i]->setFlat(true);
			m_bitButtons[i]->setFont(m_bitNormalFont);
			m_bitButtons[i]->setMinimumWidth(25);
			m_bitButtons[i]->setMinimumHeight(15);
			m_bitButtons[i]->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Maximum);

			if ( m_bitEnableMask & (0x01 << i) ) {
				m_bitButtons[i]->setPalette(m_log0palette);
			} else {
				m_bitButtons[i]->setDisabled(true);
			}

			m_primaryLayout->addWidget(m_bitButtons[i]);
		}

		if ( NULL != toolsTips ) {
			assert ( 8 == toolsTips->size() );
			for ( int i = 0; i < 8; i++ ) {
				m_bitButtons[i]->setToolTip(toolsTips->at(i));
			}
		}
		if ( NULL != statusTips ) {
			assert ( 8 == statusTips->size() );
			for ( int i = 0; i < 8; i++ ) {
				m_bitButtons[i]->setStatusTip(statusTips->at(i));
			}
		}
	}
}

RegDisplay::~RegDisplay() {
	delete m_primaryLayout;
	delete m_regNameLabel;
	delete m_hexLineEdit;
	if ( NULL != m_bitButtons[0] ) {
		for ( int i = 0; i < 8; i++ ) {
			delete m_bitButtons[i];
		}
	}
}

inline void RegDisplay::setupConnections() {
	if ( true == m_bitButtonsAvailable ) {
		connect(m_bitButtons[0], SIGNAL(clicked()), this, SLOT(bit0ButtonClicked()));
		connect(m_bitButtons[1], SIGNAL(clicked()), this, SLOT(bit1ButtonClicked()));
		connect(m_bitButtons[2], SIGNAL(clicked()), this, SLOT(bit2ButtonClicked()));
		connect(m_bitButtons[3], SIGNAL(clicked()), this, SLOT(bit3ButtonClicked()));
		connect(m_bitButtons[4], SIGNAL(clicked()), this, SLOT(bit4ButtonClicked()));
		connect(m_bitButtons[5], SIGNAL(clicked()), this, SLOT(bit5ButtonClicked()));
		connect(m_bitButtons[6], SIGNAL(clicked()), this, SLOT(bit6ButtonClicked()));
		connect(m_bitButtons[7], SIGNAL(clicked()), this, SLOT(bit7ButtonClicked()));
	}
	connect(m_hexLineEdit, SIGNAL(textEdited(const QString &)), this, SLOT(hexTextEdited(const QString &)));
	connect(m_hexLineEdit, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(hexTextCurChange(int, int)));
}

void RegDisplay::hexTextCurChange(int , int) {
	setHighlighted(false);
}

void RegDisplay::bit0ButtonClicked() {
	bitButtonClicked(0);
}
void RegDisplay::bit1ButtonClicked() {
	bitButtonClicked(1);
}
void RegDisplay::bit2ButtonClicked() {
	bitButtonClicked(2);
}
void RegDisplay::bit3ButtonClicked() {
	bitButtonClicked(3);
}
void RegDisplay::bit4ButtonClicked() {
	bitButtonClicked(4);
}
void RegDisplay::bit5ButtonClicked() {
	bitButtonClicked(5);
}
void RegDisplay::bit6ButtonClicked() {
	bitButtonClicked(6);
}
void RegDisplay::bit7ButtonClicked() {
	bitButtonClicked(7);
}

inline void RegDisplay::bitButtonClicked(int i) {
	if ( true == m_readOnly ) {
		return;
	}

	uint value = getValue() ^ (0x01 << i);

	setValue(value);
	setHighlighted(false);

	emit(valueChanged(value));
}

void RegDisplay::hexTextEdited(const QString & text) {
	if ( 0 == text.size() ) {
		return;
	}

	uint value = text.toInt(NULL, 16);

	refreshBitButtons(value);
	setHighlighted(false);

	emit(valueChanged(value));
}

void RegDisplay::setHighlighted(bool highlighted) {
	if ( true == highlighted ) {
		m_hexLineEdit->setPalette(m_highlightedPalette);
	} else {
		m_hexLineEdit->setPalette(m_normalPalette);
	}
}

void RegDisplay::setReadOnly(bool readOnly) {
	m_readOnly = readOnly;
	m_hexLineEdit->setReadOnly(m_readOnly);
	if ( true == m_bitButtonsAvailable ) {
		for ( int i = 0; i < 8; i++ ) {
			m_bitButtons[i]->setDisabled(readOnly);
		}
	}
}

inline void RegDisplay::refreshBitButtons(uint value) {
	if ( false == m_bitButtonsAvailable ) {
		return;
	}

	for ( uint i = 0, mask = 0x01; i < 8; i++, mask <<= 1 ) {
		if ( 0 == (m_bitEnableMask & mask) ) {
			continue;
		}

		if ( value & mask ) {
			m_bitButtons[i]->setPalette(m_log1palette);
		} else {
			m_bitButtons[i]->setPalette(m_log0palette);
		}
	}
}

void RegDisplay::setValue(uint value) {
	m_hexLineEdit->setText(QString("%1").arg (value, 2, 16, QChar('0')));
	refreshBitButtons(value);
}

uint RegDisplay::getValue() {
	if ( 0 == m_hexLineEdit->text().size() ) {
		return 0;
	}

	return m_hexLineEdit->text().toUInt(NULL, 16);
}
