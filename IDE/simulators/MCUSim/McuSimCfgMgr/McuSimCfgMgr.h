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
 * @file McuSimCfgMgr.h
 */
// =============================================================================

#ifndef MCUSIMCFGMGR_H
#define MCUSIMCFGMGR_H

// Forward declarations
class McuDeviceSpec;
class AdjSimProcDef;
class McuSimCfgMgrAVR8;
class McuSimCfgMgrPIC8;

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
class McuSimCfgMgr : public QXmlDefaultHandler
{
    ////    Constructors and Destructors    ////
    private:
        /**
         * @brief
         */
        McuSimCfgMgr();

        /**
         * @brief
         */
        virtual ~McuSimCfgMgr();

    ////    Static Public Operations    ////
    public:
        /**
         * @brief
         * @return
         */
        static McuSimCfgMgr * getInstance();

    ////    Public Operations    ////
    public:
        /**
         * @brief
         * @param[in] filename
         * @return
         */
        bool openConfigFile ( const char * filename );

        /**
         * @brief
         * @param[in] mcuName
         * @param[out] mcuConfig
         * @param[in] procDef
         * @return
         */
        bool setupSimulator ( const std::string & mcuName,
                              MCUSimConfig & mcuConfig,
                              const AdjSimProcDef * procDef = NULL ) const;

        /**
         * @brief
         * @param[in] mcuName
         * @return
         */
        const McuDeviceSpec * getDeviceSpec ( const std::string & mcuName ) const;

    ////    Private Operations    ////
    private:
        /**
         * @brief
         * @param[in] ch
         * @return
         */
        bool characters ( const QString & ch );

        /**
         * @brief
         * @param[in] namespaceURI
         * @param[in] localName
         * @param[in] qName
         * @return
         */
        bool endElement ( const QString & namespaceURI,
                          const QString & localName,
                          const QString & qName );

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
         * @param[in] namespaceURI
         * @param[in] localName
         * @param[in] qName
         * @param[in] atts
         * @return
         */
        bool startElementAVR8 ( const QString & namespaceURI,
                                const QString & localName,
                                const QString & qName,
                                const QXmlAttributes & atts );

    ////    Inline Private Operations    ////
    private:
        /**
         * @brief
         */
        inline void clear();

        /**
         * @brief
         * @param[in] localName
         * @param[in] atts
         * @return
         */
        inline bool attributesAVR8 ( const QString & localName,
                                     const QXmlAttributes & atts );

        /**
         * @brief
         * @param[in] ch
         * @return
         */
        inline bool charactersAVR8 ( const QString & ch );

    ////    Private Attributes    ////
    private:
        static McuSimCfgMgr * m_instance;       ///<
        QVector<McuDeviceSpec*> m_devices;      ///<

        McuSimCfgMgrAVR8 * m_mcuSimCfgMgrAVR8;  ///<
        McuSimCfgMgrPIC8 * m_mcuSimCfgMgrPIC8;  ///<

        QStack<QString> m_currentXMLElement;    ///<
        QStringList m_expectedXMLElements;      ///<
        bool m_expectCharacters;                ///<
        int m_auxInt0;                          ///< Auxiliary integer variable #1
        int m_auxInt1;                          ///< Auxiliary integer variable #2
};

#endif // MCUSIMCFGMGR_H
