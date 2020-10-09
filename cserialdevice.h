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
#ifndef CSERIALDEVICE_H
#define CSERIALDEVICE_H

#include "cdevice.h"
#include "cserial.h"

#include <QTimerEvent>

class CSerialDevice : public CDevice
{
	Q_OBJECT
	public:
		CSerialDevice(QObject *parent=NULL, int retries=4, int rxTimeout=4);
		CSerialDevice(const CSerialDevice& other);
		virtual ~CSerialDevice();
		CSerialDevice&			copy(const CSerialDevice& other);
		CSerialDevice& operator=(const CSerialDevice &other) {return copy(other);}
		tDeviceType					deviceType() {return SerialDevice;}

	protected:
		virtual QString			name();
		virtual bool			isOpen();
		virtual bool			open();
		virtual void			close();
		CSerial*				serialPort()								{return mSerialPort;}
		CSerial*				setSerialPort(CSerial* serialPort)			{return(mSerialPort=serialPort);}
	public slots:
		virtual	void			tx(QByteArray& data);
	protected slots:
		void					rxSerialPort();								/* receive pending data */
	private:
		CSerial*				mSerialPort;
};


#endif // CSerialDEVICE_H
