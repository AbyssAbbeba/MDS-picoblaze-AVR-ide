/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * Copyright: See COPYING file that comes with this distribution.
 *
 * @author Martin Ošmera <martin.osmera@gmail.com>, (C) 2012
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgr.h
 */

#ifndef MCUSIMCFGMGR_H
#define MCUSIMCFGMGR_H

class McuDeviceSpec;

#include "MCUSim.h"

#include <string>
#include <QString>
#include <QVector>
#include <QStack>
#include <QStringList>
#include <QXmlDefaultHandler>
#include <QXmlAttributes>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuSimCfgMgr
 */
class McuSimCfgMgr : public QXmlDefaultHandler {
public:
	McuSimCfgMgr();
	McuSimCfgMgr(const char * filename);
	~McuSimCfgMgr();

	/**
	 *
	 * @return
	 */
	bool openConfigFile(const char * filename);

	/**
	 *
	 * @return
	 */
	bool setupSimulator(const char * mcuName, MCUSim::Config & mcuConfig);

private:
	QVector<McuDeviceSpec*> m_devices;	///<
	QStack<QString> m_currentXMLElement;	///<
	QStringList m_expectedXMLElements;	///<
	bool m_expectCharacters;		///<
	int m_auxInt0;				///< An auxiliary integer variable
	int m_auxInt1;

	inline void clear();

	/**
	 *
	 * @param localName
	 * @param atts
	 * @return
	 */
	inline bool attributesAVR8(const QString & localName, const QXmlAttributes & atts);

	/**
	 *
	 * @param ch
	 * @return
	 */
	bool characters(const QString & ch);

	/**
	 *
	 * @param ch
	 * @return
	 */
	inline bool charactersAVR8(const QString & ch);

	/**
	 *
	 * @param namespaceURI
	 * @param localName
	 * @param qName
	 * @return
	 */
	bool endElement(
		const QString & namespaceURI,
		const QString & localName,
		const QString & qName);

	/**
	 *
	 * @param namespaceURI
	 * @param localName
	 * @param qName
	 * @param atts
	 * @return
	 */
	bool startElement(
		const QString & namespaceURI,
		const QString & localName,
		const QString & qName,
		const QXmlAttributes & atts);

	/**
	 *
	 * @param namespaceURI
	 * @param localName
	 * @param qName
	 * @param atts
	 * @return
	 */
	bool startElementAVR8(
		const QString & namespaceURI,
		const QString & localName,
		const QString & qName,
		const QXmlAttributes & atts);
};

#endif // MCUSIMCFGMGR_H
