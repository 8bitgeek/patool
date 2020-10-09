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
#include "cserver.h"
#include "csettings.h"
#include "cserialdevice.h"
#include "ctcpdevice.h"

QList<CSession*> CServer::mSessionList;

/** ***********************************************************************
** @brief Constructor
**************************************************************************/
CServer::CServer(CDevice* device)
: mTcpServer(NULL)
, mDevice(device)

{
}

/** ***********************************************************************
** @brief Destructor
**************************************************************************/
CServer::~CServer()
{
	if ( mDevice != NULL )
	{
		delete mDevice;
	}
	if ( mTcpServer != NULL )
	{
	}
}

/** ***************************************************************************
* @brief Initialize important variables.
******************************************************************************/
void CServer::initialize()
{
	/**
	 ** Create a TCP server.
	 */
	if ( mTcpServer != NULL )
	{
		delete mTcpServer;
		mTcpServer = NULL;
	}
	mTcpServer = new QTcpServer();
	mTcpServer->setMaxPendingConnections ( gSettings.getBacklog() );
	QObject::connect ( mTcpServer, SIGNAL ( newConnection() ), this, SLOT ( newConnection() ) );
	if ( mTcpServer->listen ( QHostAddress::Any, gSettings.getListenPort() ) )
	{
		emit statusMessage ( this, 2, "SERVER: LISTENING ON PORT " + QString::number(mTcpServer->serverPort()) );
	}
	else
	{
		emit statusMessage ( this, 1, "SERVER: LISTEN FAILED '" + mTcpServer->errorString() + "'" );
	}
}

/** ***************************************************************************
* @brief When a session has been closed, decommission it.
******************************************************************************/
void CServer::sessionClosed()
{
	for ( int index=0; index < mSessionList.count(); index++ )
	{
		CSession* session = mSessionList.at ( index );
		if ( session->isFinished() )
		{
			mSessionList.removeAt ( index );
			emit statusMessage ( session, 2, tr ( "SESSION CLOSED" ) );
			session->deleteLater();
		}
	}
}

/** ***************************************************************************
* @brief Connect a new session.
******************************************************************************/
void CServer::newConnection()
{
	QTcpSocket* socket;
	while ( ( socket = mTcpServer->nextPendingConnection() ) != NULL )
	{
		spawnNewSession(socket);
	}
}

/** ***************************************************************************
* @brief Spawn a new session to support an inbound TCP connection.
* @param socket The newly connected inbound socket connection.
******************************************************************************/
void CServer::spawnNewSession ( QTcpSocket* socket )
{
	CSession* session = new CSession(socket,*mDevice,this);
	mSessionList.append(session);
	QObject::connect(session,SIGNAL(finished()),this,SLOT ( sessionClosed()));
	QObject::connect(session,SIGNAL(statusMessage(void*,int,QString)),this,SIGNAL(statusMessage(void*,int,QString)));
	emit statusMessage(this,2,tr( "SESSION COUNT : '" )+QString::number(mSessionList.count()) +QString("'"));
	session->start();
}

