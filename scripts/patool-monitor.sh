#/bin/bash
SENSORDEVICE=/dev/ttyS5
DISPLAYDEVICE=/dev/ttyS0

stty ignbrk -brkint -icrnl -imaxbel -onlcr -icanon -echo 19200 < $DISPLAYDEVICE
echo -e -n "\xFE\x58" > $DISPLAYDEVICE
while [ true ]; do
	HUMIDITY=`patool --device $SENSORDEVICE --baud 2400 --protocol 1  --readvariable HUMIDITY | cut -c 1-5`
	TEMPERATURE_C=`patool --device $SENSORDEVICE --baud 2400 --protocol 1  --readvariable TEMPERATURE_C | cut -c 1-5`
	DEWPOINT_C=`patool --device $SENSORDEVICE --baud 2400 --protocol 1  --readvariable DEWPOINT_C | cut -c 1-5`
	TIME=`date +%H:%M`
	if [ "$HUMIDITY" != "" -a "$TEMPERATURE_C" != "" -a "$DEWPOINT_C" != "" ]; then 
		echo -e -n "\xFE\x47\x01\x01" > $DISPLAYDEVICE
		echo -e -n "RH:"$HUMIDITY"%" > $DISPLAYDEVICE
		echo -e -n " "$TEMPERATURE_C"C" > $DISPLAYDEVICE
		echo -e -n "\xFE\x47\x01\x02" > $DISPLAYDEVICE
		echo -e -n "DP:"$DEWPOINT_C"C" > $DISPLAYDEVICE
		echo -e -n " "$TIME > $DISPLAYDEVICE
		sleep 15
	fi
done
