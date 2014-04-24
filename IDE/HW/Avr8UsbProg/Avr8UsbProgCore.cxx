#include "Avr8UsbProgCore.h"

#include "HexFile.h"
#include "SrecFile.h"
#include "BinFile.h"

#include <QVector>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <cctype>


Avr8UsbProgCore::Avr8UsbProgCore()
{
        Q_INIT_RESOURCE(Avr8UsbProg);

        qRegisterMetaType<Avr8UsbProgCore::FuseBits>("Avr8UsbProgCore::FuseBits");
        qRegisterMetaType<Avr8UsbProgCore::LockBits>("Avr8UsbProgCore::LockBits");
        qRegisterMetaType<QVector<unsigned char> >("QVector<unsigned char>");

        detectedMcu=-1;
        abortVariable=false;
}

const Avr8UsbProgDeviceDB::Device * Avr8UsbProgCore::getMcuDetails()
{
        if(detectedMcu == -1)
                return 0;

        return &devicedb[detectedMcu];
}

void Avr8UsbProgCore::searchForProgrammers()
{
    QStringList programmers;
    isp.findProgrammers(programmers);
qDebug() << "searchForProgrammers() --> '" << programmers << "'";
    if ( m_programmers.size() != programmers.size() )
    {
        m_programmers = programmers;
        emit(newProgrammersFound(m_programmers));
    }
    else
    {
        foreach ( QString prog, programmers )
        {
            if ( false == m_programmers.contains(prog) )
            {
                m_programmers = programmers;
                emit(newProgrammersFound(m_programmers));
                break;
            }
        }
    }
}

void Avr8UsbProgCore::startProgramming(const QString & devSerialNumber, int speedLevel)
{
        abortVariable=false;

        // No MCU detected yet
        detectedMcu=-1;

        try {
                bool success=false;

                // Open connection with programmer HW
                isp.openDevice(devSerialNumber, speedLevel);

                // Open connection with MCU in the programmer
                for(int tryNo=0; tryNo<PROGRAMMER_MAX_TRYES; tryNo++) {
                        if(abortVariable) {
                                emit(aborted());
                                return;
                        }

                        if(isp.ProgrammingEnable()) {
                                success=true;
                                break;
                        }

                        isp.resetPositivePulse();
                }
                if(!success)
                        throw MAXIMUM_NUMBER_OF_TRYES_EXCEEDED;
                emit(connectionEstablished());

#ifdef DEBUG
        qDebug() << "Devices == " << devicedb.size();
#endif // DEBUG

                // Identify (detect) the MCU acording to its signature
                unsigned char signatureByte[3];
                for(int i=0; i<3; i++)
                        signatureByte[i]=isp.readSignatureByte(i);
                for(int devNo=0; devNo<devicedb.size(); devNo++) {
                        bool detected=true;

#ifdef DEBUG
        qDebug() << "Reading signature bytes -- device " << devNo+1 << " of " << devicedb.size();
#endif // DEBUG

                        for(int i=0; i<3; i++) {
                                if(abortVariable) {
                                        emit(aborted());
                                        return;
                                }

#ifdef DEBUG
        qDebug() << "  address: " << hex << int(devicedb[devNo].signature[i].address)
             << "  expected=" << int(devicedb[devNo].signature[i].byte)
             << "  read=" << int(signatureByte[i]);
#endif // DEBUG

                                if(signatureByte[i] != devicedb[devNo].signature[i].byte) {
                                        detected=false;
                                        break;
                                }
                        }

                        if(detected) {
                                detectedMcu=devNo;
                                emit(deviceDetected());
                                break;
                        }
                }
                if(detectedMcu == -1)
                        throw NO_MCU_DETECTED;

                isp.setDelays(devicedb[detectedMcu].fuses.delay,
                        devicedb[detectedMcu].flash.delay,
                        devicedb[detectedMcu].eeprom.delay,
                        devicedb[detectedMcu].eraseDelay);


//         } catch(HWInterface::Exception e) {
//                 switch(e) {
//                         case HWInterface::NOT_A_TERMINAL:
//                                 emit(errorOccured(tr("Selected communication interface is not usable")));
//                                 break;
//                         case HWInterface::UNABLE_TO_OPEN_FILE:
//                                 emit(errorOccured(tr("Unable to open selected communication interface")));
//                                 break;
//                         case HWInterface::INVALID_DEVICE_FILE:
//                                 emit(errorOccured(tr("No communication interface specified")));
//                                 break;
//                         case HWInterface::IOTCL_ERROR:
//                                 emit(errorOccured(tr("Unknown IO error")));
//                                 break;
//                         default:
//                                 emit(errorOccured(tr("Unknown exception")));
//                 }

        } catch(Avr8UsbProgCore::Exception e) {
                switch(e) {
                        case Avr8UsbProgCore::MAXIMUM_NUMBER_OF_TRYES_EXCEEDED:
                                emit(errorOccured(tr("Microcontoller has not responded.")));
                                break;
                        case Avr8UsbProgCore::NO_MCU_DETECTED:
                                emit(errorOccured(tr("Unable to recognize connected microcontoller")));
                                break;
                        default:
                                emit(errorOccured(tr("Unknown exception")));
                }
        }
}

