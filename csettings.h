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
#ifndef CSETTINGS_H
#define CSETTINGS_H

#include <QObject>
#include <QSettings>
#include <QString>
#include <QStringList>

#ifdef Q_OS_WIN32
	#define DEFAULT_DEVICE				"COM1"
#else
	#define DEFAULT_DEVICE				"/dev/ttyUSB0"
#endif
#define	DEFAULT_BAUD					2400
#define DEFAULT_SERVER					false
#define DEFAULT_LISTEN_PORT				20100
#define DEFAULT_CONNECT_HOST			""
#define DEFAULT_CONNECT_PORT			20100
#define	DEFAULT_BROADCAST_PORT			20200
#define	DEFAULT_BACKLOG					20
#define DEFAULT_RX_TIMEOUT				4
#define DEFAULT_RX_RETRIES				5
#define DEFAULT_LOGFILE					""
#define DEFAULT_READREGISTER			""
#define DEFAULT_READVARIABLE			""
#define DEFAULT_SEPCHAR					"\t"
#define DEFAULT_OPENDELAY				50						/** Delay (milliseconds) after opening the device to allow for it to boot up */
#define DEFAULT_OUTPUTFORMAT			0						/** 0=all */
#define DEFAULT_PROTOCOL                0                       /** 0=PA10 1=PA1100 */

class CSettings : public QObject
{
	Q_OBJECT
	public:
		CSettings(QObject *parent = 0);
		virtual ~CSettings();
		QStringList	getSettingsStringList();
		
		QString				trademark()								{return "PATool";}
        QString				version()								{return "1.5";}
        QString				copyright()								{return "(c) 2010-2019 Pike Aerospace Research Corp.";}
        QString				contact()								{return "Sales & Support <sales@pikeaero.com> | http://www.pikeaero.com";}
		
		/** getters... */
		inline QString		getDevice()								{return mDevice;}
		inline int			getBaud()								{return mBaud;}
		inline bool			getServer()								{return mServer;}
		inline int			getListenPort()							{return mListenPort;}
		inline QString		getConnectHost()						{return mConnectHost;}
		inline int			getConnectPort()						{return mConnectPort;}
		inline int			getBroadcastPort()						{return mBroadcastPort;}
		inline int			getBacklog()							{return mBacklog;}
		inline int			getLogging()							{return mLogging;}
		inline int			getRxTimeout()							{return mRxTimeout;}
		inline int			getRxRetries()							{return mRxRetries;}
		inline QString		getLogfile()							{return mLogfile;}
		inline QString		getReadRegister()						{return mReadRegister;}
		inline QString		getReadVariable()						{return mReadVariable;}
		inline QString		getSepchar()							{return mSepchar;}
		inline int			getOpenDelay()							{return mOpenDelay;}
		inline int			getOutputFormat()						{return mOutputFormat;}
        inline int          getProtocol()                           {return mProtocol;}

		/** setters... */
		inline void			setDevice(QString n)					{mDevice=n;}
		inline void			setBaud(int n)							{mBaud=n;}
		inline void			setServer(bool n)						{mServer=n;}
		inline void			setListenPort(int n)					{mListenPort=n;}
		inline void			setConnectHost(QString n)				{mConnectHost=n;}
		inline void			setConnectPort(int n)					{mConnectPort=n;}
		inline void			setBroadcastPort(int n)					{mBroadcastPort=n;}
		inline void			setBacklog(int n)						{mBacklog=n;}
		inline void			setLogging(int n)						{mLogging=n;}
		inline void			setRxTimeout(int n)						{mRxTimeout=n;}
		inline void			setRxRetries(int n)						{mRxRetries=n;}
		inline void			setLogfile(QString n)					{mLogfile=n;}
		inline void			setReadRegister(QString n)				{mReadRegister=n;}
		inline void			setReadVariable(QString n)				{mReadVariable=n;}
		inline void			setSepchar(QString n)					{mSepchar=n;}
		inline void			setOpenDelay(int n)						{mOpenDelay=n;}
		inline void			setOutputFormat(int n)					{mOutputFormat=n;}
        inline void         setProtocol(int n)                      {mProtocol=n;}

		/** operational... */
		int					parseCommandLine(int argc, char* argv[]);

	public slots:
		void				printHelp();
		void				printVersion();
		void				save();
		void				restore();

	private:
		QString				mDevice;					/** the device name */
		int					mBaud;						/** the device baud rate */
		bool				mServer;					/** is it server mode? */
		int					mListenPort;				/** base port to listen on */
		QString				mConnectHost;				/** the host to connect to */
		int					mConnectPort;				/** base port to connect on */
		int					mBroadcastPort;				/** port to broadcast on */
		int					mBacklog;					/** connection backlog */
		int					mLogging;					/** logging level */
		bool				mSaveSettings;				/** save/nosave settings */
		QString				mSettingsFile;				/** settings file */
		int					mRxRetries;					/** the number of reciever timeouts */
		int					mRxTimeout;					/** the timeout period in seconds for receive packets */
		QString				mLogfile;					/** the logfle */
		QString				mReadRegister;				/** the register to read */
		QString				mReadVariable;				/** the variable to read */
		QString				mSepchar;					/** the field separation character */
		int					mOpenDelay;					/** delay time after device open to allow time to boot up (in milliseconds) */
		int					mOutputFormat;				/** output format */
        int                 mProtocol;                  /** protocol version */

};

#endif

extern CSettings gSettings;

