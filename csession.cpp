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
#include "csession.h"
#include "cserver.h"
#include "cserialdevice.h"
#include "ctcpdevice.h"

#define inherited QThread

/** ***********************************************************************
** @brief Constructor
**************************************************************************/
CSession::CSession(QTcpSocket* socket, CDevice& device, CServer* parent)
: inherited ( parent )
, mSocket(socket)
, mServer(parent)
{
	switch ( device.deviceType() )
	{
	case CDevice::SerialDevice:
		mDevice = new CSerialDevice((CSerialDevice&)device);
		break;
	case CDevice::TcpDevice:
		mDevice = new CTcpDevice((CTcpDevice&)device);
		break;
	default:
		mDevice=NULL;
	}
	this->socket()->setSocketOption(QAbstractSocket::LowDelayOption,1);
	QObject::connect(this->socket(),SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
	QObject::connect(this->socket(),SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
	QObject::connect(this->socket(),SIGNAL(hostFound()),this,SLOT(slotHostFound()));
	QObject::connect(this->socket(),SIGNAL(stateChanged(QAbstractSocket::SocketState)),this,SLOT(slotStateChanged(QAbstractSocket::SocketState)));
	QObject::connect(this->socket(),SIGNAL(readyRead()),this,SLOT(rxSocket()));
	mTimer = startTimer( 250 );
}

/** ***********************************************************************
** @brief Destructor
**************************************************************************/
CSession::~CSession()
{
	if ( socket()->isOpen() )
	{
		socket()->close();
	}
	delete mDevice;
	mDevice=NULL;
}

/** ***********************************************************************
** @brief Close the socket
**************************************************************************/
void CSession::close()
{
	emit statusMessage( this, 2, "TCP CLOSE '" + socket()->peerAddress().toString() + ":" + QString::number( socket()->peerPort() ) + "' -> " + socket()->localAddress().toString() + ":" + QString::number( socket()->localPort() ) + "'" );
	socket()->close();
	quit();
}

/** ***********************************************************************
 **	@brief Transmit a packet onto the wire...
**************************************************************************/
void CSession::txSocket(QByteArray& data)
{
	socket()->write( data.data(), data.length() );
	socket()->waitForBytesWritten();
	emit statusMessage( this, 2, "TCP TX: [" + QString( data.toPercentEncoding() ) + "]" );
}

/** ***********************************************************************
 **	@brief Transmit a packet onto the wire...
**************************************************************************/
void CSession::txSocket(QString data)
{
    QByteArray bytes( data.toLocal8Bit() );
	bytes += "\r\n";
	txSocket( bytes );
}

/** ***********************************************************************
 ** @brief Recieve data from the TCP socket. Emits the rx( QByteArray )
 ** @brief signal with actual data to transmit to device.
**************************************************************************/
void CSession::rxSocket()
{
	mTXPacket.clear();
	while ( socket()->bytesAvailable() )
	{
		mTXPacket += socket()->readAll();
	}
	emit statusMessage( this, 2, "TCP RX: [" + QString( mTXPacket.toPercentEncoding() ) + "]" );
	device()->readRaw(mTXPacket,&mRXPacket);
	txSocket( mRXPacket );
}

/** ***********************************************************************
 ** @brief Receive a fault message.
**************************************************************************/
void CSession::fault( QString msg )
{
	emit statusMessage( this, 1, msg );
	msg += "\n";
    socket()->write( msg.toLocal8Bit().data() );
}

/** ***********************************************************************
 ** @brief Run the thread
**************************************************************************/
void CSession::run()
{
	exec();
}

/** ***********************************************************************
 ** @brief The socket has been disconnected.
**************************************************************************/
void CSession::slotDisconnected()
{
	emit statusMessage ( this, 2, tr ( "SESSION DISCONNECT" ) );
	close();
}

/** ***********************************************************************
 ** @brief A socket error has occured.
**************************************************************************/
void CSession::slotError ( QAbstractSocket::SocketError err )
{
	emit statusMessage ( this, 1, tr ( "SESSION FAULT : '" )+QString::number ( err )+"'" );
}

/** ***********************************************************************
 ** @brief The host lookup succeeded.
**************************************************************************/
void CSession::slotHostFound()
{
	emit statusMessage ( this, 1, tr ( "SESSION HOST NOT FOUND" ) );
}

/** ***********************************************************************
 ** @brief The socket state has changed.
**************************************************************************/
void CSession::slotStateChanged ( QAbstractSocket::SocketState state )
{
	emit statusMessage ( this, 9, tr ( "SESSION STATE : '" )+QString::number ( state )+"'" );
}

/** ***********************************************************************
 ** @brief Handle a timer event.
 ** @param e The timer event.
**************************************************************************/
void CSession::timerEvent(QTimerEvent* e)
{
	if ( e->timerId() == mTimer )
	{
	}
}

