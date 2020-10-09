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

#include "cconsole.h"
#include "csettings.h"

#include <QDateTime>

/** *****************************************************************
 ** @brief Output a status message.
 ** @param p A pointer to the object emitting the message.
 ** @param l The logging level threshold for the message.
 ** @param s The message string.
 ********************************************************************/
void CConsole::slotStatusMessage( void* p, int l, QString s)
{
	if ( gSettings.getLogging()>=l)
	{
		FILE* output;
		if ( gSettings.getLogfile().length() )
		{
            output = fopen( gSettings.getLogfile().toLocal8Bit().data(), "a");
		}
		else
		{
			output = stderr;
		}
        fprintf( output, "%s|%p|%d|%s\n", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz").toLocal8Bit().data(), p, l, s.toLocal8Bit().data() );
		if ( output != stderr )
		{
			fclose( output );
		}
	}
}

/** *****************************************************************
 ** @brief A variable has been read, output it..
 ** @param variable Pointer to a variable instance.
 ********************************************************************/
void CConsole::slotReadVariable(CVariable* variable)
{
    char sepchar=gSettings.getSepchar().toLocal8Bit().data()[0];
	switch( gSettings.getOutputFormat() )
	{
	default:
	case 0:
		{
            printf( "%s\n", variable->value().toString().toLocal8Bit().data() );
		}
		break;
	case 1:
		{
            printf( "%s %s\n", variable->value().toString().toLocal8Bit().data(), variable->shortUnitOfMeasure().toLocal8Bit().data() );
		}
		break;
	case 2:
		{
			printf( "%s%c%s%c%s%c%s%c%s%c%s%c",
                    variable->registerName().toLocal8Bit().data(),
					sepchar,
                    variable->type().toLocal8Bit().data(),
					sepchar,
                    variable->access().toLocal8Bit().data(),
					sepchar,
                    variable->value().toString().toLocal8Bit().data(),
					sepchar,
                    variable->shortUnitOfMeasure().toLocal8Bit().data(),
					sepchar,
                    variable->name().toLocal8Bit().data(),
					sepchar
					);
		}
		break;
	case 3:
		{
            printf( "%s=%s\n", variable->registerName().toLocal8Bit().data(), variable->value().toString().toLocal8Bit().data() );
		}
		break;
	case 4:
		{
            printf( "%s=%s\n", variable->name().toLocal8Bit().data(), variable->value().toString().toLocal8Bit().data() );
		}
		break;
	}
}


