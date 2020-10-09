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
#ifndef CVARIABLE_H
#define CVARIABLE_H

#include <QObject>
#include <QVariant>
#include <QStringList>

class CVariable : public QObject
{
	Q_OBJECT
	public:
		CVariable();
		CVariable(const CVariable& other);

		const CVariable&		copy(const CVariable& other);
		const CVariable&		operator=(const CVariable &other) {return copy(other);}

	public slots:
		void						refresh(QString packet);
		void						setRegisterName(QString registerName)				{mRegisterName=registerName;}
		void						setType(QString type)								{mType=type;}
        void						setAccess(QString access)							{mAccess=access;}
        void						setSystemAccess(QString access)						{mSystemAccess=access;}
        void						setValue(QVariant value)							{mValue=value;}
		void						setShortUnitOfMeasure(QString shortUnitOfMeasure)	{mShortUnitOfMeasure=shortUnitOfMeasure;}
		void						setName(QString name)								{mName=name;}
	public:
		QString						registerName()										{return mRegisterName;}
		QString						type()												{return mType;}
		QString						access()											{return mAccess;}
        QString                     systemAccess()                                      {return mSystemAccess;}
		QVariant					value()												{return mValue;}
		QString						shortUnitOfMeasure()								{return mShortUnitOfMeasure;}
		QString						name()												{return mName;}

		bool						isReadOnly()										{return access()=="R";}
		bool						isReadWrite()										{return access()=="W";}

	private:
		QString						mRegisterName;
		QString						mType;
        QString						mAccess;
        QString						mSystemAccess;
        QVariant					mValue;
		QString						mShortUnitOfMeasure;
		QString						mName;
};

#endif // CVARIABLE_H