void Avr8UsbProgCore::endProgramming()
{
//         try {
                isp.closeDevice();
/*
        } catch(HWInterface::Exception e) {
                emit(errorOccured(tr("IO Error")));
        }*/

        emit(connectionTerminated());
}

void Avr8UsbProgCore::abort()
{
        abortVariable=true;
}

void Avr8UsbProgCore::eraseChip()
{
        if(abortVariable || detectedMcu == -1)
                return;

//         try {
                isp.chipErase();
                emit(chipErased());
/*
        } catch(HWInterface::Exception e) {
                emit(errorOccured(tr("Hardware IO error")));
        }*/
}

void Avr8UsbProgCore::readCalibrationByte()
{
        if(abortVariable || detectedMcu == -1)
                return;

//         try {
                QVector<unsigned char> calibrationBytes;

                for(int i=0; i<devicedb[detectedMcu].numberOfCalibrationBytes; i++) {
                        calibrationBytes << isp.readCalibrationByte(i);
                }

                emit(calibrationByteRead(calibrationBytes));

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));
//         }
}

void Avr8UsbProgCore::downloadLockBits()
{
        if(abortVariable || detectedMcu == -1)
                return;

//         try {
                bool bytePresent;
                LockBits lockBits;

                // Read lock bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].lockBits.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=isp.readLockBits();

                        for(int i=0; i<8; i++) {
                                lockBits.bit[i]=byte & (1 << i);
                        }
                }
                emit(lockBitsDownloaded(lockBits));

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));
//         }
}

void Avr8UsbProgCore::uploadLockBits(LockBits & lockBits)
{
        if(abortVariable || detectedMcu == -1)
                return;

//         try {
                bool bytePresent;
                unsigned char byte=0xff;

                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].lockBits.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(!bytePresent)
                        throw UNSUPPORTED_OPERATION;

                for(int i=0; i<8; i++) {
                        if(!lockBits.bit[i])
                                byte ^= 1<<i;
                }

                if(byte != 0xff)
                        isp.writelockbits(byte);
/*
        } catch(HWInterface::Exception e) {
                emit(errorOccured(tr("Hardware IO error")));
        }*/
}

void Avr8UsbProgCore::uploadFuseBitsDefaults()
{
        if(abortVariable || detectedMcu == -1)
                return;

//         try {
                FuseBits fuseBits;

                for(int i=0; i<8; i++) {
                        fuseBits.extendedBit[i] = devicedb[detectedMcu].fuses.feb.defaultState[i];
                        fuseBits.highBit[i]     = devicedb[detectedMcu].fuses.fhb.defaultState[i];
                        fuseBits.lowBit[i]      = devicedb[detectedMcu].fuses.flb.defaultState[i];
                }

                uploadFuseBits(fuseBits);

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));
//         }
}

