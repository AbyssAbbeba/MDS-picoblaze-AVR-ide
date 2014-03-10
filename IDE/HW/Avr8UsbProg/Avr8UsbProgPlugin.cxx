#include <QDebug>

#include "Avr8UsbProgPlugin.h"

#include <QDir>
#include <QIcon>
#include <QVariant>
#include <QFileInfo>
#include <QScrollBar>
#include <QWhatsThis>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>

Avr8UsbProgPlugin::Avr8UsbProgPlugin()
{
	Q_INIT_RESOURCE(Avr8UsbProg);

	setupUi(this);
	createMenus();
	initializeObjectVariables();
	finalizeGui();
	setStandAlone(standAloneFlag);
	setConnections();
	programmerThread.start(QThread::NormalPriority);
}

Avr8UsbProgPlugin::~Avr8UsbProgPlugin()
{
}

void Avr8UsbProgPlugin::initializeObjectVariables()
{
	standAloneFlag=false;
	busyFlag=false;
	deviceDetectedFlag=false;
	reprogrammingStage=INACTIVE_STAGE;

	fileSystemWatcher=new QFileSystemWatcher(this);
	Q_CHECK_PTR(fileSystemWatcher);

	lockBitCheckBoxes[7]=lockBit7CheckBox;
	lockBitCheckBoxes[6]=lockBit6CheckBox;
	lockBitCheckBoxes[5]=lockBit5CheckBox;
	lockBitCheckBoxes[4]=lockBit4CheckBox;
	lockBitCheckBoxes[3]=lockBit3CheckBox;
	lockBitCheckBoxes[2]=lockBit2CheckBox;
	lockBitCheckBoxes[1]=lockBit1CheckBox;
	lockBitCheckBoxes[0]=lockBit0CheckBox;

	fuseBitCheckBoxes[7]=fuseBit7CheckBox;
	fuseBitCheckBoxes[6]=fuseBit6CheckBox;
	fuseBitCheckBoxes[5]=fuseBit5CheckBox;
	fuseBitCheckBoxes[4]=fuseBit4CheckBox;
	fuseBitCheckBoxes[3]=fuseBit3CheckBox;
	fuseBitCheckBoxes[2]=fuseBit2CheckBox;
	fuseBitCheckBoxes[1]=fuseBit1CheckBox;
	fuseBitCheckBoxes[0]=fuseBit0CheckBox;

	fuseBitHighCheckBoxes[7]=fuseHighBit7CheckBox;
	fuseBitHighCheckBoxes[6]=fuseHighBit6CheckBox;
	fuseBitHighCheckBoxes[5]=fuseHighBit5CheckBox;
	fuseBitHighCheckBoxes[4]=fuseHighBit4CheckBox;
	fuseBitHighCheckBoxes[3]=fuseHighBit3CheckBox;
	fuseBitHighCheckBoxes[2]=fuseHighBit2CheckBox;
	fuseBitHighCheckBoxes[1]=fuseHighBit1CheckBox;
	fuseBitHighCheckBoxes[0]=fuseHighBit0CheckBox;

	extendedFuseBitCheckBoxes[7]=extendedFuseBit7CheckBox;
	extendedFuseBitCheckBoxes[6]=extendedFuseBit6CheckBox;
	extendedFuseBitCheckBoxes[5]=extendedFuseBit5CheckBox;
	extendedFuseBitCheckBoxes[4]=extendedFuseBit4CheckBox;
	extendedFuseBitCheckBoxes[3]=extendedFuseBit3CheckBox;
	extendedFuseBitCheckBoxes[2]=extendedFuseBit2CheckBox;
	extendedFuseBitCheckBoxes[1]=extendedFuseBit1CheckBox;
	extendedFuseBitCheckBoxes[0]=extendedFuseBit0CheckBox;
}

