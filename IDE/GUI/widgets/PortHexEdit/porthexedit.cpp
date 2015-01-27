/**
 * @brief
 * C++ Implementation: ...
 *
 * ...
 *
 * (C) copyright 2013, 2014 Moravia Microsystems, s.r.o.
 *
 * @author Erik Chalupa <erik.chalupa@moravia-microsystems.com>
 * @ingroup PortHexEdit
 * @file porthexedit.cpp
 */


#include "porthexedit.h"

//#include <QGridLayout>
#include <QtGui>

#include "../HexEdit/hexedit.h"
#include "../../../simulators/SimControl/MCUSimControl.h"
#include "../HexEdit/hexedit.h"
#include "../../../simulators/MCUSim/MCUSimPureLogicIO.h"
#include "../../../simulators/MCUSim/PicoBlaze/PicoBlazeIO.h"
#include <cstdint>
//#include "McuDeviceSpecAVR8.h"

PortHexEdit::PortHexEdit(QWidget * parent, MCUSimControl * controlUnit, MCUSimSubsys::SubsysId subsys)
	: QWidget(parent),
	  m_startingAddress(0),
	  m_size(0),
	  m_hexEditIn(NULL),
	  m_hexEditOut(NULL)
{
   //qDebug() << "PortHexEdit: PortHexEdit()";
    std::vector<int> mask;
    this->subsys = subsys;
    m_layout = new QGridLayout(this);
    this->setLayout(m_layout);
    m_layout->setVerticalSpacing(0);
    m_layout->setHorizontalSpacing(0);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_WRITE);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_READ);
    mask.push_back(PicoBlazeIO::EVENT_PICOBLAZEIO_OUTPUTK);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_READ_END);
    mask.push_back(MCUSimPureLogicIO::EVENT_PLIO_WRITE_END);
	controlUnit->registerObserver(
		this,
		subsys,
        mask);

    if ( NULL == controlUnit )
    {
       //qDebug() << "PortHexEdit: controlUnit is NULL";
    }
	//m_layout = new QHBoxLayout(this);
	//setLayout(m_layout);
	//this->m_hexEditOut->hide();
    this->visibleIn = true;

    m_lblRD = new QLabel("RD", this);
    m_lblWR = new QLabel("WR", this);
    m_lblInput = new QLabel("Input Ports", this);
    m_lblOutput = new QLabel("Output Ports", this);
    m_lblWR->show();
    m_lblRD->show();
    m_lblInput->show();
    m_lblOutput->show();
    m_layout->addWidget(m_lblRD, 0,4);
    m_layout->addWidget(m_lblWR, 0,9);
    m_layout->addWidget(m_lblInput, 0,1, Qt::AlignLeft);
    m_layout->addWidget(m_lblOutput, 0,6, Qt::AlignLeft);
    m_lblWRK = NULL;

	deviceChanged();
   //qDebug() << "PortHexEdit: return PortHexEdit()";
}

PortHexEdit::~PortHexEdit()
{
	deleteHexEdit();
	//delete m_layout;
}

inline void PortHexEdit::deleteHexEdit()
{
	if ( NULL != m_hexEditIn )
    {
		delete m_hexEditIn;
	}
	if ( NULL != m_hexEditOut )
    {
        delete m_hexEditOut;
    }
}

void PortHexEdit::handleEvent(int subsysId, int eventId, int locationOrReason, int /*detail*/)
{
	if ( this->subsys != subsysId )
    {
		qDebug("PortHexEdit: Invalid event received, event ignored.");
		return;
 	}

	//int idx = locationOrReason - m_startingAddress;
	if ( (locationOrReason < 0) || (locationOrReason > m_size) )
    {
		qDebug("PortHexEdit: Invalid address, event ignored.");
		return;
	}

	switch ( eventId )
    {
        case MCUSimPureLogicIO::EVENT_PLIO_WRITE:
        {
            m_lblWR->setStyleSheet("color: #00ff00");
            uint value = m_plio->getOutputArray()[locationOrReason];
            //qDebug() << "PortHexEdit: event: mem cell changed to" << (unsigned char)value;

            m_hexEditOut->setVal(locationOrReason, (unsigned char)value);
            m_hexEditOut->setHighlighted(locationOrReason, true);

            break;
        }
        case PicoBlazeIO::EVENT_PICOBLAZEIO_OUTPUTK:
        {
            m_lblWRK->setStyleSheet("color: #00ff00");
            uint value = m_plio->getOutputArray()[locationOrReason];
           //qDebug() << "PortHexEdit: event: mem cell changed to" << (unsigned char)value;

            m_hexEditOut->setVal(locationOrReason, (unsigned char)value);
            m_hexEditOut->setHighlighted(locationOrReason, true);

            break;
        }
        case MCUSimPureLogicIO::EVENT_PLIO_READ:
        {
            m_lblRD->setStyleSheet("color: #00ff00");
            uint value = m_plio->getInputArray()[locationOrReason];
           //qDebug() << "PortHexEdit: event: mem cell changed to" << (unsigned char)value;

            m_hexEditIn->setVal(locationOrReason, (unsigned char)value);
            m_hexEditIn->setHighlighted(locationOrReason, true);

            break;
        }
        case MCUSimPureLogicIO::EVENT_PLIO_WRITE_END:
        {
            m_lblWR->setStyleSheet("color: none");
            if (NULL != m_lblWRK)
            {
                m_lblWRK->setStyleSheet("color: none");
            }
            break;
        }
        case MCUSimPureLogicIO::EVENT_PLIO_READ_END:
        {
            m_lblRD->setStyleSheet("color: none");
            break;
        }
		default:
			qDebug("Invalid event received, event ignored.");
			break;
	}
}


