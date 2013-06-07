// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgrAVR8.h
 */
// =============================================================================

#ifndef MCUSIMCFGMGRAVR8_H
#define MCUSIMCFGMGRAVR8_H

// Forward declarations
class McuDeviceSpec;

#include "MCUSim.h"

#include <QStringList>
#include <QVector>
#include <QString>
#include <QStack>
#include <QXmlAttributes>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuSimCfgMgrAVR8
 */
class McuSimCfgMgrAVR8
{
    ////    Constructors and Destructors    ////
    public:
        /**
         * @brief
         * @param[in,out] devices
         * @param[in,out] currentXMLElement
         * @param[in,out] expectedXMLElements
         * @param[in,out] expectCharacters
         * @param[in,out] auxInt0
         * @param[in,out] auxInt1
         */
        McuSimCfgMgrAVR8 ( QVector<McuDeviceSpec*> * devices,
                           QStack<QString> * currentXMLElement,
                           QStringList * expectedXMLElements,
                           bool * expectCharacters,
                           int * auxInt0,
                           int * auxInt1 );

    private:
        /**
         * @brief Forbidden constructor.
         */
        McuSimCfgMgrAVR8();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] namespaceURI
         * @param[in] localName
         * @param[in] qName
         * @param[in] atts
         * @return
         */
        bool startElement ( const QString & namespaceURI,
                            const QString & localName,
                            const QString & qName,
                            const QXmlAttributes & atts );
        /**
         * @brief
         * @param[in] localName
         * @param[in] atts
         * @return
         */
        bool attributes ( const QString & localName,
                          const QXmlAttributes & atts );

        /**
         * @brief
         * @param[in] ch
         * @return
         */
        bool characters ( const QString & ch );

    ////    Private Attributes    ////
    private:
        QVector<McuDeviceSpec*> & m_devices;   ///<
        QStack<QString> & m_currentXMLElement; ///<
        QStringList & m_expectedXMLElements;   ///<
        bool & m_expectCharacters;             ///<
        int & m_auxInt0;                       ///< Auxiliary integer variable #1
        int & m_auxInt1;                       ///< Auxiliary integer variable #2
};

#endif // MCUSIMCFGMGRAVR8_H
