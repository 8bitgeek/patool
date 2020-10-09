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
#include "ctcpdevice.h"

#define inherited CDevice

/** *****************************************************************
* @brief Constructor.
********************************************************************/
CTcpDevice::CTcpDevice(QString host, int port, QObject *parent, int retries, int rxTimeout)
: inherited(parent, retries, rxTimeout)
, mHost(host)
, mPort(port)
{
	QObject::connect(socket(),SIGNAL(readyRead()),this,SLOT(rxTcpPort()));
}

/** *****************************************************************
* @brief Constructor
********************************************************************/
CTcpDevice::CTcpDevice(const CTcpDevice& other)
: CDevice(other)
{
	copy( other );
	QObject::connect(socket(),SIGNAL(readyRead()),this,SLOT(rxTcpPort()));
}

/** *****************************************************************
* @brief Destructor.
********************************************************************/
CTcpDevice::~CTcpDevice()
{
}

/** ***************************************************************************
* @brief Copy from another instance.
******************************************************************************/
CTcpDevice& CTcpDevice::copy(const CTcpDevice &other)
{
	if ( &other != this )
	{
		mHost	= other.mHost;
		mPort	= other.mPort;
	}
	return *this;
}
/** *****************************************************************
* @brief Return the name of this device.
********************************************************************/
QString CTcpDevice::name()
{
	return socket()->peerName() + ":" + QString::number(socket()->peerPort());
}

/** *****************************************************************
 ** @brief Is the socket open?
 ********************************************************************/
bool CTcpDevice::isOpen()
{
	return socket()->isOpen();
}

/** *****************************************************************
 ** @brief Open the connection.
 ********************************************************************/
bool CTcpDevice::open()
{
	if ( !isOpen() )
	{
		socket()->connectToHost(mHost,mPort);
		if ( socket()->waitForConnected() )
		{
			return true;
		}
		return false;
	}
	return true;
}

/** *****************************************************************
 ** @brief Close the connection.
 ********************************************************************/
void CTcpDevice::close()
{
	socket()->disconnect();
}

/** *****************************************************************
 ** @brief Transmit data.
 ** @param data A byte array of data to transmit.
 ********************************************************************/
void CTcpDevice::tx(QByteArray& data)
{
	socket()->write( data.data(), data.length() );
	emit statusMessage( this, 8, "SERIAL TX: [" + QString( data.toPercentEncoding() ) + "]" );
}

/** *****************************************************************
 ** @brief Receive data from the tcp port..
 ********************************************************************/
void CTcpDevice::rxTcpPort()
{
	mRXPacket = socket()->readAll();
	emit statusMessage( this, 8, "SERIAL RX: [" + QString( mRXPacket.toPercentEncoding() ) + "]" );
}

/** *****************************************************************
 ** @brief TCP connection was successful.
 ********************************************************************/
void CTcpDevice::connected()
{
	emit statusMessage( this, 8, "CONNECTED" );
}

/** *****************************************************************
 ** @brief TCP disconnected.
 ********************************************************************/
void CTcpDevice::disconnected()
{
	emit statusMessage( this, 8, "DISCONNECTED" );
}

/** *****************************************************************
 ** @brief TCP error.
 ********************************************************************/
void CTcpDevice::error( QAbstractSocket::SocketError socketError )
{
    emit statusMessage( this, 8, "SOCKET ERROR " + QString::number(static_cast<int>(socketError)) );
}

/** *****************************************************************
 ** @brief TCP host was found.
 ********************************************************************/
void CTcpDevice::hostFound()
{
	emit statusMessage( this, 8, "HOST FOUND" );
}

/** *****************************************************************
 ** @brief TCP state has chenged.
 ********************************************************************/
void CTcpDevice::stateChanged ( QAbstractSocket::SocketState socketState )
{
    emit statusMessage( this, 8, "STATE CHANGED " + QString::number(static_cast<int>(socketState)) );
}
