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
#include "csettings.h"

#include <unistd.h>
#include <stdio.h>


/** ***************************************************************************
* @brief Constructor
******************************************************************************/
CSettings::CSettings(QObject *parent)
 : QObject(parent)
, mDevice( DEFAULT_DEVICE )
, mBaud( DEFAULT_BAUD )
, mServer( DEFAULT_SERVER )
, mListenPort ( DEFAULT_LISTEN_PORT )
, mConnectHost ( DEFAULT_CONNECT_HOST )
, mConnectPort ( DEFAULT_CONNECT_PORT )
, mBroadcastPort( DEFAULT_BROADCAST_PORT )
, mBacklog ( DEFAULT_BACKLOG )
, mLogging ( 0 )
, mSaveSettings( true )
, mRxRetries( DEFAULT_RX_RETRIES )
, mRxTimeout( DEFAULT_RX_TIMEOUT )
, mLogfile( DEFAULT_LOGFILE )
, mSepchar( DEFAULT_SEPCHAR )
, mOpenDelay( DEFAULT_OPENDELAY )
, mOutputFormat( DEFAULT_OUTPUTFORMAT )
, mProtocol( DEFAULT_PROTOCOL )
{
	restore();
}

/** ***************************************************************************
* @brief Destructor
******************************************************************************/
CSettings::~CSettings()
{
}

/** ***************************************************************************
* @brief Return a string list of the settings for display
******************************************************************************/
QStringList CSettings::getSettingsStringList()
{
	QStringList result;
	result.append("WORD SIZE           : "+QString::number ( sizeof(char*)*8 ) + tr ( " BITS" ) );
	result.append("BASE LISTEN PORT    : "+QString::number(mListenPort));
	result.append("BROADCAST PORT      : "+QString::number(mBroadcastPort));
	result.append("CONNECTION BACKLOG  : "+QString::number(mBacklog));
	result.append("SERIAL DEVICE       : "+mDevice);
	result.append("SERIAL BAUD RATE    : "+QString::number(mBaud));
	result.append("LOGGING LEVEL       : "+QString::number(mLogging));
	result.append("RX TIMEOUT          : "+QString::number(mRxTimeout));
	result.append("RECEIVER RETRIES    : "+QString::number(mRxRetries));
	result.append("LOG FILE            : "+mLogfile);
    result.append("PROTOCOL            : "+QString::number(mProtocol));
	return result;
}

/** ***************************************************************************
* @brief Save the settings.
******************************************************************************/
void CSettings::save()
{
	if ( mSaveSettings )
	{
		if ( mSettingsFile.length() )
		{
			QSettings settings ( mSettingsFile, QSettings::IniFormat );

			settings.beginGroup("serial");
				settings.setValue("device",					mDevice);
				settings.setValue("baud",					mBaud);
                settings.setValue("protocol",               mProtocol);
			settings.endGroup();

			settings.beginGroup("network");
				settings.setValue("listen_port",			mListenPort);
				settings.setValue("broadcast_port",			mBroadcastPort);
				settings.setValue("backlog",				mBacklog);
			settings.endGroup();

			settings.beginGroup("server");
				settings.setValue("logging",				mLogging);
				settings.setValue("rxtimeout",				mRxTimeout);
				settings.setValue("rxretries",				mRxRetries);
				settings.setValue("logfile",				mLogfile);
			settings.endGroup();

		}
	}
}

