#include "Avr8UsbProgThread.h"

#include <QDebug>
#include <QMetaObject>
#include <QMutexLocker>

Avr8UsbProgThread::Avr8UsbProgThread(QObject* parent) : QThread(parent)
{
        createConnections();
        abortVariable=false;
        executeCommandsFlag=false;
}

Avr8UsbProgThread::~Avr8UsbProgThread()
{
        abortNow();
        m_programmerCore.endProgramming();
        wait();
}

inline void Avr8UsbProgThread::createConnections()
{
        connect(&m_programmerCore,
                SIGNAL(errorOccured(QString)),
                SLOT(errorOccuredSlot(QString)));
        connect(&m_programmerCore,
                SIGNAL(newProgrammersFound(const QStringList &)),
                SIGNAL(newProgrammersFound(const QStringList &)));
        connect(&m_programmerCore,
                SIGNAL(aborted()),
                SIGNAL(aborted()));
        connect(&m_programmerCore,
                SIGNAL(aborted()),
                SIGNAL(aborted()));
        connect(&m_programmerCore,
                SIGNAL(connectionEstablished()),
                SIGNAL(connectionEstablished()));
        connect(&m_programmerCore,
                SIGNAL(connectionTerminated()),
                SIGNAL(connectionTerminated()));
        connect(&m_programmerCore,
                SIGNAL(deviceDetected()),
                SIGNAL(deviceDetected()));
        connect(&m_programmerCore,
                SIGNAL(lockBitsUploaded()),
                SIGNAL(lockBitsUploaded()));
        connect(&m_programmerCore,
                SIGNAL(lockBitsDownloaded(Avr8UsbProgCore::LockBits)),
                SIGNAL(lockBitsDownloaded(Avr8UsbProgCore::LockBits)));
        connect(&m_programmerCore,
                SIGNAL(fuseBitsUploaded()),
                SIGNAL(fuseBitsUploaded()));
        connect(&m_programmerCore,
                SIGNAL(fuseBitsDownloaded(Avr8UsbProgCore::FuseBits)),
                SIGNAL(fuseBitsDownloaded(Avr8UsbProgCore::FuseBits)));
        connect(&m_programmerCore,
                SIGNAL(flashUploaded()),
                SIGNAL(flashUploaded()));
        connect(&m_programmerCore,
                SIGNAL(flashDownloaded()),
                SIGNAL(flashDownloaded()));
        connect(&m_programmerCore,
                SIGNAL(eepromUploaded()),
                SIGNAL(eepromUploaded()));
        connect(&m_programmerCore,
                SIGNAL(eepromDownloaded()),
                SIGNAL(eepromDownloaded()));
        connect(&m_programmerCore,
                SIGNAL(calibrationByteRead(QVector<unsigned char>)),
                SIGNAL(calibrationByteRead(QVector<unsigned char>)));
        connect(&m_programmerCore,
                SIGNAL(chipErased()),
                SIGNAL(chipErased()));
        connect(&m_programmerCore,
                SIGNAL(progressChanged(int)),
                SIGNAL(progressChanged(int)));
}

void Avr8UsbProgThread::errorOccuredSlot(QString errorInfo)
{
        abortNow();
        emit errorOccured(errorInfo);
}

void Avr8UsbProgThread::run()
{
    qDebug() << "Avr8UsbProgThread::run()";
//         argumentQueue.clear();
//         commandQueue.clear();
//         executeCommandsFlag=false;

        forever {
                msleep(100);
                commandLoop();
        }
}

inline void Avr8UsbProgThread::commandLoop()
{
//     qDebug() << "Avr8UsbProgThread::commandLoop()";
        QMutexLocker locker(&mutex);

        if(!executeCommandsFlag)
                return;

        while(executeNextCommand());

        executeCommandsFlag=false;
}

