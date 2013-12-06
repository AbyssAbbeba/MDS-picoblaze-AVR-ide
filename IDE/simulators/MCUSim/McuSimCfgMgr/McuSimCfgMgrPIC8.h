// =============================================================================
/**
 * @brief
 * C++ Interface: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Martin Ošmera <martin.osmera@moravia-microsystems.com>
 * @ingroup McuSimCfgMgr
 * @file McuSimCfgMgrPIC8.h
 */
// =============================================================================

#ifndef MCUSIMCFGMGRPIC8_H
#define MCUSIMCFGMGRPIC8_H

// Forward declarations
class McuDeviceSpec;

#include "MCUSim.h"

#include "PIC8/PIC8PinNames.h"
#include <QStringList>
#include <QVector>
#include <QString>
#include <QStack>
#include <QXmlAttributes>

/**
 * @brief
 * @ingroup McuSimCfgMgr
 * @class McuSimCfgMgrPIC8
 */
class McuSimCfgMgrPIC8
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
        McuSimCfgMgrPIC8 ( QVector<McuDeviceSpec*> * devices,
                           QStack<QString> * currentXMLElement,
                           QStringList * expectedXMLElements,
                           bool * expectCharacters,
                           int * auxInt0,
                           int * auxInt1 );

    private:
        /**
         * @brief Forbidden constructor.
         */
        McuSimCfgMgrPIC8();

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

    ////    Inline Private Operations    ////
    private:

        /**
         * @brief
         * @param[in] str
         * @param[out] ok
         * @return
         */
        inline PIC8PinNames::PIN sfunc2pinEnum ( const QString & str,
                                                 bool * ok = NULL );

        /**
         * @brief
         * @param[in] str
         * @param[out] ok
         * @return
         */
        inline int name2spfEnum ( const QString & str,
                                  bool * ok = NULL );

        /**
         * @brief
         * @param[in] str
         * @param[out] ok
         * @return
         */
        inline int string2interruptNumber ( const QString & str,
                                            bool * ok = NULL );

    ////    Private Attributes    ////
    private:
        QVector<McuDeviceSpec*> & m_devices;   ///<
        QStack<QString> & m_currentXMLElement; ///<
        QStringList & m_expectedXMLElements;   ///<
        bool & m_expectCharacters;             ///<
        int & m_auxInt0;                       ///< Auxiliary integer variable #1
        int & m_auxInt1;                       ///< Auxiliary integer variable #2
};

#endif // MCUSIMCFGMGRPIC8_H