/** ***************************************************************************
* @brief Restore the settings.
******************************************************************************/
void CSettings::restore()
{
	if ( mSettingsFile.length() )
	{
		QSettings settings( mSettingsFile, QSettings::IniFormat );

		settings.beginGroup("serial");
			mDevice				= settings.value("device",					DEFAULT_DEVICE).toString();
			mBaud				= settings.value("baud",					DEFAULT_BAUD).toInt();
            mProtocol           = settings.value("protocol",                DEFAULT_PROTOCOL).toInt();
		settings.endGroup();

		settings.beginGroup("network");
			mListenPort			= settings.value("listen_port",				DEFAULT_LISTEN_PORT).toInt();
			mBroadcastPort		= settings.value("proadcast_port",			DEFAULT_BROADCAST_PORT).toInt();
			mBacklog			= settings.value("backlog",					DEFAULT_BACKLOG).toInt();
		settings.endGroup();

		settings.beginGroup("server");
			mLogging		= settings.value("logging", 9).toInt();
			mRxTimeout		= settings.value("rxtimeout",					DEFAULT_RX_TIMEOUT ).toInt();
			mRxRetries		= settings.value("rxretries",					DEFAULT_RX_RETRIES ).toInt();
			mLogfile		= settings.value("logfile",						DEFAULT_LOGFILE).toString();
		settings.endGroup();
		
	}
}

/** ***************************************************************************
* @brief Print the version.
******************************************************************************/
void CSettings::printVersion()
{
    printf( "%s Version %s\n", trademark().toLocal8Bit().data(), version().toLocal8Bit().data() );
    printf( "%s\n", copyright().toLocal8Bit().data() );
    printf( "%s\n", contact().toLocal8Bit().data() );
}

/** ***************************************************************************
* @brief Print the help.
******************************************************************************/
void CSettings::printHelp()
{
	printf( "** Transient Settings:\n\n" );
	printf( "--version     -v       Print Version\n" );
	printf( "--nosave      -n       Don't save command line setting (default=save)\n" );
	printf( "--settings    -s <n>   Settings file to use\n" );
	printf( "--help        -h -?    Print this help\n");
	printf( "\n** Server settings:\n\n" );
    printf( "--device       -d <n>  Set the serial device [%s]\n",								getDevice().toLocal8Bit().data() );
	printf( "--baud         -b <n>  Set the serial device [%d]\n",								getBaud() );
	printf( "--server       -S      Go into server mode.\n");
	printf( "--serverport   -p <n>  Set TCP listen port [%d]\n",								getListenPort() );
    printf( "--connecthost  -H <n>  Set TCP connect host [%s]\n",								getConnectHost().toLocal8Bit().data() );
	printf( "--connectport  -P <n>  Set TCP connect port [%d]\n",								getConnectPort() );
	printf( "--udp          -u <n>  Set UDP broadcast port [%d]\n",								getBroadcastPort() );
	printf( "--backlog         <n>  Set backlog [%d]\n",										getBacklog() );
	printf( "--logging      -l <n>  Set Logging Level 0..9 [%d]\n",								getLogging() );
	printf( "--rxtimeout    -x <n>  Set the timeout for received packets [%d]\n",				getRxTimeout() );
	printf( "--rxretries    -t <n>  Set the number of reciever retry attempts [%d]\n",			getRxRetries() );
    printf( "--logfile      -f <n>  Set the logfile output (current=%s)\n",						getLogfile().toLocal8Bit().data() );
	printf( "--readregister -R <n>  Read the device register <n>.\n");
	printf( "--readvariable -V <n>  Read the device variable <n> by name.\n");
	printf( "--sepchar      -S <n>  Set the output field separator character to <n>.\n");
	printf( "--opendelay    -o <n>  Set the open delay in milliseconds.\n");
	printf( "--outputformat -O <n>  Set the output format [%d]\n",								getOutputFormat() );
    printf( "--protocol        <n>  Set protocol version [%d]\n",                               getProtocol() );
}