Avr8UsbProgCore::FuseBits Avr8UsbProgCore::downloadFuseBits(bool emitSignalAfterCompletion)
{
        FuseBits fuseBits;

        if(abortVariable || detectedMcu == -1)
                return fuseBits;

//         try {
                bool bytePresent;

                // Read extended fuse bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].fuses.feb.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=isp.readExtendedFuseBits();

                        for(int i=0; i<8; i++) {
                                fuseBits.extendedBit[i]=byte & (1 << i);
                        }
                }

                // Read fuse high bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].fuses.fhb.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=isp.readFuseHighBits();

                        for(int i=0; i<8; i++) {
                                fuseBits.highBit[i]=byte & (1 << i);
                        }
                }

                // Read fuse bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].fuses.flb.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=isp.readFuseBits();

                        for(int i=0; i<8; i++) {
                                fuseBits.lowBit[i]=byte & (1 << i);
                        }
                }

                if(emitSignalAfterCompletion)
                        emit(fuseBitsDownloaded(fuseBits));

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));
//         }

        return fuseBits;
}
void Avr8UsbProgCore::uploadFuseBits(FuseBits & fuseBits)
{
        if(abortVariable || detectedMcu == -1)
                return;

//         try {
                bool bytePresent;

                // Write extended fuse bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].fuses.feb.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=0xff;

                        for(int i=0; i<8; i++) {
                                if(!fuseBits.extendedBit[i])
                                        byte ^= 1<<i;
                        }

                        isp.writeExtendedFuseBits(byte);
                }

                // Write fuse high bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].fuses.fhb.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=0xff;

                        for(int i=0; i<8; i++) {
                                if(!fuseBits.highBit[i])
                                        byte ^= 1<<i;
                        }

                        isp.writeFuseHighBits(byte);
                }

                // Write fuse bits if present
                bytePresent=false;
                for(int i=0; i<8; i++) {
                        if(!devicedb[detectedMcu].fuses.flb.bitName[i].isEmpty()) {
                                bytePresent=true;
                                break;
                        }
                }
                if(bytePresent) {
                        unsigned char byte=0xff;

                        for(int i=0; i<8; i++) {
                                if(!fuseBits.lowBit[i])
                                        byte ^= 1<<i;
                        }

                        isp.writeFuseBits(byte);
                }

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));
//         }
}

void Avr8UsbProgCore::downloadEEPROM(QString filename, bool makeBackup)
{
        if(abortVariable || detectedMcu == -1)
                return;

        try {
                downloadData(filename, EEPROM, makeBackup);

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));

        } catch(const DataFileException & e) {
//                 switch(e) {
//                         case HexFile::IO_ERROR:
//                                 emit(errorOccured(tr("Download EEPROM: Unable to write to the specified file")));
//                                 break;
//                         default:
//                                 emit(errorOccured(tr("Download EEPROM: Invalid data file")));
//                 }
        } catch(const Avr8UsbProgCore::Exception & e) {
                switch(e) {
                        case Avr8UsbProgCore::REQUESTED_MEMORY_IS_NOT_PRESENT:
                                emit(errorOccured(tr("Download EEPROM: This MCU has no interface to read EEPROM memory")));
                                break;
                        default:
                                emit(errorOccured(tr("Download EEPROM: Unknown exception")));
                }
        }
}
void Avr8UsbProgCore::uploadEEPROM(QString filename, bool verify)
{
        if(abortVariable || detectedMcu == -1)
                return;

        try {
                uploadData(filename, EEPROM, verify);

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));

        } catch(const DataFileException & e) {
//                 switch(e) {
//                         case HexFile::IO_ERROR:
//                                 emit(errorOccured(tr("Upload EEPROM: Unable to open data file")));
//                                 break;
//                         case HexFile::BAD_RECORD_LENGTH:
//                                 emit(errorOccured(tr("Upload EEPROM: Bad record length")));
//                                 break;
//                         case HexFile::BAD_CRC:
//                                 emit(errorOccured(tr("Upload EEPROM: Bad record CRC")));
//                                 break;
//                         case HexFile::BAD_RECORD_TYPE:
//                                 emit(errorOccured(tr("Upload EEPROM: Bad record type")));
//                                 break;
//                         case HexFile::MEMORY_OVERFLOW:
//                                 emit(errorOccured(tr("Upload EEPROM: File contains more data than can be loaded into this MCU")));
//                                 break;
//                         default:
//                                 emit(errorOccured(tr("Upload EEPROM: Invalid data file")));
//                 }
        } catch(Avr8UsbProgCore::Exception e) {
                switch(e) {
                        case Avr8UsbProgCore::REQUESTED_MEMORY_IS_NOT_PRESENT:
                                emit(errorOccured(tr("Upload EEPROM: This MCU has no interface to program EEPROM memory")));
                                break;
                        case Avr8UsbProgCore::UNSUPPORTED_OPERATION:
                                emit(errorOccured(tr("Upload EEPROM: Unsupported operation, that means SOFTWARE BUG !")));
                                break;
                        case Avr8UsbProgCore::VERIFYCATION_FAILED:
                                emit(errorOccured(tr("Upload EEPROM: Verification failed")));
                                break;
                        default:
                                emit(errorOccured(tr("Upload EEPROM: Unknown exception")));
                }
        }
}