void Avr8UsbProgPlugin::createMenus()
{
	QAction* qaction;
	helpMenu=new QMenu(this);
	Q_CHECK_PTR(helpMenu);

	qaction=helpMenu->addAction(QIcon(":/Avr8UsbProg/images/contexthelp.png"), tr("&What's this ?"));
	connect(qaction, SIGNAL(triggered()), SLOT(whatsThisEnter()));

	qaction=helpMenu->addAction(tr("&Help"));
	connect(qaction, SIGNAL(triggered()), SLOT(helpDialog()));

	helpMenu->addSeparator();

	qaction=helpMenu->addAction(tr("&About"));
	connect(qaction, SIGNAL(triggered()), SLOT(aboutDialog()));

	qaction=helpMenu->addAction(QIcon(":/Avr8UsbProg/images/qt-logo.png"), tr("About &Qt"));
	connect(qaction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void Avr8UsbProgPlugin::whatsThisEnter()
{
	QWhatsThis::enterWhatsThisMode();
}

void Avr8UsbProgPlugin::helpDialog()
{
	QMessageBox::information(this,
		tr("Help"),
		tr("(Help text is not avaliable yet.)"));
}

void Avr8UsbProgPlugin::aboutDialog()
{
	QMessageBox::about(this,
		tr("About..."),
		tr("(More text is not avaliable yet.)"));
}

void Avr8UsbProgPlugin::finalizeGui()
{
	bottomPanel->hide();
	menuButton->setMenu(helpMenu);

	statusIcon[RED]=new QPixmap(":/Avr8UsbProg/images/ledred.png");
	statusIcon[ORANGE]=new QPixmap(":/Avr8UsbProg/images/ledorange.png");
	statusIcon[GREEN]=new QPixmap(":/Avr8UsbProg/images/ledgreen.png");
	statusIcon[YELLOW]=new QPixmap(":/Avr8UsbProg/images/ledyellow.png");
	for(int i=0; i<4; i++) {
		Q_CHECK_PTR(statusIcon[i]);
	}

	QLineEditNormalPalette=programFileLineEdit->palette();
	QLineEditGoodPalette=QLineEditNormalPalette;
	QLineEditGoodPalette.setColor(QPalette::Base, QColor(230,255,230));
	QLineEditBadPalette=QLineEditNormalPalette;
	QLineEditBadPalette.setColor(QPalette::Base, QColor(255,220,220));

	QLabelNormalPalette=programmerStatusTextLabel->palette();
	QLabelGreenPalette=QLabelNormalPalette;
	QLabelGreenPalette.setColor(QPalette::WindowText, QColor(0,200,0));
	QLabelRedPalette=QLabelNormalPalette;
	QLabelRedPalette.setColor(QPalette::WindowText, QColor(200,0,0));
	QLabelBrightRedPalette=QLabelNormalPalette;
	QLabelBrightRedPalette.setColor(QPalette::WindowText, QColor(255,0,0));
	QLabelYellowPalette=QLabelNormalPalette;
	QLabelYellowPalette.setColor(QPalette::WindowText, QColor(200,100,0));
}

void Avr8UsbProgPlugin::setConnections()
{
	connect(fileSystemWatcher,
		SIGNAL(directoryChanged(const QString &)),
		SLOT(directoryContentsChanged(const QString &)));

        connect(&programmerThread,
                SIGNAL(newProgrammersFound(const QStringList &)),
                SLOT(programmerThread_newProgrammersFound(const QStringList &)));
	connect(&programmerThread,
		SIGNAL(aborted()),
		SLOT(programmerThread_aborted()));
	connect(&programmerThread,
		SIGNAL(errorOccured(QString)),
		SLOT(programmerThread_errorOccured(QString)));
	connect(&programmerThread,
		SIGNAL(connectionEstablished()),
		SLOT(programmerThread_connectionEstablished()));
	connect(&programmerThread,
		SIGNAL(connectionTerminated()),
		SLOT(programmerThread_connectionTerminated()));
	connect(&programmerThread,
		SIGNAL(deviceDetected()),
		SLOT(programmerThread_deviceDetected()));
	connect(&programmerThread,
		SIGNAL(lockBitsUploaded()),
		SLOT(programmerThread_lockBitsUploaded()));
	connect(&programmerThread,
		SIGNAL(lockBitsDownloaded(Avr8UsbProgCore::LockBits)),
		SLOT(programmerThread_lockBitsDownloaded(Avr8UsbProgCore::LockBits)));
	connect(&programmerThread,
		SIGNAL(fuseBitsUploaded()),
		SLOT(programmerThread_fuseBitsUploaded()));
	connect(&programmerThread,
		SIGNAL(fuseBitsDownloaded(Avr8UsbProgCore::FuseBits)),
		SLOT(programmerThread_fuseBitsDownloaded(Avr8UsbProgCore::FuseBits)));
	connect(&programmerThread,
		SIGNAL(flashUploaded()),
		SLOT(programmerThread_flashUploaded()));
	connect(&programmerThread,
		SIGNAL(flashDownloaded()),
		SLOT(programmerThread_flashDownloaded()));
	connect(&programmerThread,
		SIGNAL(eepromUploaded()),
		SLOT(programmerThread_eepromUploaded()));
	connect(&programmerThread,
		SIGNAL(eepromDownloaded()),
		SLOT(programmerThread_eepromDownloaded()));
	connect(&programmerThread,
		SIGNAL(calibrationByteRead(QVector<unsigned char>)),
		SLOT(programmerThread_calibrationByteRead(QVector<unsigned char>)));
	connect(&programmerThread,
		SIGNAL(chipErased()),
		SLOT(programmerThread_chipErased()));
	connect(&programmerThread,
		SIGNAL(progressChanged(int)),
		SLOT(programmerThread_progressChanged(int)));

	connect(&programmerThread,
		SIGNAL(terminatingConnection()),
		SLOT(programmerThread_terminatingConnection()));

	connect(&programmerThread,
		SIGNAL(erasingChip()),
		SLOT(programmerThread_erasingChip()));

	connect(&programmerThread,
		SIGNAL(readingCalibrationByte()),
		SLOT(programmerThread_readingCalibrationByte()));

	connect(&programmerThread,
		SIGNAL(downloadingLockBits()),
		SLOT(programmerThread_downloadingLockBits()));

	connect(&programmerThread,
		SIGNAL(uploadingLockBits()),
		SLOT(programmerThread_uploadingLockBits()));

	connect(&programmerThread,
		SIGNAL(downloadingFuseBits()),
		SLOT(programmerThread_downloadingFuseBits()));

	connect(&programmerThread,
		SIGNAL(uploadingFuseBits()),
		SLOT(programmerThread_uploadingFuseBits()));

	connect(&programmerThread,
		SIGNAL(uploadingFuseBitsDefaults()),
		SLOT(programmerThread_uploadingFuseBitsDefaults()));

	connect(&programmerThread,
		SIGNAL(downloadingEEPROM()),
		SLOT(programmerThread_downloadingEEPROM()));

	connect(&programmerThread,
		SIGNAL(uploadingEEPROM()),
		SLOT(programmerThread_uploadingEEPROM()));

	connect(&programmerThread,
		SIGNAL(uploadingFlash()),
		SLOT(programmerThread_uploadingFlash()));

	connect(&programmerThread,
		SIGNAL(downloadingFlash()),
		SLOT(programmerThread_downloadingFlash()));
}

void Avr8UsbProgPlugin::directoryContentsChanged(const QString & /*path*/)
{
	on_eepromFileLineEdit_textChanged(eepromFileLineEdit->text());
	on_programFileLineEdit_textChanged(programFileLineEdit->text());
}

void Avr8UsbProgPlugin::refreshWatchedDirectories()
{
	fileSystemWatcher->removePaths(fileSystemWatcher->directories());

	if(QFileInfo(programFileLineEdit->text()).exists())
		fileSystemWatcher->addPath(
			QFileInfo(programFileLineEdit->text()).dir().absolutePath());

	if(QFileInfo(eepromFileLineEdit->text()).exists())
		fileSystemWatcher->addPath(
			QFileInfo(eepromFileLineEdit->text()).dir().absolutePath());
}

void Avr8UsbProgPlugin::on_programFileLineEdit_textChanged(const QString & text)
{
	bool exists=QFileInfo(text).exists();
	bool enableButtons=exists && !busyFlag && deviceDetectedFlag && programmerThread.getMcuDetails()->flash.size;

	programOpenHexButton->setEnabled(exists && !standAloneFlag);
	uploadProgramButton->setEnabled(enableButtons);
	downloadProgramButton->setEnabled(enableButtons);

	if(text.isEmpty()) {
		programFileLineEdit->setPalette(QLineEditNormalPalette);
	} else if(exists) {
		programFileLineEdit->setPalette(QLineEditGoodPalette);
	} else {
		programFileLineEdit->setPalette(QLineEditBadPalette);
	}

	refreshWatchedDirectories();
}

void Avr8UsbProgPlugin::on_eepromFileLineEdit_textChanged(const QString & text)
{
	bool exists=QFileInfo(text).exists();
	bool enableButtons=exists && !busyFlag && deviceDetectedFlag && programmerThread.getMcuDetails()->eeprom.size;

	eepromOpenHexButton->setEnabled(exists && !standAloneFlag);
	uploadEepromButton->setEnabled(enableButtons);
	downloadEepromButton->setEnabled(enableButtons);

	if(text.isEmpty()) {
		eepromFileLineEdit->setPalette(QLineEditNormalPalette);
	} else if(exists) {
		eepromFileLineEdit->setPalette(QLineEditGoodPalette);
	} else {
		eepromFileLineEdit->setPalette(QLineEditBadPalette);
	}

	refreshWatchedDirectories();
}

void Avr8UsbProgPlugin::on_speedToDefaultButton_clicked()
{
	speedSlider->setValue(4);
}

void Avr8UsbProgPlugin::on_speedSlider_valueChanged(int value)
{
    bool kHz = false;
	switch(value) {
                case 7: // 8 MHz (highest)
                        value = 8;
                        break;
                case 6: // 4 MHz
                        value = 4;
                        break;
		case 5:	// 2 MHz
			value = 2;
			break;
		case 4:	// 1 Mhz
			value = 1;
			break;
		case 3:	// 100 kHz
                        kHz = true;
			value=100;
			break;
		case 2:	// 250 kHz
                        kHz = true;
			value=250;
			break;
		case 1:	// 125 kHz (lowest)
                        kHz = true;
			value=125;
			break;
	}

	speedLabel->setText(QString("%1 %2Hz").arg(value).arg(kHz ? "k" : "M"));
}

void Avr8UsbProgPlugin::on_collapseAllButton_clicked()
{
	for(int i=0; i<8; i++)
		collapseExpand(i, true, false);
}

void Avr8UsbProgPlugin::on_expandAllButton_clicked()
{
	for(int i=0; i<8; i++)
		collapseExpand(i, false, true);
}

void Avr8UsbProgPlugin::on_programOpenHexButton_clicked()
{
	emit openHexEditor(programFileLineEdit->text());
}

void Avr8UsbProgPlugin::on_programOpenFileButton_clicked()
{
	QString filename=programFileLineEdit->text();

	if(filename.isEmpty())
		filename=QDir::homePath();

	QFileDialog fileDialog(this,
		tr("Select program data file"),
		QFileInfo(filename).dir().absolutePath(),
		tr("All known formats (*.hex *.ihx *.srec *.sre *.s2 *.s19 *.mot *.bin *.raw *.rom);;Intel® HEX (*.hex *.ihx);;Motorola® S-Record (*.srec *.sre *.s2 *.s19 *.mot);;Binary files (*.bin *.raw *.rom);;All files (*)"));
	if(fileDialog.exec() != QDialog::Accepted)
		return;
	filename=fileDialog.selectedFiles()[0];

	if(!filename.isEmpty())
		programFileLineEdit->setText(filename);
}

void Avr8UsbProgPlugin::on_eepromOpenHexButton_clicked()
{
	emit(openHexEditor(eepromFileLineEdit->text()));
}

void Avr8UsbProgPlugin::on_eepromOpenFileButton_clicked()
{
	QString filename=eepromFileLineEdit->text();

	if(filename.isEmpty())
		filename=QDir::homePath();

	QFileDialog fileDialog(this,
		tr("Select data file"),
		QFileInfo(filename).dir().absolutePath(),
		tr("All known formats (*.hex *.ihx *.srec *.sre *.s2 *.s19 *.mot *.bin *.raw *.rom);;Intel® HEX (*.hex *.ihx);;Motorola® S-Record (*.srec *.sre *.s2 *.s19 *.mot);;Binary files (*.bin *.raw *.rom);;All files (*)"));
	if(fileDialog.exec() != QDialog::Accepted)
		return;
	filename=fileDialog.selectedFiles()[0];

	if(!filename.isEmpty())
		eepromFileLineEdit->setText(filename);
}

void Avr8UsbProgPlugin::on_programmerCeButton_clicked()		{collapseExpand(0);}
void Avr8UsbProgPlugin::on_specialFunctionCeButton_clicked()	{collapseExpand(1);}
void Avr8UsbProgPlugin::on_detectedAvrCeButton_clicked()		{collapseExpand(2);}
void Avr8UsbProgPlugin::on_programMemoryCeButton_clicked()		{collapseExpand(3);}
void Avr8UsbProgPlugin::on_eepromMemoryCeButton_clicked()		{collapseExpand(4);}
void Avr8UsbProgPlugin::on_fuseBitsCeButton_clicked()		{collapseExpand(5);}
void Avr8UsbProgPlugin::on_lockBitsCeButton_clicked()		{collapseExpand(6);}
void Avr8UsbProgPlugin::on_optionsCeButton_clicked()		{collapseExpand(7);}

void Avr8UsbProgPlugin::collapseExpand(int id, bool allowCollapse, bool allowExpand)
{
	QFrame * sectionFrame=0;
	QWidget * sectionWidget=0;
	QPushButton * button;

	switch(id) {
		case 0:	sectionFrame=programmerSecFrame;
			sectionWidget=programmerSecWidget;
			button=programmerCeButton;
			break;

		case 1:	sectionFrame=specialFunctionSecFrame;
			sectionWidget=specialFunctionSecWidget;
			button=specialFunctionCeButton;
			break;

		case 2:	sectionFrame=detectedAvrSecFrame;
			sectionWidget=detectedAvrSecWidget;
			button=detectedAvrCeButton;
			break;

		case 3:	sectionFrame=programMemorySecFrame;
			sectionWidget=programMemorySecWidget;
			button=programMemoryCeButton;
			break;

		case 4:	sectionFrame=eepromMemorySecFrame;
			sectionWidget=eepromMemorySecWidget;
			button=eepromMemoryCeButton;
			break;

		case 5:	sectionFrame=fuseBitsSecFrame;
			sectionWidget=fuseBitsSecWidget;
			button=fuseBitsCeButton;
			break;

		case 6:	sectionFrame=lockBitsSecFrame;
			sectionWidget=lockBitsSecWidget;
			button=lockBitsCeButton;
			break;

		case 7:	sectionFrame=optionsSecFrame;
			sectionWidget=optionsSecWidget;
			button=optionsCeButton;
			break;
	}

	// Expand
	if(sectionWidget->isHidden()) {
		if(!allowExpand)
			return;

		button->setIcon(QIcon(":/Avr8UsbProg/images/1downarrow.png"));
		button->setFlat(true);
		sectionWidget->show();
		sectionFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);

	// Collapse
	} else {
		if(!allowCollapse)
			return;

		button->setIcon(QIcon(":/Avr8UsbProg/images/1rightarrow.png"));
		button->setFlat(false);
		sectionWidget->hide();
		sectionFrame->setFrameStyle(QFrame::NoFrame | QFrame::Plain);
	}
}

void Avr8UsbProgPlugin::setStandAlone(bool standAlone)
{
	standAloneFlag=standAlone;

	compileFirtsCheckBox->setEnabled(!standAloneFlag);
	setProgramFileButton->setEnabled(!standAloneFlag);
	programOpenHexButton->setEnabled(!standAloneFlag);
	eepromOpenHexButton->setEnabled(!standAloneFlag);
}

void Avr8UsbProgPlugin::on_setProgramFileButton_clicked()
{
	emit(requestProgramFileName());
}

bool Avr8UsbProgPlugin::standAlone() const
{
	return standAloneFlag;
}

void Avr8UsbProgPlugin::setProgramFileName(QString filename)
{
	programFileLineEdit->setText(filename);
}

bool Avr8UsbProgPlugin::enterLeaveProgMode(EnterLeave enterLeave)
{
	bool enable = (enterLeave == LEAVE);
	const Avr8UsbProgDeviceDB::Device * device=programmerThread.getMcuDetails();

	if(enterLeave == ENTER) {
		QString devSerialNumber=portComboBox->currentText();

		if(busyFlag) {
			qDebug() << "BUG: Avr8UsbProgPlugin::enterLeaveProgMode -- busyFlag";
			return false;
		}

		if(devSerialNumber.isEmpty()) {
			QMessageBox::critical(this, tr("Missing information"), tr("You must specify <b>serial number</b> of the USB device which you want to establish communication with."), QMessageBox::Ok, QMessageBox::Ok);
			return false;
		}

		deviceDetectedFlag=false;
		busyFlag=true;

		programmerThread.startProgramming(devSerialNumber, speedSlider->value());

		progressBar->setValue(0);
		bottomPanel->show();

		programmerStatusIconLabel->setPixmap(*statusIcon[YELLOW]);
		programmerStatusTextLabel->setText(tr("Opening port..."));
		programmerStatusTextLabel->setPalette(QLabelYellowPalette);
		currentOperationLabel->setText(tr("Starting..."));

	} else {
		currentOperationLabel->setText("");
		bottomPanel->hide();
		busyFlag=false;

		// End reprogramming sequence
		if(reprogrammingStage != INACTIVE_STAGE)
			reprogrammingStage=INACTIVE_STAGE;

		if(deviceDetectedFlag) {
			programmerStatusIconLabel->setPixmap(*statusIcon[GREEN]);
			programmerStatusTextLabel->setText(tr("READY"));
			programmerStatusTextLabel->setPalette(QLabelGreenPalette);
		} else {
			programmerStatusIconLabel->setPixmap(*statusIcon[ORANGE]);
			programmerStatusTextLabel->setText(tr("No MCU detected"));
			programmerStatusTextLabel->setPalette(QLabelNormalPalette);

			typeLabel->setText(QString("none"));
			flashSizeLabel->setText(QString("%1 B").arg(0));
			eepromSizeLabel->setText(QString("%1 B").arg(0));
		}
	}

	portComboBox->setEnabled(enable);
	speedSlider->setEnabled(enable);
	speedToDefaultButton->setEnabled(enable);
	detectMcuButton->setEnabled(enable);

	resetMcuButton->setEnabled(enable && deviceDetectedFlag);
	reprogramButton->setEnabled(enable && deviceDetectedFlag);
	eraseChipButton->setEnabled(enable && deviceDetectedFlag);
	readCalibrationButton->setEnabled(enable && deviceDetectedFlag && device->numberOfCalibrationBytes);

	programOpenFileButton->setEnabled(enable);
	setProgramFileButton->setEnabled(enable && !standAloneFlag);
	programFileLineEdit->setEnabled(enable);
	on_programFileLineEdit_textChanged(programFileLineEdit->text());

	eepromOpenFileButton->setEnabled(enable);
	eepromFileLineEdit->setEnabled(enable);
	on_eepromFileLineEdit_textChanged(eepromFileLineEdit->text());

	compileFirtsCheckBox->setEnabled(enable && !standAloneFlag);
	makeBackupCheckBox->setEnabled(enable);
	enableVerificationsCheckBox->setEnabled(enable);

	if(enable && deviceDetectedFlag) {
		bool hasAtLeastOneBitInThisByte=false;
		bool hasAtLeastOneBit=false;
		QString bitName;

		for(int i=0; i<8; i++) {
			bitName=device->fuses.feb.bitName[i];

			extendedFuseBitCheckBoxes[i]->setEnabled(!bitName.isEmpty());

			if(bitName.isEmpty()) {
				bitName="--";
			} else {
				hasAtLeastOneBit=true;
				hasAtLeastOneBitInThisByte=true;
			}

			extendedFuseBitCheckBoxes[i]->setText(bitName);
			extendedFuseBitCheckBoxes[i]->setToolTip(QString("<b>Bit %1:</b> %2").arg(i).arg(device->fuses.feb.bitDescription[i]));
		}
		extendedFuseBitsWidget->setEnabled(hasAtLeastOneBitInThisByte);
		hasAtLeastOneBitInThisByte=false;

		for(int i=0; i<8; i++) {
			bitName=device->fuses.fhb.bitName[i];

			fuseBitHighCheckBoxes[i]->setEnabled(!bitName.isEmpty());

			if(bitName.isEmpty()) {
				bitName="--";
			} else {
				hasAtLeastOneBit=true;
				hasAtLeastOneBitInThisByte=true;
			}

			fuseBitHighCheckBoxes[i]->setText(bitName);
			fuseBitHighCheckBoxes[i]->setToolTip(QString("<b>Bit %1:</b> %2").arg(i).arg(device->fuses.fhb.bitDescription[i]));
		}
		fuseHighBitsWidget->setEnabled(hasAtLeastOneBitInThisByte);
		hasAtLeastOneBitInThisByte=false;

		for(int i=0; i<8; i++) {
			bitName=device->fuses.flb.bitName[i];

			fuseBitCheckBoxes[i]->setEnabled(!bitName.isEmpty());

			if(bitName.isEmpty()) {
				bitName="--";
			} else {
				hasAtLeastOneBit=true;
				hasAtLeastOneBitInThisByte=true;
			}

			fuseBitCheckBoxes[i]->setText(bitName);
			fuseBitCheckBoxes[i]->setToolTip(QString("<b>Bit %1:</b> %2").arg(i).arg(device->fuses.flb.bitDescription[i]));
		}
		fuseBitsWidget->setEnabled(hasAtLeastOneBitInThisByte);
		hasAtLeastOneBitInThisByte=false;

		setFuseBitsButton->setEnabled(hasAtLeastOneBit);
		getFuseBitsButton->setEnabled(hasAtLeastOneBit);
		setFusesToDefaultButton->setEnabled(hasAtLeastOneBit);

	} else {
		for(int i=0; i<8; i++) {
			fuseBitCheckBoxes[i]->setEnabled(false);
			fuseBitHighCheckBoxes[i]->setEnabled(false);
			extendedFuseBitCheckBoxes[i]->setEnabled(false);
		}

		setFuseBitsButton->setEnabled(false);
		getFuseBitsButton->setEnabled(false);
		setFusesToDefaultButton->setEnabled(false);
	}


	if(enable && deviceDetectedFlag) {
		bool hasAtLeastOneBit=false;
		QString bitName;

		for(int i=0; i<8; i++) {
			bitName=device->lockBits.bitName[i];

			lockBitCheckBoxes[i]->setEnabled(!bitName.isEmpty());

			if(bitName.isEmpty())
				bitName="--";
			else
				hasAtLeastOneBit=true;

			lockBitCheckBoxes[i]->setText(bitName);
			lockBitCheckBoxes[i]->setToolTip(QString("<b>Bit %1:</b> %2").arg(i).arg(device->lockBits.bitDescription[i]));
		}

		setLockBitsButton->setEnabled(hasAtLeastOneBit);
		getLockBitsButton->setEnabled(hasAtLeastOneBit);

	} else {
		for(int i=0; i<8; i++)
			lockBitCheckBoxes[i]->setEnabled(false);

		setLockBitsButton->setEnabled(false);
		getLockBitsButton->setEnabled(false);
	}

	return true;
}

void Avr8UsbProgPlugin::on_detectMcuButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.downloadLockBits();
	programmerThread.downloadFuseBits();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_resetMcuButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::reprogramDevice()
{
	if(busyFlag)
		return;

	reprogrammingStage=COMPILATION_STAGE;
	CommitReprogrammingStage();
}

void Avr8UsbProgPlugin::on_reprogramButton_clicked()
{
	reprogramDevice();
}

void Avr8UsbProgPlugin::CommitReprogrammingStage()
{
	switch(reprogrammingStage) {
		case INACTIVE_STAGE:
			break;
		case COMPILATION_STAGE:
			if(programFileLineEdit->text().isEmpty()) {
				QMessageBox::critical(this, tr("Missing file name"), tr("No program file specified. Please select program file in section Program memory"), QMessageBox::Ok, QMessageBox::Ok);
				reprogrammingStage=INACTIVE_STAGE;
				return;
			}
			if(!QFileInfo(programFileLineEdit->text()).exists()) {
				QMessageBox::critical(this, tr("File not found"), tr("Specified program file does not exist."), QMessageBox::Ok, QMessageBox::Ok);
				reprogrammingStage=INACTIVE_STAGE;
				return;
			}
			if(!eepromFileLineEdit->text().isEmpty() && !QFileInfo(programFileLineEdit->text()).exists()) {
				QMessageBox::critical(this, tr("File not found"), tr("Specified data file for EEPROM does not exist."), QMessageBox::Ok, QMessageBox::Ok);
				reprogrammingStage=INACTIVE_STAGE;
				return;
			}

			if(!enterLeaveProgMode(ENTER)) {
				reprogrammingStage=INACTIVE_STAGE;
				return;
			}

			if(!standAloneFlag)
				emit requestCompilation();
			else
				compilationComplete();

			break;
		case PROGRAMMING_STAGE: {
			// Determinate state of lock bits
			Avr8UsbProgCore::LockBits lockBits;
			for(int i=0; i<8; i++) {
				if(lockBitCheckBoxes[i]->text() != "--") {
					lockBits.bit[i]=!(lockBitCheckBoxes[i]->isChecked());
				}
			}

			// Erase chip
			programmerThread.eraseChip();

			// Upload program (and data)
			programmerThread.uploadFlash(
				programFileLineEdit->text(),
				enableVerificationsCheckBox->isChecked());

			if(!eepromFileLineEdit->text().isEmpty()) {
				programmerThread.uploadEEPROM(
					eepromFileLineEdit->text(),
					enableVerificationsCheckBox->isChecked());
			}

			// Set lock bits
			programmerThread.uploadLockBits(lockBits);

			// Finalize
			programmerThread.endProgramming();
			programmerThread.executeCommands();

			break;
		}
	}
}

void Avr8UsbProgPlugin::compilationComplete(bool success)
{
	if(!busyFlag || reprogrammingStage != COMPILATION_STAGE)
		return;

	if(success)
		reprogrammingStage=PROGRAMMING_STAGE;
	else
		reprogrammingStage=INACTIVE_STAGE;

	CommitReprogrammingStage();
}

bool Avr8UsbProgPlugin::isBusy()
{
	return busyFlag;
}

void Avr8UsbProgPlugin::on_eraseChipButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.eraseChip();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_readCalibrationButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.readCalibrationByte();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_uploadProgramButton_clicked()
{
	QString filename=programFileLineEdit->text();

	if(filename.isEmpty()) {
		qDebug() << "BUG: Avr8UsbProgPlugin::on_uploadProgramButton_clicked -- filename.isEmpty()";
		return;
	}

	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.uploadFlash(filename, enableVerificationsCheckBox->isChecked());
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_downloadProgramButton_clicked()
{
	QString filename=programFileLineEdit->text();

	if(filename.isEmpty()) {
		qDebug() << "BUG: Avr8UsbProgPlugin::on_downloadProgramButton_clicked -- filename.isEmpty()";
		return;
	}

	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.downloadFlash(filename, makeBackupCheckBox->isChecked());
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_uploadEepromButton_clicked()
{
	QString filename=eepromFileLineEdit->text();

	if(filename.isEmpty()) {
		qDebug() << "BUG: Avr8UsbProgPlugin::on_uploadEepromButton_clicked -- filename.isEmpty()";
		return;
	}

	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.uploadEEPROM(filename, enableVerificationsCheckBox->isChecked());
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_downloadEepromButton_clicked()
{
	QString filename=eepromFileLineEdit->text();

	if(filename.isEmpty()) {
		qDebug() << "BUG: Avr8UsbProgPlugin::on_downloadEepromButton_clicked -- filename.isEmpty()";
		return;
	}

	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.downloadEEPROM(filename, makeBackupCheckBox->isChecked());
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_setFuseBitsButton_clicked()
{
	Avr8UsbProgCore::FuseBits fuseBits;

	QMessageBox::StandardButtons response=QMessageBox::warning(this,
		tr("Dangerous operation!"),
		tr("<span style='color: red'><b>WARNING</b></span><br /><br />Changing state of fuse bits <b>may cause serious problems</b> with future usage of your microcontroller! Do not change fuse bits unless you are absolutely sure about what are you doing.<br /><br />So do you really want to change state of MCU fuse bits?"),
		QMessageBox::Yes | QMessageBox::No,
		QMessageBox::No);

	if(response != QMessageBox::Yes) {
		return;
	}

	if(!enterLeaveProgMode(ENTER))
		return;

	for(int i=0; i<8; i++) {
		if(extendedFuseBitCheckBoxes[i]->text() != "--") {
			fuseBits.extendedBit[i]=!(extendedFuseBitCheckBoxes[i]->isChecked());
		}

		if(fuseBitHighCheckBoxes[i]->text() != "--") {
			fuseBits.highBit[i]=!(fuseBitHighCheckBoxes[i]->isChecked());
		}

		if(fuseBitCheckBoxes[i]->text() != "--") {
			fuseBits.lowBit[i]=!(fuseBitCheckBoxes[i]->isChecked());
		}
	}

	programmerThread.uploadFuseBits(fuseBits);
	programmerThread.downloadFuseBits();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}
void Avr8UsbProgPlugin::on_getFuseBitsButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.downloadFuseBits();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}
void Avr8UsbProgPlugin::on_setFusesToDefaultButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.uploadFuseBitsDefaults();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::on_setLockBitsButton_clicked()
{
	Avr8UsbProgCore::LockBits lockBits;

	if(!enterLeaveProgMode(ENTER))
		return;

	for(int i=0; i<8; i++) {
		if(lockBitCheckBoxes[i]->text() != "--") {
			lockBits.bit[i]=!(lockBitCheckBoxes[i]->isChecked());
		}
	}

	programmerThread.uploadLockBits(lockBits);
	programmerThread.downloadLockBits();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}
void Avr8UsbProgPlugin::on_getLockBitsButton_clicked()
{
	if(!enterLeaveProgMode(ENTER))
		return;

	programmerThread.downloadLockBits();
	programmerThread.endProgramming();
	programmerThread.executeCommands();
}

void Avr8UsbProgPlugin::abort()
{
	if(!busyFlag)
		return;

	reprogrammingStage=INACTIVE_STAGE;
	programmerStatusTextLabel->setText(tr("Aborting..."));
	programmerStatusTextLabel->setPalette(QLabelBrightRedPalette);
	bottomPanel->hide();
	QCoreApplication::processEvents();

	programmerThread.abort();
}

void Avr8UsbProgPlugin::on_abortButton_clicked()
{
	abort();
}

QVariant * Avr8UsbProgPlugin::configuration() const
{
	Configuration conf;
	QVariant* result=new QVariant;

	conf.sections.programmer.hidden=programmerSecFrame->isHidden();
	conf.sections.programmer.port=portComboBox->currentText();
	conf.sections.programmer.speed=speedSlider->value();

	conf.sections.specialFunctions.hidden=specialFunctionSecFrame->isHidden();

	conf.sections.detectedAvr.hidden=detectedAvrSecFrame->isHidden();

	conf.sections.programMemory.hidden=programMemorySecFrame->isHidden();
	conf.sections.programMemory.filename=programFileLineEdit->text();

	conf.sections.eepromMemory.hidden=eepromMemorySecFrame->isHidden();
	conf.sections.eepromMemory.filename=eepromFileLineEdit->text();

	conf.sections.fuseBits.hidden=fuseBitsSecFrame->isHidden();

	conf.sections.lockBits.hidden=lockBitsSecFrame->isHidden();

	conf.sections.options.hidden=optionsSecFrame->isHidden();
	conf.sections.options.enableVerifications=enableVerificationsCheckBox->isChecked();
	conf.sections.options.makeBackupFiles=makeBackupCheckBox->isChecked();
	conf.sections.options.compileFirts=compileFirtsCheckBox->isChecked();

	conf.general.scrollAreaPosition=scrollArea->verticalScrollBar()->value();


	result->setValue(conf);
	return result;
}

void Avr8UsbProgPlugin::setConfiguration(QVariant * configuration)
{
	Configuration conf;

	if(!configuration->canConvert<Avr8UsbProgPlugin::Configuration>()) {
		qDebug() << "ERROR: Avr8UsbProgPlugin::setConfiguration -- !conf->canConvert<Avr8UsbProgPlugin::Configuration>()";
		return;
	}

	conf=configuration->value<Avr8UsbProgPlugin::Configuration>();


	collapseExpand(0,
		conf.sections.programmer.hidden,
		!conf.sections.programmer.hidden);
	portComboBox->setCurrentIndex(portComboBox->findText(conf.sections.programmer.port));
	speedSlider->setValue(conf.sections.programmer.speed);

	collapseExpand(1,
		conf.sections.specialFunctions.hidden,
		!conf.sections.specialFunctions.hidden);

	collapseExpand(2,
		conf.sections.detectedAvr.hidden,
		!conf.sections.detectedAvr.hidden);

	collapseExpand(3,
		conf.sections.programMemory.hidden,
		!conf.sections.programMemory.hidden);
	programFileLineEdit->setText(conf.sections.programMemory.filename);

	collapseExpand(4,
		conf.sections.eepromMemory.hidden,
		!conf.sections.eepromMemory.hidden);
	eepromFileLineEdit->setText(conf.sections.eepromMemory.filename);

	collapseExpand(5,
		conf.sections.fuseBits.hidden,
		!conf.sections.fuseBits.hidden);

	collapseExpand(6,
		conf.sections.lockBits.hidden,
		!conf.sections.lockBits.hidden);

	collapseExpand(7,
		conf.sections.options.hidden,
		!conf.sections.options.hidden);

	enableVerificationsCheckBox->setChecked(conf.sections.options.enableVerifications);
	makeBackupCheckBox->setChecked(conf.sections.options.makeBackupFiles);
	compileFirtsCheckBox->setChecked(conf.sections.options.compileFirts);

	scrollArea->verticalScrollBar()->setValue(conf.general.scrollAreaPosition);
}

void Avr8UsbProgPlugin::programmerThread_newProgrammersFound(const QStringList & programmers)
{
        QString currentItem=portComboBox->currentText();

        portComboBox->clear();
        foreach(QString id, programmers) {
                portComboBox->addItem(QIcon(":/Avr8UsbProg/images/usb.png"), id);
        }

        portComboBox->setCurrentIndex(portComboBox->findText(currentItem));
}

void Avr8UsbProgPlugin::programmerThread_aborted()
{
	currentOperationLabel->setText(tr("Process aborted"));
}

void Avr8UsbProgPlugin::programmerThread_errorOccured(QString errorInfo)
{
	reprogrammingStage=INACTIVE_STAGE;
	QMessageBox::critical(this, tr("Programmer error"), errorInfo, QMessageBox::Ok, QMessageBox::Ok);
}

void Avr8UsbProgPlugin::programmerThread_connectionEstablished()
{
	programmerStatusTextLabel->setText(tr("Detecting MCU..."));
	programmerStatusIconLabel->setPixmap(*statusIcon[RED]);
	programmerStatusTextLabel->setPalette(QLabelYellowPalette);
}

void Avr8UsbProgPlugin::programmerThread_connectionTerminated()
{
	enterLeaveProgMode(LEAVE);
}

void Avr8UsbProgPlugin::programmerThread_deviceDetected()
{
	programmerStatusTextLabel->setText(tr("Working..."));
	programmerStatusIconLabel->setPixmap(*statusIcon[RED]);
	programmerStatusTextLabel->setPalette(QLabelRedPalette);

	deviceDetectedFlag=true;
	const Avr8UsbProgDeviceDB::Device * device=programmerThread.getMcuDetails();

	typeLabel->setText(QString("<a href='%1'>%2</a>").arg(device->web).arg(device->name));
	flashSizeLabel->setText(QString("%1 B").arg(device->flash.size));
	eepromSizeLabel->setText(QString("%1 B").arg(device->eeprom.size));
}

void Avr8UsbProgPlugin::programmerThread_lockBitsUploaded()
{
	currentOperationLabel->setText(tr("Lock bits set"));
}

void Avr8UsbProgPlugin::programmerThread_lockBitsDownloaded(Avr8UsbProgCore::LockBits lockBits)
{
	for(int i=0; i<8; i++) {
		if(lockBitCheckBoxes[i]->text() == "--") {
			lockBitCheckBoxes[i]->setChecked(false);
		} else {
			lockBitCheckBoxes[i]->setChecked(!lockBits.bit[i]);
		}
	}

	currentOperationLabel->setText(tr("Lock bits read"));
}

void Avr8UsbProgPlugin::programmerThread_fuseBitsUploaded()
{
	currentOperationLabel->setText(tr("Fuse bits set"));
}

void Avr8UsbProgPlugin::programmerThread_fuseBitsDownloaded(Avr8UsbProgCore::FuseBits fuseBits)
{
	for(int i=0; i<8; i++) {
		if(fuseBitCheckBoxes[i]->text() == "--") {
			fuseBitCheckBoxes[i]->setChecked(false);
		} else {
			fuseBitCheckBoxes[i]->setChecked(!fuseBits.lowBit[i]);
		}

		if(fuseBitHighCheckBoxes[i]->text() == "--") {
			fuseBitHighCheckBoxes[i]->setChecked(false);
		} else {
			fuseBitHighCheckBoxes[i]->setChecked(!fuseBits.highBit[i]);
		}

		if(extendedFuseBitCheckBoxes[i]->text() == "--") {
			extendedFuseBitCheckBoxes[i]->setChecked(false);
		} else {
			extendedFuseBitCheckBoxes[i]->setChecked(!fuseBits.extendedBit[i]);
		}
	}
	currentOperationLabel->setText(tr("Fuses read"));
}

void Avr8UsbProgPlugin::programmerThread_flashUploaded()
{
	currentOperationLabel->setText(tr("Program written"));
}

void Avr8UsbProgPlugin::programmerThread_flashDownloaded()
{
	currentOperationLabel->setText(tr("Program read"));
}

void Avr8UsbProgPlugin::programmerThread_eepromUploaded()
{
	currentOperationLabel->setText(tr("Data written"));
}

void Avr8UsbProgPlugin::programmerThread_eepromDownloaded()
{
	currentOperationLabel->setText(tr("Data read"));
}

void Avr8UsbProgPlugin::programmerThread_calibrationByteRead(QVector<unsigned char> calibrationBytes)
{
	QString message=tr("<b>Calibration bytes</b><br />");
	int size=calibrationBytes.size();

	for(int i=0; i<size; i++) {
		message.append(QString("<br />&nbsp;&nbsp;Byte 0x%1: ").arg(i, 2, 16, QChar('0')));
		message.append(QString("<b>0x%1</b>").arg(calibrationBytes[i], 2, 16, QChar('0')));
	}

	QMessageBox::information(this,
		tr("Calibration bytes"),
		message,
		QMessageBox::Ok,
		QMessageBox::Ok);
}

void Avr8UsbProgPlugin::programmerThread_chipErased()
{
	currentOperationLabel->setText(tr("Chip erased"));
}

void Avr8UsbProgPlugin::programmerThread_progressChanged(int progress)
{
	progressBar->setValue(progress);
}

void Avr8UsbProgPlugin::programmerThread_terminatingConnection()
{
	currentOperationLabel->setText(tr("Exiting ..."));
	programmerStatusTextLabel->setText(tr("Closing port ..."));
	programmerStatusTextLabel->setPalette(QLabelYellowPalette);
}

void Avr8UsbProgPlugin::programmerThread_erasingChip()
{
	currentOperationLabel->setText(tr("Erasing chip"));
}
void Avr8UsbProgPlugin::programmerThread_readingCalibrationByte()
{
	currentOperationLabel->setText(tr("Reading calibration bytes"));
}
void Avr8UsbProgPlugin::programmerThread_downloadingLockBits()
{
	currentOperationLabel->setText(tr("Reading lock bits"));
}
void Avr8UsbProgPlugin::programmerThread_uploadingLockBits()
{
	currentOperationLabel->setText(tr("Setting lock bits"));
}
void Avr8UsbProgPlugin::programmerThread_downloadingFuseBits()
{
	currentOperationLabel->setText(tr("Reading fuse bits"));
}
void Avr8UsbProgPlugin::programmerThread_uploadingFuseBits()
{
	currentOperationLabel->setText(tr("Setting fuse bits"));
}
void Avr8UsbProgPlugin::programmerThread_uploadingFuseBitsDefaults()
{
	currentOperationLabel->setText(tr("Setting fuse bits to defaults"));
}
void Avr8UsbProgPlugin::programmerThread_downloadingEEPROM()
{
	currentOperationLabel->setText(tr("Reading from EEPROM"));
}
void Avr8UsbProgPlugin::programmerThread_uploadingEEPROM()
{
	currentOperationLabel->setText(tr("Writing to EEPROM"));
}
void Avr8UsbProgPlugin::programmerThread_uploadingFlash()
{
	currentOperationLabel->setText(tr("Writing to flash memory"));
}
void Avr8UsbProgPlugin::programmerThread_downloadingFlash()
{
	currentOperationLabel->setText(tr("Reading from Flash memory"));
}

Q_EXPORT_PLUGIN2(Avr8UsbProgPlugin, Avr8UsbProgPlugin);
