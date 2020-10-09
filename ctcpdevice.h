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
#ifndef CTCPDEVICE_H
#define CTCPDEVICE_H

#include "cdevice.h"

#include <QByteArray>
#include <QTcpSocket>

class CTcpDevice : public CDevice
{
	Q_OBJECT
	public:
		CTcpDevice(QString host, int port, QObject *parent=NULL, int retries=4, int rxTimeout=4);
		CTcpDevice(const CTcpDevice& other);
		virtual ~CTcpDevice();
		CTcpDevice& operator=(const CTcpDevice &other) {return copy(other);}
		CTcpDevice&				copy(const CTcpDevice& other);
		QTcpSocket*				socket()	{return &mSocket;}
		tDeviceType				deviceType() {return TcpDevice;}

	protected:
		virtual QString			name();
		virtual bool			isOpen();
		virtual bool			open();
		virtual void			close();
	public slots:
		virtual	void			tx(QByteArray& data);
	protected slots:
		void					connected();
		void					disconnected();
		void					error( QAbstractSocket::SocketError socketError );
		void					hostFound();
		void					stateChanged ( QAbstractSocket::SocketState socketState );
		void					rxTcpPort();								/* receive pending data */
	private:
		QTcpSocket				mSocket;
		QString					mHost;
		int						mPort;
};


#endif // CTCPDEVICE_H