/** ***************************************************************************
* @brief Parse the command line.
******************************************************************************/
int CSettings::parseCommandLine(int argc, char* argv[])
{
	/**
	* Command line option interpreter.
	*/
	int rc=1;
	int n=0;
	for( n=1; n<argc && rc==1; ) {
		QString arg(argv[n]);
		if ( arg == "--version" || arg == "-v" )
		{
			printVersion();
			rc=-3;
		}
		else if ( arg == "--nosave" || arg == "-n" )
		{
			mSaveSettings=false;
		}
		else if ( arg == "--settings"		|| arg == "-s" )	{	if ( n+1 < argc ) mSettingsFile = QString(argv[++n]);					else rc=-1;		}
		else if ( arg == "--device"			|| arg == "-d" )	{	if ( n+1 < argc ) setDevice(QString(argv[++n]));						else rc=-1;		}
		else if ( arg == "--baud"			|| arg == "-b" )	{	if ( n+1 < argc ) setBaud(QString(argv[++n]).toInt());					else rc=-1;		}
		else if ( arg == "--server"			|| arg == "-S" )	{	setServer(true);																		}
		else if ( arg == "--serverport"		|| arg == "-p" )	{	if ( n+1 < argc ) setListenPort(QString(argv[++n]).toInt());			else rc=-1;		}
		else if ( arg == "--connecthost"	|| arg == "-H" )	{	if ( n+1 < argc ) setConnectHost(argv[++n]);							else rc=-1;		}
		else if ( arg == "--connectport"	|| arg == "-P" )	{	if ( n+1 < argc ) setConnectPort(QString(argv[++n]).toInt());			else rc=-1;		}
		else if ( arg == "--udp"			|| arg == "-u" )	{	if ( n+1 < argc ) setBroadcastPort(QString(argv[++n]).toInt());			else rc=-1;		}
		else if ( arg == "--backlog" )							{	if ( n+1 < argc ) setBacklog(QString(argv[++n]).toInt());				else rc=-1;		}
		else if ( arg == "--logging"		|| arg == "-l" )	{	if ( n+1 < argc ) setLogging(QString(argv[++n]).toInt());				else rc=-1;		}
		else if ( arg == "--rxtimeout"		|| arg == "-x" )	{	if ( n+1 < argc ) setRxTimeout(QString(argv[++n]).toInt());				else rc=-1;		}
		else if ( arg == "--rxretries"		|| arg == "-t" )	{	if ( n+1 < argc ) setRxRetries(QString(argv[++n]).toInt());				else rc=-1;		}
		else if ( arg == "--readregister"	|| arg == "-R" )	{	if ( n+1 < argc ) setReadRegister(argv[++n]);							else rc=-1;		}
		else if ( arg == "--readvariable"	|| arg == "-V" )	{	if ( n+1 < argc ) setReadVariable(argv[++n]);							else rc=-1;		}
		else if ( arg == "--sepchar"		|| arg == "-S" )	{	if ( n+1 < argc ) setSepchar(argv[++n]);								else rc=-1;		}
		else if ( arg == "--logfile"		|| arg == "-f" )	{	if ( n+1 < argc ) setLogfile(argv[++n]);								else rc=-1;		}
		else if ( arg == "--opendelay"		|| arg == "-o" )	{	if ( n+1 < argc ) setOpenDelay(QString(argv[++n]).toInt());				else rc=-1;		}
		else if ( arg == "--outputformat"	|| arg == "-O" )	{	if ( n+1 < argc ) setOutputFormat(QString(argv[++n]).toInt());			else rc=-1;		}
        else if ( arg == "--protocol" )                         {   if ( n+1 < argc ) setProtocol(QString(argv[++n]).toInt());              else rc=-1;     }

		else if ( arg == "--help" || arg == "-h" || arg == "-?")
		{
			printHelp();
			rc=-3;
		}
		else
		{
			rc=-2;
		}
		if ( rc >= 0 ) ++n;
	}
	switch( rc ) {
		case -1:
			fprintf( stderr, "%s: missing argument for '%s'\n", argv[0], argv[n] );
			break;
		case -2:
			fprintf( stderr, "%s: bad command '%s'\n", argv[0], argv[n] );
			break;
		default:
			break;
	}
	return rc;

}


