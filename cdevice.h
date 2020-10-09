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
#ifndef CDEVICE_H
#define CDEVICE_H

#include <QObject>
#include <QTimerEvent>
#include <QMutex>

#include "cvariable.h"
#include "cfault.h"

//#define USE_TX_CRC						1		/** Use CRC on transmit packets? */

#define DEFAULT_DEVICE_RETRIES_COUNT	(3)
#define DEFAULT_DEVICE_TIMEOUT_SECONDS	(4)

#define SEPCHAR ':'

class CDevice : public QObject
{
	Q_OBJECT
	public:
		typedef enum
		{
			SerialDevice,
			TcpDevice,
		} tDeviceType;
		CDevice(QObject *parent,
				 int retries=DEFAULT_DEVICE_RETRIES_COUNT,
				 int rxTimeout=DEFAULT_DEVICE_TIMEOUT_SECONDS);
		CDevice(const CDevice& other);
		CDevice& operator=(const CDevice &other) {return copy(other);}
		virtual ~CDevice();

		CDevice&					copy(const CDevice& other);

		int							rxRetries()					{return mRxRetries;}
		void						setRxRetries(int rxRetries)	{mRxRetries=rxRetries;}
		void						setRxTimeout(int rxTimeout)	{mRxTimeout=rxTimeout;}
		int							rxTimeout()					{return mRxTimeout;}
		int							numVariables()				{return mVariables[findVariableByRegisterName("R0")]->value().toInt();}
		virtual tDeviceType			deviceType()=0;

	signals:
		void						statusMessage( void* sender, int level, QString msg);
		void						readVariable( CVariable* variable );
		void						readingsComplete();

	public slots:
		virtual	void				readRaw(QByteArray txData, QByteArray* rxData, int timeout=4);
		virtual void				readSensor();
		virtual void				readSensor(QString registerName);
		virtual void				readSensorVariable(QString name);
		virtual	void				tx(QByteArray& data)=0;

	protected:
		int							findVariableByRegisterName( QString registerName );
		CVariable*					findOrCreateVariableByRegisterName( QString registerName );
		virtual QString				name()=0;
		virtual	bool				isOpen()=0;
		virtual	bool				open()=0;
		virtual	void				close()=0;
		QString						toHex(unsigned short i);
		void						generateCRCTab16(void);
		unsigned short				crcTab16( unsigned char ch );
		unsigned short				updateCRC16( unsigned short crc, char c );
		QString						appendCRC16(QString command);
        bool						checkCRC( QStringList packet  );
        bool						checkChecksum( QStringList packet  );
        CVariable*					parse(QString packet);
		CVariable*					getReply(QString command);
		CVariable*					readRegister(QString registerName,bool emitVariable=true);
		bool						readRegisters();

		QByteArray					mTXPacket;										/* Outbound packet. */
		QByteArray					mRXPacket;										/* Incoming packet from device. */

	private:
		static QMutex				mDeviceMutex;
		bool						_open();
		void						_close();
		int							mRxRetries;										/** Number of retries */
		int							mRxTimeout;										/** max time to wait for a packet to arrive */
		unsigned short				mCRC[256];										/** The CRC table */
		QList<CVariable*>			mVariables;										/** The device variables */
	
	};


#endif // CDevice_H
