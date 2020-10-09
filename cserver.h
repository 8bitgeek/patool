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
#ifndef CSERVER_H
#define CSERVER_H

#include "csession.h"
#include "cdevice.h"
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

class CServer : public QObject
{
	Q_OBJECT
	public:
		CServer(CDevice* device);
		virtual ~CServer();

		CDevice*							device()			{return mDevice;}

	public slots:
		void								initialize();

	signals:
		void								statusMessage ( void*, int level, QString msg );
		void								quit();

	private slots:
		void								spawnNewSession ( QTcpSocket* socket );
		void								newConnection();
		void								sessionClosed();

	private:
		static	QList<CSession*>			mSessionList;		/* TCP sessions */
		QTcpServer*							mTcpServer;			/* The TCP Server. */
		CDevice*							mDevice;

};

#endif // CSERVER_H