void PortHexEdit::deviceChanged()
{
   //qDebug() << "PortHexEdit: deviceChanged()";
    if ( NULL == m_simControlUnit )
    {
       qDebug() << "PortHexEdit: m_simControlUnit is NULL";
    }
    if ( NULL == m_simControlUnit->getSimSubsys(this->subsys) )
    {
        qDebug() << "PortHexEdit: SubsysId " << this->subsys;
        qDebug() << "PortHexEdit: m_simControlUnit->getSimSubsys(this->subsys) is NULL";
    }
	m_plio = dynamic_cast<MCUSimPureLogicIO*>(m_simControlUnit->getSimSubsys(this->subsys));


    //if ( NULL == m_memory )
    //{
    //    qDebug() << "PortHexEdit: m_memory is NULL";
    //}
	/*switch ( m_simControlUnit->getArch() )
    {
		case MCUSim::ARCH_AVR8:
        {
			const McuDeviceSpecAVR8 * avr8devSpec = dynamic_cast<const McuDeviceSpecAVR8*>(m_simControlUnit->getDeviceSpec());

			m_startingAddress = 0;
			m_size = avr8devSpec->m_dataMemory.m_regFileSize;

			break;
		}
		default:
			qDebug("Unknown device architecture.");
			return;
	}*/
    m_size = m_plio->getNumberOfPorts();
	deleteHexEdit();
	m_hexEditIn = new HexEdit(this, false, m_size, 8);
    //m_hexEditIn->move(0, 20);
    m_hexEditIn->show();
    //m_hexEditIn->verticalScrollBar()->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_hexEditIn->getTextEdit()->
    //m_hexEditIn->verticalScrollBar()->hide();
    //m_hexEditIn->verticalScrollBar()->resize(0, 0);
    //m_hexEditIn->horizontalScrollBar()->hide();
    //m_hexEditIn->horizontalScrollBar()->resize(0, 0);
    m_hexEditOut = new HexEdit(this, false, m_size, 8);
    //m_hexEditOut->move(m_hexEditIn->width(), 20);
    m_hexEditOut->show();
    m_layout->addWidget(m_hexEditIn, 1, 0, 1, 5);
    m_layout->addWidget(m_hexEditOut, 1, 5, 1, 5);
    
    //m_hexEditOut->hide();
    //m_hexEditOut->verticalScrollBar()->hide();
    //m_hexEditOut->getTextEdit()->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //m_hexEditOut->verticalScrollBar()->hide();
    //m_hexEditOut->verticalScrollBar()->resize(0, 0);
    //m_hexEditOut->horizontalScrollBar()->hide();
    //m_hexEditOut->horizontalScrollBar()->resize(0, 0);
    QTimer::singleShot(300, this, SLOT(fixHeight()));
    m_hexEditIn->fixHeight();
    m_hexEditOut->fixHeight();
    //this->setFixedWidth(m_hexEditIn->width() + m_hexEditOut->width());
    //m_lblWR->move(this->width() - m_lblWR->width() - 10,0);
    //m_lblRD->move(m_hexEditIn->width() - m_lblRD->width() - 10,0);
    //m_lblInput->move(0,0);
    //m_lblOutput->move(m_hexEditIn->width(), 0);
    if (MCUSim::FAMILY_KCPSM6 == m_simControlUnit->getFamily())
    {
        if (NULL == m_lblWRK)
        {
            m_lblWRK = new QLabel("WRK  ", this);
            m_lblWRK->show();
            m_layout->addWidget(m_lblWRK, 0,8);
            //m_lblWRK->move(this->width() - m_lblWR->width() - m_lblWRK->width() - 10, 0);
        }
    }
    else if (NULL != m_lblWRK)
    {
        delete m_lblWRK;
        m_lblWRK = NULL;
    }
	connect(m_hexEditIn, SIGNAL(textChanged(int)), this, SLOT(changeValueIn(int)));
    connect(m_hexEditOut, SIGNAL(textChanged(int)), this, SLOT(changeValueOut(int)));
	//m_layout->addWidget(m_hexEditIn);

	deviceReset();
   //qDebug() << "PortHexEdit: return deviceChanged()";
}