void Avr8UsbProgCore::downloadFlash(QString filename, bool makeBackup)
{
        if(abortVariable || detectedMcu == -1)
                return;

        try {
                downloadData(filename, FLASH, makeBackup);

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));

        } catch(const DataFileException & e) {
//                 switch(e) {
//                         case HexFile::IO_ERROR:
//                                 emit(errorOccured(tr("Download flash: Unable to write to the specified file")));
//                                 break;
//                         default:
//                                 emit(errorOccured(tr("Download flash: Invalid data file")));
//                 }
        } catch(Avr8UsbProgCore::Exception e) {
                switch(e) {
                        case Avr8UsbProgCore::REQUESTED_MEMORY_IS_NOT_PRESENT:
                                emit(errorOccured(tr("Download flash: This MCU has no interface to read flash memory")));
                                break;
                        default:
                                emit(errorOccured(tr("Download flash: Unknown exception")));
                }
        }
}

void Avr8UsbProgCore::uploadFlash(QString filename, bool verify)
{
        if(abortVariable || detectedMcu == -1)
                return;

        try {
                uploadData(filename, FLASH, verify);

//         } catch(HWInterface::Exception e) {
//                 emit(errorOccured(tr("Hardware IO error")));

        } catch(const DataFileException & e) {
//                 switch(e) {
//                         case HexFile::IO_ERROR:
//                                 emit(errorOccured(tr("Upload flash: Unable to open data file")));
//                                 break;
//                         case HexFile::BAD_RECORD_LENGTH:
//                                 emit(errorOccured(tr("Upload flash: Bad record length")));
//                                 break;
//                         case HexFile::BAD_CRC:
//                                 emit(errorOccured(tr("Upload flash: Bad record CRC")));
//                                 break;
//                         case HexFile::BAD_RECORD_TYPE:
//                                 emit(errorOccured(tr("Upload flash: Bad record type")));
//                                 break;
//                         case HexFile::MEMORY_OVERFLOW:
//                                 emit(errorOccured(tr("Upload flash: File contains more data than can be loaded into this MCU")));
//                                 break;
//                         default:
//                                 emit(errorOccured(tr("Upload flash: Invalid data file")));
//                 }
        } catch(Avr8UsbProgCore::Exception e) {
                switch(e) {
                        case Avr8UsbProgCore::REQUESTED_MEMORY_IS_NOT_PRESENT:
                                emit(errorOccured(tr("Upload flash: This MCU has no interface to program flash memory")));
                                break;
                        case Avr8UsbProgCore::UNSUPPORTED_OPERATION:
                                emit(errorOccured(tr("Upload flash: Unsupported operation, that means SOFTWARE BUG !")));
                                break;
                        case Avr8UsbProgCore::VERIFYCATION_FAILED:
                                emit(errorOccured(tr("Upload flash: Verification failed")));
                                break;
                        default:
                                emit(errorOccured(tr("Upload flash: Unknown exception")));
                }
        }
}

