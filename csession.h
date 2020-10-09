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
#ifndef CSESSION_H
#define CSESSION_H

#include "cdevice.h"

#include <QThread>
#include <QTcpSocket>
#include <QTimerEvent>

class CServer;
class CSession : public QThread
{
	Q_OBJECT
	public:
		CSession(QTcpSocket* socket, CDevice& device, CServer* parent);
		virtual ~CSession();

		QTcpSocket*					socket()					{return mSocket;}

	signals:
		void						statusMessage( void* sender, int level, QString msg );

	public slots:
		virtual		void			txSocket( QByteArray& data );
		virtual		void			txSocket(QString data);

		virtual	void				close();
		virtual		void			fault( QString msg );

	protected:
		CDevice*					device()									{return mDevice;}
		CServer*					server()									{return mServer;}
		virtual		void			run();
		virtual		void			timerEvent(QTimerEvent* e);

	protected slots:
		virtual	void				rxSocket();										/* receive pending data */
		virtual	void				slotDisconnected();
		virtual	void				slotError(QAbstractSocket::SocketError err);
		virtual	void				slotHostFound();
		virtual	void				slotStateChanged(QAbstractSocket::SocketState state);

	private:
		QTcpSocket*					mSocket;					/** the socket */
		CDevice*					mDevice;					/** the device */
		CServer*					mServer;					/** the server */
		int							mTimer;
		QByteArray					mTXPacket;										/* outbound packet */
		QByteArray					mRXPacket;										/* incoming packet from device */
};

#endif // CSESSION_H