void PortHexEdit::changeValueIn(int address)
{
 	m_plio->getInputArray()[address] = m_hexEditIn->getVal(address);
}


void PortHexEdit::changeValueOut(int address)
{
    m_plio->getOutputArray()[address] = m_hexEditIn->getVal(address);
}


void PortHexEdit::deviceReset()
{
   //qDebug() << "PortHexEdit: deviceReset()";
	if ( NULL == m_hexEditIn || NULL == m_hexEditOut )
    {
		return;
	}
	
    uint value;
	for ( int i = 0; i < m_size; i++ )
    {

		//uint address = i;
        value = m_plio->getInputArray()[i];
        if ( value > 255 )
        {
            value = 255;
        }
 		m_hexEditIn->setVal(i, (unsigned char)value);
        //qDebug() << "PortHexEdit: in value is" << (unsigned char)value;

        value = m_plio->getOutputArray()[i];
        if ( value > 255 )
        {
            value = 255;
        }
        m_hexEditOut->setVal(i, (unsigned char)value);
        //qDebug() << "PortHexEdit: out value is" << (unsigned char)value;
	}
    m_hexEditIn->fixHeight();
    m_hexEditOut->fixHeight();
    m_hexEditIn->scrollToTop();
    m_hexEditOut->scrollToTop();
   //qDebug() << "PortHexEdit: return deviceReset()";
}


void PortHexEdit::setReadOnly(bool readOnly)
{
   //qDebug() << "PortHexEdit: setReadOnly()";
	if ( NULL == m_hexEditIn || NULL == m_hexEditOut )
    {
		return;
	}

 	m_hexEditIn->setReadOnly(readOnly);
    m_hexEditOut->setReadOnly(readOnly);
// 	for ( int i = 0; i < m_size; i++ ) {
// 		if ( false == readOnly ) {
// 			m_hexEdit->setHighlighted(i, false);
// 		}
// 	}
   //qDebug() << "PortHexEdit: return setReadOnly()";
}


void PortHexEdit::fixHeight()
{
    //qDebug() << "PortHexEdit: fixHeight()";
    m_hexEditIn->fixHeight();
    m_hexEditOut->fixHeight();
    //qDebug() << "PortHexEdit: return fixHeight()";
}


void PortHexEdit::switchPorts()
{
    //qDebug() << "PortHexEdit: switchPorts()";
    if ( true == this->visibleIn )
    {
        this->m_hexEditIn->hide();
        this->m_hexEditOut->show();
        this->visibleIn = false;
    }
    else
    {
        this->m_hexEditOut->hide();
        this->m_hexEditIn->show();
        this->visibleIn = true;
    }
    this->fixHeight();
    //qDebug() << "PortHexEdit: return switchPorts()";
}


void PortHexEdit::unhighlight()
{
    this->m_hexEditIn->unhighlight();
    this->m_hexEditOut->unhighlight();
    m_lblRD->setStyleSheet("color: none");
    m_lblWR->setStyleSheet("color: none");
    if (NULL != m_lblWRK)
    {
        m_lblWRK->setStyleSheet("color: none");
    }
}


void PortHexEdit::updateWidget()
{
    //qDebug() << "PortHexEdit: update";
    uint value;
    for ( int i = 0; i < m_size; i++ )
    {

        //uint address = i;
        value = m_plio->getInputArray()[i];
        if ( value > 255 )
        {
            value = 255;
        }
        if (value != m_hexEditIn->getVal(i))
        {
            m_hexEditIn->setVal(i, (unsigned char)value);
            m_hexEditIn->setHighlighted(i, true);
        }
        /*else
        {
            m_hexEditIn->setHighlighted(i, false);
        }*/
        //qDebug() << "PortHexEdit: in value is" << (unsigned char)value;

        value = m_plio->getOutputArray()[i];
        if ( value > 255 )
        {
            value = 255;
        }
        if (value != m_hexEditOut->getVal(i))
        {
            m_hexEditOut->setVal(i, (unsigned char)value);
            m_hexEditOut->setHighlighted(i, true);
        }
        /*else
        {
            m_hexEditOut->setHighlighted(i, false);
        }*/
        //qDebug() << "PortHexEdit: out value is" << (unsigned char)value;
    }
}


void PortHexEdit::resizeEvent(QResizeEvent */*event*/)
{
    m_hexEditIn->fixHeight();
    m_hexEditOut->fixHeight();
}