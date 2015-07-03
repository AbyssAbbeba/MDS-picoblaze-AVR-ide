#include "testporttool.h"
#include <QtGui>

TestPortTool::TestPortTool(QWidget *parent, MCUSimControl *controlUnit) :
    QWidget(parent),
    ui(new Ui::TestPortTool)
{
    ui->setupUi(this);

    status = false;
    fileBuffer.clear();

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

    // FOR TESTING ONLY, SIMULATE IN/OUT events
    QTimer *timer = new QTimer(this);
    connect(timer,
            SIGNAL(timeout()),
            this,
            SLOT(inputEvent()));
    timer->start(500);

    // LIST
    //bootDeviceList << "UART" << "Buttons" << "Leds" << "I2C_interface" << "LCD_interface" << "SPI_slave";
  //  connect(ui->listVystup,
     //       SIGNAL(itemDoubleClicked(QListWidgetItem*)),
     //       this,
     //       SLOT(insertItem(QListWidgetItem*))
    //       );
  //  ui->listVystup->setContextMenuPolicy(Qt::CustomContextMenu);
  //  connect(ui->listVystup,
  //          SIGNAL(customContextMenuRequested(QPoint)),
  //          SLOT(customMenuRequested(QPoint))
  //          );

    int random;
    qsrand( QDateTime::currentDateTime().toTime_t() );
    random = randInt(0,10);//qrand();

    qDebug() << random;
}

void TestPortTool::inputEvent()
{

}

int TestPortTool::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

void TestPortTool::activatePushed()
{
    // check if file is loaded
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

    qDebug() << fileBuffer.size();
    for (int i = 0; i < fileBuffer.size(); i++)
    {
        buffer = fileBuffer.at(i).split(" ");
        qDebug() << "splt addr > " << buffer;
        address << buffer[0];
        data << buffer[1];
       //both = QString("Address:    0x%1, Data:    0x%2").arg(buffer[0].toUpper(), buffer[1].toUpper());
    }
    qDebug() << "address: " << address;
    qDebug() << "data: " << data;
}

void TestPortTool::outFileClicked()
{
    // get path
    loadPath.clear();
    loadPath = QFileDialog::getSaveFileName(this,"Output file",".","*.txt");

    QFile outFile(loadPath);
    ui->lineOutFile->setText(loadPath);
    // open device for read only
    outFile.open(QIODevice::WriteOnly | QIODevice::Text);

    cursor->insertText("Output file has been succesfully selected\n");
    ui->textLog->setTextCursor(*cursor);

    outFileText = new QTextStream(&outFile);
    *outFileText << "shjxdwskjchskcjhk";
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

void TestPortTool::cancelApp()
{
    this->close();
}

TestPortTool::~TestPortTool()
{
    delete ui;
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

void TestPortTool::handleEvent(int subsysId, int eventId, int /*locationOrReason*/, int /*detail*/) //8
{
    if (MCUSimSubsys::ID_PLIO == subsysId)
    {
         qDebug()<< "handle event" << eventId;
        switch ( eventId )
        {
            case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
            {
                qDebug()<< "sim switch handle event";
                //this->value = m_plio->getOutputArray()[this->address];
                break;
            }
            case MCUSimPureLogicIO::EVENT_PLIO_READ:
            {
                qDebug()<< "sim switch handle event";
                //this->value = m_plio->getOutputArray()[this->address];
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
