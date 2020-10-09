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
#include "cvariable.h"

/** ***********************************************************************
** @brief Constructor
**************************************************************************/
CVariable::CVariable()
{
}

/** ***********************************************************************
** @brief Destructor
**************************************************************************/
CVariable::CVariable(const CVariable& other) : QObject()
{
	copy(other);
}

/** *****************************************************************
* @brief Copy from another.
* @param other A reference to an instance of the other.
* @return Reference to this..
********************************************************************/
const CVariable& CVariable::copy( const CVariable& other)
{
	if ( &other != this )
	{
		mRegisterName			= other.mRegisterName;
		mType					= other.mType;
		mAccess					= other.mAccess;
        mSystemAccess           = other.mSystemAccess;
		mValue					= other.mValue;
		mShortUnitOfMeasure		= other.mShortUnitOfMeasure;
		mName					= other.mName;
	}
	return *this;
}

/** *****************************************************************
* @brief Parse a packet into it's various fields.
* @brief Format Rn:type:access:value:unitofmeasure:name:crc
* @return true upon success, else false.
********************************************************************/
void CVariable::refresh(QString packet)
{
	QStringList columns = packet.split(':');
    if ( columns.count() == 11 )
    {
        mRegisterName				= columns[1];
        mType						= columns[3];
        mAccess						= columns[4];
        mSystemAccess				= columns[5];
        mValue						= columns[6];
        mShortUnitOfMeasure			= columns[7];
        mName						= columns[2];
    }
    else
	if ( columns.count() == 7 )
	{
		mRegisterName				= columns[0];
		mType						= columns[1];
		mAccess						= columns[2];
		mValue						= columns[3];
		mShortUnitOfMeasure			= columns[4];
		mName						= columns[5];
	}
}

