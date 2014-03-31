
#ifndef AVR8USBPROGPLUGIN_H
#define AVR8USBPROGPLUGIN_H

// Programmer HW driver
#include "Avr8UsbProgThread.h"
// Interface of this plugin
#include "HWPluginInterface.h"
// GUI of this plugin
#include "ui_Avr8UsbProgPlugin.h"

#include <QMenu>
#include <QString>
#include <QVector>
#include <QPixmap>
#include <QVariant>
#include <QPalette>
#include <QFileSystemWatcher>

class Avr8UsbProgPlugin : public HWPluginInterface,
                          private Ui::Avr8UsbProgPlugin
{

Q_OBJECT
Q_INTERFACES(HWPluginInterface);

public:
    Avr8UsbProgPlugin();
    ~Avr8UsbProgPlugin();

    typedef struct {
        struct {
            struct {
                bool hidden;
                QString port;
                int speed;
            } programmer;
            struct {
                bool hidden;
            } specialFunctions;
            struct {
                bool hidden;
            } detectedAvr;
            struct {
                bool hidden;
                QString filename;
            } programMemory;
            struct {
                bool hidden;
                QString filename;
            } eepromMemory;
            struct {
                bool hidden;
            } fuseBits;
            struct {
                bool hidden;
            } lockBits;
            struct {
                bool hidden;
                bool enableVerifications;
                bool makeBackupFiles;
                bool compileFirts;
            } options;
        } sections;
        struct {
            int scrollAreaPosition;
        } general;
    } Configuration;

    QVariant * configuration() const;
    void setConfiguration(QVariant * conf=0);

    void setStandAlone(bool standAlone=true);
    bool standAlone() const;

    bool isBusy();

private:
    typedef enum {
        ENTER,
        LEAVE
    } EnterLeave;

    typedef enum {
        RED=0,
        ORANGE=1,
        GREEN=2,
        YELLOW=3
    } StatusIcon;

    typedef enum {
        INACTIVE_STAGE,
        COMPILATION_STAGE,
        PROGRAMMING_STAGE
    } ReprgStage;

    Avr8UsbProgThread programmerThread;
    QFileSystemWatcher* fileSystemWatcher;

    QPixmap* statusIcon[4];

    QPalette QLabelNormalPalette;
    QPalette QLabelGreenPalette;
    QPalette QLabelRedPalette;
    QPalette QLabelBrightRedPalette;
    QPalette QLabelYellowPalette;

    QPalette QLineEditNormalPalette;
    QPalette QLineEditGoodPalette;
    QPalette QLineEditBadPalette;

    bool standAloneFlag;
    bool busyFlag;
    bool deviceDetectedFlag;
    ReprgStage reprogrammingStage;

    QCheckBox* lockBitCheckBoxes[8];
    QCheckBox* fuseBitCheckBoxes[8];
    QCheckBox* fuseBitHighCheckBoxes[8];
    QCheckBox* extendedFuseBitCheckBoxes[8];

    QMenu* helpMenu;

    void createMenus();
    void finalizeGui();
    void initializeObjectVariables();
    void setConnections();
    void collapseExpand(int id, bool allowCollapse=true, bool allowExpand=true);

    void refreshWatchedDirectories();
    bool enterLeaveProgMode(EnterLeave enterLeave);
    void CommitReprogrammingStage();

public slots:
    void reprogramDevice();
    void compilationComplete(bool success=true);
    void setProgramFileName(QString filename);

    void abort();

private slots:
    // Menu(s)
    void helpDialog();
    void whatsThisEnter();
    void aboutDialog();

    // Top bar
    void on_expandAllButton_clicked();
    void on_collapseAllButton_clicked();

    // Collapse / Expand buttons
    void on_programmerCeButton_clicked();
    void on_specialFunctionCeButton_clicked();
    void on_detectedAvrCeButton_clicked();
    void on_programMemoryCeButton_clicked();
    void on_eepromMemoryCeButton_clicked();
    void on_fuseBitsCeButton_clicked();
    void on_lockBitsCeButton_clicked();
    void on_optionsCeButton_clicked();

    // section: Programmer
    void on_speedSlider_valueChanged(int value);
    void on_speedToDefaultButton_clicked();

    // section: Special functions
    void on_detectMcuButton_clicked();
    void on_resetMcuButton_clicked();
    void on_reprogramButton_clicked();
    void on_eraseChipButton_clicked();
    void on_readCalibrationButton_clicked();

    // section: Program memory
    void on_programOpenHexButton_clicked();
    void on_programOpenFileButton_clicked();
    void on_setProgramFileButton_clicked();
    void on_programFileLineEdit_textChanged(const QString & text);
    void on_uploadProgramButton_clicked();
    void on_downloadProgramButton_clicked();

    // section: EEPROM memory
    void on_eepromOpenFileButton_clicked();
    void on_eepromOpenHexButton_clicked();
    void on_eepromFileLineEdit_textChanged(const QString & text);
    void on_uploadEepromButton_clicked();
    void on_downloadEepromButton_clicked();

    // Fuse bits
    void on_setFuseBitsButton_clicked();
    void on_getFuseBitsButton_clicked();
    void on_setFusesToDefaultButton_clicked();

    // Lock bits
    void on_setLockBitsButton_clicked();
    void on_getLockBitsButton_clicked();

    // Bottom bar
    void on_abortButton_clicked();

    // Internal not related to GUI
    void directoryContentsChanged(const QString & path);

    // Intercomunication with the programmer HW driver thread
        void programmerThread_newProgrammersFound(const QStringList & programmers);

    void programmerThread_aborted();
    void programmerThread_errorOccured(QString errorInfo);

    void programmerThread_connectionEstablished();
    void programmerThread_connectionTerminated();

    void programmerThread_deviceDetected();

    void programmerThread_lockBitsUploaded();
    void programmerThread_lockBitsDownloaded(Avr8UsbProgCore::LockBits lockBits);

    void programmerThread_fuseBitsUploaded();
    void programmerThread_fuseBitsDownloaded(Avr8UsbProgCore::FuseBits fuseBits);

    void programmerThread_flashUploaded();
    void programmerThread_flashDownloaded();

    void programmerThread_eepromUploaded();
    void programmerThread_eepromDownloaded();

    void programmerThread_calibrationByteRead(QVector<unsigned char> calibrationBytes);
    void programmerThread_chipErased();

    void programmerThread_progressChanged(int progress);

    void programmerThread_terminatingConnection();
    void programmerThread_erasingChip();
    void programmerThread_readingCalibrationByte();
    void programmerThread_downloadingLockBits();
    void programmerThread_uploadingLockBits();
    void programmerThread_downloadingFuseBits();
    void programmerThread_uploadingFuseBits();
    void programmerThread_uploadingFuseBitsDefaults();
    void programmerThread_downloadingEEPROM();
    void programmerThread_uploadingEEPROM();
    void programmerThread_uploadingFlash();
    void programmerThread_downloadingFlash();
};
Q_DECLARE_METATYPE(Avr8UsbProgPlugin::Configuration);

#endif // AVR8USBPROGPLUGIN_H
