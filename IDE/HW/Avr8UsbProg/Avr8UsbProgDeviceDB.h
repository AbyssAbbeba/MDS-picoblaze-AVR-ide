#ifndef AVR8USBPROGDEVICEDB_H
#define AVR8USBPROGDEVICEDB_H

#include <QString>
#include <QVector>
#include <QStack>
#include <QStringList>
#include <QXmlDefaultHandler>
#include <QXmlAttributes>

/**
 * Interface to XML "database" of supported devices
 *
 * This class can identify target MCU by its signature. And can gain some
 * informations specifing how the device can be programmed.
 */
class Avr8UsbProgDeviceDB : public QXmlDefaultHandler
{
        public:
                Avr8UsbProgDeviceDB();

                typedef enum {
                        XML_PARSE_ERROR
                } Exception;

                /**
                 *
                 */
                typedef struct {
                        QString name;
                        QString web;
                        int eraseDelay;
                        int numberOfCalibrationBytes;
                        ///
                        struct Signature {
                                unsigned char address;  ///<
                                unsigned char byte;     ///<
                        } signature[3];
                        struct Flash {
                                int size;               ///<
                                int page;               ///<
                                int delay;              ///<
                        } flash;
                        struct Eeprom {
                                int size;               ///<
                                int page;               ///<
                                int delay;              ///<
                        } eeprom;
                        ///
                        struct Fuses {
                                int delay;
                                ///
                                struct Flb {
                                        QString bitName[8];             ///<
                                        QString bitDescription[8];      ///<
                                        bool defaultState[8];           ///<
                                } flb;
                                ///
                                struct Fhb {
                                        QString bitName[8];             ///<
                                        QString bitDescription[8];      ///<
                                        bool defaultState[8];           ///<
                                } fhb;
                                ///
                                struct Feb {
                                        QString bitName[8];             ///<
                                        QString bitDescription[8];      ///<
                                        bool defaultState[8];           ///<
                                } feb;
                        } fuses;
                        ///
                        struct LockBits {
                                QString bitName[8];                     ///<
                                QString bitDescription[8];              ///<
                        } lockBits;
                } Device;

                QString errorMessage;   ///<

                /**
                 *
                 * @return
                 */
                int size() const;
                /**
                 *
                 * @param idx
                 * @return
                 */
                const Device & at(int idx) const;
                /**
                 *
                 * @param idx
                 * @return
                 */
                const Device & operator[](int idx) const;

        private:
                QVector<Device*> devices;               ///<
                QStack<QString> currentXMLElement;      ///<
                QStringList expectedXMLElements;        ///<
                bool expectCharacters;                  ///<
                int bitNumber;                          ///<

                /**
                 *
                 * @return
                 */
                bool initiateBD();

                /**
                 *
                 * @return
                 */
                const QString & errorString();

                /**
                 *
                 * @param localName
                 * @param atts
                 * @return
                 */
                bool parseAttributes(const QString &localName, const QXmlAttributes &atts);

                /**
                 *
                 * @param ch
                 * @return
                 */
                bool characters(const QString & ch);

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
};

#endif // AVR8USBPROGDEVICEDB_H
