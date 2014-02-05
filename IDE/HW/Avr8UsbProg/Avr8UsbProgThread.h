#ifndef AVR8USBPROGTHREAD_H
#define AVR8USBPROGTHREAD_H

#include "Avr8UsbProgCore.h"

#include <QQueue>
#include <QMutex>
#include <QThread>
#include <QVariant>

class Avr8UsbProgThread : public QThread
{
    Q_OBJECT

public:
        Avr8UsbProgThread(QObject* parent=0);
        ~Avr8UsbProgThread();

        typedef enum {
                INVALID_OPERATION
        } Exception;

        void run();

        const Avr8UsbProgDeviceDB::Device * getMcuDetails();

public slots:
        void executeCommands();

        void searchForProgrammers();
        void startProgramming(QString devSerialNumber, int speedLevel = 4);
        void endProgramming();

        void abort();

        void eraseChip();
        void readCalibrationByte();

        void downloadLockBits();
        void uploadLockBits(Avr8UsbProgCore::LockBits & lockBits);

        void downloadFuseBits();
        void uploadFuseBits(Avr8UsbProgCore::FuseBits & fuseBits);
        void uploadFuseBitsDefaults();

        void downloadEEPROM(QString filename, bool makeBackup=true);
        void uploadEEPROM(QString filename, bool verify=true);

        void downloadFlash(QString filename, bool makeBackup=true);
        void uploadFlash(QString filename, bool verify=true);

private:
        typedef enum {
                SEARCH_FOR_PROGRAMMERS,

                START_PROGRAMMING,
                END_PROGRAMMING,

                ERASE_CHIP,
                READ_CALIBRATION_BYTE,

                DOWNLOAD_LOCK_BITS,
                UPLOAD_LOCK_BITS,

                DOWNLOAD_FUSE_BITS,
                UPLOAD_FUSE_BITS,
                UPLOAD_FUSE_BITS_DEFAULTS,

                DOWNLOAD_EEPROM,
                UPLOAD_EEPROM,

                DOWNLOAD_FLASH,
                UPLOAD_FLASH
        } Command;

        QMutex mutex;
        QQueue<Command> commandQueue;
        QQueue<QVariant> argumentQueue;

        bool executeCommandsFlag;
        bool abortVariable;

        Avr8UsbProgCore m_programmerCore;

        void abortNow();
        inline void createConnections();
        inline void commandLoop();
        inline bool executeNextCommand();

private slots:
        void errorOccuredSlot(QString errorInfo);

signals:
        void newProgrammersFound(const QStringList & programmers);

        void aborted();
        void errorOccured(QString errorInfo);

        void connectionEstablished();

        void terminatingConnection();
        void connectionTerminated();

        void deviceDetected();

        void uploadingLockBits();
        void lockBitsUploaded();

        void downloadingLockBits();
        void lockBitsDownloaded(Avr8UsbProgCore::LockBits lockBits);

        void uploadingFuseBits();
        void fuseBitsUploaded();

        void downloadingFuseBits();
        void fuseBitsDownloaded(Avr8UsbProgCore::FuseBits fuseBits);

        void uploadingFlash();
        void flashUploaded();

        void downloadingFlash();
        void flashDownloaded();

        void uploadingEEPROM();
        void eepromUploaded();

        void downloadingEEPROM();
        void eepromDownloaded();

        void readingCalibrationByte();
        void calibrationByteRead(QVector<unsigned char> calibrationBytes);

        void erasingChip();
        void chipErased();

        void uploadingFuseBitsDefaults();

        void progressChanged(int progress);
};

#endif // AVR8USBPROGTHREAD_H