void Avr8UsbProgCore::uploadData(QString filename, FlashOrEeprom flashOrEeprom, bool verify)
{
        int pageSize;
        int memSize;

        if(flashOrEeprom == EEPROM) {
                pageSize=devicedb[detectedMcu].eeprom.page;
                memSize=devicedb[detectedMcu].eeprom.size;
        } else {
                pageSize=devicedb[detectedMcu].flash.page;
                memSize=devicedb[detectedMcu].flash.size;
        }

        if(!memSize)
                throw REQUESTED_MEMORY_IS_NOT_PRESENT;


        switch(determinateFileFormat(filename)) {
                case SREC:
                        emit fileFormatRecognized("Motorola S-Record");
                        datafile=new SrecFile(memSize);
                        break;
                case BIN:
                        emit fileFormatRecognized("Binary file");
                        datafile=new BinFile(memSize);
                        break;
                default:
                        emit fileFormatRecognized("Intel HEX");
                        datafile=new HexFile(memSize);
        }

        Q_CHECK_PTR(datafile);
        datafile->clearAndLoad(filename.toStdString());

        emit(progressChanged(0));

        /* BYTE ACCESS */
        if(!pageSize) {
                for(int addr=0; addr<memSize; addr++) {
                        int byte=datafile->get(addr);

                        if(abortVariable) {
                                emit(aborted());
                                return;
                        }

                        if(byte == -1 || byte == 0xff)
                                continue;

                        if(flashOrEeprom == EEPROM) {
                                isp.writeEepromMemory(addr, byte);

                                if(verify && byte != isp.readEepromMemory(addr))
                                        throw VERIFYCATION_FAILED;

                        } else {
                                throw UNSUPPORTED_OPERATION;
                        }


                        emit(progressChanged(int(addr * 100 / memSize)));
                }

        /* PAGE ACCESS */
        } else {
                int totalPages=memSize/pageSize;
                int nonEmptyHalfPages= 2 * datafile->getNumberOfNotEmptyPages(pageSize);
                int halfPagesWritten=0;
                int pageOffset=-pageSize;

                for(int page=0; page<totalPages; page++) {
                        pageOffset+=pageSize;

                        if(datafile->isPageEmpty(pageSize, page))
                                continue;

                        for(int addr=0; addr<pageSize; addr++) {
                                int byte=datafile->at(pageOffset+addr);

                                if(abortVariable) {
                                        emit(aborted());
                                        return;
                                }

                                if(byte == -1)
                                        byte = 0xff;

                                if(flashOrEeprom == EEPROM) {
                                        isp.loadEepromMemoryPage(addr, (unsigned char)byte);
                                } else {
                                        isp.loadProgramMemoryPage(addr, (unsigned char)byte);
                                }
                        }

                        if(flashOrEeprom == EEPROM) {
                                isp.writeEepromMemoryPage(pageOffset);
                        } else {
                                isp.writeProgramMemoryPage(pageOffset);
                        }

                        halfPagesWritten++;
                        if(verify) {
                                emit(progressChanged(int(halfPagesWritten * 100 / nonEmptyHalfPages)));

                                for(int addr=0; addr<pageSize; addr++) {
                                        int absoluteAddress=pageOffset+addr;
                                        int expectedByte=datafile->at(absoluteAddress);
                                        int byteRead;

                                        if(abortVariable) {
                                                emit(aborted());
                                                return;
                                        }

                                        if(expectedByte == -1)
                                                continue;

                                        if(flashOrEeprom == EEPROM) {
                                                byteRead=isp.readEepromMemory(absoluteAddress);
                                        } else {
                                                byteRead=isp.readProgramMemory(absoluteAddress);
                                        }

                                        if(byteRead != expectedByte)
                                                throw VERIFYCATION_FAILED;
                                }
                        }

                        halfPagesWritten++;
                        emit(progressChanged(int(halfPagesWritten * 100 / nonEmptyHalfPages)));
                }
        }

        emit(progressChanged(100));
        if(datafile)
                delete datafile;

        if(flashOrEeprom == EEPROM) {
                emit(eepromUploaded());
        } else {
                emit(flashUploaded());
        }
}

