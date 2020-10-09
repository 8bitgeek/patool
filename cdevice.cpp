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
#include "cdevice.h"

#include <QEventLoop>
#include <QTime>
#include <QStringList>

#include "csettings.h"

#include <unistd.h>

QMutex CDevice::mDeviceMutex;

/** ***************************************************************************
* @brief Constructor
******************************************************************************/
CDevice::CDevice(QObject *parent, int retries, int rxTimeout)
: QObject(parent)
, mRxRetries(retries)
, mRxTimeout(rxTimeout)
{
	generateCRCTab16();
}

/** ***************************************************************************
* @brief Constructor
******************************************************************************/
CDevice::CDevice(const CDevice& other)
: QObject(other.parent())
{
	generateCRCTab16();
	copy( other );
}

/** ***************************************************************************
* @brief Destructor
******************************************************************************/
CDevice::~CDevice()
{
	for( int n=0; n < mVariables.count(); n++ )
	{
		delete mVariables[n];
	}
}

/** ***************************************************************************
* @brief Copy from another instance.
******************************************************************************/
CDevice& CDevice::copy(const CDevice &other)
{
	if ( &other != this )
	{
		mRxRetries	= other.mRxRetries;					/** Number of retries */
		mRxTimeout	= other.mRxTimeout;				/** max time to wait for a packet to arrive */
	}
	return *this;
}

/** *****************************************************************
* @brief Test the Checksum and return the packet data
* @param packet The packet split out into a string list.
* @return true if Checksum is okay
********************************************************************/
bool CDevice::checkChecksum( QStringList packet )
{
    bool ok;
    int columns = gSettings.getProtocol() ? 11 : 7;
    ushort packetChecksum;

    if ( packet.count() == columns )
    {
        ushort checksum = 0;
        for( int x=0; x < columns-1; x++)
        {
            QString field = packet[x];
            for ( int y=0; y < field.length(); y++ )
            {
                checksum += static_cast<unsigned short>(field[y].toLatin1());
            }
            if ( x < columns-1 )
            {
                checksum += static_cast<unsigned short>(SEPCHAR);
            }
        }
        checksum = ~checksum;
        packetChecksum = packet[columns-1].trimmed().toUShort(&ok, 16);
        return ( packetChecksum == checksum );
    }
    return false;
}

/** ***************************************************************************
* @brief The CRC's are computed using polynomials. The  coefficients
* @brief for the algorithms are defined by the following constants.
******************************************************************************/
#define	P_16        0xA001		 /* 16 bit polynomial */
void CDevice::generateCRCTab16(void)
{
	unsigned short crc, c;
	for (int i=0; i<256; i++)
	{
		crc = 0;
		c   = (unsigned short) i;
		for (int j=0; j<8; j++)
		{
			if ( (crc ^ c) & 0x0001 )
				crc = ( crc >> 1 ) ^ P_16;
			else
				crc =   crc >> 1;
			c = c >> 1;
		}
		mCRC[i]=crc;
	}
}

/** ***************************************************************************
* @brief The CRC's are computed using polynomials. The  coefficients
* @brief for the algorithms are defined by the following constants.
* @param ch The character to lookup.
* @return The CRC
******************************************************************************/
unsigned short CDevice::crcTab16( unsigned char ch )
{
	return mCRC[ch];
}

/** *****************************************************************
* @brief The function update_crc_16 calculates a  new  CRC-16  value
* @brief based  on  the  previous value of the CRC and the next byte
* @brief of the data to be checked.
* @param crc The crrent CRC value.
* @param c The next character to use in the CRC calculation.
* @return The new CRC value.
********************************************************************/
unsigned short CDevice::updateCRC16( unsigned short crc, char c )
{
	unsigned short tmp, short_c;
	short_c = 0x00ff & (unsigned short) c;
	tmp = crc ^ short_c;
	crc = (crc >> 8) ^ crcTab16( tmp & 0xff );
	return crc;
}

/** *****************************************************************
* @brief Test the CRC and return the packet data
* @param packet The packet split out into a string list.
* @return true if CRC is okay
********************************************************************/
bool CDevice::checkCRC( QStringList packet )
{
	bool ok;
    int columns = gSettings.getProtocol() ? 11 : 7;
    if ( packet.count() == columns )
	{
		unsigned short crc = 0;
        for( int x=0; x < columns-1; x++)
		{
			QString field = packet[x];
			for ( int y=0; y < field.length(); y++ )
			{
                crc = updateCRC16(crc,field[y].toLatin1());
			}
            if ( x < columns-1 )
			{
				crc = updateCRC16(crc,SEPCHAR);
			}
		}
        ushort packetCRC;
        if (gSettings.getProtocol())
        {
            QString num = packet[columns-1];
            num = num.trimmed();
            packetCRC = num.toUShort(&ok, 16);
        }
        else
            packetCRC = packet[columns-1].toShort(&ok, 10);
        return ( packetCRC == crc );
	}
    return false;
}

