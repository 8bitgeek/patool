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
#include <QtCore/QCoreApplication>
#include <QString>

#include "csettings.h"
#include "cserialdevice.h"
#include "ctcpdevice.h"
#include "cconsole.h"
#include "cserver.h"

CSettings gSettings;
int gFault=0;

/** *****************************************************************
* @brief Main entry point.
********************************************************************/
int main(int argc, char *argv[])
{
	CDevice* device = NULL;
	QCoreApplication a(argc, argv);
	if ( gSettings.parseCommandLine(argc,argv) > 0 )
	{
		gSettings.restore();
		CConsole console;
		if ( gSettings.getConnectHost().length() )
		{
			device = new CTcpDevice(gSettings.getConnectHost(),gSettings.getConnectPort());
		}
		else
		{
			device = new CSerialDevice();
		}
		QObject::connect(device,SIGNAL(statusMessage(void*,int,QString)),&console,SLOT(slotStatusMessage(void*,int,QString)));
		QObject::connect(device,SIGNAL(readVariable(CVariable*)),&console,SLOT(slotReadVariable(CVariable*)));
		if ( gSettings.getServer() )
		{
			CServer server(device);
			QObject::connect(&server,SIGNAL(statusMessage(void*,int,QString)),&console,SLOT(slotStatusMessage(void*,int,QString)));
			server.initialize();
			a.exec();
		}
		else
		{
			if ( gSettings.getReadRegister().length() )
			{
				device->readSensor(gSettings.getReadRegister());
			}
			else if ( gSettings.getReadVariable().length() )
			{
				device->readSensorVariable(gSettings.getReadVariable());
			}
			else
			{
				device->readSensor();
			}
		}
		gSettings.save();
		delete device;
		if ( gSettings.getOutputFormat() == 3 )
		{
			printf("\n");
		}
		exit(gFault);
	}
}