void Avr8UsbProgCore::downloadData(QString filename, FlashOrEeprom flashOrEeprom, bool makeBackup)
{
        int memSize;

        if(flashOrEeprom == EEPROM) {
                memSize=devicedb[detectedMcu].eeprom.size;
        } else {
                memSize=devicedb[detectedMcu].flash.size;
        }

        if(!memSize)
                throw REQUESTED_MEMORY_IS_NOT_PRESENT;


        switch(determinateFileFormat(filename)) {
                case SREC:
                        emit fileFormatRecognized("Motorola S-Record");
                        datafile=new SrecFile(memSize);
                        break;
                case BIN:
                        emit fileFormatRecognized("Binary file");
                        datafile=new BinFile(memSize);
                        break;
                default:
                        emit fileFormatRecognized("Intel HEX");
                        datafile=new HexFile(memSize);
        }

        Q_CHECK_PTR(datafile);
        datafile->clear();

        emit(progressChanged(0));

        for(int addr=0; addr<memSize; addr++) {
                if(abortVariable) {
                        emit(aborted());
                        return;
                }

                if(flashOrEeprom == EEPROM) {
                        datafile->set(addr, isp.readEepromMemory(addr));

                } else {
                        datafile->set(addr, isp.readProgramMemory(addr));
                }

                emit(progressChanged(int(addr * 100 / memSize)));
        }

        emit(progressChanged(100));

        datafile->save(filename.toStdString(), makeBackup);
        if(datafile)
                delete datafile;

        if(flashOrEeprom == EEPROM) {
                emit(eepromDownloaded());
        } else {
                emit(flashDownloaded());
        }
}

Avr8UsbProgCore::FileFormat Avr8UsbProgCore::determinateFileFormat(const QString & filename)
{

                QFileInfo fileInfo(filename);

                // Not existing file --> Determinate from suffix
                if(!fileInfo.exists()) {
                        // Determinate file name suffix
                        QString ext=fileInfo.suffix().toLower();

                        if(ext == "hex" || ext == "ihx" ) {
                                return HEX;
                        } else if(ext == "srec" || ext == "sre" || ext == "s2" || ext == "mot" || ext == "s19") {
                                return SREC;
                        } else if(ext == "bin" || ext == "raw" || ext == "rom") {
                                return BIN;
                        } else {
                                return UNKNOWN;
                        }

                // Existing file --> Determinate from its contents
                } else {
                        // Try to open the file
                        QFile file(filename);
                        if(!file.open(QIODevice::ReadOnly))
                                return UNKNOWN;

                        // Read first 5 lines
                        for(int i=0; i<5; i++) {
                                QByteArray line=file.readLine(20);
                                int size=line.size();
                                bool detected=true;

                                // Read error --> Unable to determinate file format
                                if(file.error() != QFile::NoError)
                                        return UNKNOWN;

                                // Possibly Intel® HEX record (e.g. ":100130003F0156702B5E712B722B732146013421C7")
                                if(size > 10 && line[0] == ':') {

                                        // All other characters must be hexadecimal digits
                                        for(int i=1; i<size; i++) {
                                                if(!isxdigit(line[i])) {
                                                        detected=false;
                                                        break;
                                                }
                                        }

                                        // Hex file recognized
                                        if(detected)
                                                return HEX;


                                // Possibly Motorola® S-Record (e.g. "S111003848656C6C6F20776F726C642E0A0042")
                                } else if(size > 9 && line[0] == 'S') {
                                        // Second character must be number 1..9
                                        if(!isdigit(line[1]))
                                                continue;

                                        // All other characters must be hexadecimal digits
                                        for(int i=2; i<size; i++) {
                                                if(!isxdigit(line[i])) {
                                                        detected=false;
                                                        break;
                                                }
                                        }

                                        // S-Record file recognized
                                        if(detected)
                                                return SREC;
                                }
                        }

                        // Not recognized so it is an ordinary binary file
                        return BIN;
                }
}