/** *****************************************************************
* @brief Convert an integer to a hex string
* @param i The integer to convert.
* @return A hex string.
********************************************************************/
#define itox(c) ((c)>=0x0A&&(c)<=0x0F?(c)+0x37:(c)+'0')
QString CDevice::toHex(unsigned short i)
{
	QString str="";
	str += (char)itox((i&0xF000)>>12);
	str += (char)itox((i&0x0F00)>>8);
	str += (char)itox((i&0x00F0)>>4);
	str += (char)itox((i&0x000F));
	return str;
}

/** *****************************************************************
 ** @brief Append a CRC to the command.
 ** @return The command string with CRC appended.
 ********************************************************************/
QString CDevice::appendCRC16(QString command)
{
	#ifdef USE_TX_CRC
		unsigned short crc=0;
		for( int n=0; n < command.length(); n++ )
		{
            crc = updateCRC16( crc, command.toLocal8Bit().data()[n] );
		}
		command += ":" + toHex(crc);
	#endif
	command += "\r";
	return command;
}

/** *****************************************************************
 ** @brief Get the reply from the serial device.
 ** @brief Test the CRC and retransmit if nessesary.
 ** @param command The command to send to the serial device.
 ** @return true on success, else false.
 ********************************************************************/
CVariable* CDevice::getReply(QString command)
{
	QEventLoop eventLoop;
	if ( command.length() )
	{
		command = appendCRC16(command);
        mTXPacket = command.toLocal8Bit();
		mRXPacket.clear();
		tx(mTXPacket);
		QTime start = QTime::currentTime();
		/** wait for inbound packet or timeout... */
		while ( QTime::currentTime() < start.addSecs(rxTimeout()) && !(mRXPacket.contains(':') && mRXPacket.contains('\r') && mRXPacket.contains('\n') ) )
		{
			eventLoop.processEvents();
            usleep(50*1000);
		}
		return parse(mRXPacket);
	}
	return NULL;
}

/** *****************************************************************
 ** @brief Atomic fa variable by register name else create it
 ** @param registerName The Register Name
 ** @return Index of variable
 ********************************************************************/
int CDevice::findVariableByRegisterName( QString registerName )
{
	for( int n=0; n < mVariables.count(); n++ )
	{
		if ( mVariables[n]->registerName() == registerName )
		{
			return n;
		}
	}
	return -1;
}

/** *****************************************************************
 ** @brief Atomic fa variable by register name else create it
 ** @param Register name
 ** @return Reference to the variable
 ********************************************************************/
CVariable* CDevice::findOrCreateVariableByRegisterName( QString registerName )
{
	int n = findVariableByRegisterName( registerName );
	if ( n >= 0 )
	{
			return mVariables[n];
	}
	else
	{
		CVariable* newVariable = new CVariable();
		newVariable->setRegisterName(registerName);
		mVariables.append(newVariable);
		return newVariable;
	}
}

/** *****************************************************************
 ** @brief Parse a packet into it's various fields.
 ** @brief Format Rn:type:access:value:unitofmeasure:name:crc
 ** @return true upon success, else false.
 ********************************************************************/
CVariable* CDevice::parse(QString packet)
{
    QStringList columns = packet.split(':');
    if ( gSettings.getProtocol() )
    {
        if ( columns.count() == 11 )
        {
            if (checkChecksum(columns) || checkCRC(columns))
            {
                CVariable* variable = findOrCreateVariableByRegisterName( columns[1] );
                variable->refresh( packet );
                return variable;
            }
        }
    }
    else
    {
        if ( columns.count() == 7 )
        {
            if (checkChecksum(columns) || checkCRC(columns))
            {
                QString registerName = columns[0];
                if ( registerName[0] == 'R' || registerName[0] == 'r' )
                {
                    registerName.remove(0,1);
                }
                CVariable* variable = findOrCreateVariableByRegisterName( registerName );
                variable->refresh( packet );
                return variable;
            }
        }
    }
	return NULL;
}

/** *****************************************************************
 ** @brief Read a register
 ** @return variable
 ********************************************************************/
CVariable* CDevice::readRegister(QString registerName,bool emitVariable)
{
	for( int tries = 0; tries < rxRetries(); tries++ )
	{
		CVariable* variable;
        QString command;
        if (gSettings.getProtocol())
            command = "R:"+registerName;
        else
            command = "R"+registerName;
        if ( ( variable = getReply( command ) ) != NULL )
		{
			if ( emitVariable )
			{
				emit readVariable(variable);
			}
			return variable;
		}
	}
	emit statusMessage( this, 1, "DEVICE TIMEOUT '" + name() + "' @ '"+registerName+"'" );
	set_fault(FAULT_TIMEOUT);
	return NULL;
}