inline bool Avr8UsbProgThread::executeNextCommand()
{
qDebug() << "executeNextCommand()";
        if(abortVariable) {
                abortNow();
                emit terminatingConnection();
                m_programmerCore.endProgramming();
                return false;
        }

        if(commandQueue.isEmpty())
        {
                return false;
        }

        switch(commandQueue.dequeue()) {

                case SEARCH_FOR_PROGRAMMERS:
                    m_programmerCore.searchForProgrammers();
                    break;

                case START_PROGRAMMING: {
                        QVariant funcArg0=argumentQueue.dequeue();
                        QVariant funcArg1=argumentQueue.dequeue();

                        m_programmerCore.startProgramming(
                                funcArg0.value<QString>(),
                                funcArg1.value<int>());
                        break;
                }
                case END_PROGRAMMING:
                        emit terminatingConnection();
                        m_programmerCore.endProgramming();
                        break;
                case ERASE_CHIP:
                        emit erasingChip();
                        m_programmerCore.eraseChip();
                        break;
                case READ_CALIBRATION_BYTE:
                        emit readingCalibrationByte();
                        m_programmerCore.readCalibrationByte();
                        break;
                case DOWNLOAD_LOCK_BITS:
                        emit downloadingLockBits();
                        m_programmerCore.downloadLockBits();
                        break;
                case UPLOAD_LOCK_BITS: {
                        emit uploadingLockBits();
                        Avr8UsbProgCore::LockBits lockBits;
                        lockBits=argumentQueue.dequeue().value<Avr8UsbProgCore::LockBits>();
                        m_programmerCore.uploadLockBits(lockBits);
                        break;
                }
                case DOWNLOAD_FUSE_BITS:
                        emit downloadingFuseBits();
                        m_programmerCore.downloadFuseBits();
                        break;
                case UPLOAD_FUSE_BITS: {
                        emit uploadingFuseBits();
                        Avr8UsbProgCore::FuseBits fuseBits;
                        fuseBits=argumentQueue.dequeue().value<Avr8UsbProgCore::FuseBits>();
                        m_programmerCore.uploadFuseBits(fuseBits);
                        break;
                }
                case UPLOAD_FUSE_BITS_DEFAULTS:
                        emit uploadingFuseBitsDefaults();
                        m_programmerCore.uploadFuseBitsDefaults();
                        break;
                case DOWNLOAD_EEPROM: {
                        emit downloadingEEPROM();
                        QVariant funcArg0=argumentQueue.dequeue();
                        QVariant funcArg1=argumentQueue.dequeue();

                        m_programmerCore.downloadEEPROM(
                                funcArg0.value<QString>(),
                                funcArg1.value<bool>());
                        break;
                }
                case UPLOAD_EEPROM: {
                        emit uploadingEEPROM();
                        QVariant funcArg0=argumentQueue.dequeue();
                        QVariant funcArg1=argumentQueue.dequeue();

                        m_programmerCore.uploadEEPROM(
                                funcArg0.value<QString>(),
                                funcArg1.value<bool>());
                        break;
                }
                case DOWNLOAD_FLASH: {
                        emit downloadingFlash();
                        QVariant funcArg0=argumentQueue.dequeue();
                        QVariant funcArg1=argumentQueue.dequeue();

                        m_programmerCore.downloadFlash(
                                funcArg0.value<QString>(),
                                funcArg1.value<bool>());
                        break;
                }
                case UPLOAD_FLASH: {
                        emit uploadingFlash();
                        QVariant funcArg0=argumentQueue.dequeue();
                        QVariant funcArg1=argumentQueue.dequeue();

                        m_programmerCore.uploadFlash(
                                funcArg0.value<QString>(),
                                funcArg1.value<bool>());
                        break;
                }
                default:
                        throw INVALID_OPERATION;
        }

        return true;
}

void Avr8UsbProgThread::executeCommands()
{
        QMutexLocker locker(&mutex);
        executeCommandsFlag=true;
}

void Avr8UsbProgThread::searchForProgrammers()
{
    QMutexLocker locker(&mutex);
    commandQueue.enqueue(SEARCH_FOR_PROGRAMMERS);
}

void Avr8UsbProgThread::startProgramming(QString devPath, int speedLevel)
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(START_PROGRAMMING);
        argumentQueue.enqueue(devPath);
        argumentQueue.enqueue(speedLevel);
}
void Avr8UsbProgThread::endProgramming()
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(END_PROGRAMMING);
}

void Avr8UsbProgThread::abortNow()
{
        m_programmerCore.abort();

        commandQueue.clear();
        argumentQueue.clear();

        abortVariable=false;
}

void Avr8UsbProgThread::abort()
{
        m_programmerCore.abort();
        abortVariable=true;
}

void Avr8UsbProgThread::eraseChip()
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(ERASE_CHIP);
}
void Avr8UsbProgThread::readCalibrationByte()
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(READ_CALIBRATION_BYTE);
}

void Avr8UsbProgThread::downloadLockBits()
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(DOWNLOAD_LOCK_BITS);
}
void Avr8UsbProgThread::uploadLockBits(Avr8UsbProgCore::LockBits & lockBits)
{
        QVariant v;
        v.setValue(lockBits);

        QMutexLocker locker(&mutex);
        commandQueue.enqueue(UPLOAD_LOCK_BITS);
        argumentQueue.enqueue(v);
}

void Avr8UsbProgThread::downloadFuseBits()
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(DOWNLOAD_FUSE_BITS);
}
void Avr8UsbProgThread::uploadFuseBits(Avr8UsbProgCore::FuseBits & fuseBits)
{
        QVariant v;
        v.setValue(fuseBits);

        QMutexLocker locker(&mutex);
        commandQueue.enqueue(UPLOAD_FUSE_BITS);
        argumentQueue.enqueue(v);
}
void Avr8UsbProgThread::uploadFuseBitsDefaults()
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(UPLOAD_FUSE_BITS_DEFAULTS);
}

void Avr8UsbProgThread::downloadEEPROM(QString filename, bool makeBackup)
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(DOWNLOAD_EEPROM);
        argumentQueue.enqueue(filename);
        argumentQueue.enqueue(makeBackup);
}
void Avr8UsbProgThread::uploadEEPROM(QString filename, bool verify)
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(UPLOAD_EEPROM);
        argumentQueue.enqueue(filename);
        argumentQueue.enqueue(verify);
}

void Avr8UsbProgThread::downloadFlash(QString filename, bool makeBackup)
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(DOWNLOAD_FLASH);
        argumentQueue.enqueue(filename);
        argumentQueue.enqueue(makeBackup);
}

void Avr8UsbProgThread::uploadFlash(QString filename, bool verify)
{
        QMutexLocker locker(&mutex);
        commandQueue.enqueue(UPLOAD_FLASH);
        argumentQueue.enqueue(filename);
        argumentQueue.enqueue(verify);
}

const Avr8UsbProgDeviceDB::Device * Avr8UsbProgThread::getMcuDetails()
{
        return m_programmerCore.getMcuDetails();
}
