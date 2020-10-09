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
#ifndef CCONSOLE_H
#define CCONSOLE_H

#include "cvariable.h"

#include <QObject>
#include <stdio.h>

class CConsole : public  QObject
{
	Q_OBJECT
	public:
	CConsole() {};
	public slots:
		void	slotStatusMessage( void* p, int l, QString s);
		void	slotReadVariable(CVariable* variable);
};


#endif // CSTATUSHANDLER_H