/** *****************************************************************
 ** @brief Execute the next serial device command.
 ** @return true upon success, else false.
 ********************************************************************/
bool CDevice::readRegisters()
{
	CVariable* variable=NULL;
	QString registerName;
	int varCount=0;
	int varIndex=0;
	for ( varIndex=0; (varIndex == 0 || varIndex < varCount ); varIndex++ )
	{
        registerName = QString::number(varIndex);
		if ( (variable=readRegister( registerName )) != NULL )
		{
            if ( gSettings.getProtocol() )
                varCount = mVariables[findVariableByRegisterName("0")]->value().toInt();
            else
                varCount = mVariables[findVariableByRegisterName("R0")]->value().toInt();
			if ( varIndex == varCount-1 )
			{
				emit readingsComplete();
				return true;
			}
		}
		else
		{
			return false;
		}
	}
	return true;
}


/** *****************************************************************
 ** @brief Private device open function
 ********************************************************************/
bool CDevice::_open()
{
	if ( !isOpen() )
	{
		QEventLoop loop;
		if ( !mDeviceMutex.tryLock() )
		{
			loop.processEvents();
            usleep(50*1000);
		}
		if ( open() )
		{
			emit statusMessage( this, 2, "DEVICE OPEN '" + name() + "'" );
			return true;
		}
		emit statusMessage( this, 1, "DEVICE OPEN FAILED '" + name() + "'" );
		return false;
	}
	return true;
}

/** *****************************************************************
 ** @brief Private device close function
 ********************************************************************/
void CDevice::_close()
{
	emit statusMessage( this, 2, "DEVICE CLOSE '" + name() + "'" );
	close();
	mDeviceMutex.unlock();
}

/** *****************************************************************
 ** @brief Write and read raw data.
 ********************************************************************/
void CDevice::readRaw(QByteArray txData, QByteArray* rxData, int timeout)
{
	rxData->clear();
	if ( _open() )
	{
		this->tx(txData);
		for( QTime start=QTime::currentTime(); start.secsTo(QTime::currentTime()) < timeout; )
		{
			QEventLoop loop;
			loop.processEvents();
			rxData->append(mRXPacket);
			mRXPacket.clear();
			if ( rxData->length() > 0 && rxData->at(rxData->length()-1) == 0x0A )
			{
				break;
			}
            else
            {
                usleep(50*1000);
            }
		}
		_close();
	}
}

/** *****************************************************************
 ** @brief The entry point to begin reading the entire sensor.
 ********************************************************************/
void CDevice::readSensor()
{
	if ( _open() )
	{
		if ( !readRegisters() )
		{
			emit statusMessage( this, 1, "DEVICE OFFLINE '" + name() + "'" );
			set_fault(FAULT_OFFLINE);
		}
		_close();
	}
}

/** *****************************************************************
 ** @brief The entry point to begin reading a sensor register.
 ** @param registerName The name of the register to read.
 ********************************************************************/
void CDevice::readSensor(QString registerName)
{
	if ( _open() )
	{
		if ( readRegister(registerName) == NULL )
		{
			emit statusMessage( this, 1, "DEVICE OFFLINE '" + name() + "'" );
			set_fault(FAULT_OFFLINE);
		}
		_close();
	}
}

/** *****************************************************************
 ** @brief The entry point to begin reading a sensor variable by name.
 ** @param name The name of the variable to read.
 ********************************************************************/
void CDevice::readSensorVariable(QString name)
{
	if ( _open() )
	{
		CVariable* variable=NULL;
		QString registerName;
		int varCount=0;
		int varIndex=0;
		for ( varIndex=0; (varIndex == 0 || varIndex < varCount ); varIndex++ )
		{
            registerName = QString::number(varIndex);
			if ( (variable=readRegister( registerName, false )) != NULL )
			{
                if ( gSettings.getProtocol() )
                    varCount = mVariables[findVariableByRegisterName("0")]->value().toInt();
                else
                    varCount = mVariables[findVariableByRegisterName("R0")]->value().toInt();
				if ( variable->name().compare( name, Qt::CaseInsensitive ) == 0 )
				{
					emit readVariable(variable);
					return;
				}
			}
			else
			{
				break;
			}
		}
		emit statusMessage( this, 1, "NO VARIABLE '" + name + "'" );
		set_fault(FAULT_NOVARIABLE);
		_close();
	}
}


