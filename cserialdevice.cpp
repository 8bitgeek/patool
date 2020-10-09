/**************************************************************************
*   Author <mike@pikeaero.com> Mike Sharkey                               *
*   Copyright (C) 2010 by Pike Aerospace Research Corporation             *
*                                                                         *
*   This program is free software: you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation, either version 3 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
**************************************************************************/
#include "cserialdevice.h"
#include "csettings.h"

#define inherited CDevice

/** ***************************************************************************
* @brief Constructor
******************************************************************************/
CSerialDevice::CSerialDevice(QObject *parent, int retries, int rxTimeout)
: inherited(parent, retries, rxTimeout)
{
	mSerialPort = new CSerial(gSettings.getDevice());
	QObject::connect(serialPort(),SIGNAL(readyRead()),this,SLOT(rxSerialPort()));
	QObject::connect(serialPort(),SIGNAL(statusMessage(void*,int,QString)),this,SIGNAL(statusMessage(void*,int,QString)));
}

/** ***************************************************************************
* @brief Constructor
******************************************************************************/
CSerialDevice::CSerialDevice(const CSerialDevice& other)
: CDevice( other )
{
	mSerialPort = new CSerial(gSettings.getDevice());
	QObject::connect(serialPort(),SIGNAL(readyRead()),this,SLOT(rxSerialPort()));
	QObject::connect(serialPort(),SIGNAL(statusMessage(void*,int,QString)),this,SIGNAL(statusMessage(void*,int,QString)));
	copy( other );
}

/** ***************************************************************************
* @brief Destructor
******************************************************************************/
CSerialDevice::~CSerialDevice()
{
	delete mSerialPort;
	mSerialPort=NULL;
}

/** ***************************************************************************
* @brief Copy from another instance.
******************************************************************************/
CSerialDevice& CSerialDevice::copy(const CSerialDevice &other)
{
	if ( &other != this )
	{
	}
	return *this;
}

/** ***************************************************************************
* @brief Name of the serial device
******************************************************************************/
QString CSerialDevice::name()
{
	return serialPort()->name();
}

/** ***************************************************************************
* @brief Is the device open?
* @return true if device is open.
******************************************************************************/
bool CSerialDevice::isOpen()
{
	return serialPort()->isOpen();
}

/** ***************************************************************************
* @brief Open the device.
* @return true if the open was a success.
******************************************************************************/
bool CSerialDevice::open()
{
	if ( !isOpen() )
	{
		if ( serialPort()->open() )
		{
			serialPort()->setLineControl(gSettings.getBaud(), 8, 1, "NONE", "NONE" );
			#if defined (WIN32)
				Sleep( gSettings.getOpenDelay() );
			#else
				usleep( gSettings.getOpenDelay() * 1000 );
			#endif
			return true;
		}
		return false;
	}
	return true;
}

/** ***************************************************************************
* @brief Close the device.
******************************************************************************/
void CSerialDevice::close()
{
	serialPort()->close();
}

/** ***************************************************************************
* @brief Transmit a byte array.
******************************************************************************/
void CSerialDevice::tx(QByteArray& data)
{
	serialPort()->write( data.data(), data.length() );
	emit statusMessage( this, 8, "SERIAL TX: [" + QString( data.toPercentEncoding() ) + "]" );
}

/** ***************************************************************************
* @brief Recieve data from the TCP socket. Emits the rx( QByteArray ) signal with actual data to transmit to device.
******************************************************************************/
void CSerialDevice::rxSerialPort()
{
	char ch;
	while ( serialPort()->getChar(&ch) )
	{
        if ( ch > '\0' )
        {
            mRXPacket.append( ch );
            if ( ch == 0x0A )
            {
                emit statusMessage( this, 8, "SERIAL RX: [" + QString( mRXPacket.toPercentEncoding() ) + "]" );
            }
        }
	}
}

