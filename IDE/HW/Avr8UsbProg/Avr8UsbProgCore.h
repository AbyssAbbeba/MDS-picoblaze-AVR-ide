#ifndef AVR8USBPROGCORE_H
#define AVR8USBPROGCORE_H

#include "Avr8UsbProgIsp.h"
#include "Avr8UsbProgDeviceDB.h"

#include "DataFile.h"

#include <QMetaType>
#include <QString>
#include <QObject>
#include <QStringList>

#define PROGRAMMER_MAX_TRYES 32

/**
 * High level programming interface
 *
 * Implements high level programming interface for this AVR programmer.
 * That means than you don't need to care about particular ISP programming
 * instructions. So you can for instance upload data to flash memory directly
 * from a HEX file, et catera.
 */
class Avr8UsbProgCore : public QObject
{

Q_OBJECT

public:
        Avr8UsbProgCore();

        typedef enum {
                MAXIMUM_NUMBER_OF_TRYES_EXCEEDED,
                NO_MCU_DETECTED,
                REQUESTED_MEMORY_IS_NOT_PRESENT,
                UNSUPPORTED_OPERATION,
                VERIFYCATION_FAILED
        } Exception;

        typedef enum {
                HEX,
                SREC,
                BIN,
                UNKNOWN
        } FileFormat;

        typedef struct Fuses {
                Fuses() {
                        for(int i=0; i<8; i++) {
                                lowBit[i]=true;
                                highBit[i]=true;
                                extendedBit[i]=true;
                        }
                }

                bool lowBit[8];
                bool highBit[8];
                bool extendedBit[8];
        } FuseBits;

        typedef struct Locks {
                Locks() {
                        for(int i=0; i<8; i++)
                                bit[i]=true;
                }
                bool bit[8];
        } LockBits;

        const Avr8UsbProgDeviceDB::Device * getMcuDetails();

private:
        typedef enum {
                FLASH,
                EEPROM
        } FlashOrEeprom;

        bool abortVariable;
        int detectedMcu;

        Avr8UsbProgIsp isp;
        Avr8UsbProgDeviceDB devicedb;
        DataFile * datafile;
        QStringList m_programmers;

        void uploadData(QString filename, FlashOrEeprom flashOrEeprom, bool verify=true);
        void downloadData(QString filename, FlashOrEeprom flashOrEeprom, bool makeBackup=true);

        FileFormat determinateFileFormat(const QString & filename);

public slots:
        void searchForProgrammers();

        void startProgramming(const QString & devSerialNumber, int speedLevel = 2);
        void endProgramming();

        void abort();

        void eraseChip();
        void readCalibrationByte();

        void downloadLockBits();
        void uploadLockBits(LockBits & lockBits);

        FuseBits downloadFuseBits(bool emitSignalAfterCompletion=true);
        void uploadFuseBits(FuseBits & fuseBits);
        void uploadFuseBitsDefaults();

        void downloadEEPROM(QString filename, bool makeBackup=true);
        void uploadEEPROM(QString filename, bool verify=true);

        void downloadFlash(QString filename, bool makeBackup=true);
        void uploadFlash(QString filename, bool verify=true);

signals:
        void newProgrammersFound(const QStringList & programmers);

        void aborted();
        void errorOccured(QString errorInfo);

        void connectionEstablished();
        void connectionTerminated();

        void deviceDetected();

        void lockBitsUploaded();
        void lockBitsDownloaded(Avr8UsbProgCore::LockBits lockBits);

        void fuseBitsUploaded();
        void fuseBitsDownloaded(Avr8UsbProgCore::FuseBits fuseBits);

        void flashUploaded();
        void flashDownloaded();

        void eepromUploaded();
        void eepromDownloaded();

        void calibrationByteRead(QVector<unsigned char> calibrationBytes);
        void chipErased();

        void progressChanged(int progress);

        void fileFormatRecognized(QString formatName);
};

Q_DECLARE_METATYPE(Avr8UsbProgCore::FuseBits);
Q_DECLARE_METATYPE(Avr8UsbProgCore::LockBits);
Q_DECLARE_METATYPE(QVector<unsigned char>);

#endif // AVR8USBPROGCORE_H
