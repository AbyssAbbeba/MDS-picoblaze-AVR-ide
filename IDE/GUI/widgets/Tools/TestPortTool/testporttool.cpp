#include "testporttool.h"
#include <QtGui>

TestPortTool::TestPortTool(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent),
    ui(new Ui::TestPortTool)
{
    ui->setupUi(this);

    status = false;
    fileBuffer.clear();
    index = 0;

    ui->labelStatus->setText("Inactive");

    // initializations
    this->setWindowTitle("File interface");
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->lineInFile->setReadOnly(true);
    ui->lineOutFile->setReadOnly(true);
    ui->textLog->setReadOnly(true);


    m_simControlUnit = controlUnit;
    
    std::vector<int> mask = { //8
                                MCUSimPureLogicIO::EVENT_PLIO_WRITE,
                                MCUSimPureLogicIO::EVENT_PLIO_READ
                            };
    m_simControlUnit->registerObserver(this, MCUSimSubsys::ID_PLIO, mask); //8

    // info log
    cursor = new QTextCursor(ui->textLog->textCursor());

    connect(ui->pushActivate,
            SIGNAL(clicked()),
            this,
            SLOT(activatePushed())
           );
    connect(ui->pushCancel,
            SIGNAL(clicked()),
            this,
            SLOT(cancelApp())
           );
    connect(ui->pushLoadIn,
            SIGNAL(clicked()),
            this,
            SLOT(readFileClicked())
           );
    connect(ui->pushLoadOut,
            SIGNAL(clicked()),
            this,
            SLOT(outFileClicked())
           );

    deviceChanged();

}

void TestPortTool::inputEvent()
{

}

void TestPortTool::activatePushed()
{
    // check if file is loaded
    index = 0;
    address.clear();
    data.clear();

    if ( loadPath.isNull() )
    {
        cursor->insertText("Select input/output file first..\n");
        ui->textLog->setTextCursor(*cursor);
        return;
    }

    // change buttons and status
    status = !status;
    if ( status == true)
    {
        ui->labelStatus->setText("Active");
        ui->pushActivate->setText("Deactivate");
    }
    else
    {
        ui->labelStatus->setText("Inactive");
        ui->pushActivate->setText("Activate");
    }
    fillBuffer();
}

void TestPortTool::fillBuffer()
{
    QStringList buffer;
    data.clear();
    address.clear();

    qDebug() << fileBuffer.size();
    for (int i = 0; i < fileBuffer.size(); i++)
    {
        buffer = fileBuffer.at(i).split(" ");
        qDebug() << "splt addr > " << buffer;
        address << buffer[0];
        data << buffer[1];
    }
    qDebug() << "address: " << address;
    qDebug() << "data: " << data;
}

void TestPortTool::outFileClicked()
{
    // get path
    loadPath.clear();
    loadPath = QFileDialog::getSaveFileName(this,"Output file",".","*.txt");

    ui->lineOutFile->setText(loadPath);
    // open device for read only
    outFile.setFileName(loadPath);
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);

    cursor->insertText("Output file has been succesfully selected\n");
    ui->textLog->setTextCursor(*cursor);

    outFileText = new QTextStream(&outFile);
    //*outFileText << "shjxdwskjchskcjhk";
}

void TestPortTool::readFileClicked()
{
    // get path
    loadPath.clear();
    loadPath = QFileDialog::getOpenFileName(this,"Input file",".","*.txt");
    QFile inFile(loadPath);
    ui->lineInFile->setText(loadPath);

    // open device for read only
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);

    cursor->insertText("Input values has been succesfully loaded\n");
    ui->textLog->setTextCursor(*cursor);

    // read from file, get input values

    QTextStream in(&inFile);
    QString line = in.readLine();

    // reading loop
    while (!line.isNull())
    {
        qDebug() << "line: " << line;
        fileBuffer << line;
        line = in.readLine();
    }

    inFile.close(); //670100-2210281372/6210
}


void TestPortTool::deviceChanged()  //8
{
    if ( NULL == m_simControlUnit )
    {
       qDebug() << "PortHexEdit: m_simControlUnit is NULL";
    }
    if ( NULL == m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_PLIO) )
    {
        qDebug() << "PortHexEdit: m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_PLIO) is NULL";
    }
    m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControlUnit->getSimSubsys(MCUSimSubsys::ID_PLIO));
    deviceReset();
}


void TestPortTool::deviceReset()  //8
{
    //this->value = m_plio->getOutputArray()[this->address];
}

void TestPortTool::handleUpdateRequest(int mask) //8
{
    if ( 4 & mask)
    {
        //this->value = m_plio->getOutputArray()[this->address];
    }
}

void TestPortTool::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/) //8
{
    if (MCUSimSubsys::ID_PLIO == subsysId)
    {
         qDebug()<< "subsys" << subsysId;
         qDebug()<< "eventid" << eventId;
         qDebug()<< "locationorreason " << locationOrReason;

        switch ( eventId )
        {
            case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
            {
                //QTextStream outFileText(&outFile);
                qDebug()<< "sim switch handle event";
                this->outValue = m_plio->getOutputArray()[locationOrReason];
                *outFileText << "Portout: " << locationOrReason << " " << (unsigned char)outValue << endl;
                outFileText->flush();
                cursor->insertText(QString("Portout: %1 %2 \n").arg(locationOrReason).arg((unsigned char)outValue));
                ui->textLog->setTextCursor(*cursor);
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_READ:
            {

                if  ( index < address.size() )
                {
                    qDebug() << address.at(index).toInt(0,16);
                    qDebug() << data.at(index).toInt(0,16);
                    if ( locationOrReason == address.at(index).toInt(0,16) )
                    {
                        m_plio->getInputArray()[locationOrReason] = data.at(index).toInt(0,16);
                        cursor->insertText(QString("Data read: 0x%1 0x%2 \n").arg(address.at(index)).arg(data.at(index)));
                        ui->textLog->setTextCursor(*cursor);
                        index++;
                    }
                }
                break;
            }
            default:
            {
                break;
            }
        }
    }
}


void TestPortTool::setReadOnly(bool /*readOnly*/)
{
}
void TestPortTool::cancelApp()
{
    this->close();
}

TestPortTool::~TestPortTool()
{
    outFile.close();
    inFile.close();
    delete ui;
}
